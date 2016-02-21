#include "LanguageTranslations.h"
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
using namespace Wll;
using namespace std;

int main()
{
	LanguageTranslations empty_t1;
	LanguageTranslations empty_t2;
	assert(empty_t1==empty_t2);

	LanguageTranslations t1;
	t1.source_rule = LanguageRules(Symbols("abc"),LanguageExpressions("abc"));
	t1.destination_rule = LanguageRules(Symbols("abc"),LanguageExpressions(Symbols(REMARK_SYMBOL,"abc")));
	LanguageTranslations t2 = t1;
	assert(t1==t2);
	assert(!(t1==empty_t1));

	stringstream output;
	output<<t1;
	assert(output.str()=="<abc>-->abc==><abc>-->$abc");

	cout<<"PASS"<<endl;
}
