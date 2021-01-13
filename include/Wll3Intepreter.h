/*
 * Wll3Intepreter.h
 *
 *  Created on: 2016-1-20
 *      Author: wll
 */

#ifndef WLL3INTEPRETER_H
#define WLL3INTEPRETER_H

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "WllIntepreter.h"
#include <vector>

class Wll3Intepreter : public WllIntepreter
{
public:
	Wll3Intepreter(Compiler* compiler) : WllIntepreter(compiler) { }
	virtual bool IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols);
};


#endif /* WLL3INTEPRETER_H */
