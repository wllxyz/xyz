/*
 * Wll1Loader.h
 *
 *  Created on: 2015-12-13
 *      Author: wll
 */

#ifndef WLL1LOADER_H_
#define WLL1LOADER_H_

#include "LanguageTypes.h"
#include "WllLoader.h"
#include <vector>
using namespace std;

class Wll1Loader
{
public:
	Wll1Loader();
	Wll1Loader(const vector<Symbols>& input_symbols);
	void SetInputSymbols(const char* text);
	bool LoadWll0(vector<LanguageTranslations>& translations);
	void ShowErrorMessage();
protected:
	//Load* 函数一般都含有比较复杂的语法成分,当匹配输入的符号串时返回true,不匹配返回false; 不匹配时不消耗输入符号
	bool LoadTranslation(LanguageTranslations& translation);
	bool LoadSourceRule(LanguageRules& source_rule);
	bool LoadDestinationRule(LanguageRules& destination_rule);
	bool LoadRule(LanguageRules& rule);
	bool LoadRootSymbol(Symbols& root_symbol);
	bool LoadExpression(LanguageExpressions& expression);
	bool LoadSymbol(LanguageExpressions& symbol);
	bool LoadVariable(Symbols& variable);
	bool LoadConstant(LanguageExpressions& constant);
	bool LoadRemark(Symbols& remark);
	bool LoadIdent(string& ident);
	bool LoadString(LanguageExpressions& str);
	bool LoadOriginalString(LanguageExpressions& str);
	bool EncountRemark(Symbols& remark);
	//Expect* 函数在匹配对应类型字符后,会消耗该输入符号,并把它通过引用参数保存起来,以便下一步使用;不匹配的时候不消耗输入符号
	bool ExpectRemark(Symbols& remark);
	bool ExpectLetter(char& c);
	bool ExpectDigit(char& c);
	bool ExpectCharacter(Symbols& symbol);
	bool ExpectSpace(char& c);
	bool SkipSpaces();
	//Accept* 函数在匹配指定字符或者表达式字符串时会消耗输入符号串,不匹配的时候并不消耗输入符号
	bool Accept(const Symbols& symbol);
	bool Accept(const LanguageExpressions& expression);
	//Encount* 函数测试下一个输入符号是否和指定的符号匹配,并不消耗输入符号
	bool Encount(const Symbols& symbol);
	const Symbols& GetSymbol();
protected:
	vector<Symbols> input_symbols;
	int input_pos;
};


#endif /* WLL1LOADER_H_ */
