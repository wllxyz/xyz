/*
 * TestWll1Loader.cpp
 *
 *  Created on: 2013-12-14
 *      Author: wangliliang
 */
#define protected public
#define private public
#include "Wll1Loader.h"
#include "gtest/gtest.h"
#include <cassert>
#include <iostream>
using namespace std;


GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


TEST(Wll1LoaderTest,LoadWll0)
{
	vector<Symbols> input_symbols = LanguageExpressions("<source_rule>--><rule>==><source_rule>--><rule>$SOURCE_RULE;").symbols;
	Wll1Loader loader(input_symbols);
	vector<LanguageTranslations> translations;
	EXPECT_TRUE(loader.LoadWll(translations));
}

TEST(Wll1LoaderTest,LoadTranslation)
{
	vector<Symbols> input_symbols = LanguageExpressions("<a>-->\"abc\"==><a>-->\"bcd\";\n").symbols;
	Wll1Loader loader(input_symbols);
	LanguageTranslations translation;
	EXPECT_TRUE(loader.LoadTranslation(translation));
	cout<<translation<<endl;
}

TEST(Wll1LoaderTest,LoadSourceRule)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>-->\"ab\"<b><bcd>").symbols;
	Wll1Loader loader(input_symbols);
	LanguageRules rule;
	EXPECT_TRUE(loader.LoadSourceRule(rule));
}

TEST(Wll1LoaderTest,LoadDestinationRule)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>-->\"ab\"<b><bcd>").symbols;
	Wll1Loader loader(input_symbols);
	LanguageRules rule;
	EXPECT_TRUE(loader.LoadDestinationRule(rule));
}

TEST(Wll1LoaderTest,LoadRule)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>-->\"ab\"<b><bcd>").symbols;
	Wll1Loader loader(input_symbols);
	LanguageRules rule;
	EXPECT_TRUE(loader.LoadRule(rule));
}

TEST(Wll1LoaderTest,LoadRootSymbol)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>").symbols;
	Wll1Loader loader(input_symbols);
	Symbols symbol;
	EXPECT_TRUE(loader.LoadRootSymbol(symbol));
}

TEST(Wll1LoaderTest,LoadExpression)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>\"dkjfdlf\"<bd>").symbols;
	Wll1Loader loader(input_symbols);
	LanguageExpressions expression;
	EXPECT_TRUE(loader.LoadExpression(expression));
}

TEST(Wll1LoaderTest,LoadSymbol)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>").symbols;
	Wll1Loader loader(input_symbols);
	LanguageExpressions symbol;
	EXPECT_TRUE(loader.LoadSymbol(symbol));
}

TEST(Wll1LoaderTest,LoadVariable)
{
	vector<Symbols> input_symbols = LanguageExpressions("<abc>").symbols;
	Wll1Loader loader(input_symbols);
	Symbols symbol;
	EXPECT_TRUE(loader.LoadVariable(symbol));
}

TEST(Wll1LoaderTest,LoadConstant)
{
	vector<Symbols> input_symbols = LanguageExpressions("\"abcde\"").symbols;
	Wll1Loader loader(input_symbols);
	LanguageExpressions constant;
	EXPECT_TRUE(loader.LoadConstant(constant));
}

TEST(Wll1LoaderTest,LoadRemark)
{
	vector<Symbols> input_symbols = LanguageExpressions("$REMARK").symbols;
	Wll1Loader loader(input_symbols);
	Symbols remark;
	EXPECT_TRUE(loader.LoadRemark(remark));
}

TEST(Wll1LoaderTest,LoadIdent)
{
	vector<Symbols> input_symbols = LanguageExpressions("abce_2d").symbols;
	Wll1Loader loader(input_symbols);
	string ident;
	EXPECT_TRUE(loader.LoadIdent(ident));
}

TEST(Wll1LoaderTest,LoadString)
{
	vector<Symbols> input_symbols = LanguageExpressions("dfkeife(*&*3\">><").symbols;
	Wll1Loader loader(input_symbols);
	LanguageExpressions str;
	EXPECT_TRUE(loader.LoadString(str));
}

TEST(Wll1LoaderTest,ExpectLetter)
{
	vector<Symbols> input_symbols = LanguageExpressions("B").symbols;
	Wll1Loader loader(input_symbols);
	char c;
	EXPECT_TRUE(loader.ExpectLetter(c));
	EXPECT_TRUE(c=='B');
}

TEST(Wll1LoaderTest,ExpectDigit)
{
	vector<Symbols> input_symbols = LanguageExpressions("8").symbols;
	Wll1Loader loader(input_symbols);
	char c;
	EXPECT_TRUE(loader.ExpectDigit(c));
	EXPECT_TRUE(c=='8');
}

TEST(Wll1LoaderTest,ExpectCharacter)
{
	vector<Symbols> input_symbols = LanguageExpressions("&").symbols;
	Wll1Loader loader(input_symbols);
	Symbols c;
	EXPECT_TRUE(loader.ExpectCharacter(c));
	EXPECT_TRUE(c=='&');
}

TEST(Wll1LoaderTest,ExpectSpace)
{
	vector<Symbols> input_symbols = LanguageExpressions("\n").symbols;
	Wll1Loader loader(input_symbols);
	char c;
	EXPECT_TRUE(loader.ExpectSpace(c));
	EXPECT_TRUE(c=='\n');
}

TEST(Wll1LoaderTest,SkipSpaces)
{
	vector<Symbols> input_symbols = LanguageExpressions("\n\n \t").symbols;
	Wll1Loader loader(input_symbols);
	EXPECT_TRUE(loader.SkipSpaces());
}

TEST(Wll1LoaderTest,AcceptSymbol)
{
	vector<Symbols> input_symbols = LanguageExpressions("^").symbols;
	Wll1Loader loader(input_symbols);
	Symbols symbol('^');
	EXPECT_TRUE(loader.Accept(symbol));
}

TEST(Wll1LoaderTest,AcceptExpression)
{
	vector<Symbols> input_symbols = LanguageExpressions("abckdle").symbols;
	Wll1Loader loader(input_symbols);
	LanguageExpressions expression("abckdle");
	EXPECT_TRUE(loader.Accept(expression));
}

TEST(Wll1LoaderTest,Encount)
{
	vector<Symbols> input_symbols = LanguageExpressions("^").symbols;
	Wll1Loader loader(input_symbols);
	EXPECT_TRUE(loader.Encount('^'));
}

TEST(Wll1LoaderTest,GetSymbol)
{
	vector<Symbols> input_symbols = LanguageExpressions("c").symbols;
	Wll1Loader loader(input_symbols);
	EXPECT_TRUE(loader.GetSymbol()=='c');
}

