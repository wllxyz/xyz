/*
 * WllIntepreter.h
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */

#ifndef WLL_INTEPRETER_H
#define WLL_INTEPRETER_H

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include <vector>

class WllIntepreter
{
public:
	virtual bool IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>* translations)=0;
protected:
	int SplitParameters(const vector<Symbols>&symbols,  vector< vector<Symbols> >& fields);
};

#endif /* WLL_INTEPRETER_H */
