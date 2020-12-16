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
#include "WllCommand.h"
#include "IntepretException.h"

Wll2IntepreterCompactLL1Impl::Wll2IntepreterCompactLL1Impl(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, WllIntepreter* intepreter)
:input_symbols(input_symbols),output_symbols(output_symbols),intepreter(intepreter)
{
	this->eval_switch = true;
}

//<input>--><expression>
bool Wll2IntepreterCompactLL1Impl::IntepretWll()
{
	INFO("start IntepretWll, symbols="<<this->input_symbols);
	Symbols compected_input_symbol(COMPACT_SYMBOL);
	if (!CompactSExpression(this->input_symbols, compected_input_symbol)) {
		ERROR("compact input_symbols failed");
		return false;
	}
	INFO("compected_input_symbol=" << compected_input_symbol.ToString());

	return this->IntepretExpression(compected_input_symbol, this->output_symbols);

}

bool Wll2IntepreterCompactLL1Impl::IntepretExpression(const Symbols& compected_symbol, std::vector<Symbols>& result)
{
	if (compected_symbol.type == COMPACT_SYMBOL)
	{
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
	}
	else if (compected_symbol.type == S_EXP_SYMBOL)
	{
		if (!this->IntepretSExpression(compected_symbol, result))
		{
			INFO("IntepretSExpression failed");
			return false;				
		}		
	}
	else
	{
		result.push_back(compected_symbol);
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

	std::vector<Symbols> cmd;
	if(!this->IntepretExpression(s_exp_symbol.GetList().front(), cmd)) 
	{
		INFO("IntepretExpression failed at the cmd part");
		return false;
	}
	Symbols symbol = cmd[0];
	assert(symbol.IsRemark());

	if(symbol == Symbols::REMARK_IGNORE)
	{
		this->eval_switch = false;
	}
	else if(symbol == Symbols::EXEC)
	{
		this->eval_switch = true;			//$EXEC的子节点开启求值(允许内层屏蔽节点先求值)
	}

	vector<vector<Symbols> > parameter_fields;
	parameter_fields.push_back(cmd);

	//<expression-list>--><expression>$SEPERATOR<expression-list>
	for(vector<Symbols>::const_iterator i = s_exp_symbol.GetList().begin() + 1; i != s_exp_symbol.GetList().end(); i++)
	{
		vector<Symbols> parameter;
		if(!this->IntepretExpression(*i, parameter)) 
		{
			INFO("IntepretExpression failed at the parameter part");
			return false;
		}
		parameter_fields.push_back(parameter);
	}

	if(local_eval_switch || symbol == Symbols::EXEC)    //EXEC命令本身也需要求值
	{
		WllCommand* command = WllCommandFactory::CreateCommand(symbol, parameter_fields,this->intepreter);
		assert(command!=NULL);
		vector<Symbols> command_result;
		try{
			retval = command->Intepret(command_result);
		}
		catch(IntepretException& e)
		{
			TERM_ERROR(e);
			retval = false;
		}
		
		INFO(symbol<<" command_result="<<command_result);
		result += command_result;
		delete command;
	}
	else
	{
		result.push_back(s_exp_symbol);
	}

	this->eval_switch = local_eval_switch;

	return retval;
}



