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
	Wll1Loader loader(LanguageExpressions("<source_rule>--><rule>==><source_rule>--><rule>$SOURCE_RULE;").symbols);
	vector<LanguageTranslations> translations;
	EXPECT_TRUE(loader.LoadWll(translations));
}

TEST(Wll1LoaderTest,LoadTranslation)
{
	Wll1Loader loader(LanguageExpressions("<a>-->\"abc\"==><a>-->\"bcd\";\n").symbols);
	LanguageTranslations translation;
	EXPECT_TRUE(loader.LoadTranslation(translation));
	cout<<translation<<endl;
}

TEST(Wll1LoaderTest,LoadSourceRule)
{
	Wll1Loader loader(LanguageExpressions("<abc>-->\"ab\"<b><bcd>").symbols);
	LanguageRules rule;
	EXPECT_TRUE(loader.LoadSourceRule(rule));
}

TEST(Wll1LoaderTest,LoadDestinationRule)
{
	Wll1Loader loader(LanguageExpressions("<abc>-->\"ab\"<b><bcd>").symbols);
	LanguageRules rule;
	EXPECT_TRUE(loader.LoadDestinationRule(rule));
}

TEST(Wll1LoaderTest,LoadRule)
{
	Wll1Loader loader(LanguageExpressions("<abc>-->\"ab\"<b><bcd>").symbols);
	LanguageRules rule;
	EXPECT_TRUE(loader.LoadRule(rule));
}

TEST(Wll1LoaderTest,LoadRootSymbol)
{
	Wll1Loader loader(LanguageExpressions("<abc>").symbols);
	Symbols symbol;
	EXPECT_TRUE(loader.LoadRootSymbol(symbol));
}

TEST(Wll1LoaderTest,LoadExpression)
{
	Wll1Loader loader(LanguageExpressions("<abc>\"dkjfdlf\"<bd>").symbols);
	LanguageExpressions expression;
	EXPECT_TRUE(loader.LoadExpression(expression));
}

TEST(Wll1LoaderTest,LoadSymbol)
{
	Wll1Loader loader(LanguageExpressions("<abc>").symbols);
	LanguageExpressions symbol;
	EXPECT_TRUE(loader.LoadSymbol(symbol));
}

TEST(Wll1LoaderTest,LoadVariable)
{
	Wll1Loader loader(LanguageExpressions("<abc>").symbols);
	Symbols symbol;
	EXPECT_TRUE(loader.LoadVariable(symbol));
}

TEST(Wll1LoaderTest,LoadConstant)
{
	Wll1Loader loader(LanguageExpressions("\"abcde\"").symbols);
	LanguageExpressions constant;
	EXPECT_TRUE(loader.LoadConstant(constant));
}

TEST(Wll1LoaderTest,LoadRemark)
{
	Wll1Loader loader(LanguageExpressions("$REMARK").symbols);
	Symbols remark;
	EXPECT_TRUE(loader.LoadRemark(remark));
}

TEST(Wll1LoaderTest,LoadIdent)
{
	Wll1Loader loader(LanguageExpressions("abce_2d").symbols);
	string ident;
	EXPECT_TRUE(loader.LoadIdent(ident));
}

TEST(Wll1LoaderTest,LoadString)
{
	Wll1Loader loader(LanguageExpressions("dfkeife(*&*3\">><").symbols);
	LanguageExpressions str;
	EXPECT_TRUE(loader.LoadString(str));
}

TEST(Wll1LoaderTest,ExpectLetter)
{
	Wll1Loader loader(LanguageExpressions("B").symbols);
	char c;
	EXPECT_TRUE(loader.ExpectLetter(c));
	EXPECT_TRUE(c=='B');
}

TEST(Wll1LoaderTest,ExpectDigit)
{
	Wll1Loader loader(LanguageExpressions("8").symbols);
	char c;
	EXPECT_TRUE(loader.ExpectDigit(c));
	EXPECT_TRUE(c=='8');
}

TEST(Wll1LoaderTest,ExpectCharacter)
{
	Wll1Loader loader(LanguageExpressions("&").symbols);
	Symbols c;
	EXPECT_TRUE(loader.ExpectCharacter(c));
	EXPECT_TRUE(c=='&');
}

TEST(Wll1LoaderTest,ExpectSpace)
{
	Wll1Loader loader(LanguageExpressions("\n").symbols);
	char c;
	EXPECT_TRUE(loader.ExpectSpace(c));
	EXPECT_TRUE(c=='\n');
}

TEST(Wll1LoaderTest,SkipSpaces)
{
	Wll1Loader loader(LanguageExpressions("\n\n \t").symbols);
	EXPECT_TRUE(loader.SkipSpaces());
}

TEST(Wll1LoaderTest,AcceptSymbol)
{
	Wll1Loader loader(LanguageExpressions("^").symbols);
	Symbols symbol('^');
	EXPECT_TRUE(loader.Accept(symbol));
}

TEST(Wll1LoaderTest,AcceptExpression)
{
	Wll1Loader loader(LanguageExpressions("abckdle").symbols);
	LanguageExpressions expression("abckdle");
	EXPECT_TRUE(loader.Accept(expression));
}

TEST(Wll1LoaderTest,Encount)
{
	Wll1Loader loader(LanguageExpressions("^").symbols);
	EXPECT_TRUE(loader.Encount('^'));
}

TEST(Wll1LoaderTest,GetSymbol)
{
	Wll1Loader loader(LanguageExpressions("c").symbols);
	EXPECT_TRUE(loader.GetSymbol()=='c');
}

