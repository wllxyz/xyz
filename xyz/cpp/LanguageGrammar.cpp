#include "LanguageGrammar.h"

void LanguageGrammar::Initialize()
{
	this->source_rules.rules.clear();
	this->source_rules.rule_index.clear();
	this->destination_rules.rules.clear();
	this->destination_rules.rule_index.clear();

	size_t i = 0;
	for(vector<LanguageTranslations>::iterator it = translation_rules.begin(); it != translation_rules.end(); ++it)
	{
		this->source_rules.rules.push_back(&it->source_rule);
		this->source_rules.rule_index.insert(pair<Symbols,size_t>(it->source_rule.symbol, i));
		this->destination_rules.rules.push_back(&it->destination_rule);
		this->destination_rules.rule_index.insert(pair<Symbols,size_t>(it->destination_rule.symbol, i));
		++i;
	}
}

ostream& operator<< (ostream& o, const LanguageGrammar& lg)
{
	size_t rule_no = 0;
	for(vector<LanguageTranslations>::const_iterator it = lg.translation_rules.begin(); it != lg.translation_rules.end(); ++it)
	{
		o<<rule_no++<<":\t"<<*it<<endl;
	}
	return o;
}

