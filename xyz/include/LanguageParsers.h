#ifndef	LANGUAGE_PARSERS_H
#define LANGUAGE_PARSERS_H

#include "LanguageTypes.h"
#include "FirstSet.h"
#include <vector>
#include <set>
#include <map>
#include <Wll0Loader.h>
using namespace std;

static const char* GRAMMAR_FILE_NAME = "data/grammar";

//common language parser class
class LanguageParsers
{
public:
	LanguageParsers();
	LanguageParsers(const LanguageParsers& that);
	virtual ~LanguageParsers();
public:
	//input istream --> output ostream
	virtual bool Process(istream& inf,ostream& outf);
	virtual bool Process(const vector<Symbols>& input_symbols, vector<Symbols>& output_symbols, Symbols start_symbol);
	//for debug display languages
	virtual void DisplayLanguage();
	virtual void DisplayStates();
public:
	//input stream --> input symbols
	virtual bool LoadInput(istream& ins, vector<Symbols>& input_symbols);
	//set symbols as input symbols
	virtual void SetInput(const vector<Symbols>& symbols);
	//设置文法开始符号
	virtual void SetStartSymbol(Symbols start_symbol);
	//获得默认文法开始符号(第一条文法的root_symbol)
	virtual Symbols GetDefaultStartSymbol();
	//input symbols --> source parse grammar tree
	virtual bool Parse()=0;
	//source parse grammar tree --> destination parse grammar tree --> stand output ostream
	virtual bool Translate();
	//input symbols --> output symbols
	virtual bool ParseAndTranslate();
protected:
	virtual bool IsXyzLanguage(const vector<Symbols>& symbols);
	//calculate first sets of symbols
	virtual bool AnalyzeLanguage();
public:
	//load default wll0 language
	virtual bool LoadLanguage();
	//load language from input stream
	virtual bool LoadLanguage(istream& ins);
protected:
	vector<Symbols> input_symbols;
	vector<Symbols> output_symbols; 
	LanguageTree *source_tree; 
	LanguageTree *destination_tree; 
	LanguageGrammar languages;
	FirstCalculator first_calculator;
	Symbols start_symbol;
	bool is_analyzed;
};//end of LanguageParsers

#endif //LANGUAGE_PARSERS_H

