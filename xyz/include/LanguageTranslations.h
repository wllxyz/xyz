#ifndef LANGUAGE_TRANSLATIONS_H
#define LANGUAGE_TRANSLATIONS_H

#include "LanguageRules.h"
struct LanguageTranslations
{
public:
	LanguageTranslations();
	LanguageTranslations(const LanguageRules& source,const LanguageRules& destination);
public:
	LanguageRules source_rule;
	LanguageRules destination_rule;
};

bool operator== (const LanguageTranslations& a, const LanguageTranslations& b);
ostream& operator<< (ostream& o, const LanguageTranslations& lt);

#endif	//LANGUAGE_TRANSLATIONS_H

