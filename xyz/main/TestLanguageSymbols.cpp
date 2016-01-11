#include "LanguageSymbols.h"
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace Wll;
using namespace std;
#include "gtest/gtest.h"

TEST(LanguageSymbolTest, Construstor)
{
	Symbols unknow_symbol;
	Symbols variable_symbol("variable");
	Symbols constant_symbol('c');
	Symbols remark_symbol(REMARK_SYMBOL,"REMARK");
	EXPECT_TRUE(!unknow_symbol.IsVariable() && !unknow_symbol.IsConstant() && !unknow_symbol.IsRemark());
	EXPECT_TRUE(variable_symbol.IsVariable());
	EXPECT_TRUE(constant_symbol.IsConstant());
	EXPECT_TRUE(remark_symbol.IsRemark());
}

TEST(LanguageSymbolTest, OutputAndEqual)
{
	Symbols unknow_symbol;
	Symbols variable_symbol("<variable>");
	Symbols constant_symbol('c');
	Symbols remark_symbol(REMARK_SYMBOL,"$REMARK");

	stringstream sv;
	sv<<variable_symbol;
	EXPECT_TRUE(sv.str()=="<variable>");

	stringstream sc;
	sc<<constant_symbol;
	EXPECT_TRUE(sc.str()=="c");

	stringstream sr;
	sr<<remark_symbol;
	EXPECT_TRUE(sr.str()=="$REMARK");
}

TEST(LanguageSymbolTest, Compare)
{
	Symbols s1('a');
	Symbols s2('a');
	Symbols s3('b');
	EXPECT_TRUE(s1==s2);
	EXPECT_TRUE(s2<s3);
}

TEST(LanguageSymbolTest, Split)
{
	vector<Symbols> symbols;
	vector< vector<Symbols> > fields;
	int n;
	n = Split(symbols, Symbols::NULL_SYMBOL, fields);
	EXPECT_TRUE(n==0);
	EXPECT_TRUE(fields.size()==0);

	symbols.push_back('a');
	symbols.push_back('b');
	symbols.push_back(Symbols::NULL_SYMBOL);
	n = Split(symbols, Symbols::NULL_SYMBOL, fields);
	EXPECT_TRUE(n==2);
	EXPECT_TRUE(fields.size()==2);
	stringstream s;
	s<<fields[0];
	EXPECT_TRUE(s.str()=="ab");
	EXPECT_TRUE(fields[1].empty());

	symbols.push_back('a');
	symbols.push_back('b');
	symbols.push_back(Symbols("<variable>"));
	symbols.push_back(Symbols::NULL_SYMBOL);
	n = Split(symbols, Symbols::NULL_SYMBOL, fields);
	EXPECT_TRUE(n==3);
	vector<Symbols> field;
	field.push_back('a');
	field.push_back('b');
	field.push_back(Symbols("<variable>"));
	//stringstream s1,s2;
	//s1<<fields[1];
	//s2<<field;
	//cout<<s1.str()<<"=="<<s2.str()<<endl;
	//EXPECT_TRUE(s1.str()==s2.str());
	EXPECT_TRUE(fields[1]==field);

	for(int i=0; i<n; i++)
	{
		cout<<fields[i]<<endl;
	}
}

GTEST_API_ int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
