#include "LanguageTranslations.h"

LanguageTranslations::LanguageTranslations()
{
}

LanguageTranslations::LanguageTranslations(const LanguageRules& source, const LanguageRules& destination) :
source_rule(source),
destination_rule(destination)
{
}

bool operator== (const LanguageTranslations& a, const LanguageTranslations& b)
{
	return (a.source_rule==b.source_rule && a.destination_rule==b.destination_rule);
}

ostream& operator<< (ostream& o, const LanguageTranslations& lt)
{
	return o<<lt.source_rule<<"==>"<<lt.destination_rule<<";";
}
