/*
 * Wll2IntepreterCompactLL1Impl.cpp
 *
 *  Created on: 2016-1-20
 *      Author: wll
 */

#include "Wll2IntepreterCompactLL1Impl.h"
#include "Wll1Loader.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>
#include "WllCompactCommand.h"
#include "IntepretException.h"
#include "Calculate.h"

Wll2IntepreterCompactLL1Impl::Wll2IntepreterCompactLL1Impl(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, WllIntepreter* intepreter)
:input_symbols(input_symbols),output_symbols(output_symbols),intepreter(intepreter)
{
	this->eval_switch = true;
}

//<input>--><expression>
bool Wll2IntepreterCompactLL1Impl::IntepretWll()
{
	INFO("start IntepretWll, symbols="<<this->input_symbols);
	bool eval_flag = true;
	vector<size_t> left_quote_starts;

	for (vector<Symbols>::const_iterator i = this->input_symbols.begin(); i != this->input_symbols.end(); i++)
	{
		if (*i == Symbols::REMARK_IGNORE)
		{
			assert(i+1 != this->input_symbols.end());
			this->output_symbols.push_back(*++i);
		}
		else if (*i == Symbols::LEFT_QUOTE)
		{
			eval_flag = false;
			left_quote_starts.push_back(this->output_symbols.size());
		}
		else if (*i == Symbols::RIGHT_QUOTE)
		{
			assert(!left_quote_starts.empty());
			size_t start = left_quote_starts.back();
			left_quote_starts.pop_back();
			assert(this->output_symbols.begin()+start <= this->output_symbols.end());
			Symbols compacted_symbol(COMPACT_SYMBOL, vector<Symbols>(this->output_symbols.begin()+start, this->output_symbols.end()));
			this->output_symbols.resize(start);
			this->output_symbols.push_back(compacted_symbol);
			if (left_quote_starts.empty())
			{
				eval_flag = true;
			}
		}
		else if (i->IsRemark() && eval_flag || (*i == Symbols::EXEC)) 
		{
			if (!IntepretCommand(*i, this->output_symbols, this->intepreter))
			{
				ERROR("IntepretCommand" << *i << "failed!!!");
				return false;
			}	
		}	
		else
		{
			this->output_symbols.push_back(*i);
		}
	}
	assert(left_quote_starts.empty());

	return true;
}
