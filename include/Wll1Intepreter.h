/*
 * Wll1Intepreter.h
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */

#ifndef WLL1_INTEPRETER_H
#define WLL1_INTEPRETER_H

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "WllIntepreter.h"
#include <vector>

class Wll1Intepreter : public WllIntepreter
{
public:
	Wll1Intepreter(Compiler* compiler) : WllIntepreter(compiler) { }
	virtual bool IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols);
};

#endif /* WLL1_INTEPRETER_H */
