/*
 * VariableStack.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: wll
 */
#include "VariableStack.h"
#include <cassert>
using namespace std;

bool VariableStack::Push()
{
	this->variable_stack.push_back(map<string, Symbols*>());
	return true;
}

bool VariableStack::Pop()
{
	if(this->variable_stack.empty()) return false;
	for(map<string,Symbols*>::iterator i = this->variable_stack.back().begin(); i != this->variable_stack.back().end(); ++i)
	{
		delete i->second;
	}

	this->variable_stack.pop_back();
	return true;
}

Symbols* VariableStack::Lookup(string variable_name)
{
	for(vector< map<string, Symbols*> >::reverse_iterator i = this->variable_stack.rbegin(); i != this->variable_stack.rend(); ++i)
	{
		map<string, Symbols*>::iterator j = i->find(variable_name);
		if(j != i->end())
		{
			return j->second;
		}
	}
	return NULL;
}

Symbols* VariableStack::Register(string variable_name)
{
	assert(!this->variable_stack.empty());
	map<string, Symbols*>& top = this->variable_stack.back();
	return top[variable_name] = new Symbols;
}

Symbols* VariableStack::LookupOrRegister(string variable_name)
{
	Symbols* symbol = this->Lookup(variable_name);
	if(symbol == NULL)
	{
		symbol = this->Register(variable_name);
	}
	return symbol;
}

