#include "LanguageSymbols.h"
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
using namespace Wll;
using namespace std;

int main()
{
	Symbols unknow_symbol;
	Symbols variable_symbol("variable");
	Symbols constant_symbol('c');
	Symbols remark_symbol(REMARK_SYMBOL,"REMARK");	
	assert(!unknow_symbol.IsVariable() && !unknow_symbol.IsConstant() && !unknow_symbol.IsRemark());
	assert(variable_symbol.IsVariable());
	assert(constant_symbol.IsConstant());
	assert(remark_symbol.IsRemark());

	stringstream sv;
	sv<<variable_symbol;
	assert(sv.str()=="<variable>");

	stringstream sc;
	sc<<constant_symbol;
	assert(sc.str()=="c");

	stringstream sr;
	sr<<remark_symbol;
	assert(sr.str()=="$REMARK");

	Symbols s1('a');
	Symbols s2('a');
	Symbols s3('b');
	assert(s1==s2);
	assert(s2<s3);

	cout<<"PASS"<<endl;
}

