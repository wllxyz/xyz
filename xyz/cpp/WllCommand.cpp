/*
 * WllCommand.cpp
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#include "WllCommand.h"
#include "VariableNode.h"
#include "Wll0Loader.h"
#include "Wll1Loader.h"
#include "Wll1Intepreter.h"
#include "WllTrace.h"
#include "WllSingleton.h"
using namespace std;

WllCommand::WllCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* wll_intepreter)
: command(cmd), parameters(parameter_fields), intepreter(wll_intepreter)
{

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
	this->intepreter->GetTranslations().clear();
	if( !this->parameters[1].empty() && this->parameters[1].front()==Symbols::REMARK_WLL0 )
	{
		INFO("wll0loader="<<this->parameters[1]);
		Wll0Loader loader(this->parameters[1]);
		if(!loader.LoadWll(this->intepreter->GetTranslations()))
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
		if(!loader.LoadWll0(this->intepreter->GetTranslations()))
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

AddTranslationsCommand::AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool AddTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	//this->intepreter->GetTranslations().clear();
	if( !this->parameters[1].empty() && this->parameters[1].front()==Symbols::REMARK_WLL0 )
	{
		Wll0Loader loader(this->parameters[1]);
		if(!loader.LoadWll(this->intepreter->GetTranslations()))
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
		if(!loader.LoadWll0(this->intepreter->GetTranslations()))
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
			/*
			for(std::vector<Symbols>::const_iterator j = this->parameters[i+1].begin(); j != this->parameters[i+1].end(); ++j)
			{
				result.push_back(*j);
			}
			*/
			Wll1Intepreter intepreter(this->parameters[i+1], result, this->intepreter->GetTranslations());
			intepreter.IntepretWll();
			break;
		}
	}
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

SetCommand::SetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
: WllCommand(cmd,parameter_fields,intepreter)
{

}

bool SetCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size() == 3);
	vector<VariableTable>* variable_table_stack = Singleton<vector<VariableTable> >::GetInstance();
	assert(!variable_table_stack->empty());
	VariableTable& variable_table = variable_table_stack->back();
	string variable_name;
	string variable_value;
	ToString(variable_name, this->parameters[1]);
	ToString(variable_value, this->parameters[2]);
	variable_table[variable_name] = variable_value;
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
	VariableTable& variable_table = variable_table_stack->back();
	string variable_name;
	ToString(variable_name, this->parameters[1]);
	result += (variable_table[variable_name].value);
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
	variable_table_stack->pop_back();
	return true;
}


WllCommand* WllCommandFactory::CreateCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter)
{
	WllCommand* command = NULL;
	if(cmd == Symbols::LOAD_TRANSLATIONS)
	{
		command = new LoadTranslationsCommand(cmd, parameter_fields, intepreter);
	}//LOAD_TRANSLATIONS
	else if(cmd == Symbols::ADD_TRANSLATIONS)
	{
		command = new AddTranslationsCommand(cmd, parameter_fields, intepreter);
	}//LOAD_TRANSLATIONS
	else if(cmd == Symbols::ADD)
	{
		command = new AddCommand(cmd, parameter_fields, intepreter);
	}//ADD
	else if(cmd == Symbols::SUB)
	{
		command = new SubCommand(cmd, parameter_fields, intepreter);
	}//SUB
	else if(cmd == Symbols::MUL)
	{
		command = new MulCommand(cmd, parameter_fields, intepreter);
	}//MUL
	else if(cmd == Symbols::DIV)
	{
		command = new DivCommand(cmd, parameter_fields, intepreter);
	}//DIV
	else if(cmd == Symbols::COND)
	{
		command = new CondCommand(cmd, parameter_fields, intepreter);
	}//COND
	else if(cmd == Symbols::EQ)
	{
		command = new EqCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::LT)
	{
		command = new LtCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::AND)
	{
		command = new AndCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::OR)
	{
		command = new OrCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::NOT)
	{
		command = new NotCommand(cmd, parameter_fields, intepreter);
	}
	else if(cmd == Symbols::SHELL)
	{
		command = new ShellCommand(cmd,parameter_fields, intepreter);
	}
	else if(cmd == Symbols::SET)
	{
		command = new SetCommand(cmd,parameter_fields, intepreter);
	}
	else if(cmd == Symbols::GET)
	{
		command = new GetCommand(cmd,parameter_fields, intepreter);
	}
	else if(cmd == Symbols::PUSH_DATA)
	{
		command = new PushDataCommand(cmd,parameter_fields, intepreter);
	}
	else if(cmd == Symbols::POP_DATA)
	{
		command = new PopDataCommand(cmd,parameter_fields, intepreter);
	}

	return command;
}
