#ifndef	LANGUAGE_PARSERS_H
#define LANGUAGE_PARSERS_H

#include "LanguageTypes.h"
#include "LanguageGrammar.h"
#include "Compiler.h"
#include <vector>
using namespace std;

class Compiler;
//common language parser class
class LanguageParsers
{
public:
	LanguageParsers(Compiler* compiler) { this->compiler = compiler; }
	//input symbols --> source parse grammar tree
	virtual bool Parse(const LanguageGrammar& languages, const std::vector<Symbols>& input_symbols, LanguageTree*& source_tree, Symbols start_symbol) = 0;

private:
	Compiler* compiler;
};//end of LanguageParsers

#endif //LANGUAGE_PARSERS_H

