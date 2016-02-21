#ifndef	LANGUAGE_PARSERS_H
#define LANGUAGE_PARSERS_H

#include "LanguageTypes.h"
#include "FirstSet.h"
#include <vector>
#include <set>
#include <map>
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
	//for debug display languages
	virtual void DisplayLanguage();
	virtual void DisplayStates();
protected:
	//input stream --> input symbols
	virtual bool LoadInput(istream& ins);
	//set symbols as input symbols
	virtual void SetInput(const vector<Symbols>& symbols);
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
private:
	//LL(1) 分析算法 LoadLanguage
	bool LoadStart(LanguageGrammar& grammar);
	bool LoadWll0(vector<LanguageTranslations>& translations);
	bool LoadTranslation(LanguageTranslations& translation);
	bool LoadSourceRule(LanguageRules& source_rule);
	bool LoadDestinationRule(LanguageRules& destination_rule);
	bool LoadRule(LanguageRules& rule);
	bool LoadRootSymbol(Symbols& root_symbol);
	bool LoadExpression(LanguageExpressions& expression);
	bool LoadSymbol(LanguageExpressions& symbol);
	bool LoadVariable(Symbols& variable);
	bool LoadConstant(LanguageExpressions& constant);
	bool LoadRemark(Symbols& remark);
	bool LoadIdent(string& ident);
	bool LoadString(LanguageExpressions& str);
	bool ExpectLetter(char& c);
	bool ExpectDigit(char& c);
	bool ExpectCharacter(Symbols& symbol);
	bool ExpectSpace(char& c);
	bool SkipSpaces();
	bool Accept(const Symbols& symbol);
	bool Accept(const LanguageExpressions& expression);
	bool Encount(const Symbols& symbol);
	const Symbols& GetSymbol();
	int input_pos;
};//end of LanguageParsers

#endif //LANGUAGE_PARSERS_H

