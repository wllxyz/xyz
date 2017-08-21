/*
 * VariableStack.h
 *
 *  Created on: 2017年6月29日
 *      Author: wll
 */

#ifndef INCLUDE_VARIABLESTACK_H_
#define INCLUDE_VARIABLESTACK_H_

#include "LanguageSymbols.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

class VariableStack
{
public:
	bool Push();
	bool Pop();
public:
	Symbols* Lookup(string variable_name);
	Symbols* Register(string variable_name);
private:
	vector< map<string, Symbols*> > variable_stack;
};

#endif /* INCLUDE_VARIABLESTACK_H_ */
