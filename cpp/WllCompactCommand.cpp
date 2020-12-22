/*
 * WllCompactCommand.cpp
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#include "WllCompactCommand.h"
#include "VariableStack.h"
#include "Wll0Loader.h"
#include "Wll1Loader.h"
#include "WllIntepreter.h"
#include "Wll2IntepreterCompactLL1Impl.h"
#include "WllTrace.h"
#include "WllSingleton.h"
#include "WllString.h"
#include "LanguageAlgorithm.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <cassert>
#include "CompilerManager.h"
#include "IntepretException.h"
#include "Calculate.h"
using namespace std;
using namespace Wll::Util;


//load SYMBOLS by WllLoader to replace current languages
//$LOAD_TRANSLATION(COMPACT_SYMBOL) => VOID_SYMBOL
inline bool IntepretLoadTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();
	assert(compacted_symbol.type == COMPACT_SYMBOL);

	return LoadLanguage(compacted_symbol.GetList(), intepreter->compiler->languages, false);
}

//load SYMBOLS by WllLoader append into current languages
//$ADD_TRANSLATION(COMPACT_SYMBOL) => VOID_SYMBOL
inline bool IntepretAddTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();
	assert(compacted_symbol.type == COMPACT_SYMBOL);

	return LoadLanguage(compacted_symbol.GetList(), intepreter->compiler->languages, true);
}

//load SYMBOLS of TRANSLATION(MAP_SYMBOL) to replace current languages
//$WLL0(COMPACT_SYMBOL) => VOID_SYMBOL
inline bool IntepretWll0Command(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();
	assert(compacted_symbol.type == COMPACT_SYMBOL);

	intepreter->compiler->languages.translation_rules.clear();
	const vector<Symbols>& symbols = compacted_symbol.GetList();
	for(vector<Symbols>::const_iterator i = symbols.cbegin(); i != symbols.cend(); ++i)
	{
		const map<string,Symbols>& m = (*i).GetMap();
		
		LanguageTranslations translation(
			LanguageRules(
				(m.at("source_rule").GetMap()).at("root"),
				(m.at("source_rule").GetMap()).at("expression").GetList()
			),
			LanguageRules(
				m.at("destination_rule").GetMap().at("root"),
				m.at("destination_rule").GetMap().at("expression").GetList()			
			)
		);
		intepreter->compiler->languages.translation_rules.push_back(translation);
	}
		
	intepreter->compiler->languages.Initialize();
	return true;
}


//compose source_rule(MAP_SYMBOL) and destination_rule(MAP_SYMBOL) as translation(MAP_SYMBOL)
//$TRANSLATION(MAP_SYMBOL,MAP_SYMBOL) => MAP_SYMBOL
inline bool IntepretTranslationCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols destination_rule = data_stack.back();
	data_stack.pop_back();
	Symbols source_rule = data_stack.back();
	data_stack.pop_back();
	assert(source_rule.type == MAP_SYMBOL);
	assert(destination_rule.type == MAP_SYMBOL);

	Symbols translation(MAP_SYMBOL);
	translation.GetMap()["source_rule"] = source_rule;
	translation.GetMap()["destination_rule"] = destination_rule;
	data_stack.push_back(translation);
	
	return true;
}

//compose root_symbol(VARIABLE_SYMBOL) and expression(SYMBOLS) as rule(MAP_SYMBOL)
//$RULE(VARIABLE_SYMBOL,COMPACT_SYMBOL) => MAP_SYMBOL
inline bool IntepretRuleCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols root = data_stack.back();
	data_stack.pop_back();
	Symbols expression = data_stack.back();
	data_stack.pop_back();
	assert(root.IsVariable());
	assert(expression.type == COMPACT_SYMBOL);

	Symbols rule(MAP_SYMBOL);
	rule.GetMap()["root"] = root;
	rule.GetMap()["expression"] = expression;
	data_stack.push_back(rule);

	return true;
}


//compose constants as name of a variable(VARAIBLE_SYMBOL) 
//$VARIABLE(COMPACT_SYMBOL) => VARIABLE_SYMBOL
inline bool IntepretVariableCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();
	assert(compacted_symbol.type == COMPACT_SYMBOL);
	AssertSymbolsType(compacted_symbol.GetList(), CONSTANT_SYMBOL);

	string variable_name;
	ToString(variable_name, compacted_symbol.GetList());
	Symbols variable(variable_name.c_str());
	data_stack.push_back(variable);

	return true;
}


//keep input symbols(SYMBOLS) as constants
//$CONSTANT(COMPACT_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretConstantCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols& constants = data_stack.back();
	assert(constants.type == COMPACT_SYMBOL);
	AssertSymbolsType(constants.GetList(), CONSTANT_SYMBOL);

	return true;
}

//compose constants as name of a remark symbol(REMARK_SYMBOL)
//$REMARK(COMPACT_SYMBOL) => VARIABLE_SYMBOL
inline bool IntepretRemarkCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();
	assert(compacted_symbol.type == COMPACT_SYMBOL);
	AssertSymbolsType(compacted_symbol.GetList(), CONSTANT_SYMBOL);

	string remark_name;
	ToString(remark_name, compacted_symbol.GetList());
	Symbols remark(REMARK_SYMBOL, remark_name.c_str());
	data_stack.push_back(remark);

	return true;
}

//evalue symbols' result, whether execute evalue or not is controlled by ignore state
////$EVAL(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
inline bool IntepretEvalCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back(); 

	if (symbol.type == COMPACT_SYMBOL)
	{
		Wll2IntepreterCompactLL1Impl* i = (Wll2IntepreterCompactLL1Impl*)(intepreter);
		return i->IntepretExpression(symbol, data_stack);
	}
	else if (symbol.type == S_EXP_SYMBOL)
	{
		Wll2IntepreterCompactLL1Impl* i = (Wll2IntepreterCompactLL1Impl*)(intepreter);
		return i->IntepretSExpression(symbol, data_stack);		
	}
	else
	{
		data_stack.push_back(symbol);
		return true;
	}
}

//evalue symbols' result, execute evalue will always do, regardness ignore state
//$EXEC(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
inline bool IntepretExecCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);
{
	//SAME AS EVAL COMMAND EXCEPT EXEC TIME
	return IntepretEvalCommand(data_stack, intepreter);
}

//$IGNORE(ANY_SYMBOL) => ANY_SYMBOL
inline bool IntepretIgnoreCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);
{
	assert(data_stack.size() >= 1);

	return true;
}

//$COMPACT(ANY_SYMBOL,ANY_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCompactCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols data2 = data_stack.back();
	data_stack.pop_back();
	Symbols data1 = data_stack.back();
	data_stack.pop_back();
	
	Symbols compacted_symbol(COMPACT_SYMBOL);
	Flat(data1, compacted_symbol.GetList());
	Flat(data2, compacted_symbol.GetList());
	data_stack.push_back(compacted_symbol);

	return true;	
}

//$LIST ($LIST, A, B, ... , N, n) = (A, B, ... , N)
//$LIST(ANY_SYMBOL, ... , ANY_SYMBOL, n) = S_EXP_SYMBOL
inline bool IntepretListCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols n = data_stack.back();
	data_stack.pop_back();
	assert(n.type == INTEGER_SYMBOL);
	assert(data_stack.size() >= n.i);

	Symbols list(S_EXP_SYMBOL);
	list.GetList().resize(n.i);
	for(vector<Symbols>::reverse_iterator i = list.GetList().rbegin(); i != list.GetList().rend(); i++)
	{
		*i = data_stack.back();
		data_stack.pop_back();
	}
	data_stack.push_back(list);

	return true;
}

//$APPEND(S_EXP_SYMBOL, S_EXP_SYMBOL) = S_EXP_SYMBOL
//($APPEND, (A,B), (C,D)) = (A, B, C, D)
inline bool IntepretAppendCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols tail_list = data_stack.back();
	data_stack.pop_back();
	Symbols head_list = data_stack.back();
	data_stack.pop_back();
	assert(head_list.type == S_EXP_SYMBOL);
	assert(tail_list.type == S_EXP_SYMBOL);

	Symbols list(head_list);
	list.GetList() += tail_list.GetList();
	data_stack.push_back(list);

	return true;
}

//$CAR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CAR, (A,B,C)) = A
inline bool IntepretCarCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	if ((symbol.type == S_EXP_SYMBOL || symbol.type == COMPACT_SYMBOL) && (symbol.GetList().size() > 0))
	{
		data_stack.push_back(symbol.GetList().front());
	}
	else if (symbol.type == STRING_SYMBOL && (*symbol.s)->size() > 0)
	{
		data_stack.push_back(Symbols(CONSTANT_SYMBOL, (*symbol.s)->front()));
	}
	else
	{
		data_stack.push_back(Symbols(COMPACT_SYMBOL));
	}

	return true;
}

//$CDR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CDR, (A,B,C)) = (B, C)
inline bool IntepretCdrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	if ((symbol.type == S_EXP_SYMBOL || symbol.type == COMPACT_SYMBOL) && (symbol.GetList().size() > 1))
	{
		data_stack.push_back(Symbols(symbol.type, vector<Symbols>(symbol.GetList().begin()+1, symbol.GetList().end())));
	}
	else if (symbol.type == STRING_SYMBOL && (*symbol.s)->size() > 0)
	{
		(*(*symbol.s)) = (*symbol.s)->substr(1);
		data_stack.push_back(symbol);
	}
	else
	{
		data_stack.push_back(Symbols(COMPACT_SYMBOL));
	}

	return true;
}

//$COND(TRUE|FALSE, COMPACT_SYMBOL|S_EXP_SYMBOL)
//($COND, CONDITION, THEN, ELSE)
inline bool IntepretCondCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size >= 2);
	Symbols else_part = data_stack.back();
	data_stack.pop_back();
	Symbols then_part = data_stack.back();
	data_stack.pop_back();
	Symbols condition = data_stack.back();
	data_stack.pop_back();

	if (condition == Symbols::TRUE)
	{
		if (then_part.type == COMPACT_SYMBOL || then_part.type == S_EXP_SYMBOL)
		{
			intepreter->IntepretWll(then_part.GetList(), data_stack);
		}
		else
		{
			data_stack.push_back(then_part);
		}
		
	}
	else
	{
		if (else_part.type == COMPACT_SYMBOL || else_part.type == S_EXP_SYMBOL)
		{
			intepreter->IntepretWll(else_part.GetList(), data_stack);
		}
		else
		{
			data_stack.push_back(else_part);
		}
		
	}
	
	return true;
}


//$LOOP(COMPACT,COMPACT)
//($LOOP, CONDITION, REPEAT)
inline bool IntepretLoopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols condition = data_stack.back();
	data_stack.pop_back();
	Symbols repeat = data_stack.back();
	data_stack.pop_back();
	assert(repeat.type == COMPACT_SYMBOL || repeat.type == S_EXP_SYMBOL);

	do
	{
		Symbols condition_result;
		if (condition.type == COMPACT_SYMBOL || condition.type == S_EXP_SYMBOL)
		{
			intepreter->IntepretWll(condition.GetList(), data_stack);
			condition_result = data_stack.back();
			data_stack.pop_back();
		}
		else
		{
			condition_result = condition;
		}
		
		if (!(condition_result == Symbols::TRUE)) break;

		intepreter->IntepretWll(repeat.GetList(), data_stack);
	} while (true);

	return true;
}

//$CALL(VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($CALL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols input_symbols = data_stack.back();
	data_stack.pop_back();
	Symbols start_symbol = data_stack.back();
	data_stack.pop_back();
	assert(input_symbols.type == COMPACT_SYMBOL);
	assert(start_symbol.type == VARIABLE_SYMBOL);

	vector<Symbols> result;
	bool retval = intepreter->compiler->Process(input_symbols.GetList(), result, start_symbol);
	if (retval) data_stack.push_back(Encode(result, true));

	return retval;
}

//$FCALL(STRING_SYMBOL,VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($FCALL, STRING_SYMBOL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 3);
	Symbols input_symbols = data_stack.back();
	data_stack.pop_back();
	Symbols start_symbol = data_stack.back();
	data_stack.pop_back();
	Symbols file_name = data_stack.back();
	data_stack.pop_back();
	assert(input_symbols.type == COMPACT_SYMBOL);
	assert(start_symbol.type == VARIABLE_SYMBOL);
	assert(file_name.type == STRING_SYMBOL);
	
	vector<Symbols> result;
	Compiler* compiler = Singleton<CompilerManager>().GetInstance()->CreateOrGetCompiler(**file_name.s);
	bool retval = compiler->Process(input_symbols.GetList(), result, start_symbol);
	if (retval) data_stack.push_back(Encode(result, true));

	return retval;
}

Symbols* Index(Symbols* symbol, string index, bool write_flag=false)
{	
	assert(symbol != NULL);

	if(symbol->type == LIST_SYMBOL)
	{
		int index_i;
		String2Int(index,index_i);
		if(index_i == symbol->GetList().size())
		{
			if(write_flag)
			{
				symbol->GetList().resize(index_i+1);
			}
			else
			{
				throw IntepretException("read index["+index+"] out of range");
			}
		}
		else if(index_i > symbol->GetList().size() || index_i < 0)
		{
			throw IntepretException("index["+index+"] out of range");
			//throw IntepretException((write_flag?string("write"):string("read"))+" index["+index+"] out of range"); 
		}
		symbol = &(symbol->GetList()[index_i]);
	}
	else if(symbol->type == MAP_SYMBOL)
	{
		symbol = &(symbol->GetMap()[index]);
	}
	else
	{
		throw IntepretException("index with type other than LIST and MAP");
	}		

	return symbol;
}

//$REF(REFERENCE,STRING)
inline bool IntepretLRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();
	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->LookupOrRegister(*(*(symbol.s)));
		data_stack.push_back(Symbols(reference_symbol));	
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(data_stack.size() >= 1);
		Symbols index = data_stack.back();
		data_stack.pop_back();
		assert(index.type == STRING_SYMBOL);
		Symbols* reference_symbol = Index(symbol.reference, **index.s, true);
		data_stack.push_back(Symbols(reference_symbol));
	}
	
	return true;
}

//$RREF(REFERENCE,STRING)
inline bool IntepretRRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();
	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->Lookup(*(*(symbol.s)));
		data_stack.push_back(Symbols(reference_symbol));	
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(data_stack.size() >= 1);
		Symbols index = data_stack.back();
		data_stack.pop_back();
		assert(index.type == STRING_SYMBOL);
		Symbols* reference_symbol = Index(symbol.reference, **index.s, false);
		data_stack.push_back(Symbols(reference_symbol));
	}
	
	return true;	
}

//$DEF(STRING, ANY_SYMBOL)
inline bool IntepretDefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols value = data_stack.back();
	data_stack.pop_back();
	Symbols symbol = data_stack.back();
	data_stack.pop_back();
	assert(symbol.type == STRING_SYMBOL);

	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());
	Symbols* reference_symbol = variable_table_stack->Register(*(*(symbol.s)));
	*reference_symbol = value;
	
	return true;
}

//$SET(STRING|REFERENCE, ANY_SYMBOL)
inline bool IntepretSetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols value = data_stack.back();
	data_stack.pop_back();
	Symbols symbol = data_stack.back();
	data_stack.pop_back();
	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->LookupOrRegister(*(*(symbol.s)));
		*reference_symbol = value;
	} 
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(symbol.reference != NULL);
		*symbol.reference = value;
	}
	
	return true;
}

//$GET(STRING|REFERENCE, ANY_SYMBOL)
inline bool IntepretGetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();
	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->Lookup(*(*(symbol.s)));
		assert(reference_symbol != NULL);
		data_stack.push_back(*reference_symbol);	
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(symbol.reference != NULL);
		data_stack.push_back(*symbol.reference);
	}

	return true;
}

//$PUSHDATA()
//($PUSHDATA)
inline bool IntepretPushDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	variable_table_stack->Push();

	return true;
}

//$POPDATA()
//($POPDATA)
inline bool IntepretPopDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());
	variable_table_stack->Pop();

	return true;
}

//$PUSH
inline bool IntepretPushCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	vector<Symbols>*parameter_stack = Singleton<vector<Symbols> >::GetInstance();
	parameter_stack->push_back(data_stack.back());

	return true;
}

//$POP(STRING_SYMBOL|REFERENCE)
inline bool IntepretPopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	vector<Symbols>*parameter_stack = Singleton<vector<Symbols> >::GetInstance();
	assert(!parameter_stack.empty());

	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();
	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->LookupOrRegister(*(*(symbol.s)));
		*reference_symbol = parameter_stack->back();
		parameter_stack->pop_back();	
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(symbol.reference != NULL);
		*symbol.reference = parameter_stack->back();
		parameter_stack->pop_back();
	}
	
	return true;
}

//$ARRAY()
//($ARRAY)
inline bool IntepretArrayCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	data_stack.push_back(Symbols(LIST_SYMBOL));

	return true;
}

//$MAP()
//($MAP)
inline bool IntepretMapCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	data_stack.push_back(Symbols(MAP_SYMBOL));

	return true;
}

//$CAST(REMARK_SYMBOL,SYMBOL)
inline bool IntepretCastCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols value = data_stack.back();
	data_stack.pop_back();
	Symbols type = data_stack.back();
	data_stack.pop_back();
	assert(type.IsRemark());

	//TODO: CastTo: COMPACT_SYMBOL => INTEGER|LONG|FLOAT|DOUBLE	
	data_stack.push_back(CastTo(type, value));

	return true;
}

//($ADD, SYMBOLS, SYMBOLS) = SYMBOLS + SYMBOLS
inline bool IntepretAddCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();
	assert(
		n1.type == CHAR_SYMBOL ||
		n1.type == INTEGER_SYMBOL ||
		n1.type == LONG_SYMBOL ||
		n1.type == FLOAT_SYMBOL ||
		n1.type == DOUBLE_SYMBOL ||
		n1.type == COMPACT_SYMBOL
	);
	assert(
		n2.type == CHAR_SYMBOL ||
		n2.type == INTEGER_SYMBOL ||
		n2.type == LONG_SYMBOL ||
		n2.type == FLOAT_SYMBOL ||
		n2.type == DOUBLE_SYMBOL ||
		n2.type == COMPACT_SYMBOL
	);	

	if (n1.type == COMPACT_SYMBOL)
	{
		n1 = CastTo(n1.GetList(), true);
	}
	if (n2.type == COMPACT_SYMBOL)
	{
		n2 = CastTo(n2.GetList(), true);
	}	
	AddTo(n1, n2);
	data_stack.push_back(n1);

	return true;
}

//($SUB, SYMBOLS, SYMBOLS) = SYMBOLS - SYMBOLS
inline bool IntepretSubCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();
	assert(
		n1.type == CHAR_SYMBOL ||
		n1.type == INTEGER_SYMBOL ||
		n1.type == LONG_SYMBOL ||
		n1.type == FLOAT_SYMBOL ||
		n1.type == DOUBLE_SYMBOL ||
		n1.type == COMPACT_SYMBOL
	);
	assert(
		n2.type == CHAR_SYMBOL ||
		n2.type == INTEGER_SYMBOL ||
		n2.type == LONG_SYMBOL ||
		n2.type == FLOAT_SYMBOL ||
		n2.type == DOUBLE_SYMBOL ||
		n2.type == COMPACT_SYMBOL
	);	

	if (n1.type == COMPACT_SYMBOL)
	{
		n1 = CastTo(n1.GetList(), true);
	}
	if (n2.type == COMPACT_SYMBOL)
	{
		n2 = CastTo(n2.GetList(), true);
	}	
	n1 -= n2;
	data_stack.push_back(n1);

	return true;
}

//($MUL, SYMBOLS, SYMBOLS) = SYMBOLS * SYMBOLS
inline bool IntepretMulCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();
	assert(
		n1.type == CHAR_SYMBOL ||
		n1.type == INTEGER_SYMBOL ||
		n1.type == LONG_SYMBOL ||
		n1.type == FLOAT_SYMBOL ||
		n1.type == DOUBLE_SYMBOL ||
		n1.type == COMPACT_SYMBOL
	);
	assert(
		n2.type == CHAR_SYMBOL ||
		n2.type == INTEGER_SYMBOL ||
		n2.type == LONG_SYMBOL ||
		n2.type == FLOAT_SYMBOL ||
		n2.type == DOUBLE_SYMBOL ||
		n2.type == COMPACT_SYMBOL
	);	

	if (n1.type == COMPACT_SYMBOL)
	{
		n1 = CastTo(n1.GetList(), true);
	}
	if (n2.type == COMPACT_SYMBOL)
	{
		n2 = CastTo(n2.GetList(), true);
	}	
	n1 *= n2;
	data_stack.push_back(n1);

	return true;
}

//($DIV, SYMBOLS, SYMBOLS) = SYMBOLS / SYMBOLS
inline bool IntepretDivCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();
	assert(
		n1.type == CHAR_SYMBOL ||
		n1.type == INTEGER_SYMBOL ||
		n1.type == LONG_SYMBOL ||
		n1.type == FLOAT_SYMBOL ||
		n1.type == DOUBLE_SYMBOL ||
		n1.type == COMPACT_SYMBOL
	);
	assert(
		n2.type == CHAR_SYMBOL ||
		n2.type == INTEGER_SYMBOL ||
		n2.type == LONG_SYMBOL ||
		n2.type == FLOAT_SYMBOL ||
		n2.type == DOUBLE_SYMBOL ||
		n2.type == COMPACT_SYMBOL
	);	

	if (n1.type == COMPACT_SYMBOL)
	{
		n1 = CastTo(n1.GetList(), true);
	}
	if (n2.type == COMPACT_SYMBOL)
	{
		n2 = CastTo(n2.GetList(), true);
	}	
	n1 /= n2;
	data_stack.push_back(n1);

	return true;
}

//($EQ, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
inline bool IntepretEqCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();
	assert(
		n1.type == CHAR_SYMBOL ||
		n1.type == INTEGER_SYMBOL ||
		n1.type == LONG_SYMBOL ||
		n1.type == FLOAT_SYMBOL ||
		n1.type == DOUBLE_SYMBOL ||
		n1.type == COMPACT_SYMBOL
	);
	assert(
		n2.type == CHAR_SYMBOL ||
		n2.type == INTEGER_SYMBOL ||
		n2.type == LONG_SYMBOL ||
		n2.type == FLOAT_SYMBOL ||
		n2.type == DOUBLE_SYMBOL ||
		n2.type == COMPACT_SYMBOL
	);

	if (n1.type == COMPACT_SYMBOL)
	{
		n1 = CastTo(n1.GetList(), true);
	}
	if (n2.type == COMPACT_SYMBOL)
	{
		n2 = CastTo(n2.GetList(), true);
	}
	data_stack.push_back(Equal(n1, n2) ? Symbols::TRUE : Symbols::FALSE);

	return true;
}

//($LT, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
inline bool IntepretLtCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();
	assert(
		n1.type == CHAR_SYMBOL ||
		n1.type == INTEGER_SYMBOL ||
		n1.type == LONG_SYMBOL ||
		n1.type == FLOAT_SYMBOL ||
		n1.type == DOUBLE_SYMBOL ||
		n1.type == COMPACT_SYMBOL
	);
	assert(
		n2.type == CHAR_SYMBOL ||
		n2.type == INTEGER_SYMBOL ||
		n2.type == LONG_SYMBOL ||
		n2.type == FLOAT_SYMBOL ||
		n2.type == DOUBLE_SYMBOL ||
		n2.type == COMPACT_SYMBOL
	);

	if (n1.type == COMPACT_SYMBOL)
	{
		n1 = CastTo(n1.GetList(), true);
	}
	if (n2.type == COMPACT_SYMBOL)
	{
		n2 = CastTo(n2.GetList(), true);
	}
	data_stack.push_back(LessThan(n1, n2) ? Symbols::TRUE : Symbols::FALSE);

	return true;
}

//$AND(TRUE|FALSE, TRUE|FALSE)
inline bool IntepretAndCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols b2 = data_stack.back();
	data_stack.pop_back();
	Symbols b1 = data_stack.back();
	data_stack.pop_back();
	assert(b1 == Symbols::TRUE || b1 == Symbols::FALSE);
	assert(b2 == Symbols::TRUE || b2 == Symbols::FALSE);

	data_stack.push_back((b1 == Symbols::TRUE && b2 == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE);
	
	return true;
}

//$OR(TRUE|FALSE, TRUE|FALSE)
inline bool IntepretOrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols b2 = data_stack.back();
	data_stack.pop_back();
	Symbols b1 = data_stack.back();
	data_stack.pop_back();
	assert(b1 == Symbols::TRUE || b1 == Symbols::FALSE);
	assert(b2 == Symbols::TRUE || b2 == Symbols::FALSE);

	data_stack.push_back((b1 == Symbols::TRUE || b2 == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE);
	
	return true;
}

//$NOT(TRUE|FALSE, TRUE|FALSE)
inline bool IntepretNotCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols b1 = data_stack.back();
	data_stack.pop_back();
	assert(b1 == Symbols::TRUE || b1 == Symbols::FALSE);

	data_stack.push_back((b1 == Symbols::FALSE) ? Symbols::TRUE : Symbols::FALSE);
	
	return true;
}

ShellCommand::ShellCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCompactCommand(cmd,parameter_fields,intepreter)
{
}

bool ShellCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 2);
	string input;
	ToString(input,this->parameters[1]);
	FILE* fp = popen(input.c_str(),"r");
	if(fp==NULL)
	{
		ERROR("popen failed");
		return false;
	}
	char c;
	while((c=fgetc(fp))!=EOF)
	{
		result.push_back(c);
	}
	if(pclose(fp)!=0)
	{
		ERROR("pclose failed");
		return false;
	}
	return true;
}


CatCommand::CatCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCompactCommand(cmd,parameter_fields,intepreter)
{

}

bool CatCommand::Intepret(std::vector<Symbols>& result)
{

	assert(this->parameters.size() == 2);
	//($CAT, "<file_name>")
	string file_name;
	ToString(file_name, this->parameters[1]);
	INFO("file_name="<<file_name);
	ifstream input_file(file_name.c_str());
	if(input_file.fail()) return false;
	INFO("result="<<result);
	input_file>>result;
	INFO("result="<<result);
	return true;
}


SubStrCommand::SubStrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCompactCommand(cmd,parameter_fields,intepreter)
{

}

bool SubStrCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==3 || this->parameters.size()==4);

	string str;
	ToString(str, this->parameters[1]);

	string from_str;
	ToString(from_str, this->parameters[2]);
	int from;
	String2Int(from_str, from);

	int size = string::npos;
	if(this->parameters.size()==4)
	{
		string size_str;
		ToString(size_str, this->parameters[3]);
		String2Int(size_str, size);
	}

	if(from<0)
	{
		from = str.size() + from;
		if(from<0) from = 0;
	}

	string sub_str = str.substr(from,size);

	for(string::const_iterator i = sub_str.begin(); i != sub_str.end(); ++i)
	{
		result.push_back(Symbols(*i));
	}

	return true;
}

WllCompactCommand* WllCommandFactory::CreateCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
{
	INFO("cmd="<<cmd);
	INFO("parameters=");
	for(int  i = 0; i<parameter_fields.size(); ++i)
	{
		INFO("parameter["<<i<<"]="<<parameter_fields[i]);
	}

	WllCompactCommand* command = NULL;
	if(cmd == Symbols::LOAD_TRANSLATIONS)
	{
		command = new LoadTranslationsCommand(cmd,parameter_fields,intepreter);
	}//LOAD_TRANSLATIONS
	else if(cmd == Symbols::ADD_TRANSLATIONS)
	{
		command = new AddTranslationsCommand(cmd,parameter_fields,intepreter);
	}//LOAD_TRANSLATIONS
	else if(cmd == Symbols::REMARK_WLL0)
	{
		command = new Wll0Command(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_TRANSLATION)
	{
		command = new TranslationCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_RULE)
	{
		command = new RuleCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_VARIABLE)
	{
		command = new VariableCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_CONSTANT)
	{
		command = new ConstantCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_REMARK)
	{
		command = new RemarkCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::LIST)
	{
		command = new ListCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::APPEND)
	{
		command = new AppendCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::CAR)
	{
		command = new CarCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::CDR)
	{
		command = new CdrCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::EVAL)
	{
		command = new EvalCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::EXEC)
	{
		command = new ExecCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_IGNORE)
	{
		command = new IgnoreCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::CALL)
	{
		command = new CallCommand(cmd,parameter_fields,intepreter);
	}	
	else if(cmd == Symbols::COND)
	{
		command = new CondCommand(cmd,parameter_fields,intepreter);
	}//COND
	else if(cmd == Symbols::LOOP)
	{
		command = new LoopCommand(cmd,parameter_fields,intepreter);
	}//LOOP
	else if(cmd == Symbols::DEF)
	{
		command = new DefCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::SET)
	{
		command = new SetCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::GET)
	{
		command = new GetCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::PUSH_DATA)
	{
		command = new PushDataCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::POP_DATA)
	{
		command = new PopDataCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::PUSH)
	{
		command = new PushCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::POP)
	{
		command = new PopCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::ARRAY)
	{
		command = new ArrayCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::MAP)
	{
		command = new MapCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::SUB_STR)
	{
		command = new SubStrCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::CAST)
	{
		command = new CastCommand(cmd, parameter_fields, intepreter);
	}	
	else if(cmd == Symbols::ADD)
	{
		command = new AddCommand(cmd,parameter_fields,intepreter);
	}//ADD
	else if(cmd == Symbols::SUB)
	{
		command = new SubCommand(cmd,parameter_fields,intepreter);
	}//SUB
	else if(cmd == Symbols::MUL)
	{
		command = new MulCommand(cmd,parameter_fields,intepreter);
	}//MUL
	else if(cmd == Symbols::DIV)
	{
		command = new DivCommand(cmd,parameter_fields,intepreter);
	}//DIV
	else if(cmd == Symbols::EQ)
	{
		command = new EqCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::LT)
	{
		command = new LtCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::AND)
	{
		command = new AndCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::OR)
	{
		command = new OrCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::NOT)
	{
		command = new NotCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::SHELL)
	{
		command = new ShellCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::CAT)
	{
		command = new CatCommand(cmd, parameter_fields, intepreter);
	}


	assert(command);
	return command;
}


