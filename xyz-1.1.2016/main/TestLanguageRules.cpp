#include "LanguageRules.h"
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
using namespace Wll;
using namespace std;

int main()
{
	LanguageRules empty_rule1;
	LanguageRules empty_rule2;
	assert(empty_rule1==empty_rule2);

	LanguageExpressions e1('a');
	LanguageRules rule1("var1",e1);
	assert(!(empty_rule1==rule1));

	LanguageRules r2=rule1;
	assert(rule1==r2);

	LanguageRules r3("var1",LanguageExpressions('a'));
	assert(r2==r3);

	stringstream output;
	output<<LanguageRules(Symbols("abc"),LanguageExpressions("abc"));
	assert(output.str()=="<abc>-->abc");

	cout<<"PASS"<<endl;
}
