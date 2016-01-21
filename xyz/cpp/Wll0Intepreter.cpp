/*
 * Wll0Intepreter.cpp
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */


#include "Wll0Intepreter.h"
#include "Wll0Loader.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>

bool Wll0Intepreter::IntepretWll(const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>* translations)
{
	if( !input_symbols.empty() && input_symbols.front()==Symbols::REMARK_WLL0 )
	{
		Wll0Loader loader(input_symbols);
		translations->clear();
		if(!loader.LoadWll(*translations))
		{
			loader.ShowErrorMessage();
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		output_symbols = input_symbols;
		return true;
	}
}
