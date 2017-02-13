/*
 * Wll0Intepreter1.cpp
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */


#include "Wll0Intepreter1.h"
#include "Wll0Loader.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>
#include "VariableNode.h"
#include "Wll0Command.h"

Wll0Intepreter1::Wll0Intepreter1(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations)
:Wll0Intepreter(input_symbols,output_symbols,translations)
{

}

bool Wll0Intepreter1::IntepretWll0()
{
	DEBUG_LOG("symbols="<<symbols);

	vector<Symbols>& data_stack = output_symbols;

	for(vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		Symbols symbol = *i;
		DEBUG_LOG("explain symbol["<<symbol<<"] ...");

		//TODO: 以下分支结构可以优化
		//REMARK_IGNORE在压栈过程中进行优先处理,以避免$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE括号内部的内容被求值
		if(symbol == Symbols::REMARK_IGNORE)
		{
			//$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE : 对括号内的符号不进行求值,原样输出
			++i;
			assert(*i == Symbols::LEFT_QUOTE);
			int depth = 1;
			while(++i != symbols.end())
			{
				if(*i == Symbols::RIGHT_QUOTE && depth == 1) break;
				if(*i == Symbols::LEFT_QUOTE) depth++;
				if(*i == Symbols::RIGHT_QUOTE) depth--;
				data_stack.push_back(*i);
			}
		}
		//我们采用后缀表达式压栈技术来求值,以减少前缀表达式扫描处理中操作数的求值发生的过程调用的压栈开销
		//一般情况下,内层的括号表达式先求值,然后才是外层的括号表达式
		//当存在IGNORE忽略求值的括号表达式时,会出现内层的括号表达式没有求值的情况,此时外层的括号表达式边界的匹配不能简单地认为往前找的第一个左括号就是边界,而需要括号的深度得到相应的匹配
		else if(symbol == Symbols::RIGHT_QUOTE)
		{
			int depth = -1;
			deque<Symbols> parameters;
			while(!data_stack.empty())
			{
				symbol = data_stack.back();
				data_stack.pop_back();
				if(symbol == Symbols::LEFT_QUOTE && depth == -1) break;
				if(symbol == Symbols::LEFT_QUOTE) depth++;
				if(symbol == Symbols::RIGHT_QUOTE) depth--;
				parameters.push_front(symbol);
			}
			if(!parameters.empty())
			{
				INFO("parameters="<<vector<Symbols>(parameters.begin(),parameters.end()));
				vector< vector<Symbols> > parameter_fields;
				this->SplitParameters(vector<Symbols>(parameters.begin(), parameters.end()), parameter_fields);
				symbol = parameter_fields[0][0];
				DEBUG_LOG("symbol="<<symbol);

				if(symbol == Symbols::LOAD_TRANSLATIONS)
				{
					LoadTranslationsCommand command(symbol, parameter_fields, this);
					command.Intepret(data_stack);
				}//LOAD_TRANSLATIONS
				else if(symbol == Symbols::ADD_TRANSLATIONS)
				{
					AddTranslationsCommand command(symbol, parameter_fields, this);
					command.Intepret(data_stack);
				}//LOAD_TRANSLATIONS
				else if(symbol == Symbols::ADD)
				{
					AddCommand command(symbol, parameter_fields, this);
					command.Intepret(data_stack);
				}//ADD
				else if(symbol == Symbols::SUB)
				{
					SubCommand command(symbol, parameter_fields, this);
					command.Intepret(data_stack);
				}//SUB
				else if(symbol == Symbols::MUL)
				{
					MulCommand command(symbol, parameter_fields, this);
					command.Intepret(data_stack);
				}//MUL
				else if(symbol == Symbols::DIV)
				{
					DivCommand command(symbol, parameter_fields, this);
					command.Intepret(data_stack);
				}//DIV
				//TODO:
			}//parameters not empty
		}
		//没到求值时机,先压栈
		else
		{
			data_stack.push_back(symbol);
		}
	}//end of for

	return true;
}