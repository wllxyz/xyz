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
bool IntepretLoadTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::LOAD_TRANSLATIONS << ", op1=" << compacted_symbol);

	assert(compacted_symbol.type == COMPACT_SYMBOL);

	return LoadLanguage(compacted_symbol.GetList(), intepreter->compiler->languages, false);
}

//load SYMBOLS by WllLoader append into current languages
//$ADD_TRANSLATION(COMPACT_SYMBOL) => VOID_SYMBOL
bool IntepretAddTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::ADD_TRANSLATIONS << ", op1=" << compacted_symbol);

	assert(compacted_symbol.type == COMPACT_SYMBOL);

	return LoadLanguage(compacted_symbol.GetList(), intepreter->compiler->languages, true);
}

//load SYMBOLS of TRANSLATION(MAP_SYMBOL) to replace current languages
//$WLL0(COMPACT_SYMBOL) => VOID_SYMBOL
bool IntepretWll0Command(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::REMARK_WLL0 << ", op1=" << compacted_symbol);

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
bool IntepretTranslationCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols destination_rule = data_stack.back();
	data_stack.pop_back();
	Symbols source_rule = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::REMARK_TRANSLATION << ", op1=" << source_rule << ", op2=" << destination_rule);

	assert(source_rule.type == MAP_SYMBOL);
	assert(destination_rule.type == MAP_SYMBOL);

	Symbols translation(MAP_SYMBOL);
	translation.GetMap()["source_rule"] = source_rule;
	translation.GetMap()["destination_rule"] = destination_rule;
	data_stack.push_back(translation);

	INFO("command=" << Symbols::REMARK_TRANSLATION << ", result=" << translation);
	
	return true;
}

//compose root_symbol(VARIABLE_SYMBOL) and expression(SYMBOLS) as rule(MAP_SYMBOL)
//$RULE(VARIABLE_SYMBOL,COMPACT_SYMBOL) => MAP_SYMBOL
bool IntepretRuleCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols expression = data_stack.back();
	data_stack.pop_back();
	Symbols root = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::REMARK_RULE << ", op1=" << root << ", op2=" << expression);

	assert(root.IsVariable());
	assert(expression.type == COMPACT_SYMBOL);

	Symbols rule(MAP_SYMBOL);
	rule.GetMap()["root"] = root;
	rule.GetMap()["expression"] = expression;
	data_stack.push_back(rule);

	INFO("command=" << Symbols::REMARK_RULE << ", result=" << rule);

	return true;
}


//compose constants as name of a variable(VARAIBLE_SYMBOL) 
//$VARIABLE(COMPACT_SYMBOL) => VARIABLE_SYMBOL
bool IntepretVariableCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::REMARK_VARIABLE << ", op1=" << compacted_symbol);

	assert(compacted_symbol.type == COMPACT_SYMBOL);
	AssertSymbolsType(compacted_symbol.GetList(), CONSTANT_SYMBOL);

	string variable_name;
	ToString(variable_name, compacted_symbol.GetList());
	Symbols variable(variable_name.c_str());
	data_stack.push_back(variable);

	INFO("command=" << Symbols::REMARK_VARIABLE << ", result=" << variable);

	return true;
}


//keep input symbols(SYMBOLS) as constants
//$CONSTANT(COMPACT_SYMBOL) => COMPACT_SYMBOL
bool IntepretConstantCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols& constants = data_stack.back();

	INFO("command=" << Symbols::REMARK_CONSTANT << ", op1=" << constants);

	assert(constants.type == COMPACT_SYMBOL || constants.type == CONSTANT_SYMBOL);
	if (constants.type == COMPACT_SYMBOL) 
	{
		AssertSymbolsType(constants.GetList(), CONSTANT_SYMBOL);
	}

	INFO("command=" << Symbols::REMARK_CONSTANT << ", result=" << constants);

	return true;
}

//compose constants as name of a remark symbol(REMARK_SYMBOL)
//$REMARK(COMPACT_SYMBOL) => VARIABLE_SYMBOL
bool IntepretRemarkCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols compacted_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::REMARK_REMARK << ", op1=" << compacted_symbol);

	assert(compacted_symbol.type == COMPACT_SYMBOL);
	AssertSymbolsType(compacted_symbol.GetList(), CONSTANT_SYMBOL);

	string remark_name;
	ToString(remark_name, compacted_symbol.GetList());
	Symbols remark(REMARK_SYMBOL, remark_name.c_str());
	data_stack.push_back(remark);

	INFO("command=" << Symbols::REMARK_REMARK << ", op1=" << remark);

	return true;
}

///!!! EVAL, EXEC, IGNORE命令都不做任何事情,其區別在控制求值開關狀態
//evalue symbols' result, whether execute evalue or not is controlled by ignore state
////$EVAL(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
bool IntepretEvalCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);

	INFO("command=" << Symbols::EVAL <<", op1=" << data_stack.back());
	INFO("command=" << Symbols::EVAL <<", result=" << data_stack.back());

	return true;
}

//evalue symbols' result, execute evalue will always do, regardness ignore state
//$EXEC(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
bool IntepretExecCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);

	INFO("command=" << Symbols::EXEC <<", op1=" << data_stack.back());
	INFO("command=" << Symbols::EXEC <<", result=" << data_stack.back());

	return true;
}

//$IGNORE(ANY_SYMBOL) => ANY_SYMBOL
bool IntepretIgnoreCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);

	INFO("command=" << Symbols::REMARK_IGNORE <<", op1=" << data_stack.back());
	INFO("command=" << Symbols::REMARK_IGNORE <<", result=" << data_stack.back());

	return true;
}

//$COMPACT(ANY_SYMBOL,ANY_SYMBOL) => COMPACT_SYMBOL
bool IntepretCompactCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols data2 = data_stack.back();
	data_stack.pop_back();
	Symbols data1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::COMPACT <<", op1=" << data1 << ", op2=" << data2);
	
	Symbols compacted_symbol(COMPACT_SYMBOL);
	Flat(data1, compacted_symbol.GetList());
	Flat(data2, compacted_symbol.GetList());
	data_stack.push_back(compacted_symbol);

	INFO("command=" << Symbols::COMPACT <<", result=" << compacted_symbol);

	return true;	
}

//$LIST ($LIST, A, B, ... , N, n) = (A, B, ... , N)
//$LIST(ANY_SYMBOL, ... , ANY_SYMBOL, n) = S_EXP_SYMBOL
bool IntepretListCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols n = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::LIST <<", op1=" << n);

	assert(n.type == INTEGER_SYMBOL);
	assert(data_stack.size() >= n.i);

	Symbols list(S_EXP_SYMBOL);
	list.GetList().resize(n.i);
	for(vector<Symbols>::reverse_iterator i = list.GetList().rbegin(); i != list.GetList().rend(); i++)
	{
		*i = data_stack.back();
		INFO("pop list element=" << *i);
		data_stack.pop_back();
	}
	data_stack.push_back(list);

	INFO("command=" << Symbols::LIST <<", result=" << list);

	return true;
}

//$APPEND(S_EXP_SYMBOL, S_EXP_SYMBOL) = S_EXP_SYMBOL
//($APPEND, (A,B), (C,D)) = (A, B, C, D)
bool IntepretAppendCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols tail_list = data_stack.back();
	data_stack.pop_back();
	Symbols head_list = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::APPEND <<", op1=" << head_list << ", op2=" << tail_list);

	assert(head_list.type == S_EXP_SYMBOL);
	assert(tail_list.type == S_EXP_SYMBOL);

	Symbols list(head_list);
	list.GetList() += tail_list.GetList();
	data_stack.push_back(list);

	INFO("command=" << Symbols::APPEND << ", result=" << list);

	return true;
}

//$CAR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CAR, (A,B,C)) = A
bool IntepretCarCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::CAR << ", op1=" << symbol);

	Symbols result;
	if ((symbol.type == S_EXP_SYMBOL || symbol.type == COMPACT_SYMBOL) && (symbol.GetList().size() > 0))
	{
		result = symbol.GetList().front();
	}
	else if (symbol.type == STRING_SYMBOL && (*symbol.s)->size() > 0)
	{
		result = Symbols(CONSTANT_SYMBOL, (*symbol.s)->front());
	}
	else
	{
		result = Symbols(COMPACT_SYMBOL);
	}
	data_stack.push_back(result);

	INFO("command=" << Symbols::CAR << ", result=" << result);

	return true;
}

//$CDR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CDR, (A,B,C)) = (B, C)
bool IntepretCdrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::CDR << ", op1=" << symbol);

	Symbols result;
	if ((symbol.type == S_EXP_SYMBOL || symbol.type == COMPACT_SYMBOL) && (symbol.GetList().size() > 1))
	{
		result = Symbols(symbol.type, vector<Symbols>(symbol.GetList().begin()+1, symbol.GetList().end()));
	}
	else if (symbol.type == STRING_SYMBOL && (*symbol.s)->size() > 0)
	{
		(*(*symbol.s)) = (*symbol.s)->substr(1);
		result = symbol;
	}
	else
	{
		result = Symbols(COMPACT_SYMBOL);
	}
	data_stack.push_back(result);

	return true;
}

//$COND(TRUE|FALSE, COMPACT_SYMBOL|S_EXP_SYMBOL)
//($COND, CONDITION, THEN, ELSE)
bool IntepretCondCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 3);
	Symbols else_part = data_stack.back();
	data_stack.pop_back();
	Symbols then_part = data_stack.back();
	data_stack.pop_back();
	Symbols condition = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::COND << ", condition=" << condition << ", then_part=" << then_part << ", else_part=" << else_part);

	vector<Symbols> result;
	if (condition == Symbols::TRUE)
	{
		if (then_part.type == COMPACT_SYMBOL || then_part.type == S_EXP_SYMBOL)
		{
			intepreter->IntepretWll(then_part.GetList(), result);
		}
		else
		{
			result.push_back(then_part);
		}
	}
	else
	{
		if (else_part.type == COMPACT_SYMBOL || else_part.type == S_EXP_SYMBOL)
		{
			intepreter->IntepretWll(else_part.GetList(), result);
		}
		else
		{
			result.push_back(else_part);
		}
	}
	data_stack += result;

	INFO("command=" << Symbols::COND << ", result=" << result);
	
	return true;
}


//$LOOP(COMPACT,COMPACT)
//($LOOP, CONDITION, REPEAT)
bool IntepretLoopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols repeat = data_stack.back();
	data_stack.pop_back();
	Symbols condition = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::LOOP << ", condition=" << condition << ", repeat=" << repeat);

	assert(repeat.type == COMPACT_SYMBOL || repeat.type == S_EXP_SYMBOL);

	vector<Symbols>::iterator r = data_stack.end();
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

	INFO("command=" << Symbols::LOOP << ", result=" << vector<Symbols>(r,data_stack.end()));

	return true;
}

//$CALL(VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($CALL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols input_symbols = data_stack.back();
	data_stack.pop_back();
	Symbols start_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::CALL << ", start_symbol=" << start_symbol << ", input_symbols=" << input_symbols);

	assert(input_symbols.type == COMPACT_SYMBOL);
	assert(start_symbol.type == VARIABLE_SYMBOL);

	vector<Symbols> result;
	bool retval = intepreter->compiler->Process(input_symbols.GetList(), result, start_symbol);
	if (retval) data_stack.push_back(Encode(result, true));
	
	INFO("command=" << Symbols::CALL << ", result=" << result);

	return retval;
}

//$FCALL(STRING_SYMBOL,VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($FCALL, STRING_SYMBOL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
bool IntepretFCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 3);
	Symbols input_symbols = data_stack.back();
	data_stack.pop_back();
	Symbols start_symbol = data_stack.back();
	data_stack.pop_back();
	Symbols file_name = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::FCALL << ", file_name=" << file_name << ", start_symbol=" << start_symbol << ", input_symbols=" << input_symbols);

	assert(input_symbols.type == COMPACT_SYMBOL);
	assert(start_symbol.type == VARIABLE_SYMBOL);
	assert(file_name.type == STRING_SYMBOL);
	
	vector<Symbols> result;
	Compiler* compiler = Singleton<CompilerManager>().GetInstance()->CreateOrGetCompiler(**file_name.s);
	bool retval = compiler->Process(input_symbols.GetList(), result, start_symbol);
	if (retval) data_stack.push_back(Encode(result, true));

	INFO("command=" << Symbols::FCALL << ", result=" << result);

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
bool IntepretLRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::LREF << ", op1=" << symbol);

	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	Symbols result((Symbols*)(NULL));
	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		result.reference = variable_table_stack->LookupOrRegister(*(*(symbol.s)));
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(data_stack.size() >= 1);
		Symbols index = data_stack.back();
		data_stack.pop_back();
		assert(index.type == STRING_SYMBOL);
		result.reference = Index(symbol.reference, **index.s, true);
	}
	data_stack.push_back(result);

	INFO("command=" << Symbols::LREF << ", result=" << result);
	
	return true;
}

//$RREF(REFERENCE,STRING)
bool IntepretRRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::RREF << ", op1=" << symbol);

	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	Symbols result;
	result.type = REFERENCE_SYMBOL;
	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		result.reference = variable_table_stack->Lookup(*(*(symbol.s)));
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(data_stack.size() >= 1);
		Symbols index = data_stack.back();
		data_stack.pop_back();
		assert(index.type == STRING_SYMBOL);
		result.reference = Index(symbol.reference, **index.s, false);
	}
	data_stack.push_back(result);

	INFO("command=" << Symbols::RREF << ", result=" << result);
	
	return true;	
}

//$DEF(STRING, ANY_SYMBOL)
bool IntepretDefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols value = data_stack.back();
	data_stack.pop_back();
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::DEF << ", symbol=" << symbol << ", value=" << value);

	assert(symbol.type == STRING_SYMBOL);

	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());
	Symbols* reference_symbol = variable_table_stack->Register(*(*(symbol.s)));
	*reference_symbol = value;
	
	return true;
}

//$SET(STRING|REFERENCE, ANY_SYMBOL)
bool IntepretSetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols value = data_stack.back();
	data_stack.pop_back();
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::SET << ", symbol=" << symbol << ", value=" << value);

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
bool IntepretGetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::GET << ", symbol=" << symbol);

	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->Lookup(*(*(symbol.s)));
		assert(reference_symbol != NULL);
		data_stack.push_back(*reference_symbol);
		INFO("command=" << Symbols::GET << ", result=" << *reference_symbol);	
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(symbol.reference != NULL);
		data_stack.push_back(*symbol.reference);
		INFO("command=" << Symbols::GET << ", result=" << *symbol.reference);
	}

	return true;
}

//$PUSHDATA()
//($PUSHDATA)
bool IntepretPushDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	variable_table_stack->Push();

	INFO("command=" << Symbols::PUSH_DATA << ", variable_table_stack size=" << variable_table_stack->size());

	return true;
}

//$POPDATA()
//($POPDATA)
bool IntepretPopDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());
	variable_table_stack->Pop();

	INFO("command=" << Symbols::POP_DATA << ", variable_table_stack size=" << variable_table_stack->size());

	return true;
}

//$PUSH
bool IntepretPushCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	vector<Symbols>*parameter_stack = Singleton<vector<Symbols> >::GetInstance();
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::PUSH << ", symbol=" << symbol);

	parameter_stack->push_back(symbol);

	return true;
}

//$POP(STRING_SYMBOL|REFERENCE)
bool IntepretPopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	vector<Symbols>*parameter_stack = Singleton<vector<Symbols> >::GetInstance();
	assert(!parameter_stack->empty());

	assert(data_stack.size() >= 1);
	Symbols symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::POP << ", symbol=" << symbol);

	assert(symbol.type == STRING_SYMBOL || symbol.type == REFERENCE_SYMBOL);

	if (symbol.type == STRING_SYMBOL)
	{
		VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
		assert(!variable_table_stack->empty());
		Symbols* reference_symbol = variable_table_stack->LookupOrRegister(*(*(symbol.s)));
		*reference_symbol = parameter_stack->back();
		parameter_stack->pop_back();
		INFO("command=" << Symbols::POP << ", value=" << *reference_symbol);	
	}
	else if (symbol.type == REFERENCE_SYMBOL)
	{
		assert(symbol.reference != NULL);
		*symbol.reference = parameter_stack->back();
		parameter_stack->pop_back();
		INFO("command=" << Symbols::POP << ", value=" << *symbol.reference);
	}
	
	return true;
}

//$ARRAY()
//($ARRAY)
bool IntepretArrayCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	Symbols symbol(LIST_SYMBOL);
	data_stack.push_back(symbol);

	INFO("command=" << Symbols::ARRAY << ", result=" << symbol);

	return true;
}

//$MAP()
//($MAP)
bool IntepretMapCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	Symbols symbol(MAP_SYMBOL);
	data_stack.push_back(symbol);

	INFO("command=" << Symbols::MAP << ", result=" << symbol);

	return true;
}

//$CAST(REMARK_SYMBOL,SYMBOL)
bool IntepretCastCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols value = data_stack.back();
	data_stack.pop_back();
	Symbols type = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::CAST << ", type=" << type << ", value=" << value);

	assert(type.IsRemark());

	//TODO: CastTo: COMPACT_SYMBOL => INTEGER|LONG|FLOAT|DOUBLE
	Symbols result = CastTo(type, value);	
	data_stack.push_back(result);

	INFO("command=" << Symbols::CAST << ", result=" << result);

	return true;
}

//($ADD, SYMBOLS, SYMBOLS) = SYMBOLS + SYMBOLS
bool IntepretAddCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::ADD << ", n1=" << n1 << ", n2=" << n2);

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

	INFO("command=" << Symbols::ADD << ", result=" << n1);

	return true;
}

//($SUB, SYMBOLS, SYMBOLS) = SYMBOLS - SYMBOLS
bool IntepretSubCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::SUB << ", n1=" << n1 << ", n2=" << n2);

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

	INFO("command=" << Symbols::SUB << ", result=" << n1);

	return true;
}

//($MUL, SYMBOLS, SYMBOLS) = SYMBOLS * SYMBOLS
bool IntepretMulCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::MUL << ", n1=" << n1 << ", n2=" << n2);

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

	INFO("command=" << Symbols::MUL << ", result=" << n1);

	return true;
}

//($DIV, SYMBOLS, SYMBOLS) = SYMBOLS / SYMBOLS
bool IntepretDivCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::DIV << ", n1=" << n1 << ", n2=" << n2);

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

	INFO("command=" << Symbols::DIV << ", result=" << n1);

	return true;
}

//($EQ, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
bool IntepretEqCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::EQ << ", n1=" << n1 << ", n2=" << n2);

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
	Symbols result = Equal(n1, n2) ? Symbols::TRUE : Symbols::FALSE;
	data_stack.push_back(result);

	INFO("command=" << Symbols::EQ << ", result=" << result);

	return true;
}

//($LT, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
bool IntepretLtCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols n2 = data_stack.back();
	data_stack.pop_back();
	Symbols n1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::LT << ", n1=" << n1 << ", n2=" << n2);

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
	Symbols result = LessThan(n1, n2) ? Symbols::TRUE : Symbols::FALSE;
	data_stack.push_back(result);

	INFO("command=" << Symbols::LT << ", result=" << result);

	return true;
}

//$AND(TRUE|FALSE, TRUE|FALSE)
bool IntepretAndCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols b2 = data_stack.back();
	data_stack.pop_back();
	Symbols b1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::AND << ", b1=" << b1 << ", b2=" << b2);

	assert(b1 == Symbols::TRUE || b1 == Symbols::FALSE);
	assert(b2 == Symbols::TRUE || b2 == Symbols::FALSE);

	Symbols result = (b1 == Symbols::TRUE && b2 == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE;
	data_stack.push_back(result);

	INFO("command=" << Symbols::AND << ", result=" << result);
	
	return true;
}

//$OR(TRUE|FALSE, TRUE|FALSE)
bool IntepretOrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 2);
	Symbols b2 = data_stack.back();
	data_stack.pop_back();
	Symbols b1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::OR << ", b1=" << b1 << ", b2=" << b2);

	assert(b1 == Symbols::TRUE || b1 == Symbols::FALSE);
	assert(b2 == Symbols::TRUE || b2 == Symbols::FALSE);

	Symbols result = (b1 == Symbols::TRUE || b2 == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE;
	data_stack.push_back(result);

	INFO("command=" << Symbols::OR << ", result=" << result);
	
	return true;
}

//$NOT(TRUE|FALSE, TRUE|FALSE)
bool IntepretNotCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols b1 = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::NOT << ", b1=" << b1);

	assert(b1 == Symbols::TRUE || b1 == Symbols::FALSE);

	Symbols result = (b1 == Symbols::FALSE) ? Symbols::TRUE : Symbols::FALSE;
	data_stack.push_back(result);

	INFO("command=" << Symbols::NOT << ", result=" << result);
	
	return true;
}

//$SHELL(STRING_SYMBOL) => STRING_SYMBOL
bool IntepretShellCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols input_symbol = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::SHELL << ", input_symbol=" << input_symbol);

	assert(input_symbol.type == STRING_SYMBOL || input_symbol.type == COMPACT_SYMBOL);

	string input = input_symbol.ToString();
	FILE* fp = popen(input.c_str(),"r");
	if(fp==NULL)
	{
		ERROR("popen failed");
		return false;
	}
	string output;
	char c;
	while((c=fgetc(fp))!=EOF)
	{
		output.push_back(c);
	}
	if(pclose(fp)!=0)
	{
		ERROR("pclose failed");
		return false;
	}
	Symbols result = Symbols(STRING_SYMBOL, output.c_str());
	data_stack.push_back(result);

	INFO("command=" << Symbols::SHELL << ", result=" << result);

	return true;
}

//$CAT(STRING_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCatCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 1);
	Symbols s = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::CAT << ", file_name=" << s);

	assert(s.type == STRING_SYMBOL);

	//($CAT, "<file_name>")
	string file_name = **s.s;
	ifstream input_file(file_name.c_str());
	if(input_file.fail()) 
	{
		ERROR("open file_name" << file_name << " failed!!!");
		return false;
	}
	vector<Symbols> result;
	input_file>>result;
	data_stack.push_back(Encode(result,true));

	INFO("command=" << Symbols::CAT << ", result="<<result);

	return true;
}

//$SUBSTR(STRING) => STRING
inline bool IntepretSubStrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(data_stack.size() >= 3);
	Symbols len = data_stack.back();
	data_stack.pop_back();
	Symbols from = data_stack.back();
	data_stack.pop_back();
	Symbols str = data_stack.back();
	data_stack.pop_back();

	INFO("command=" << Symbols::SUB_STR << ", str=" << str << ", from=" << from << ", len=" << len);

	assert(len.type == INTEGER_SYMBOL);
	assert(from.type == INTEGER_SYMBOL);
	assert(str.type == STRING_SYMBOL);

	**str.s = (*str.s)->substr(from.i, len.i);
	data_stack.push_back(str);

	INFO("command=" << Symbols::SUB_STR << ", result="<<str);

	return true;
}

bool IntepretCommand(const Symbols& command, std::vector<Symbols>& data_stack, WllIntepreter* intepreter)
{
	assert(command.IsRemark());

	if (command == Symbols::LOAD_TRANSLATIONS)
	{
		return IntepretLoadTranslationsCommand(data_stack, intepreter);
	}
	else if (command == Symbols::ADD_TRANSLATIONS)
	{
		return IntepretAddTranslationsCommand(data_stack, intepreter);
	}
	else if (command == Symbols::REMARK_WLL0)
	{
		return IntepretWll0Command(data_stack, intepreter);
	}	
	else if (command == Symbols::REMARK_TRANSLATION)
	{
		return IntepretTranslationCommand(data_stack, intepreter);
	}
	else if (command == Symbols::REMARK_RULE)
	{
		return IntepretRuleCommand(data_stack, intepreter);
	}
	else if (command == Symbols::REMARK_VARIABLE)
	{
		return IntepretVariableCommand(data_stack, intepreter);
	}
	else if (command == Symbols::REMARK_CONSTANT)
	{
		return IntepretConstantCommand(data_stack, intepreter);
	}
	else if (command == Symbols::REMARK_REMARK)
	{
		return IntepretRemarkCommand(data_stack, intepreter);
	}
	else if (command == Symbols::EVAL)
	{
		return IntepretEvalCommand(data_stack, intepreter);
	}
	else if (command == Symbols::EXEC)
	{
		return IntepretExecCommand(data_stack, intepreter);
	}
	else if (command == Symbols::REMARK_IGNORE)
	{
		return IntepretIgnoreCommand(data_stack, intepreter);
	}
	else if (command == Symbols::COMPACT)
	{
		return IntepretCompactCommand(data_stack, intepreter);
	}
	else if (command == Symbols::LIST)
	{
		return IntepretListCommand(data_stack, intepreter);
	}
	else if (command == Symbols::APPEND)
	{
		return IntepretAppendCommand(data_stack, intepreter);
	}
	else if (command == Symbols::CAR)
	{
		return IntepretCarCommand(data_stack, intepreter);
	}
	else if (command == Symbols::CDR)
	{
		return IntepretCdrCommand(data_stack, intepreter);
	}
	else if (command == Symbols::COND)
	{
		return IntepretCondCommand(data_stack, intepreter);
	}
	else if (command == Symbols::LOOP)
	{
		return IntepretLoopCommand(data_stack, intepreter);
	}
	else if (command == Symbols::CALL)
	{
		return IntepretCallCommand(data_stack, intepreter);
	}
	else if (command == Symbols::FCALL)
	{
		return IntepretFCallCommand(data_stack, intepreter);
	}
	else if (command == Symbols::LREF)
	{
		return IntepretLRefCommand(data_stack, intepreter);
	}
	else if (command == Symbols::RREF)
	{
		return IntepretRRefCommand(data_stack, intepreter);
	}
	else if (command == Symbols::DEF)
	{
		return IntepretDefCommand(data_stack, intepreter);
	}
	else if (command == Symbols::SET)
	{
		return IntepretSetCommand(data_stack, intepreter);
	}
	else if (command == Symbols::GET)
	{
		return IntepretGetCommand(data_stack, intepreter);
	}
	else if (command == Symbols::PUSH_DATA)
	{
		return IntepretPushDataCommand(data_stack, intepreter);
	}
	else if (command == Symbols::POP_DATA)
	{
		return IntepretPopDataCommand(data_stack, intepreter);
	}
	else if (command == Symbols::PUSH)
	{
		return IntepretPushCommand(data_stack, intepreter);
	}
	else if (command == Symbols::POP)
	{
		return IntepretPopCommand(data_stack, intepreter);
	}
	else if (command == Symbols::ARRAY)
	{
		return IntepretArrayCommand(data_stack, intepreter);
	}
	else if (command == Symbols::MAP)
	{
		return IntepretMapCommand(data_stack, intepreter);
	}
	else if (command == Symbols::CAST)
	{
		return IntepretCastCommand(data_stack, intepreter);
	}
	else if (command == Symbols::ADD)
	{
		return IntepretAddCommand(data_stack, intepreter);
	}
	else if (command == Symbols::SUB)
	{
		return IntepretSubCommand(data_stack, intepreter);
	}
	else if (command == Symbols::MUL)
	{
		return IntepretMulCommand(data_stack, intepreter);
	}
	else if (command == Symbols::DIV)
	{
		return IntepretDivCommand(data_stack, intepreter);
	}
	else if (command == Symbols::EQ)
	{
		return IntepretEqCommand(data_stack, intepreter);
	}
	else if (command == Symbols::LT)
	{
		return IntepretLtCommand(data_stack, intepreter);
	}
	else if (command == Symbols::AND)
	{
		return IntepretAndCommand(data_stack, intepreter);
	}
	else if (command == Symbols::OR)
	{
		return IntepretOrCommand(data_stack, intepreter);
	}
	else if (command == Symbols::NOT)
	{
		return IntepretNotCommand(data_stack, intepreter);
	}
	else if (command == Symbols::SHELL)
	{
		return IntepretShellCommand(data_stack, intepreter);
	}
	else if (command == Symbols::CAT)
	{
		return IntepretCatCommand(data_stack, intepreter);
	}
	else if (command == Symbols::SUB_STR)
	{
		return IntepretSubStrCommand(data_stack, intepreter);
	}

}


