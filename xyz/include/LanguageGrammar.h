#ifndef	LANGUAGE_GRAMMAR_H
#define LANGUAGE_GRAMMAR_H

#include "LanguageTranslations.h"
#include "LanguageIndex.h"
#include <vector>
#include <map>
using namespace std;

struct LanguageGrammar
{
public:
	void Initialize();
public:
	vector<LanguageTranslations> translation_rules;
public:
	LanguageIndex source_rules;
	LanguageIndex destination_rules;
};

ostream& operator<< (ostream& o, const LanguageGrammar& lg);

#endif	//LANGUAGE_GRAMMAR_H
