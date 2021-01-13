/*
 * Wll3Intepreter.cpp
 *
 *  Created on: 2016-1-20
 *      Author: wll
 */

#include "Wll3Intepreter.h"
#include "Wll1Loader.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>
#include "WllCompactCommand.h"
#include "IntepretException.h"
#include "Calculate.h"


//<input>--><expression>
bool Wll3Intepreter::IntepretWll(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols)
{
	INFO("start IntepretWll, symbols="<<input_symbols);
	bool eval_flag = true;
	vector<size_t> compact_quote_starts;
	vector<size_t> ignore_quote_starts;

	for (vector<Symbols>::const_iterator i = input_symbols.begin(); i != input_symbols.end(); i++)
	{
		if (!eval_flag && !(*i == Symbols::EXEC) && !(*i == Symbols::REMARK_IGNORE) && !(*i == Symbols::IGNORE_BEGIN) && !(*i == Symbols::IGNORE_END))
		{
			output_symbols.push_back(*i);
		}
		else if (*i == Symbols::REMARK_IGNORE)
		{
			assert(i+1 != input_symbols.end());
			if (!eval_flag && !(*(i+1) == Symbols::IGNORE_BEGIN) && !(*(i+1) == Symbols::IGNORE_END))
			{
				output_symbols.push_back(*i);
			}
			output_symbols.push_back(*++i);
		}
		else if (*i == Symbols::IGNORE_BEGIN)
		{
			ignore_quote_starts.push_back(output_symbols.size());
			eval_flag = false;
		}
		else if (*i == Symbols::IGNORE_END)
		{
			assert(!ignore_quote_starts.empty());
			size_t start = ignore_quote_starts.back();
			ignore_quote_starts.pop_back();
			assert(output_symbols.begin()+start <= output_symbols.end());
			Symbols compacted_symbol(COMPACT_SYMBOL, vector<Symbols>(output_symbols.begin()+start, output_symbols.end()));
			output_symbols.resize(start);
			output_symbols.push_back(compacted_symbol);
			if (ignore_quote_starts.empty())
			{
				eval_flag = true;
			}	
		}
		else if (*i == Symbols::COMPACT_BEGIN)
		{
			compact_quote_starts.push_back(output_symbols.size());
		}
		else if (*i == Symbols::COMPACT_END)
		{
			assert(!compact_quote_starts.empty());
			size_t start = compact_quote_starts.back();
			compact_quote_starts.pop_back();
			assert(output_symbols.begin()+start <= output_symbols.end());
			Symbols compacted_symbol(COMPACT_SYMBOL, vector<Symbols>(output_symbols.begin()+start, output_symbols.end()));
			output_symbols.resize(start);
			output_symbols.push_back(compacted_symbol);
		}
		else if (i->IsRemark() && eval_flag || (*i == Symbols::EXEC)) 
		{
			if (!IntepretCommand(*i, output_symbols, this))
			{
				ERROR("IntepretCommand" << *i << "failed!!!");
				return false;
			}	
		}	
		else
		{
			output_symbols.push_back(*i);
		}
	}
	assert(compact_quote_starts.empty());

	return true;
}
