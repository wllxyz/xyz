#include "LanguageGrammar.h"
#include <sstream>

void LanguageGrammar::Initialize()
{
	this->source_rules.rules.clear();
	this->source_rules.rule_index.clear();
	this->destination_rules.rules.clear();
	this->destination_rules.rule_index.clear();

	this->RemoveRepeatedTranslationRules();

	size_t i = 0;
	for(vector<LanguageTranslations>::iterator it = translation_rules.begin(); it != translation_rules.end(); ++it)
	{
		this->source_rules.rules.push_back(&it->source_rule);
		this->source_rules.rule_index.insert(pair<Symbols,size_t>(it->source_rule.symbol, i));
		this->destination_rules.rules.push_back(&it->destination_rule);
		this->destination_rules.rule_index.insert(pair<Symbols,size_t>(it->destination_rule.symbol, i));
		++i;
	}
	this->first_calculator.Initialize();
}

void LanguageGrammar::RemoveRepeatedTranslationRules()
{
	size_t n = this->translation_rules.size();
	size_t i,j,k;
	for(i=1,k=1; i<n; i++)
	{
		bool exist = false;
		for(j=0; j<k; j++)
		{
			if(this->translation_rules[j] == this->translation_rules[i])
			{
				exist = true;
				break;
			}
		}
		if(!exist)
		{
			if(i!=k)
			{
				this->translation_rules[k] = this->translation_rules[i];
			}
			k++;
		}
	}
	this->translation_rules.resize(k);
}

Symbols LanguageGrammar::GetDefaultStartSymbol()
{
	if(this->translation_rules.empty())
	{
		return Symbols::NULL_SYMBOL;
	}
	return this->translation_rules.front().source_rule.symbol;
}

ostream& operator<< (ostream& o, const LanguageGrammar& lg)
{
	for(vector<LanguageTranslations>::const_iterator it = lg.translation_rules.begin(); it != lg.translation_rules.end(); ++it)
	{
		o<<*it<<endl;
	}
	return o;
}

std::string LanguageGrammar::ToString()
{
	std::stringstream o;
	o<<(*this);
	return o.str();
}
