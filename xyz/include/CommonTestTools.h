#ifndef	COMMON_TEST_TOOLS
#define COMMON_TEST_TOOLS

#include "LanguageGrammar.h"

//with additional start grammar rule
void LoadExpression(LanguageGrammar& lg)
{
	lg.translation_rules.clear();

	//0
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("start"),LanguageExpressions(Symbols("e"))),
		LanguageRules(Symbols("start"),LanguageExpressions(Symbols("e")))));
	//1
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))+Symbols('+')+Symbols("e")), 
		LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))+Symbols("e")+Symbols('+'))));
	//2
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))), 
		LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t")))));
	//3
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))+Symbols('*')+Symbols("t")), 
		LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))+Symbols("t")+Symbols('*'))));
	//4
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))),
		LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f")))));
	//5
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("f"),LanguageExpressions(Symbols("i"))),
		LanguageRules(Symbols("f"),LanguageExpressions(Symbols("i")))));
	//6
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("i"),LanguageExpressions(Symbols('0'))),
		LanguageRules(Symbols("i"),LanguageExpressions(Symbols('0')))));
	//7
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("i"),LanguageExpressions(Symbols('1'))),
		LanguageRules(Symbols("i"),LanguageExpressions(Symbols('1')))));

	lg.Initialize();
}

//no more need add start grammar rule
void LoadExpression2(LanguageGrammar& lg)
{
	lg.translation_rules.clear();

	/*
	//0
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("start"),LanguageExpressions(Symbols("e"))),
		LanguageRules(Symbols("start"),LanguageExpressions(Symbols("e")))));
	*/

	//1
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))+Symbols('+')+Symbols("e")),
		LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))+Symbols("e")+Symbols('+'))));
	//2
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))),
		LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t")))));
	//3
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))+Symbols('*')+Symbols("t")),
		LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))+Symbols("t")+Symbols('*'))));
	//4
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))),
		LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f")))));
	//5
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("f"),LanguageExpressions(Symbols("i"))),
		LanguageRules(Symbols("f"),LanguageExpressions(Symbols("i")))));
	//6
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("i"),LanguageExpressions(Symbols('0'))),
		LanguageRules(Symbols("i"),LanguageExpressions(Symbols('0')))));
	//7
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("i"),LanguageExpressions(Symbols('1'))),
		LanguageRules(Symbols("i"),LanguageExpressions(Symbols('1')))));

	lg.Initialize();
}

//described with null expression
void LoadExpression3(LanguageGrammar& lg)
{
	lg.translation_rules.clear();

	/*
	//0
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("start"),LanguageExpressions(Symbols("e"))),
		LanguageRules(Symbols("start"),LanguageExpressions(Symbols("e")))));
	*/

	//1
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))+Symbols("tlist")),
		LanguageRules(Symbols("e"),LanguageExpressions(Symbols("t"))+Symbols("tlist"))));

	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("tlist"),LanguageExpressions(Symbols('+'))+Symbols("t")+Symbols("tlist")),
		LanguageRules(Symbols("tlist"),LanguageExpressions(Symbols("t"))+Symbols("tlist")+Symbols('+'))));
	//2
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("tlist"),LanguageExpressions()),
		LanguageRules(Symbols("tlist"),LanguageExpressions())));
	//3
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))+Symbols("flist")),
		LanguageRules(Symbols("t"),LanguageExpressions(Symbols("f"))+Symbols("flist"))));

	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("flist"),LanguageExpressions(Symbols('*'))+Symbols("f")+Symbols("flist")),
		LanguageRules(Symbols("flist"),LanguageExpressions(Symbols("f"))+Symbols("flist")+Symbols('*'))));
	//4
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("flist"),LanguageExpressions()),
		LanguageRules(Symbols("flist"),LanguageExpressions())));
	//5
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("f"),LanguageExpressions(Symbols("i"))),
		LanguageRules(Symbols("f"),LanguageExpressions(Symbols("i")))));
	//6
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("i"),LanguageExpressions(Symbols('0'))),
		LanguageRules(Symbols("i"),LanguageExpressions(Symbols('0')))));
	//7
	lg.translation_rules.push_back(LanguageTranslations(LanguageRules(Symbols("i"),LanguageExpressions(Symbols('1'))),
		LanguageRules(Symbols("i"),LanguageExpressions(Symbols('1')))));

	lg.Initialize();
}

#endif	//COMMON_TEST_TOOLS

