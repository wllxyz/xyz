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
#include "VariableNode.h"

Wll0Intepreter::Wll0Intepreter(const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations)
:WllIntepreter(input_symbols, output_symbols, translations)
{

}

bool Wll0Intepreter::IntepretWll()
{
	if( !this->input_symbols.empty() && this->input_symbols.front()==Symbols::REMARK_WLL0 )
	{
		Wll0Loader loader(this->input_symbols);
		this->translations.clear();
		if(!loader.LoadWll(this->translations))
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
		this->output_symbols = this->input_symbols;
		return true;
	}
}
