#include "LanguageGrammar.h"

bool LanguageIndex::GetRulesBySymbol(const Symbols symbol, vector<LanguageRules*>& rules) const
{
	rules.clear();
	pair< multimap<Symbols,size_t>::const_iterator, multimap<Symbols,size_t>::const_iterator > ret;
	ret = this->rule_index.equal_range(symbol);
	for(multimap<Symbols,size_t>::const_iterator it = ret.first; it != ret.second; ++it)
	{
		rules.push_back(this->rules[it->second]);
	}
	return !rules.empty();
}

bool LanguageIndex::GetRuleNosBySymbol(const Symbols symbol, vector<size_t>& rules) const
{
	rules.clear();
	pair< multimap<Symbols,size_t>::const_iterator, multimap<Symbols,size_t>::const_iterator > ret;
	ret = this->rule_index.equal_range(symbol);
	for(multimap<Symbols,size_t>::const_iterator it = ret.first; it != ret.second; ++it)
	{
		rules.push_back(it->second);
	}
	return !rules.empty();
}

const LanguageRules* LanguageIndex::GetRule(size_t rule_no) const
{
	if(rule_no >= this->rules.size()) return NULL;

	return this->rules[rule_no];
}

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

