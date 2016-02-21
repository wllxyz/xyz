/*
 * Wll0Intepreter.cpp
 *
 *  Created on: 2015-12-31
 *      Author: wll
 */


#include "Wll0Intepreter.h"

Wll0Intepreter::Wll0Intepreter(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& _output_symbols, std::vector<LanguageTranslations>& _translations)
:symbols(input_symbols), output_symbols(_output_symbols), translations(_translations)
{

}

int Wll0Intepreter::SplitParameters(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields)
{
	fields.clear();
	if(symbols.empty()) return 0;

	vector<Symbols> field;
	int depth = 0;
	for(vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		if(*i == Symbols(REMARK_SYMBOL, "$SEPERATOR") && depth == 0)
		{
			fields.push_back(field);
			field.clear();
		}
		else
		{
			if(*i == Symbols(REMARK_SYMBOL, "$LEFT_QUOTE"))
			{
				depth++;
			}
			else if(*i == Symbols(REMARK_SYMBOL, "$RIGHT_QUOTE"))
			{
				depth--;
			}
			field.push_back(*i);
		}
	}
	fields.push_back(field);
	return fields.size();
}
