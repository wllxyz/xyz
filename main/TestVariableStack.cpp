/*
 * TestVariableStack.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: wll
 */

#define private public
#include "VariableStack.h"
#include <gtest/gtest.h>
#include <sstream>

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(VariableStack, Test)
{
	VariableStack vs;
	vs.Push();
	Symbols* abc = vs.Register("abc");
	*abc = Symbols("c");
	EXPECT_TRUE(abc != NULL);
	Symbols* abc2 = vs.Lookup("abc");
	Symbols* abc3 = vs.Lookup("abc");
	EXPECT_TRUE(abc == abc2);
	EXPECT_TRUE(abc == abc3);
	EXPECT_TRUE(*abc == Symbols("c"));
	Symbols* b = vs.Lookup("b");
	EXPECT_TRUE(b == NULL);
	vs.Push();
	vs.Pop();
	Symbols* abc4 = vs.Lookup("abc");
	Symbols* abc5 = vs.Lookup("abc");
	EXPECT_TRUE(abc4 == abc5);
	EXPECT_TRUE(*abc4 == Symbols("c"));
	EXPECT_TRUE(abc == abc4);
}

TEST(VariableStack, Test2)
{
	VariableStack vs;
	vs.Push();
	Symbols* a = vs.Register("a");
	*a = Symbols(MAP_SYMBOL);
	Symbols* b = vs.Register("b");
	*b = *a;
}

