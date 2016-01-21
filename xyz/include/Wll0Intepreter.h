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
#include "WllIntepreter.h"
#include <vector>

class Wll0Intepreter : public WllIntepreter
{
public:
	Wll0Intepreter();
	virtual bool IntepretWll(const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>* translations);
};

#endif /* WLL0_INTEPRETER_H */
