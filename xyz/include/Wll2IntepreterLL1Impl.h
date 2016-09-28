/*
 * Wll2IntepreterLL1Impl.h
 *
 *  Created on: 2016-1-20
 *      Author: wll
 */

#ifndef WLL2INTEPRETERLL1IMPL_H_
#define WLL2INTEPRETERLL1IMPL_H_

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "WllIntepreter.h"
#include <vector>

class Wll2IntepreterLL1Impl
{
public:
	Wll2IntepreterLL1Impl(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, WllIntepreter* intepreter);
	virtual bool IntepretWll();
	void ShowErrorMessage();
protected:
	const std::vector<Symbols>& input_symbols;
	std::vector<Symbols>& output_symbols;
	WllIntepreter* intepreter;
	int input_pos;
	bool eval_switch;
	//<expression>--><sub-expression><expression>
	//<sub-expression>--><ignore-expression>
	//<sub-expression>--><quote-expression>
	//<sub-expression>--><symbol>
	//<ignore-expression>-->$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE
	//<quote-expression>-->$LEFT_QUOTE<expression-list>$RIGHT_QUOTE
	//<expression-list>--><expression>
	//<expression-list>--><expression>$SEPERATOR<expression-list>
	bool IntepretExpression(std::vector<Symbols>& result);
	bool IntepretSExpression(std::vector<Symbols>& result);
	bool Accept(const Symbols& symbol);
	bool Encount(const Symbols& symbol);
	const Symbols& GetSymbol();
};


#endif /* WLL2INTEPRETERLL1IMPL_H_ */
