/*
 * WllCommand.cpp
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#include "WllCommand.h"
//#include "ValueType.h"
#include "VariableStack.h"
#include "Wll0Loader.h"
#include "Wll1Loader.h"
#include "Wll1Intepreter.h"
#include "WllTrace.h"
#include "WllSingleton.h"
#include "WllString.h"
#include "LanguageAlgorithm.h"
//#include "VariableContainer.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <cassert>
#include "CompilerManager.h"
#include "IntepretException.h"
#include "Calculate.h"
using namespace std;
using namespace Wll::Util;

WllCommand::WllCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* wll_intepreter)
: command(cmd), parameters(parameter_fields), intepreter(wll_intepreter)
{

}

//($LOAD_TRANSLATION, SYMBOLS)
LoadTranslationsCommand::LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//load SYMBOLS by WllLoader to replace current languages
bool LoadTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return LoadLanguage(this->parameters[1], this->intepreter->compiler->languages, false);
}

//($ADD_TRANSLATIONS, SYMBOLS)
AddTranslationsCommand::AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//load SYMBOLS by WllLoader append into current languages
bool AddTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return LoadLanguage(this->parameters[1], this->intepreter->compiler->languages, true);
}

//($WLL0, SYMBOLS)
Wll0Command::Wll0Command(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//load SYMBOLS of TRANSLATION(MAP_SYMBOL) to replace current languages
bool Wll0Command::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);

	this->intepreter->compiler->languages.translation_rules.clear();
	for(vector<Symbols>::const_iterator i = this->parameters[1].begin(); i != this->parameters[1].end(); ++i)
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
		this->intepreter->compiler->languages.translation_rules.push_back(translation);
	}
		
	this->intepreter->compiler->languages.Initialize();
	return true;
}

//($TRANSLATION, MAP_SYMBOL, MAP_SYMBOL)
TranslationCommand::TranslationCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//compose source_rule(MAP_SYMBOL) and destination_rule(MAP_SYMBOL) as translation(MAP_SYMBOL)
bool TranslationCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==3);
	assert(this->parameters[1].size() == 1);
	assert(this->parameters[2].size() == 1);
	assert(this->parameters[1][0].type == MAP_SYMBOL);
	assert(this->parameters[2][0].type == MAP_SYMBOL);

	Symbols translation(MAP_SYMBOL);
	translation.GetMap()["source_rule"] = this->parameters[1][0];
	translation.GetMap()["destination_rule"] = this->parameters[2][0];
	result.push_back(translation);
	
	return true;
}

//($RULE, VARIABLE_SYMBOL, SYMBOLS)
RuleCommand::RuleCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//compose root_symbol(VARIABLE_SYMBOL) and expression(SYMBOLS) as rule(MAP_SYMBOL)
bool RuleCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==3);
	assert(this->parameters[1].size()==1);
	assert(this->parameters[1][0].IsVariable());

	Symbols rule(MAP_SYMBOL);
	rule.GetMap()["root"] = this->parameters[1][0];
	rule.GetMap()["expression"] = Symbols(LIST_SYMBOL, this->parameters[2]);
	result.push_back(rule);

	return true;
}

//($VARIABLE, SYMBOLS)
VariableCommand::VariableCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//compose constants as name of a variable(VARAIBLE_SYMBOL) 
bool VariableCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	string variable_name;
	ToString(variable_name, this->parameters[1]);
	Symbols variable(variable_name.c_str());
	result.push_back(variable);
	return true;
}

//($CONSTANT, SYMBOLS)
ConstantCommand::ConstantCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//keep input symbols(SYMBOLS) as constants
bool ConstantCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	for(vector<Symbols>::const_iterator i = this->parameters[1].begin(); i != this->parameters[1].end(); ++i)
	{
		assert(i->IsConstant());
	}
	result += this->parameters[1];

	return true;
}

RemarkCommand::RemarkCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool RemarkCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	string remark_name;
	ToString(remark_name, this->parameters[1]);
	Symbols remark(REMARK_SYMBOL, remark_name.c_str());
	result.push_back(remark);
	return true;
}

EvalCommand::EvalCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool EvalCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return this->intepreter->IntepretWll(this->parameters[1], result);
}

ExecCommand::ExecCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool ExecCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return this->intepreter->IntepretWll(this->parameters[1], result);
}

IgnoreCommand::IgnoreCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool IgnoreCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	for(vector<Symbols>::const_iterator i = this->parameters[1].begin(); i != this->parameters[1].end(); ++i)
	{
		result.push_back(*i);
	}
	return true;
}

ListCommand::ListCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool ListCommand::Intepret(std::vector<Symbols>& result)
{
	ComposeSList(this->parameters.begin()+1, this->parameters.end(), result);

	return true;
}

AppendCommand::AppendCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool AppendCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==3);
	vector<vector<Symbols> > fields1,fields2;
	SplitSList(this->parameters[1], fields1);
	SplitSList(this->parameters[2], fields2);
	insert_iterator<vector<vector<Symbols> > > insert_it(fields1,fields1.end());
	copy(fields2.begin(), fields2.end(), insert_it);
	ComposeSList(fields1.begin(), fields1.end(), result);

	return true;
}

CarCommand::CarCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool CarCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	vector<Symbols>& e = this->parameters[1];
	if(e.empty()) return true;

	if(!(e.front()==Symbols::LEFT_QUOTE && e.back()==Symbols::RIGHT_QUOTE)) return true;

	vector<vector<Symbols> > fields;
	SplitParameters(e.begin()+1, e.end()-1, fields);

	if(!fields.empty())
	{
		result += fields.front();
	}

	return true;
}

CdrCommand::CdrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool CdrCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	vector<Symbols>& e = this->parameters[1];
	if(e.empty()) return true;

	if(!(e.front()==Symbols::LEFT_QUOTE && e.back()==Symbols::RIGHT_QUOTE)) return true;

	vector<vector<Symbols> > fields;
	SplitParameters(e.begin()+1, e.end()-1, fields);

	if(fields.size()>1)
	{
		ComposeSList(fields.begin()+1, fields.end(), result);
	}

	return true;
}

AddCommand::AddCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool AddCommand::Intepret(std::vector<Symbols>& result)
{
	Symbols sum(CHAR_SYMBOL, char(0));
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		Symbols n = CastTo(*i, true);
		AddTo(sum, n);
	}

	result.push_back(sum);
	return true;
}

SubCommand::SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool SubCommand::Intepret(std::vector<Symbols>& result)
{
	Symbols sum(CHAR_SYMBOL, char(0));
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		Symbols n = CastTo(*i, true);
		
		if (i == parameters.begin()+1)
		{
			sum = n;
		}
		else
		{
			sum -= n;
		}
	}

	result.push_back(sum);
	return true;
}

MulCommand::MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool MulCommand::Intepret(std::vector<Symbols>& result)
{
	Symbols sum(CHAR_SYMBOL, char(1));
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		Symbols n = CastTo(*i, true);
		sum *= n;
	}

	result.push_back(sum);
	return true;
}

DivCommand::DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool DivCommand::Intepret(std::vector<Symbols>& result)
{
	Symbols sum(CHAR_SYMBOL, char(0));
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		Symbols n = CastTo(*i, true);
		
		if (i == parameters.begin()+1)
		{
			sum = n;
		}
		else
		{
			sum /= n;
		}
	}

	result.push_back(sum);
	return true;
}

SubStrCommand::SubStrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
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

CondCommand::CondCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool CondCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()%2==1);
	for(int i=1; i<this->parameters.size(); i+=2)
	{
		if(this->parameters[i][0] == Symbols::TRUE)
		{
			this->intepreter->IntepretWll(this->parameters[i+1], result);
			break;
		}
	}
	return true;
}

LoopCommand::LoopCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

//($LOOP <condition> <expression>)
bool LoopCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==3);
	do
	{
		std::vector<Symbols> condition_result;
		INFO("Intepret LoopCommand condition part: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
		if(!this->intepreter->IntepretWll(this->parameters[1],condition_result))
		{
			TERM_ERROR("Intepret LoopCommand condition failed: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
			return false;
		}

		INFO("condition_result="<<condition_result);
		if(condition_result[0] == Symbols::FALSE) break;

		INFO("Intepret LoopCommand expression part: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
		if(!this->intepreter->IntepretWll(this->parameters[2], result))
		{
			TERM_ERROR("Intepret LoopCommand expression failed: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
			return false;
		}
	}while(true);

	return true;
}

EqCommand::EqCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{
}

bool EqCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1] == this->parameters[2]) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

LtCommand::LtCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{
}

bool LtCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	Symbols a = CastTo(this->parameters[1]);
	Symbols b = CastTo(this->parameters[2]);
	
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
		|| a.type == STRING_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
		|| b.type == STRING_SYMBOL
	);
	
	Symbols c(b);
	if (a.type < c.type)
	{
		a = CastTo(c.type, a);
	}
	else if (a.type > c.type)
	{
		c = CastTo(a.type, c);
	}	
	
	result.push_back((a < c) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

AndCommand::AndCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{
}

bool AndCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1][0] == Symbols::TRUE && this->parameters[2][0] == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

OrCommand::OrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{
}

bool OrCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1][0] == Symbols::TRUE || this->parameters[2][0] == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}


NotCommand::NotCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{
}

bool NotCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 2);
	result.push_back((this->parameters[1][0] == Symbols::FALSE) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

ShellCommand::ShellCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
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

Symbols* Index(Symbols* symbol, vector< vector<Symbols> >&parameters, int from, int to,bool write_flag=false)
{
	int i = from;
	while(i < to)
	{
		string index;
		ToString(index,parameters[i]);
		
		if(symbol->type == LIST_SYMBOL || symbol->type == STRING_SYMBOL)
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
		
		i++;
	}
	return symbol;
} 

DefCommand::DefCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool DefCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() >= 3);
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());

	string variable_name;
	ToString(variable_name, this->parameters[1]);

	Symbols* symbol = variable_table_stack->Register(variable_name);
	symbol = Index(symbol, this->parameters, 2, this->parameters.size()-1,true);
	*symbol = Encode(this->parameters.back());
	
	return true;
}

SetCommand::SetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool SetCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() >= 3);
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());

	string variable_name;
	ToString(variable_name, this->parameters[1]);

	Symbols* symbol = variable_table_stack->LookupOrRegister(variable_name);
	symbol = Index(symbol, this->parameters, 2, this->parameters.size()-1,true);
	*symbol = Encode(this->parameters.back());
	
	return true;
}

GetCommand::GetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool GetCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() >= 2);
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());

	string variable_name;
	ToString(variable_name, this->parameters[1]);
	Symbols* symbol = variable_table_stack->Lookup(variable_name);
	symbol = Index(symbol, this->parameters, 2, this->parameters.size());
	
	if(symbol != NULL)
	{
		result += Decode(*symbol);
	}
	else
	{
		INFO("variable_name["<<variable_name<<"] not find in variable table stack, return empty");
	}
	return true;
}

PushDataCommand::PushDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PushDataCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 1);
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	variable_table_stack->Push();
	return true;
}

PopDataCommand::PopDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PopDataCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 1);
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	if(variable_table_stack->empty())		return false;
	variable_table_stack->Pop();
	return true;
}


PushCommand::PushCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PushCommand::Intepret(std::vector<Symbols>& result)
{
	vector<Symbols>*parameter_stack = Singleton<vector<Symbols> >::GetInstance();
	for(int i=1; i<this->parameters.size(); i++)
	{
		parameter_stack->push_back(Encode(this->parameters[i]));
	}
	return true;
}


PopCommand::PopCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PopCommand::Intepret(std::vector<Symbols>& result)
{
	vector<Symbols>*parameter_stack = Singleton<vector<Symbols> >::GetInstance();
	VariableStack* variable_table_stack = Singleton<VariableStack>::GetInstance();
	assert(!variable_table_stack->empty());
	for(int i = this->parameters.size()-1; i != 0; --i)
	{
		if(!parameter_stack->empty())
		{
			string variable_name;
			ToString(variable_name, this->parameters[i]);

			Symbols* symbol = variable_table_stack->LookupOrRegister(variable_name);
			*symbol = parameter_stack->back();
			
			parameter_stack->pop_back();			
		}
		else
		{
			ERROR("PopCommand Intepret error: pop when parameter_stack is empty");
			return false;
		}
	}
	return true;
}

CallCommand::CallCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool CallCommand::Intepret(std::vector<Symbols>& result)
{

	assert(this->parameters.size() == 3 || this->parameters.size() == 4);
	if(this->parameters.size() == 3)
	{
		//($CALL, "<function_name>", "<parameter>")
		string start_symbol;
		ToString(start_symbol, this->parameters[1]);
		return this->intepreter->compiler->Process(this->parameters[2], result, Symbols(start_symbol.c_str()));
	}
	else if(this->parameters.size() == 4)
	{
		//($CALL, "<file_name>", "<function_name>", "<parameter>")
		string grammar_file_name;
		ToString(grammar_file_name, this->parameters[1]);
		string start_symbol;
		ToString(start_symbol, this->parameters[2]);
		//#bug: 跨文件Call应该使用不同的编译环境
		//<update>
		//return this->intepreter->compiler->Process(grammar_file_name, this->parameters[3], result, Symbols(start_symbol.c_str()));
		//<as>
        Compiler* compiler = Singleton<CompilerManager>().GetInstance()->CreateOrGetCompiler(grammar_file_name);
		return compiler->Process(this->parameters[3], result, Symbols(start_symbol.c_str()));
		//</update>
	}
	return false;
}

CatCommand::CatCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
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

ArrayCommand::ArrayCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool ArrayCommand::Intepret(std::vector<Symbols>& result)
{

	assert(this->parameters.size() == 1 || this->parameters.size() == 2);
	
	Symbols symbol(LIST_SYMBOL);
	//($ARRAY)
	if(this->parameters.size() == 2)
	{
		//($ARRAY,<symbols>)
		symbol.GetList() = this->parameters[1];
	}
	result.push_back(symbol);
	return true;
}

MapCommand::MapCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool MapCommand::Intepret(std::vector<Symbols>& result)
{

	assert(this->parameters.size() == 1);
	//($MAP)
	result.push_back(Symbols(MAP_SYMBOL));
	return true;
}


CastCommand::CastCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool CastCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);	
	result.push_back(CastTo(this->parameters[1][0], this->parameters[2]));
	return true;
}

WllCommand* WllCommandFactory::CreateCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
{
	INFO("cmd="<<cmd);
	INFO("parameters=");
	for(int  i = 0; i<parameter_fields.size(); ++i)
	{
		INFO("parameter["<<i<<"]="<<parameter_fields[i]);
	}

	WllCommand* command = NULL;
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


