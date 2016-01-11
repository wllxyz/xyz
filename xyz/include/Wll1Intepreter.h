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
	Wll1Intepreter(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations);
	virtual bool IntepretWll();
};

#endif /* WLL1_INTEPRETER_H */
