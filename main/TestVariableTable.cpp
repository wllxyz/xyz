/*
 * TestVariableTable.cpp
 *
 *  Created on: 2016-1-21
 *      Author: wll
 */
#define private public

#include "VariableTable.h"
#include <iostream>
#include <assert.h>
using namespace std;
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(VariableTableTest,Get_And_Set)
{
	VariableTable t;
	t["a"] = "a";
	t["b"] = "b";
	t["a.b.c"] = "abc";

	EXPECT_TRUE(t["a"]=="a");
	EXPECT_TRUE(t["b"]=="b");
	EXPECT_TRUE(t["a.b.c"]=="abc");
	EXPECT_TRUE(t["d"]=="");
	cout<<t.variable_table<<endl;
}

TEST(VariableTableTest,Has)
{
	VariableTable t;
	t["a"] = "a";
	t["b"] = "b";
	t["a.b.c"] = "abc";

	EXPECT_TRUE(t.Has("a"));
	EXPECT_TRUE(t.Has("b"));
	EXPECT_TRUE(t.Has("a.b.c"));
	EXPECT_TRUE(!t.Has("d"));
}

TEST(VariableNodeTest,Get_And_Set)
{
	VariableNode t;
	t["a"] = "a";
	t["b"] = "b";
	t["a"]["b"]["c"] = "abc";

	EXPECT_TRUE(t["a"].value=="a");
	EXPECT_TRUE(t["b"].value=="b");
	EXPECT_TRUE(t["a"]["b"]["c"].value=="abc");
	EXPECT_TRUE(t["d"].value=="");
}

