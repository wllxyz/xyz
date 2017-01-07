#ifndef	LANGUAGE_GRAMMAR_H
#define LANGUAGE_GRAMMAR_H

#include "LanguageTranslations.h"
#include "LanguageIndex.h"
#include "FirstSet.h"
#include <vector>
#include <map>
using namespace std;

struct LanguageGrammar
{
public:
	LanguageGrammar():first_calculator(source_rules) {}
	void Initialize();
	void RemoveRepeatedTranslationRules();
	//获得默认文法开始符号(第一条文法的root_symbol)
	virtual Symbols GetDefaultStartSymbol();
public:
	vector<LanguageTranslations> translation_rules;
public:
	LanguageIndex source_rules;
	LanguageIndex destination_rules;
	FirstCalculator first_calculator;
};

ostream& operator<< (ostream& o, const LanguageGrammar& lg);

#endif	//LANGUAGE_GRAMMAR_H
