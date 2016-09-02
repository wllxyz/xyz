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

Wll2IntepreterLL1Impl::Wll2IntepreterLL1Impl(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, WllIntepreter* intepreter)
:input_symbols(input_symbols),output_symbols(output_symbols),intepreter(intepreter)
{
	this->input_pos = 0;
}

//<input>--><expression>
bool Wll2IntepreterLL1Impl::IntepretWll()
{
	INFO("symbols="<<this->input_symbols);

	if(!this->IntepretExpression(this->output_symbols))
	{
		this->ShowErrorMessage();
		return false;
	}
	return this->Encount(Symbols::END_SYMBOL);
}

bool Wll2IntepreterLL1Impl::IntepretExpression(std::vector<Symbols>& result)
{
	if(this->Encount(Symbols::END_SYMBOL) || this->Encount(Symbols::RIGHT_QUOTE) || this->Encount(Symbols::SEPERATOR)) return true;

	if(this->Encount(Symbols::REMARK_IGNORE))
	{
		//<expression>--><sub-expression><expression>
		//<sub-expression>--><ignore-expression>
		//<ignore-expression>-->$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE
		//$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE : 对括号内的符号不进行求值,原样输出
		this->Accept(Symbols::REMARK_IGNORE);
		if(!this->Accept(Symbols::LEFT_QUOTE)) return false;
		int depth = 1;
		while(!this->Encount(Symbols::END_SYMBOL))
		{
			if(this->Encount(Symbols::RIGHT_QUOTE) && depth == 1) break;
			if(this->Encount(Symbols::LEFT_QUOTE)) depth++;
			if(this->Encount(Symbols::RIGHT_QUOTE)) depth--;
			result.push_back(this->GetSymbol());
			this->input_pos++;
		}
		if(!this->Accept(Symbols::RIGHT_QUOTE)) return false;
		return this->IntepretExpression(result);
	}
	else if(this->Encount(Symbols::LEFT_QUOTE))
	{
		//<expression>--><sub-expression><expression>
		//<sub-expression>--><quote-expression>
		return this->IntepretSExpression(result) && this->IntepretExpression(result);
	}
	else
	{
		//<expression>--><sub-expression><expression>
		//<sub-expression>--><symbol>
		result.push_back(this->GetSymbol());
		this->input_pos++;
		return this->IntepretExpression(result);
	}

	return true;
}

//<quote-expression>-->$LEFT_QUOTE<expression-list>$RIGHT_QUOTE
bool Wll2IntepreterLL1Impl::IntepretSExpression(std::vector<Symbols>& result)
{
	if(!this->Accept(Symbols::LEFT_QUOTE)) return false;

	if(!this->Encount(Symbols::RIGHT_QUOTE))
	{
		std::vector<Symbols> cmd;
		if(!this->IntepretExpression(cmd)) return false;

		Symbols symbol = cmd[0];

		vector<vector<Symbols> >parameter_fields;
		parameter_fields.push_back(cmd);

		//<expression-list>--><expression>$SEPERATOR<expression-list>
		while(this->Encount(Symbols::SEPERATOR))
		{
			this->Accept(Symbols::SEPERATOR);

			vector<Symbols> parameter;
			if(!this->IntepretExpression(parameter)) return false;
			parameter_fields.push_back(parameter);
		}

		WllCommand* command = WllCommandFactory::CreateCommand(symbol, parameter_fields,this->intepreter);
		assert(command!=NULL);
		command->Intepret(result);
		delete command;
	}//RIGHT_QUOTE
	if(!this->Accept(Symbols::RIGHT_QUOTE)) return false;

	return true;
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

bool Wll2IntepreterLL1Impl::Encount(const Symbols& symbol)
{
	return this->GetSymbol()==symbol;
}

const Symbols& Wll2IntepreterLL1Impl::GetSymbol()
{
	return (this->input_pos >= this->input_symbols.size()) ? Symbols::END_SYMBOL : this->input_symbols[this->input_pos];
}

void Wll2IntepreterLL1Impl::ShowErrorMessage()
{
	TERM_ERROR("已经接受的输入符号:");
	for(int i = 0; i<this->input_symbols.size(); i++)
	{
		if(i == this->input_pos)
			cerr<<"["<<this->input_symbols[i]<<"]"<<endl;
		else
			cerr<<this->input_symbols[i];
	}
}


