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
using namespace std;

WllCommand::WllCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* wll_intepreter, std::vector<LanguageTranslations>* language_translations)
: command(cmd), parameters(parameter_fields), intepreter(wll_intepreter), translations(language_translations)
{

}

AddCommand::AddCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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

SubCommand::SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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

MulCommand::MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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

DivCommand::DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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

LoadTranslationsCommand::LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool LoadTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	this->translations->clear();
	if( !this->parameters[1].empty() && this->parameters[1].front()==Symbols::REMARK_WLL0 )
	{
		INFO("wll0loader="<<this->parameters[1]);
		Wll0Loader loader(this->parameters[1]);
		if(!loader.LoadWll(*(this->translations)))
		{
			loader.ShowErrorMessage();
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		INFO("wll1loader="<<this->parameters[1]);
		Wll1Loader loader(this->parameters[1]);
		if(!loader.LoadWll0(*(this->translations)))
		{
			loader.ShowErrorMessage();
			return false;
		}
		else
		{
			return true;
		}
	}
}

AddTranslationsCommand::AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool AddTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	//this->intepreter->GetTranslations().clear();
	if( !this->parameters[1].empty() && this->parameters[1].front()==Symbols::REMARK_WLL0 )
	{
		Wll0Loader loader(this->parameters[1]);
		if(!loader.LoadWll(*(this->translations)))
		{
			loader.ShowErrorMessage();
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		Wll1Loader loader(this->parameters[1]);
		if(!loader.LoadWll0(*(this->translations)))
		{
			loader.ShowErrorMessage();
			return false;
		}
		else
		{
			return true;
		}
	}
}

CondCommand::CondCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool CondCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()%2==1);
	for(int i=1; i<this->parameters.size(); i+=2)
	{
		if(this->parameters[i][0] == Symbols::TRUE)
		{
			this->intepreter->IntepretWll(this->parameters[i+1], result, this->translations);
			break;
		}
	}
	return true;
}

LoopCommand::LoopCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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
		if(!this->intepreter->IntepretWll(this->parameters[1],condition_result,this->translations))
		{
			TERM_ERROR("Intepret LoopCommand condition failed: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
			return false;
		}

		INFO("condition_result="<<condition_result);
		if(condition_result[0] == Symbols::FALSE) break;

		INFO("Intepret LoopCommand expression part: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
		if(!this->intepreter->IntepretWll(this->parameters[2], result, this->translations))
		{
			TERM_ERROR("Intepret LoopCommand expression failed: condition=["<<this->parameters[1]<<"],expression=["<<this->parameters[2]<<"]");
			return false;
		}
	}while(true);

	return true;
}

EqCommand::EqCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{
}

bool EqCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1] == this->parameters[2]) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

LtCommand::LtCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{
}

bool LtCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1] < this->parameters[2]) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

AndCommand::AndCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{
}

bool AndCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1][0] == Symbols::TRUE && this->parameters[2][0] == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

OrCommand::OrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{
}

bool OrCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	result.push_back((this->parameters[1][0] == Symbols::TRUE || this->parameters[2][0] == Symbols::TRUE) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}


NotCommand::NotCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{
}

bool NotCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 2);
	result.push_back((this->parameters[1][0] == Symbols::FALSE) ? Symbols::TRUE : Symbols::FALSE);
	return true;
}

ShellCommand::ShellCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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

SetCommand::SetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool SetCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	assert(!variable_table_stack->empty());
	VariableTable& variable_table = variable_table_stack->back();
	string variable_name;
	ToString(variable_name, this->parameters[1]);
	variable_table[variable_name] = this->parameters[2];
	return true;
}

GetCommand::GetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
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

	TERM_ERROR("GetCommand failed! variable_name["<<variable_name<<"] not find in variable table stack");
	return false;
}

PushDataCommand::PushDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool PushDataCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 1);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	variable_table_stack->push_back(VariableTable());
	return true;
}

PopDataCommand::PopDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool PopDataCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 1);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	variable_table_stack->pop_back();
	return true;
}

EvalCommand::EvalCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
: WllCommand(cmd,parameter_fields,intepreter,translations)
{

}

bool EvalCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	return this->intepreter->IntepretWll(this->parameters[1], result, this->translations);
}


WllCommand* WllCommandFactory::CreateCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations)
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
		command = new LoadTranslationsCommand(cmd,parameter_fields,intepreter,translations);
	}//LOAD_TRANSLATIONS
	else if(cmd == Symbols::ADD_TRANSLATIONS)
	{
		command = new AddTranslationsCommand(cmd,parameter_fields,intepreter,translations);
	}//LOAD_TRANSLATIONS
	else if(cmd == Symbols::ADD)
	{
		command = new AddCommand(cmd,parameter_fields,intepreter,translations);
	}//ADD
	else if(cmd == Symbols::SUB)
	{
		command = new SubCommand(cmd,parameter_fields,intepreter,translations);
	}//SUB
	else if(cmd == Symbols::MUL)
	{
		command = new MulCommand(cmd,parameter_fields,intepreter,translations);
	}//MUL
	else if(cmd == Symbols::DIV)
	{
		command = new DivCommand(cmd,parameter_fields,intepreter,translations);
	}//DIV
	else if(cmd == Symbols::COND)
	{
		command = new CondCommand(cmd,parameter_fields,intepreter,translations);
	}//COND
	else if(cmd == Symbols::LOOP)
	{
		command = new LoopCommand(cmd,parameter_fields,intepreter,translations);
	}//LOOP
	else if(cmd == Symbols::EQ)
	{
		command = new EqCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::LT)
	{
		command = new LtCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::AND)
	{
		command = new AndCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::OR)
	{
		command = new OrCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::NOT)
	{
		command = new NotCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::SHELL)
	{
		command = new ShellCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::SET)
	{
		command = new SetCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::GET)
	{
		command = new GetCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::PUSH_DATA)
	{
		command = new PushDataCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::POP_DATA)
	{
		command = new PopDataCommand(cmd,parameter_fields,intepreter,translations);
	}
	else if(cmd == Symbols::EVAL)
	{
		command = new EvalCommand(cmd,parameter_fields,intepreter,translations);
	}

	return command;
}
