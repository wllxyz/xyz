/*
 * Wll2Intepreter.cpp
 *
 *  Created on: 2016-1-5
 *      Author: wll
 */

#include "Wll2Intepreter.h"
#include "Wll2IntepreterLL1Impl.h"

bool Wll2Intepreter::IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>* translations)
{
	Wll2IntepreterLL1Impl intepreter(input_symbols, output_symbols, translations,this);
	return intepreter.IntepretWll();
}
