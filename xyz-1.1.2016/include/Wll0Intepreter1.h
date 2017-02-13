/*
 * Wll0Intepreter.h
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */

#ifndef WLL0_INTEPRETER1_H
#define WLL0_INTEPRETER1_H

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "Wll0Intepreter.h"
#include <vector>

class Wll0Intepreter1 : public Wll0Intepreter
{
public:
	Wll0Intepreter1(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations);
	virtual bool IntepretWll0();
};

#endif /* WLL0_INTEPRETER1_H */
