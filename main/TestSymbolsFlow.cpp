/*
 * TestSymbolsFlow.cpp
 *
 *  Created on: 2016年8月31日
 *      Author: wll
 */
#define private public
#include "SymbolsFlow.h"
#include <gtest/gtest.h>
#include <sstream>

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ArraySymbolsFlow, Test)
{
	std::vector<Symbols> empty_input_symbols;
	ArraySymbolsFlow empty_symbol_flow(empty_input_symbols);
	EXPECT_TRUE(empty_symbol_flow.GetSymbol() == Symbols::END_SYMBOL);

	std::vector<Symbols> input_symbols;
	input_symbols.push_back(Symbols('a'));
	input_symbols.push_back(Symbols('b'));
	input_symbols.push_back(Symbols("abc"));
	input_symbols.push_back(Symbols('\n'));
	input_symbols.push_back(Symbols(REMARK_SYMBOL, "$REMARK"));
	ArraySymbolsFlow symbol_flow(input_symbols);
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('a'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 1);
	EXPECT_TRUE(symbol_flow.Position() == 0);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('b'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 2);
	EXPECT_TRUE(symbol_flow.Position() == 1);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols("abc"));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 3);
	EXPECT_TRUE(symbol_flow.Position() == 2);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('\n'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 4);
	EXPECT_TRUE(symbol_flow.Position() == 3);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols(REMARK_SYMBOL, "$REMARK"));
	EXPECT_TRUE(symbol_flow.Row() == 2);
	EXPECT_TRUE(symbol_flow.Column() == 1);
	EXPECT_TRUE(symbol_flow.Position() == 4);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols::END_SYMBOL);
	EXPECT_TRUE(symbol_flow.Row() == 2);
	EXPECT_TRUE(symbol_flow.Column() == 2);
	EXPECT_TRUE(symbol_flow.Position() == 5);

	EXPECT_TRUE(!symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols::END_SYMBOL);
	EXPECT_TRUE(symbol_flow.Row() == 2);
	EXPECT_TRUE(symbol_flow.Column() == 2);
	EXPECT_TRUE(symbol_flow.Position() == 5);
}

TEST(StreamSymbolsFlow, Test)
{
	std::stringstream empty_input_symbols;
	StreamSymbolsFlow empty_symbol_flow(empty_input_symbols);
	EXPECT_TRUE(empty_symbol_flow.GetSymbol() == Symbols::END_SYMBOL);

	std::stringstream input_symbols;
	input_symbols<<"abc\nd";
	StreamSymbolsFlow symbol_flow(input_symbols);
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('a'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 1);
	EXPECT_TRUE(symbol_flow.Position() == 0);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('b'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 2);
	EXPECT_TRUE(symbol_flow.Position() == 1);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('c'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 3);
	EXPECT_TRUE(symbol_flow.Position() == 2);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('\n'));
	EXPECT_TRUE(symbol_flow.Row() == 1);
	EXPECT_TRUE(symbol_flow.Column() == 4);
	EXPECT_TRUE(symbol_flow.Position() == 3);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols('d'));
	EXPECT_TRUE(symbol_flow.Row() == 2);
	EXPECT_TRUE(symbol_flow.Column() == 1);
	EXPECT_TRUE(symbol_flow.Position() == 4);

	EXPECT_TRUE(symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols::END_SYMBOL);
	EXPECT_TRUE(symbol_flow.Row() == 2);
	EXPECT_TRUE(symbol_flow.Column() == 2);
	EXPECT_TRUE(symbol_flow.Position() == 5);

	EXPECT_TRUE(!symbol_flow.Advance());
	EXPECT_TRUE(symbol_flow.GetSymbol() == Symbols::END_SYMBOL);
	EXPECT_TRUE(symbol_flow.Row() == 2);
	EXPECT_TRUE(symbol_flow.Column() == 2);
	EXPECT_TRUE(symbol_flow.Position() == 5);
}
