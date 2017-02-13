#include "LanguageGrammar.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	LanguageGrammar lg;
	
	LanguageTranslations a(LanguageRules(Symbols("a"),LanguageExpressions("abc")), LanguageRules(Symbols("A"),LanguageExpressions("ABC")));
	LanguageTranslations b(LanguageRules(Symbols("a"),LanguageExpressions("bcd")), LanguageRules(Symbols("A"),LanguageExpressions("BCD")));
	LanguageTranslations c(LanguageRules(Symbols("b"),LanguageExpressions("cde")), LanguageRules(Symbols("B"),LanguageExpressions("CDE")));
	
	lg.translation_rules.push_back(a);
	lg.translation_rules.push_back(b);
	lg.translation_rules.push_back(c);
	
	lg.Initialize();
	
	vector<LanguageRules*> rules;
	bool ret;
	ret = lg.source_rules.GetRulesBySymbol(Symbols("a"), rules);
	assert(ret);
	assert(rules.size()==2);
	for(vector<LanguageRules*>::iterator it = rules.begin(); it != rules.end(); ++it)
	{
		cout<<**it<<endl;
	}
	ret = lg.source_rules.GetRulesBySymbol(Symbols("b"), rules);
	assert(ret);
	assert(rules.size()==1);
	for(vector<LanguageRules*>::iterator it = rules.begin(); it != rules.end(); ++it)
	{
		cout<<**it<<endl;
	}
	ret = lg.source_rules.GetRulesBySymbol(Symbols("c"), rules);
	assert(!ret);
	assert(rules.size()==0);
	for(vector<LanguageRules*>::iterator it = rules.begin(); it != rules.end(); ++it)
	{
		cout<<**it<<endl;
	}
	
	return 0;
}
