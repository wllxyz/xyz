/*
 * Wll2Intepreter.h
 *
 *  Created on: 2016-1-5
 *      Author: wll
 */

#ifndef WLL2INTEPRETER_H_
#define WLL2INTEPRETER_H_

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "WllIntepreter.h"
#include <vector>

class Wll2Intepreter : public WllIntepreter
{
public:
	virtual bool IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>* translations);
};


#endif /* WLL2INTEPRETER_H_ */
