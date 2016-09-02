/*
 * Wll0Loader.cpp
 *
 *  Created on: 2016-1-2
 *      Author: wll
 */

#include "Wll0Loader.h"
#include "WllTrace.h"
#include "LanguageTypes.h"
#include <string>
#include <vector>
using namespace std;

Wll0Loader::Wll0Loader(const std::vector<Symbols>& input_symbols) : WllLoader(input_symbols)
{
	grammar_file_name = "wll0.xyz";
}

bool Wll0Loader::LoadWll(std::vector<LanguageTranslations>& translations)
{
	//暂时不做语法检查
	LanguageTranslations translation;
	LanguageRules source_rule,destination_rule,rule;
	Symbols root_symbol,variable,remark;
	LanguageExpressions expression,sub_expression;
	string symbol_string;

	for(vector<Symbols>::const_iterator i = this->input_symbols.begin(); i != this->input_symbols.end(); ++i)
	{
		Symbols symbol = *i;
		DEBUG_LOG("explain symbol["<<symbol<<"] ...");
		if(symbol.IsConstant())
		{
			symbol_string.push_back(char(symbol.value));
			continue;
		}

		if(symbol == Symbols::REMARK_IGNORE)
		{
			symbol = *(++i);
			sub_expression = LanguageExpressions(symbol);
			continue;
		}

		if(symbol == Symbols::REMARK_REMARK)
		{
			remark = Symbols(REMARK_SYMBOL, symbol_string.c_str());
			sub_expression = LanguageExpressions(remark);
			symbol_string.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_CONSTANT)
		{
			//string1$CONSTANTstring2$CONSTANT...stringN$CONSTANT$SUB_SYMBOL, only the last CONSTANT effect
			//we use this feather to make $CONSTANTstring$CONSTANT the same effect as string$CONSTANT, but the leading $CONSTANT can distinguish the constant and variable
			sub_expression = LanguageExpressions(symbol_string.c_str());
			symbol_string.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_VARIABLE)
		{
			//<variable1>$VARIABLE<variable2>$VARIABLE...<variableN>$VARIABLE$SUB_SYMBOL, only the last VARIABLE effect
			variable = Symbols(symbol_string.c_str());
			sub_expression = LanguageExpressions(variable);
			symbol_string.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_SUB_SYMBOL)
		{
			//<variable1>$VARIABLE<variable2>$VARIABLE...<stringN>$CONSTANT$SUB_SYMBOL, only the last VARIABLE or CONSTANT SUB_SYMBOL effect
			expression += sub_expression;
			sub_expression = "";
			continue;
		}
		//if(symbol == Symbols::REMARK_EXPRESSION)
		//{
		//	continue;
		//}
		if(symbol == Symbols::REMARK_ROOT_SYMBOL)
		{
			root_symbol = variable;
			continue;
		}
		if(symbol == Symbols::REMARK_RULE)
		{
			rule.symbol = root_symbol;
			rule.expression = expression;
			expression.symbols.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_SOURCE_RULE)
		{
			source_rule = rule;
			continue;
		}
		if(symbol == Symbols::REMARK_DESTINATION_RULE)
		{
			destination_rule = rule;
			continue;
		}
		if(symbol == Symbols::REMARK_TRANSLATION)
		{
			translation.source_rule = source_rule;
			translation.destination_rule = destination_rule;
			translations.push_back(translation);
			continue;
		}
		//if(symbol == Symbols::REMARK_WLL0)
		//{
		//	continue;
		//}
	}//end of for

	return true;
}

void Wll0Loader::ShowErrorMessage()
{

}

bool Wll0Loader::TestLanguage()
{
	static LanguageGrammar wll_xyz_languages;
	static bool is_wll_xyz_loaded = false;

	return WllLoader::TestLanguage(wll_xyz_languages, is_wll_xyz_loaded);
}
