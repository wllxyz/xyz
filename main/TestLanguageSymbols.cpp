#include "LanguageSymbols.h"
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace Wll;
using namespace std;
#include "gtest/gtest.h"

union A
{
	int value;
	char c;
	int i;
	long l;
	float f;
	double d;
	shared_ptr<string>* s;
	shared_ptr< vector<Symbols> >* list;
	shared_ptr< map<string, Symbols> >* m;		
};

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

TEST(LanguageSymbolTest, GetList)
{
	Symbols list(LIST_SYMBOL);
	vector<Symbols>& l = list.GetList();
	l.push_back(Symbols(LIST_SYMBOL));
	l.push_back(Symbols('c'));
	l.push_back(Symbols("abc"));
	cout<<list.ToString()<<endl;
}

TEST(LanguageSymbolTest, StringSymbol)
{
	Symbols hi(STRING_SYMBOL, "hi");
	cout<<hi<<endl;
}

TEST(LanguageSymbolTest, GetMap)
{
	Symbols m2(MAP_SYMBOL);
	m2.GetMap()["f1"] = Symbols(10);
	m2.GetMap()["f2"] = Symbols(10.23);
	m2.GetMap()["f3"] = Symbols(MAP_SYMBOL);
	m2.GetMap()["f3"].GetMap()["f31"] = Symbols('A');
	cout<<m2.ToString()<<endl;
}

TEST(LanguageSymbolTest, CompactSExpression)
{
	vector<Symbols> input_symbols;
	input_symbols.push_back(Symbols::LEFT_QUOTE);
	input_symbols.push_back(Symbols::RIGHT_QUOTE);

	Symbols output_symbol(COMPACT_SYMBOL);
	Symbols expect_output_symbol(COMPACT_SYMBOL);
	expect_output_symbol.GetList().push_back(Symbols(S_EXP_SYMBOL));
	EXPECT_TRUE(CompactSExpression(input_symbols, output_symbol));
	EXPECT_TRUE(output_symbol == expect_output_symbol);

	vector<Symbols> input_symbols2;
	input_symbols2.push_back(Symbols::LEFT_QUOTE);
		input_symbols2.push_back(Symbols::REMARK_WLL0);
	input_symbols2.push_back(Symbols::SEPERATOR);
		input_symbols2.push_back(Symbols::LEFT_QUOTE);
			input_symbols2.push_back(Symbols::REMARK_TRANSLATION);
		input_symbols2.push_back(Symbols::SEPERATOR);
			input_symbols2.push_back(Symbols::LEFT_QUOTE);
				input_symbols2.push_back(Symbols::REMARK_RULE);
			input_symbols2.push_back(Symbols::SEPERATOR);
				input_symbols2.push_back(Symbols::LEFT_QUOTE);
					input_symbols2.push_back(Symbols::REMARK_VARIABLE);
				input_symbols2.push_back(Symbols::SEPERATOR);
					input_symbols2.push_back(Symbols("<i>"));
				input_symbols2.push_back(Symbols::RIGHT_QUOTE);
			input_symbols2.push_back(Symbols::SEPERATOR);
				input_symbols2.push_back(Symbols::LEFT_QUOTE);
					input_symbols2.push_back(Symbols::REMARK_CONSTANT);
				input_symbols2.push_back(Symbols::SEPERATOR);
				input_symbols2.push_back(Symbols::RIGHT_QUOTE);				
			input_symbols2.push_back(Symbols::RIGHT_QUOTE);
		input_symbols2.push_back(Symbols::SEPERATOR);
			input_symbols2.push_back(Symbols::LEFT_QUOTE);
				input_symbols2.push_back(Symbols::REMARK_RULE);
			input_symbols2.push_back(Symbols::SEPERATOR);
				input_symbols2.push_back(Symbols::LEFT_QUOTE);
					input_symbols2.push_back(Symbols::REMARK_VARIABLE);
				input_symbols2.push_back(Symbols::SEPERATOR);
					input_symbols2.push_back(Symbols("<o>"));
				input_symbols2.push_back(Symbols::RIGHT_QUOTE);
			input_symbols2.push_back(Symbols::SEPERATOR);
				input_symbols2.push_back(Symbols::LEFT_QUOTE);
					input_symbols2.push_back(Symbols::ADD);
				input_symbols2.push_back(Symbols::SEPERATOR);
					input_symbols2.push_back(Symbols('0'));
				input_symbols2.push_back(Symbols::SEPERATOR);
					input_symbols2.push_back(Symbols('1'));
					input_symbols2.push_back(Symbols('0'));
				input_symbols2.push_back(Symbols::RIGHT_QUOTE);			
			input_symbols2.push_back(Symbols::RIGHT_QUOTE);
		input_symbols2.push_back(Symbols::RIGHT_QUOTE);	
	input_symbols2.push_back(Symbols::RIGHT_QUOTE);

	Symbols output_symbol2(COMPACT_SYMBOL);
	Symbols expect_output_symbol2(COMPACT_SYMBOL);
	Symbols wll0(S_EXP_SYMBOL);
		Symbols translation(S_EXP_SYMBOL);
			Symbols source_rule(S_EXP_SYMBOL);
				Symbols i_variable(S_EXP_SYMBOL);
					i_variable.GetList().push_back(Symbols::REMARK_VARIABLE);
					i_variable.GetList().push_back(Symbols("<i>"));
				Symbols empty_constant(S_EXP_SYMBOL);
					empty_constant.GetList().push_back(Symbols::REMARK_CONSTANT);
					empty_constant.GetList().push_back(Symbols(COMPACT_SYMBOL));
				source_rule.GetList().push_back(Symbols::REMARK_RULE);
				source_rule.GetList().push_back(i_variable);
				source_rule.GetList().push_back(empty_constant);
			Symbols destination_rule(S_EXP_SYMBOL);
				Symbols o_variable(S_EXP_SYMBOL);
					o_variable.GetList().push_back(Symbols::REMARK_VARIABLE);
					o_variable.GetList().push_back(Symbols("<o>"));
				Symbols add(S_EXP_SYMBOL);
					add.GetList().push_back(Symbols::ADD);
					add.GetList().push_back(Symbols('0'));
					Symbols numbers(COMPACT_SYMBOL);
						numbers.GetList().push_back(Symbols('1'));
						numbers.GetList().push_back(Symbols('0'));
					add.GetList().push_back(numbers);
				destination_rule.GetList().push_back(Symbols::REMARK_RULE);
				destination_rule.GetList().push_back(o_variable);
				destination_rule.GetList().push_back(add);
			translation.GetList().push_back(Symbols::REMARK_TRANSLATION);
			translation.GetList().push_back(source_rule);
			translation.GetList().push_back(destination_rule);
		wll0.GetList().push_back(Symbols::REMARK_WLL0);
		wll0.GetList().push_back(translation);
	expect_output_symbol2.GetList().push_back(wll0);

	EXPECT_TRUE(CompactSExpression(input_symbols2, output_symbol2));
	cout<<output_symbol2<<endl;
	cout<<expect_output_symbol2<<endl;
	EXPECT_TRUE(output_symbol2 == expect_output_symbol2);	
}

GTEST_API_ int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
