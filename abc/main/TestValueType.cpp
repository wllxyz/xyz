/*
 * TestValueType.cpp
 *
 *  Created on: Sep 2, 2015
 *      Author: wll
 */

#define private public

#include "ValueType.h"
#include <iostream>
#include <assert.h>
using namespace std;
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ValueTypeTest,Constructor)
{
	ValueType a,b("abc"),c(b);
	EXPECT_TRUE(a==ValueType(""));
	EXPECT_TRUE(b==ValueType("abc"));

	EXPECT_TRUE(!b.IsNature());
	EXPECT_TRUE(!a.IsNature());
	EXPECT_TRUE(ValueType("123").IsNature());
}

TEST(ValueTypeTest,Compare)
{
	EXPECT_TRUE(ValueType("ab")<ValueType("ac"));
	EXPECT_TRUE(ValueType("ab")<=ValueType("ac"));
	EXPECT_TRUE(ValueType("2")>ValueType("124"));
	EXPECT_TRUE(ValueType("2")>=ValueType("124"));
	EXPECT_TRUE(ValueType("2")!=ValueType("124"));
}

TEST(NatureTest,Compare)
{
	EXPECT_TRUE(Nature("233")<Nature("1234"));
	EXPECT_TRUE(Nature("12")==Nature("12"));
	EXPECT_TRUE(Nature("12")>Nature("5"));
	EXPECT_TRUE(Nature("12")>=Nature("5"));
	EXPECT_TRUE(Nature("233")<=Nature("1234"));
	EXPECT_TRUE(Nature("233")!=Nature("1234"));
}

TEST(NatureTest,Calculate_ADD)
{
	Nature x("128907");
	Nature y("23456");
	Nature z = x + y;
	EXPECT_TRUE(z==Nature("152363"));

	Nature x1("99999");
	Nature y1("1");
	EXPECT_TRUE(x1+y1==Nature("100000"));
	EXPECT_TRUE((x1+=y1)==Nature("100000"));
}

TEST(NatureTest,Calculate_SUB)
{
	Nature x2("10000");
	Nature y2("1");
	Nature z2=x2-y2;
	x2-=y2;
	EXPECT_TRUE(z2==Nature("9999"));
	EXPECT_TRUE(x2==Nature("9999"));
}

TEST(NatureTest,Calculate_MUL_DIV)
{
	EXPECT_TRUE(Nature("23")*Nature("50")==Nature("1150"));
	EXPECT_TRUE(Nature("120")/Nature("6")==Nature("20"));
}

TEST(IntegerTest,Calculate_ADD_SUB)
{
	Integer x3("-10000");
	Integer y3("9999");
	EXPECT_TRUE(x3+y3==Integer("-1"));
	EXPECT_TRUE(Integer("123")+Integer("987")==Integer("1110"));
	EXPECT_TRUE(Integer("0")+Integer("-123")==Integer("-123"));
	EXPECT_TRUE(Integer("-123")+Integer("0")==Integer("-123"));
	EXPECT_TRUE(Integer("-123")+Integer("-123")==Integer("-246"));
	EXPECT_TRUE(Integer("123")+Integer("-987")==Integer("-864"));
	EXPECT_TRUE(Integer("-123")+Integer("987")==Integer("864"));
	EXPECT_TRUE(Integer("123")-Integer("987")==Integer("-864"));
	EXPECT_TRUE(Integer("0")-Integer("-987")==Integer("987"));
}

TEST(IntegerTest,Calculate_MUL_DIV)
{
	EXPECT_TRUE(Integer("-220")/Integer("-11")==Integer("20"));
	EXPECT_TRUE(Integer("35")*Integer("-2")==Integer("-70"));
	try{
		Integer a = Integer("200")/Integer("0");
	}
	catch(const DivZeroException& e)
	{
		cout<<e<<endl;
	}
}

