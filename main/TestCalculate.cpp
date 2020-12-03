#include "Calculate.h"
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace Wll;
using namespace std;
#include "gtest/gtest.h"

TEST(CalculateTest, CastTo_FromSingleSymbol)
{
	Symbols c(CHAR_SYMBOL, '0');
	Symbols i = CastTo(Symbols::INTEGER, c);
	EXPECT_TRUE(i == Symbols(48));
	Symbols l = CastTo(Symbols::LONG, i);
	EXPECT_TRUE(l == Symbols(48L));
	Symbols d = CastTo(Symbols::DOUBLE, l);
	cout<<d<<endl;
	EXPECT_TRUE(d == Symbols(48.0));
	Symbols s = CastTo(Symbols::STRING, d);
	cout<<s<<endl;
	EXPECT_TRUE(s == Symbols(STRING_SYMBOL, "48.000000"));
}

//Symbols CastTo(SymbolTypes type, string number_string);
TEST(CalculateTest, CastTo_FromString)
{
	Symbols c = CastTo(INTEGER_SYMBOL, string("123"));
	EXPECT_TRUE(c == Symbols(123));
	
	Symbols l = CastTo(LONG_SYMBOL, string("123456789"));
	EXPECT_TRUE(l == Symbols(123456789L));
	
	Symbols f = CastTo(FLOAT_SYMBOL, string("12345.6789"));
	EXPECT_TRUE(f == Symbols(12345.6789f));
	
	Symbols d = CastTo(DOUBLE_SYMBOL, string("12345.6789"));
	double d2 = 12345.6789;
	EXPECT_TRUE(d == Symbols(d2));
}

//Symbols CastToNumber(const vector<Symbols>& constant_symbols);
TEST(CalculateTest, CastToNumber)
{
	vector<Symbols> cs;
	cs += string("123");
	Symbols c = CastToNumber(cs);
	EXPECT_TRUE(c == Symbols(123));
	
	vector<Symbols> ls;
	ls += string("123456789");
	Symbols l = CastToNumber(ls);
	EXPECT_TRUE(l == Symbols(123456789L));
	
	vector<Symbols> fs;
	fs += string("12345.6789");
	Symbols f = CastToNumber(fs);
	EXPECT_TRUE(f == Symbols(12345.6789f));
	
	vector<Symbols> ds;
	ds += string("12345.6789012345");
	Symbols d = CastToNumber(ds);
	double d2 = 12345.6789012345;
	EXPECT_TRUE(d == Symbols(d2));
	
	vector<Symbols> s;
	s += string("HELLO");
	Symbols ss = CastToNumber(s);
	EXPECT_TRUE(ss == Symbols(STRING_SYMBOL,"HELLO"));	
}

//Symbols CastTo(const Symbols& targetType, std::vector<Symbols>& values);
TEST(CalculateTest, CastTo_FromConstantSymbols)
{
	vector<Symbols> cs;
	cs += string("123");
	Symbols c = CastTo(Symbols::INTEGER, cs);
	EXPECT_TRUE(c == Symbols(123));
	
	vector<Symbols> ls;
	ls += string("123456789");
	Symbols l = CastTo(Symbols::LONG, ls);
	EXPECT_TRUE(l == Symbols(123456789L));
	
	vector<Symbols> fs;
	fs += string("12345.6789");
	Symbols f = CastTo(Symbols::FLOAT, fs);
	EXPECT_TRUE(f == Symbols(12345.6789f));
	
	vector<Symbols> ds;
	ds += string("12345.6789012345");
	Symbols d = CastTo(Symbols::DOUBLE, ds);
	double d2 = 12345.6789012345;
	EXPECT_TRUE(d == Symbols(d2));
	
	vector<Symbols> s;
	s += string("12345.6789");
	Symbols ss = CastTo(Symbols::STRING, s);
	EXPECT_TRUE(ss == Symbols(STRING_SYMBOL, "12345.6789"));	
}

TEST(CalculateTest, CastTo_AutoTreateAsStringOrNumber)
{
	vector<Symbols> cs;
	cs += string("123");
	Symbols c = CastTo(cs,true);
	EXPECT_TRUE(c == Symbols(123));
	
	vector<Symbols> ls;
	ls.push_back(Symbols(123456789L));
	Symbols l = CastTo(ls);
	EXPECT_TRUE(l == Symbols(123456789L));
	
	vector<Symbols> fs;
	fs.push_back(Symbols(12345.6789f));
	Symbols f = CastTo(fs);
	EXPECT_TRUE(f == Symbols(12345.6789f));
	
	vector<Symbols> ds;
	ds.push_back(Symbols(12345.6789012345));
	Symbols d = CastTo(ds);
	EXPECT_TRUE(d == Symbols(12345.6789012345));
	
	vector<Symbols> s;
	s.push_back(Symbols(STRING_SYMBOL,"12345.6789"));
	Symbols ss = CastTo(s);
	EXPECT_TRUE(ss == Symbols(STRING_SYMBOL, "12345.6789"));	
}

//Symbols Add(const Symbols& a, const Symbols& b);
TEST(CalculateTest, Add)
{
	vector<Symbols> cs;
	cs += string("123");
	Symbols c = CastTo(Symbols::INTEGER, cs);
	EXPECT_TRUE(c == Symbols(123));
	
	vector<Symbols> ls;
	ls += string("123456789");
	Symbols l = CastTo(Symbols::LONG, ls);
	EXPECT_TRUE(l == Symbols(123456789L));
	
	vector<Symbols> fs;
	fs += string("12345.6789");
	Symbols f = CastTo(Symbols::FLOAT, fs);
	EXPECT_TRUE(f == Symbols(12345.6789f));
	
	vector<Symbols> ds;
	ds += string("12345.6789012345");
	Symbols d = CastTo(Symbols::DOUBLE, ds);
	double d2 = 12345.6789012345;
	EXPECT_TRUE(d == Symbols(d2));
	
	vector<Symbols> s;
	s += string("12345.6789");
	Symbols ss = CastTo(Symbols::STRING, s);
	EXPECT_TRUE(ss == Symbols(STRING_SYMBOL, "12345.6789"));
	
	cout<< Add(c, l) << endl;
	cout<< Add(l, f) << endl;
	cout<< Add(f, d) << endl;
	cout<< Add(d, ss) << endl;	
}

GTEST_API_ int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
