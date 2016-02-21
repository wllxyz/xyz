#include "LanguageExpressions.h"
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
using namespace Wll;
using namespace std;

int main()
{
	LanguageExpressions empty_exp1;
	LanguageExpressions empty_exp2;
	assert(empty_exp1==empty_exp2);

	LanguageExpressions e1('a');
	assert(!(empty_exp1==e1));

	LanguageExpressions e2=empty_exp2;
	e2+=Symbols('a');
	assert(e1==e2);

	assert(!((e1+Symbols('b'))==(e2+Symbols("b"))));
	assert((e1+Symbols('b'))==(e2+"b"));

	stringstream output;
	output<<LanguageExpressions("abc");
	output<<LanguageExpressions(Symbols("abc"))<<LanguageExpressions(Symbols(REMARK_SYMBOL,"abc"));
	assert(output.str()=="abc<abc>$abc");

	cout<<"PASS"<<endl;
}
