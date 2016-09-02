/*
 * Compiler.cpp
 *
 *  Created on: 2016年9月1日
 *      Author: wll
 */

#include "Compiler.h"
#include "Wll1Loader.h"
#include "Wll1Intepreter.h"
#include "LR1Parsers.h"
#include "LanguageAlgorithm.h"
#include "WllUtil.h"
#include <fstream>
#include "WllTrace.h"
#include <vector>
#include <cstring>
#include <cassert>
using namespace std;

Compiler::Compiler()
{
	this->LoadLanguage();
	this->parser_strategy.Set(new LR1Parsers(this));
	this->intepreter_strategy.Set(new Wll1Intepreter(this));
}

bool Compiler::LoadLanguage()
{
	char grammar_filename[Wll::Tool::LogInnerTypes::MAX_FILE_NAME_LENGTH];
	Wll::Util::GetProcessDir(grammar_filename, sizeof(grammar_filename));
	strcat(grammar_filename,"/../");
	strcat(grammar_filename, GRAMMAR_FILE_NAME);

	return ::LoadLanguage(grammar_filename, this->languages);
}

bool Compiler::LoadLanguage(const char* grammar_filename)
{
	return ::LoadLanguage(grammar_filename, this->languages);
}

bool Compiler::LoadLanguage(std::istream& input_stream)
{
	return  ::LoadLanguage(input_stream, this->languages);
}

bool Compiler::Process(istream& inf,ostream& outf)
{
	Symbols start_symbol = this->languages.GetDefaultStartSymbol();
	assert(!(start_symbol == Symbols::NULL_SYMBOL));
	vector<Symbols> input_symbols, output_symbols;
	inf >> input_symbols;
	bool retval = this->Process(input_symbols, output_symbols, start_symbol);
	outf<<output_symbols;

	return retval;
}

bool Compiler::Process(const vector<Symbols>& input_symbols, vector<Symbols>& output_symbols, Symbols start_symbol)
{
	LanguageTree* source_tree = NULL;
	LanguageTree* destination_tree = NULL;

	LanguageParsers* parser = this->parser_strategy.Get();
	if(!parser->Parse(this->languages, input_symbols, source_tree, start_symbol))
	{
		DestroyTree(source_tree);
		INFO("Parse failed");
		return false;
	}

	if(!TranslateTree(source_tree, destination_tree, this->languages.destination_rules.rules))
	{
		DestroyTree(source_tree);
		DestroyTree(destination_tree);
		INFO("Translate failed");
		return false;
	}

	vector<Symbols> translate_output_symbols;
	DisplayTreeLeaves(translate_output_symbols, destination_tree);

	INFO("translate_output_symbols="<<translate_output_symbols);

	WllIntepreter* intepreter = this->intepreter_strategy.Get();
	if(!intepreter->IntepretWll(translate_output_symbols, output_symbols))
	{
		DestroyTree(source_tree);
		DestroyTree(destination_tree);
		INFO("Intepreter failed");
		return false;
	}

	DestroyTree(source_tree);
	DestroyTree(destination_tree);

	return true;
}

//for debug
void Compiler::DisplayLanguage()
{
	cout<<this->languages<<endl;
}

