/*
 * Wll2Intepreter.cpp
 *
 *  Created on: 2016-1-5
 *      Author: wll
 */

#include "Wll2Intepreter.h"
#include "Wll2IntepreterLL1Impl.h"
#include "Wll2IntepreterCompactLL1Impl.h"

bool Wll2Intepreter::IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols)
{
	Wll2IntepreterCompactLL1Impl intepreter(input_symbols, output_symbols, this);
	//Wll2IntepreterLL1Impl intepreter(input_symbols, output_symbols, this);
	return intepreter.IntepretWll();
}
