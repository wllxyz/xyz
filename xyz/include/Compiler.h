/*
 * Compiler.h
 *
 *  Created on: 2016年8月31日
 *      Author: wll
 */

#ifndef INCLUDE_COMPILER_H_
#define INCLUDE_COMPILER_H_

#include "LanguageGrammar.h"
#include "LanguageParsers.h"
#include "WllIntepreter.h"
#include "Strategy.h"
#include <iostream>
#include <vector>

static const char* GRAMMAR_FILE_NAME = "data/grammar";

class WllIntepreter;
class LanguageParsers;
//单例模式,减少Compiler指针传递
class Compiler
{
public:
	Compiler();
public:
	//加载wll语言文法
	virtual bool LoadLanguage();
	virtual bool LoadLanguage(const char* grammar_file);
	virtual bool LoadLanguage(std::istream& input_stream);
public:
	virtual bool Process(std::istream& inf, std::ostream& outf);
	virtual bool Process(const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, Symbols start_symbol);
	virtual bool Process(const LanguageGrammar& languages, const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, Symbols start_symbol);
	virtual bool Process(const std::string& grammar_file_name, const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, Symbols start_symbol);
public:
	//for debug display languages
	virtual void DisplayLanguage();
public:
	LanguageGrammar languages;
	//Strategy<WllLoader> loader_strategy;
	//Strategy<SymbolsFlow> flow_strategy;
	Strategy<LanguageParsers> parser_strategy;
	Strategy<WllIntepreter> intepreter_strategy;
};

#endif /* INCLUDE_COMPILER_H_ */
