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

bool Wll2IntepreterCompactLL1Impl::IntepretExpression(const Symbols& compected_symbol, std::vector<Symbols>& data_stack)
{
	if (compected_symbol.type == COMPACT_SYMBOL)
	{
		vector<Symbols> result;
		for(vector<Symbols>::const_iterator i = compected_symbol.GetList().begin(); i != compected_symbol.GetList().end(); i++)
		{
			if (i->type == S_EXP_SYMBOL)
			{
				if (!this->IntepretSExpression(*i, result))
				{
					INFO("IntepretSExpression failed");
					return false;				
				}
			}
			else
			{
				result.push_back(*i);
			}	
		}
		data_stack.push_back(Encode(result, false));
	}
	else if (compected_symbol.type == S_EXP_SYMBOL)
	{
		if (!this->IntepretSExpression(compected_symbol, data_stack))
		{
			INFO("IntepretSExpression failed");
			return false;				
		}		
	}
	else
	{
		data_stack.push_back(compected_symbol);
	}

	return true;
}

//<quote-expression>-->$LEFT_QUOTE<expression-list>$RIGHT_QUOTE
bool Wll2IntepreterCompactLL1Impl::IntepretSExpression(const Symbols& s_exp_symbol,std::vector<Symbols>& result)
{
	assert(s_exp_symbol.type == S_EXP_SYMBOL);
	assert(s_exp_symbol.GetList().size() > 0);

	bool retval = true;
	bool local_eval_switch;
	local_eval_switch = this->eval_switch;

	vector<Symbols> cmd;
	this->IntepretExpression(s_exp_symbol.GetList().front(), cmd);
	assert(!cmd.empty());
	Symbols symbol = cmd.back();
	assert(symbol.IsRemark());

	if(symbol == Symbols::REMARK_IGNORE)
	{
		this->eval_switch = false;
	}
	else if(symbol == Symbols::EXEC)
	{
		this->eval_switch = true;			//$EXEC的子节点开启求值(允许内层屏蔽节点先求值)
	}

	if(local_eval_switch || symbol == Symbols::EXEC)    //EXEC命令本身也需要求值
	{
		//<expression-list>--><expression>$SEPERATOR<expression-list>
		for(vector<Symbols>::const_iterator i = s_exp_symbol.GetList().begin() + 1; i != s_exp_symbol.GetList().end(); i++)
		{
			if(!this->IntepretExpression(*i, result)) 
			{
				INFO("IntepretExpression failed at the parameter part");
				return false;
			}
		}

		try{
			retval = IntepretCommand(symbol, result, this->intepreter);
		}
		catch(IntepretException& e)
		{
			TERM_ERROR(e);
			retval = false;
		}
	}
	else
	{
		result.push_back(s_exp_symbol);
	}

	this->eval_switch = local_eval_switch;

	return retval;
}
