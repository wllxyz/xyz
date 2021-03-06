/*
 * Wll2IntepreterLL1ImplLL1Impl.cpp
 *
 *  Created on: 2016-1-20
 *      Author: wll
 */

#include "Wll2IntepreterLL1Impl.h"
#include "Wll1Loader.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>
#include "WllCommand.h"
#include "IntepretException.h"
#include "Calculate.h"

Wll2IntepreterLL1Impl::Wll2IntepreterLL1Impl(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, WllIntepreter* intepreter)
:input_symbols(input_symbols),output_symbols(output_symbols),intepreter(intepreter)
{
	this->input_pos = 0;
	this->eval_switch = true;
}

//<input>--><expression>
bool Wll2IntepreterLL1Impl::IntepretWll()
{
	INFO("start IntepretWll, symbols="<<this->input_symbols);

	if(!this->IntepretExpression(this->output_symbols))
	{
		this->ShowErrorMessage();
		return false;
	}
	return this->Encount(Symbols::END_SYMBOL);
}

bool Wll2IntepreterLL1Impl::IntepretExpression(std::vector<Symbols>& result)
{
	while(!(this->Encount(Symbols::END_SYMBOL) || this->Encount(Symbols::RIGHT_QUOTE) || this->Encount(Symbols::SEPERATOR)))
    {
	    if(this->Encount(Symbols::LEFT_QUOTE))
	    {
		    //<expression>--><sub-expression><expression>
		    //<sub-expression>--><quote-expression>
		    if(!this->IntepretSExpression(result))
		    {
		        INFO("IntepretSExpression failed");
		        return false;
		    }
	    }
	    else
	    {
		    //<expression>--><sub-expression><expression>
		    //<sub-expression>--><symbol>
		    result.push_back(this->AcceptSymbol());
	    }
	}

	return true;
}

//<quote-expression>-->$LEFT_QUOTE<expression-list>$RIGHT_QUOTE
bool Wll2IntepreterLL1Impl::IntepretSExpression(std::vector<Symbols>& result)
{
	bool retval = true;
	bool local_eval_switch;
	local_eval_switch = this->eval_switch;

	if(!this->Accept(Symbols::LEFT_QUOTE)) 
	{
	    INFO("Expect ( as beginning of S-Expression");
	    return false;
	}

	if(!this->Encount(Symbols::RIGHT_QUOTE))
	{
		std::vector<Symbols> cmd;
		if(!this->IntepretExpression(cmd)) 
		{
		    INFO("IntepretExpression failed at the cmd part");
		    return false;
		}

		Symbols symbol = cmd[0];

		if(symbol == Symbols::REMARK_IGNORE)
		{
			this->eval_switch = false;
		}
		else if(symbol == Symbols::EXEC)
		{
			this->eval_switch = true;			//$EXEC的子节点开启求值(允许内层屏蔽节点先求值)
		}

		vector<vector<Symbols> >parameter_fields;
		parameter_fields.push_back(cmd);

		//<expression-list>--><expression>$SEPERATOR<expression-list>
		while(this->Encount(Symbols::SEPERATOR))
		{
			this->Accept(Symbols::SEPERATOR);

			vector<Symbols> parameter;
			if(!this->IntepretExpression(parameter)) 
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
			ComposeSList(parameter_fields.begin(), parameter_fields.end(),result);
		}
	}//RIGHT_QUOTE
	if(!this->Accept(Symbols::RIGHT_QUOTE)) 
	{
	    INFO("Expect ) as end of S-Expression");
	    return false;
	}

	this->eval_switch = local_eval_switch;

	return retval;
}

bool Wll2IntepreterLL1Impl::Accept(const Symbols& symbol)
{
	if(this->Encount(symbol))
	{
		this->input_pos++;
		return true;
	}
	else
	{
		return false;
	}
}

bool Wll2IntepreterLL1Impl::Encount(const Symbols& symbol) const
{
	return this->GetSymbol()==symbol;
}

const Symbols& Wll2IntepreterLL1Impl::GetSymbol() const
{
	return (this->input_pos >= this->input_symbols.size()) ? Symbols::END_SYMBOL : this->input_symbols[this->input_pos];
}

const Symbols& Wll2IntepreterLL1Impl::AcceptSymbol()
{
	return (this->input_pos >= this->input_symbols.size()) ? Symbols::END_SYMBOL : this->input_symbols[this->input_pos++];
}

void Wll2IntepreterLL1Impl::ShowErrorMessage()
{
	TERM_ERROR("already accepted symbols: ");
	for(int i = 0; i<this->input_symbols.size(); i++)
	{
		if(i == this->input_pos)
			cerr<<"["<<this->input_symbols[i]<<"]"<<endl;
		else
			cerr<<this->input_symbols[i];
	}
}


