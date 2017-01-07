#ifndef VARIABLE_CONTAINER_H
#define	VARIABLE_CONTAINER_H

#include "LanguageTypes.h"
#include <vector>

struct VariableContainer
{
	std::vector<LanguageTranslations> translations;
	LanguageTranslations translation;
	LanguageRules source_rule;
	LanguageRules destination_rule;
	LanguageRules rule;
};

#endif
