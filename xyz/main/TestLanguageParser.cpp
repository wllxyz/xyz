#include "TopDownParsers.h"
//#include "LR0Parsers.h"
#include "LR1Parsers.h"
#include <fstream>
#include <iostream>
using namespace std;
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(TopDownParser, Process)
{
	TopDownParsers parser;
	ifstream grammar_file("data/grammar");
	parser.LoadLanguage(grammar_file);
	vector<Symbols> input_symbols, output_symbols;
	Symbols start_symbol;
	input_symbols.push_back('<');
	input_symbols.push_back('t');
	input_symbols.push_back('e');
	input_symbols.push_back('s');
	input_symbols.push_back('t');
	input_symbols.push_back('>');
	start_symbol = Symbols("<variable>");
	EXPECT_TRUE(parser.Process(input_symbols, output_symbols, start_symbol));
	cout<<output_symbols<<endl;

	input_symbols.push_back('-');
	input_symbols.push_back('-');
	input_symbols.push_back('>');
	input_symbols.push_back('"');
	input_symbols.push_back('a');
	input_symbols.push_back('b');
	input_symbols.push_back('c');
	input_symbols.push_back('"');
	cout<<input_symbols<<endl;
	start_symbol = Symbols("<source_rule>");
	output_symbols.clear();
	EXPECT_TRUE(parser.Process(input_symbols, output_symbols, start_symbol));
	cout<<output_symbols<<endl;
}

TEST(LR1Parser, Process)
{
	LR1Parsers parser;
	ifstream grammar_file("data/grammar");
	parser.LoadLanguage(grammar_file);
	vector<Symbols> input_symbols, output_symbols;
	Symbols start_symbol;
	input_symbols.push_back('<');
	input_symbols.push_back('t');
	input_symbols.push_back('e');
	input_symbols.push_back('s');
	input_symbols.push_back('t');
	input_symbols.push_back('>');
	start_symbol = Symbols("<variable>");
	EXPECT_TRUE(parser.Process(input_symbols, output_symbols, start_symbol));
	cout<<output_symbols<<endl;

	input_symbols.push_back('-');
	input_symbols.push_back('-');
	input_symbols.push_back('>');
	input_symbols.push_back('"');
	input_symbols.push_back('a');
	input_symbols.push_back('b');
	input_symbols.push_back('c');
	input_symbols.push_back('"');
	cout<<input_symbols<<endl;
	start_symbol = Symbols("<source_rule>");
	output_symbols.clear();
	EXPECT_TRUE(parser.Process(input_symbols, output_symbols, start_symbol));
	cout<<output_symbols<<endl;
}
