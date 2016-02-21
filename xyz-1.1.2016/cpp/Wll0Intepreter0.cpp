/*
 * Wll0Intepreter0.cpp
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */


#include "Wll0Intepreter0.h"
#include "Wll0Loader.h"
#include <stack>
#include <queue>
#include "WllTrace.h"
#include <cassert>
#include "VariableNode.h"

Wll0Intepreter0::Wll0Intepreter0(const std::vector<Symbols>& input_symbols, std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations)
:Wll0Intepreter(input_symbols, output_symbols, translations)
{

}

bool Wll0Intepreter0::IntepretWll0()
{
	//暂时不做语法检查
	LanguageTranslations translation;
	LanguageRules source_rule,destination_rule,rule;
	Symbols root_symbol,variable,remark;
	LanguageExpressions expression,sub_expression;
	string symbol_string;

	stack<Integer> data_stack;

	translations.clear();

	for(vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		Symbols symbol = *i;
		DEBUG_LOG("explain symbol["<<symbol<<"] ...");
		if(symbol.IsConstant())
		{
			symbol_string.push_back(char(symbol.value));
			continue;
		}

		if(symbol == Symbols::REMARK_IGNORE)
		{
			symbol = *(++i);
			sub_expression = LanguageExpressions(symbol);
			continue;
		}

		if(symbol == Symbols::REMARK_REMARK)
		{
			remark = Symbols(REMARK_SYMBOL, symbol_string.c_str());
			sub_expression = LanguageExpressions(remark);
			symbol_string.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_CONSTANT)
		{
			//string1$CONSTANTstring2$CONSTANT...stringN$CONSTANT$SUB_SYMBOL, only the last CONSTANT effect
			//we use this feather to make $CONSTANTstring$CONSTANT the same effect as string$CONSTANT, but the leading $CONSTANT can distinguish the constant and variable
			sub_expression = LanguageExpressions(symbol_string.c_str());
			symbol_string.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_VARIABLE)
		{
			//<variable1>$VARIABLE<variable2>$VARIABLE...<variableN>$VARIABLE$SUB_SYMBOL, only the last VARIABLE effect
			variable = Symbols(symbol_string.c_str());
			sub_expression = LanguageExpressions(variable);
			symbol_string.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_SUB_SYMBOL)
		{
			//<variable1>$VARIABLE<variable2>$VARIABLE...<stringN>$CONSTANT$SUB_SYMBOL, only the last VARIABLE or CONSTANT SUB_SYMBOL effect
			expression += sub_expression;
			sub_expression = "";
			continue;
		}
		//if(symbol == Symbols::REMARK_EXPRESSION)
		//{
		//	continue;
		//}
		if(symbol == Symbols::REMARK_ROOT_SYMBOL)
		{
			root_symbol = variable;
			continue;
		}
		if(symbol == Symbols::REMARK_RULE)
		{
			rule.symbol = root_symbol;
			rule.expression = expression;
			expression.symbols.clear();
			continue;
		}
		if(symbol == Symbols::REMARK_SOURCE_RULE)
		{
			source_rule = rule;
			continue;
		}
		if(symbol == Symbols::REMARK_DESTINATION_RULE)
		{
			destination_rule = rule;
			continue;
		}
		if(symbol == Symbols::REMARK_TRANSLATION)
		{
			translation.source_rule = source_rule;
			translation.destination_rule = destination_rule;
			translations.push_back(translation);
			continue;
		}
		//if(symbol == Symbols::REMARK_WLL0)
		//{
		//	continue;
		//}
		if(symbol == Symbols(REMARK_SYMBOL,"$INT"))
		{
			Integer i = Integer(symbol_string);
			symbol_string.clear();
			data_stack.push(i);
			DEBUG_LOG("push "<<i);
			continue;
		}
		if(symbol == Symbols(REMARK_SYMBOL,"$ADD"))
		{
			Integer a = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<a);
			Integer b = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<b);
			Integer c = b + a;
			DEBUG_LOG("add "<<a<<" and "<<b);
			data_stack.push(c);
			DEBUG_LOG("push "<<c);
			continue;
		}
		if(symbol == Symbols(REMARK_SYMBOL,"$SUB"))
		{
			Integer a = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<a);
			Integer b = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<b);
			Integer c = b - a;
			DEBUG_LOG("sub "<<b<<" and "<<a);
			data_stack.push(c);
			DEBUG_LOG("push "<<c);
			continue;
		}
		if(symbol == Symbols(REMARK_SYMBOL,"$MUL"))
		{
			Integer a = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<a);
			Integer b = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<b);
			Integer c = b * a;
			DEBUG_LOG("mul "<<b<<" and "<<a);
			data_stack.push(c);
			DEBUG_LOG("push "<<c);
			continue;
		}
		if(symbol == Symbols(REMARK_SYMBOL,"$DIV"))
		{
			Integer a = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<a);
			Integer b = data_stack.top();
			data_stack.pop();
			DEBUG_LOG("pop "<<b);
			Integer c = b / a;
			DEBUG_LOG("div "<<b<<" and "<<a);
			data_stack.push(c);
			DEBUG_LOG("push "<<c);
			continue;
		}
	}//end of for

	if(!data_stack.empty())
		cout<<data_stack.top()<<endl;

	return true;
}
