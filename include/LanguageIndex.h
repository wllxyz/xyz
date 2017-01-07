/*
 * LanguageIndex.h
 *
 *  Created on: 2016年9月1日
 *      Author: wll
 */

#ifndef INCLUDE_LANGUAGEINDEX_H_
#define INCLUDE_LANGUAGEINDEX_H_

#include "LanguageRules.h"

struct LanguageIndex
{
public:
	bool GetRulesBySymbol(const Symbols symbol, vector<LanguageRules*>& rules) const;
	bool GetRuleNosBySymbol(const Symbols symbol, vector<size_t>& rule_nos) const;
	const LanguageRules* GetRule(size_t rule_no) const;
public:
	vector<LanguageRules*> rules;
	multimap<Symbols,size_t> rule_index;
};

#endif /* INCLUDE_LANGUAGEINDEX_H_ */
