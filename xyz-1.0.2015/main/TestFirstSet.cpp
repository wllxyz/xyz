#include "LanguageGrammar.h"
#include "FirstSet.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	LanguageGrammar lg;
	
	LanguageTranslations a(LanguageRules(Symbols("a"),LanguageExpressions("abc")), LanguageRules(Symbols("A"),LanguageExpressions("ABC")));
	LanguageTranslations b(LanguageRules(Symbols("a"),LanguageExpressions("bcd")), LanguageRules(Symbols("A"),LanguageExpressions("BCD")));
	LanguageTranslations c(LanguageRules(Symbols("b"),LanguageExpressions("cde")), LanguageRules(Symbols("B"),LanguageExpressions("CDE")));
	LanguageTranslations d(LanguageRules(Symbols("a"),LanguageExpressions(Symbols("b"))+LanguageExpressions("cde")), LanguageRules(Symbols("B"),LanguageExpressions("CDE")));
	LanguageTranslations e(LanguageRules(Symbols("b"),LanguageExpressions()), LanguageRules(Symbols("B"),LanguageExpressions("CDE")));
	LanguageTranslations f(LanguageRules(Symbols("a"),LanguageExpressions(Symbols("a"))+LanguageExpressions("cde")), LanguageRules(Symbols("B"),LanguageExpressions("CDE")));
	
	lg.translation_rules.push_back(a);
	lg.translation_rules.push_back(b);
	lg.translation_rules.push_back(c);
	lg.translation_rules.push_back(d);
	lg.translation_rules.push_back(e);
	lg.translation_rules.push_back(f);
	
	lg.Initialize();

	FirstCalculator fc(lg.source_rules);
	fc.Initialize();

	set<Symbols> result;
	assert(!fc.First(Symbols("a"),result));
	cout<<Symbols("a")<<"\t";
	for(set<Symbols>::iterator i = result.begin(); i != result.end(); ++i)
	{
		cout<<*i;
	}
	cout<<endl;

	assert(fc.First(Symbols("b"),result));
	cout<<Symbols("b")<<"\t";
	for(set<Symbols>::iterator i = result.begin(); i != result.end(); ++i)
	{
		cout<<*i;
	}
	cout<<endl;

	cout<<"PASS"<<endl;	

	return 0;
}

