/*
 * Wll0Intepreter.cpp
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */


#include "Wll0Intepreter.h"
#include "Wll0Loader.h"
#include "WllSingleton.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>

bool Wll0Intepreter::IntepretWll(const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols)
{
	if( !input_symbols.empty() && input_symbols.front()==Symbols::REMARK_WLL0 )
	{
		this->compiler->languages.translation_rules.clear();
		Wll0Loader loader(input_symbols);
		if(!loader.Load( this->compiler->languages))
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
