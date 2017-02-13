/*
 * Wll0Intepreter.h
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */

#ifndef WLL0_INTEPRETER_H
#define WLL0_INTEPRETER_H

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include <vector>

class Wll0Intepreter
{
public:
	Wll0Intepreter(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations);
	virtual bool IntepretWll0()=0;
	std::vector<LanguageTranslations>& GetTranslations() { return translations; }
protected:
	int SplitParameters(const vector<Symbols>&symbols,  vector< vector<Symbols> >& fields);
protected:
	const std::vector<Symbols>& symbols;
	std::vector<Symbols>& output_symbols;
	std::vector<LanguageTranslations>& translations;
};

#endif /* WLL0_INTEPRETER_H */
