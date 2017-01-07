/*
 * LanguageIndex.cpp
 *
 *  Created on: 2016年9月1日
 *      Author: wll
 */


#include "LanguageIndex.h"

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
