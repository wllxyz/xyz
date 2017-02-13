/*
 * TestLR1Parser.cpp
 *
 *  Created on: 2013-12-14
 *      Author: wangliliang
 */

#define private public
#include "LR1Parsers.h"
#include <cassert>
#include <iostream>
using namespace std;

class TestLR1Parser : public LR1Parsers
{
public:
	void SetInputSymbols(const char* text)
	{
		this->input_symbols.clear();
		while(*text)
		{
			this->input_symbols.push_back(*text);
			text++;
		}
		this->input_pos = 0;
	}

	void TestLoadWll0()
	{
		this->SetInputSymbols("<source_rule>--><rule>==><source_rule>--><rule>$SOURCE_RULE;");
		vector<LanguageTranslations> translations;
		assert(this->LoadWll0(translations));
	}

	void TestLoadTranslation()
	{
		this->SetInputSymbols("<a>-->\"abc\"==><a>-->\"bcd\";\n");
		LanguageTranslations translation;
		assert(this->LoadTranslation(translation));
		cout<<translation<<endl;
	}

	void TestLoadSourceRule()
	{
		this->SetInputSymbols("<abc>-->\"ab\"<b><bcd>");
		LanguageRules rule;
		assert(this->LoadSourceRule(rule));
	}

	void TestLoadDestinationRule()
	{
		this->SetInputSymbols("<abc>-->\"ab\"<b><bcd>");
		LanguageRules rule;
		assert(this->LoadDestinationRule(rule));
	}

	void TestLoadRule()
	{
		this->SetInputSymbols("<abc>-->\"ab\"<b><bcd>");
		LanguageRules rule;
		assert(this->LoadRule(rule));
	}

	void TestLoadRootSymbol()
	{
		this->SetInputSymbols("<abc>");
		Symbols symbol;
		assert(this->LoadRootSymbol(symbol));
	}

	void TestLoadExpression()
	{
		this->SetInputSymbols("<abc>\"dkjfdlf\"<bd>");
		LanguageExpressions expression;
		assert(this->LoadExpression(expression));
	}

	void TestLoadSymbol()
	{
		this->SetInputSymbols("<abc>");
		LanguageExpressions symbol;
		assert(this->LoadSymbol(symbol));
	}

	void TestLoadVariable()
	{
		this->SetInputSymbols("<abc>");
		Symbols symbol;
		assert(this->LoadVariable(symbol));
	}

	void TestLoadConstant()
	{
		this->SetInputSymbols("\"abcde\"");
		LanguageExpressions constant;
		assert(this->LoadConstant(constant));
	}

	void TestLoadRemark()
	{
		this->SetInputSymbols("$REMARK");
		Symbols remark;
		assert(this->LoadRemark(remark));
	}

	void TestLoadIdent()
	{
		this->SetInputSymbols("abce_2d");
		string ident;
		assert(this->LoadIdent(ident));
	}

	void TestLoadString()
	{
		this->SetInputSymbols("dfkeife(*&*3\">><");
		LanguageExpressions str;
		assert(this->LoadString(str));
	}

	void TestExpectLetter()
	{
		this->SetInputSymbols("B");
		char c;
		assert(this->ExpectLetter(c));
		assert(c=='B');
	}

	void TestExpectDigit()
	{
		this->SetInputSymbols("8");
		char c;
		assert(this->ExpectDigit(c));
		assert(c=='8');
	}

	void TestExpectCharacter()
	{
		this->SetInputSymbols("&");
		Symbols c;
		assert(this->ExpectCharacter(c));
		assert(c=='&');
	}

	void TestExpectSpace()
	{
		this->SetInputSymbols("\n");
		char c;
		assert(this->ExpectSpace(c));
		assert(c=='\n');
	}

	void TestSkipSpaces()
	{
		this->SetInputSymbols("\n\n \t");
		assert(this->SkipSpaces());
	}

	void TestAcceptSymbol()
	{
		this->SetInputSymbols("^");
		Symbols symbol('^');
		assert(this->Accept(symbol));
	}

	void TestAcceptExpression()
	{
		this->SetInputSymbols("abckdle");
		LanguageExpressions expression("abckdle");
		assert(this->Accept(expression));
	}

	void TestEncount()
	{
		this->SetInputSymbols("^");
		assert(this->Encount('^'));
	}

	void TestGetSymbol()
	{
		this->SetInputSymbols("c");
		assert(this->GetSymbol()=='c');
	}

};

int main()
{
	TestLR1Parser test;
	test.TestGetSymbol();
	test.TestEncount();
	test.TestAcceptExpression();
	test.TestExpectCharacter();
	test.TestExpectDigit();
	test.TestExpectLetter();
	test.TestExpectSpace();
	test.TestLoadIdent();
	test.TestLoadString();
	test.TestLoadVariable();
	test.TestLoadConstant();
	test.TestLoadRemark();
	test.TestLoadSymbol();
	test.TestLoadRootSymbol();
	test.TestLoadExpression();
	test.TestLoadRule();
	test.TestLoadSourceRule();
	test.TestLoadDestinationRule();
	test.TestLoadTranslation();
	test.TestLoadWll0();

	cout<<"PASS"<<endl;
	return 0;
}

