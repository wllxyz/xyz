/*
 * WllCommand.cpp
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#include "WllCommand.h"
#include "ValueType.h"
#include "VariableTable.h"
#include "Wll0Loader.h"
#include "Wll1Loader.h"
#include "Wll1Intepreter.h"
#include "WllTrace.h"
#include "WllSingleton.h"
#include "LanguageAlgorithm.h"
#include "VariableContainer.h"
#include <iostream>
#include <fstream>
using namespace std;

WllCommand::WllCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* wll_intepreter)
: command(cmd), parameters(parameter_fields), intepreter(wll_intepreter)
{

}

Wll0Command::Wll0Command(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool Wll0Command::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	VariableContainer* container = Singleton<VariableContainer>::GetInstance();
	this->intepreter->compiler->languages.translation_rules = container->translations;
	this->intepreter->compiler->languages.Initialize();
	container->translations.clear();

	return true;
}

TranslationCommand::TranslationCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool TranslationCommand::Intepret(std::vector<Symbols>& result)
{
	//($TRANSLATION, ($RULE, ($VARIABLE, $EXPRESSION)), ($RULE, ($VARIABLE, $EXPRESSION)))
	assert(this->parameters.size()==3);
	VariableContainer* container = Singleton<VariableContainer>::GetInstance();
	container->translation.source_rule = container->source_rule;
	container->translation.destination_rule = container->destination_rule;
	container->translations.push_back(container->translation);
	return true;
}

SourceRuleCommand::SourceRuleCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool SourceRuleCommand::Intepret(std::vector<Symbols>& result)
{
	//($RULE, ($VARIABLE, $EXPRESSION))
	assert(this->parameters.size()==2);
	VariableContainer* container = Singleton<VariableContainer>::GetInstance();
	container->source_rule = container->rule;

	return true;
}

DestinationRuleCommand::DestinationRuleCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool DestinationRuleCommand::Intepret(std::vector<Symbols>& result)
{
	//($RULE, ($VARIABLE, $EXPRESSION))
	assert(this->parameters.size()==2);
	VariableContainer* container = Singleton<VariableContainer>::GetInstance();
	container->destination_rule = container->rule;

	return true;
}

RuleCommand::RuleCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool RuleCommand::Intepret(std::vector<Symbols>& result)
{
	//($RULE, ($VARIABLE, $EXPRESSION))
	assert(this->parameters.size()==3);
	VariableContainer* container = Singleton<VariableContainer>::GetInstance();
	container->rule.symbol = this->parameters[1][0];
	container->rule.expression = this->parameters[2];

	return true;
}

VariableCommand::VariableCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool VariableCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	string variable_name;
	ToString(variable_name, this->parameters[1]);
	Symbols variable(variable_name.c_str());
	result.push_back(variable);
	return true;
}

ConstantCommand::ConstantCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool ConstantCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	for(vector<Symbols>::const_iterator i = this->parameters[1].begin(); i != this->parameters[1].end(); ++i)
	{
		result.push_back(*i);
	}

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
	vector<vector<Symbols> > fields;
	SplitSList(this->parameters[1], fields);
	fields.push_back(this->parameters[2]);
	ComposeSList(fields.begin(), fields.end(), result);

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
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		string s;
		ToString(s, *i);
		Integer n(s);
		sum += n;
	}

	for(string::const_iterator i = sum.value.begin(); i != sum.value.end(); ++i)
	{
		result.push_back(Symbols(*i));
	}
	return true;
}

SubCommand::SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool SubCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		string s;
		ToString(s, *i);
		Integer n(s);

		if(i==parameters.begin()+1)
		{
			sum = n;
		}
		else
		{
			sum -= n;
		}
	}

	for(string::const_iterator i = sum.value.begin(); i != sum.value.end(); ++i)
	{
		result.push_back(Symbols(*i));
	}
	return true;
}

MulCommand::MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool MulCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		string s;
		ToString(s, *i);
		Integer n(s);

		if(i==parameters.begin()+1)
		{
			sum = n;
		}
		else
		{
			sum *= n;
		}
	}

	for(string::const_iterator i = sum.value.begin(); i != sum.value.end(); ++i)
	{
		result.push_back(Symbols(*i));
	}
	return true;
}

DivCommand::DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool DivCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		string s;
		ToString(s, *i);
		Integer n(s);

		if(i==parameters.begin()+1)
		{
			sum = n;
		}
		else
		{
			sum /= n;
		}
	}

	for(string::const_iterator i = sum.value.begin(); i != sum.value.end(); ++i)
	{
		result.push_back(Symbols(*i));
	}
	return true;
}

LoadTranslationsCommand::LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool LoadTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return LoadLanguage(this->parameters[1], this->intepreter->compiler->languages, false);
}

AddTranslationsCommand::AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool AddTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return LoadLanguage(this->parameters[1], this->intepreter->compiler->languages, true);
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
	result.push_back((this->parameters[1] < this->parameters[2]) ? Symbols::TRUE : Symbols::FALSE);
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

DefCommand::DefCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool DefCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	assert(!variable_table_stack->empty());

	string variable_name;
	ToString(variable_name, this->parameters[1]);

	VariableTable& variable_table = variable_table_stack->back();
	variable_table[variable_name] = this->parameters[2];
	return true;
}

SetCommand::SetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool SetCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	assert(!variable_table_stack->empty());

	string variable_name;
	ToString(variable_name, this->parameters[1]);
	for(vector<VariableTable>::reverse_iterator i = variable_table_stack->rbegin(); i != variable_table_stack->rend(); ++i)
	{
		if(i->Has(variable_name.substr(0,variable_name.find('.'))))
		{
			((*i)[variable_name]) = this->parameters[2];
			return true;
		}
	}

	VariableTable& variable_table = variable_table_stack->back();
	variable_table[variable_name] = this->parameters[2];
	return true;
}

GetCommand::GetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool GetCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 2);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	assert(!variable_table_stack->empty());

	string variable_name;
	ToString(variable_name, this->parameters[1]);
	for(vector<VariableTable>::reverse_iterator i = variable_table_stack->rbegin(); i != variable_table_stack->rend(); ++i)
	{
		if(i->Has(variable_name))
		{
			result += (((*i)[variable_name]).symbols);
			INFO("value="<<(((*i)[variable_name])));
			return true;
		}
	}

	INFO("variable_name["<<variable_name<<"] not find in variable table stack, return empty");
	return true;
}

PushDataCommand::PushDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PushDataCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 1);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	variable_table_stack->push_back(VariableTable());
	return true;
}

PopDataCommand::PopDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PopDataCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 1);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	if(variable_table_stack->empty())		return false;
	variable_table_stack->pop_back();
	return true;
}


PushCommand::PushCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PushCommand::Intepret(std::vector<Symbols>& result)
{
	vector<vector<Symbols> >*parameter_stack = Singleton<vector<vector<Symbols> > >::GetInstance();
	for(int i=1; i<this->parameters.size(); i++)
	{
		parameter_stack->push_back(this->parameters[i]);
	}
	return true;
}


PopCommand::PopCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool PopCommand::Intepret(std::vector<Symbols>& result)
{
	vector<vector<Symbols> >*parameter_stack = Singleton<vector<vector<Symbols> > >::GetInstance();
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	assert(!variable_table_stack->empty());
	VariableTable& variable_table = variable_table_stack->back();
	for(int i = this->parameters.size()-1; i != 0; --i)
	{
		if(!parameter_stack->empty())
		{
			VariableNode::KeyType variable_name;
			ToString(variable_name, this->parameters[i]);

			bool found = false;
			for(vector<VariableTable>::reverse_iterator i = variable_table_stack->rbegin(); i != variable_table_stack->rend(); ++i)
			{
				if(i->Has(variable_name.substr(0,variable_name.find('.'))))
				{
					((*i)[variable_name]) = parameter_stack->back();
					parameter_stack->pop_back();
					found = true;
					break;
				}
			}

			if(!found)
			{
				VariableTable& variable_table = variable_table_stack->back();
				variable_table[variable_name] = parameter_stack->back();
				parameter_stack->pop_back();
			}
		}
		else
		{
			ERROR("PopCommand Intepret error: pop when parameter_stack is empty");
			return false;
		}
	}
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
		Compiler compiler;
		ifstream input_grammar(grammar_file_name.c_str());
		if(!input_grammar)
		{
			ERROR("open gramar file["<<grammar_file_name<<"] failed");
			return false;
		}
		if(!compiler.Process(input_grammar, cout))
		{
			ERROR("process grammar_file_name["<<grammar_file_name<<"] failed");
			return false;
		}
		return compiler.Process(this->parameters[3], result, Symbols(start_symbol.c_str()));
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
	else if(cmd == Symbols::REMARK_SOURCE_RULE)
	{
		command = new SourceRuleCommand(cmd,parameter_fields,intepreter);
	}
	else if(cmd == Symbols::REMARK_DESTINATION_RULE)
	{
		command = new DestinationRuleCommand(cmd,parameter_fields,intepreter);
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
	else if(cmd == Symbols::COND)
	{
		command = new CondCommand(cmd,parameter_fields,intepreter);
	}//COND
	else if(cmd == Symbols::LOOP)
	{
		command = new LoopCommand(cmd,parameter_fields,intepreter);
	}//LOOP
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
	else if(cmd == Symbols::CAT)
	{
		command = new CatCommand(cmd, parameter_fields, intepreter);
	}

	return command;
}


