/*
 * Wll0Command.cpp
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#include "Wll0Command.h"
#include "VariableNode.h"
#include <sstream>
#include "Wll0Loader.h"
using namespace std;

Wll0Command::Wll0Command(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* wll0_intepreter)
: command(cmd), parameters(parameter_fields), intepreter(wll0_intepreter)
{

}

AddCommand::AddCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter)
: Wll0Command(cmd,parameter_fields,intepreter)
{

}

bool AddCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		stringstream s;
		s<<*i;
		Integer n(s.str());
		sum += n;
	}

	for(string::const_iterator i = sum.value.begin(); i != sum.value.end(); ++i)
	{
		result.push_back(Symbols(*i));
	}
	return true;
}

SubCommand::SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter)
: Wll0Command(cmd,parameter_fields,intepreter)
{

}

bool SubCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		stringstream s;
		s<<*i;
		Integer n(s.str());

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

MulCommand::MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter)
: Wll0Command(cmd,parameter_fields,intepreter)
{

}

bool MulCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		stringstream s;
		s<<*i;
		Integer n(s.str());

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

DivCommand::DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter)
: Wll0Command(cmd,parameter_fields,intepreter)
{

}

bool DivCommand::Intepret(std::vector<Symbols>& result)
{
	Integer sum("0");
	for(vector< vector<Symbols> >::iterator i = parameters.begin()+1; i != parameters.end(); ++i)
	{
		stringstream s;
		s<<*i;
		Integer n(s.str());

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

LoadTranslationsCommand::LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter)
: Wll0Command(cmd,parameter_fields,intepreter)
{

}

bool LoadTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	this->intepreter->GetTranslations().clear();
	Wll0Loader loader(this->parameters[1]);
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

AddTranslationsCommand::AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter)
: Wll0Command(cmd,parameter_fields,intepreter)
{

}

bool AddTranslationsCommand::Intepret(std::vector<Symbols>& result)
{
	assert(this->parameters.size()==2);
	//this->intepreter->GetTranslations().clear();
	Wll0Loader loader(this->parameters[1]);
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
