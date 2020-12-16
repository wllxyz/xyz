/*
 * Wll2IntepreterCompactLL1Impl.h
 *
 *  Created on: 2016-1-20
 *      Author: wll
 */

#ifndef WLL2INTEPRETERCompactLL1IMPL_H_
#define WLL2INTEPRETERCompactLL1IMPL_H_

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "WllIntepreter.h"
#include <vector>

class Wll2IntepreterCompactLL1Impl
{
public:
	Wll2IntepreterCompactLL1Impl(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, WllIntepreter* intepreter);
	virtual bool IntepretWll();
protected:
	const std::vector<Symbols>& input_symbols;
	std::vector<Symbols>& output_symbols;
	WllIntepreter* intepreter;
	bool eval_switch;
	//<expression>--><sub-expression><expression>
	//<sub-expression>--><ignore-expression>
	//<sub-expression>--><quote-expression>
	//<sub-expression>--><symbol>
	//<ignore-expression>-->$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE
	//<quote-expression>-->$LEFT_QUOTE<expression-list>$RIGHT_QUOTE
	//<expression-list>--><expression>
	//<expression-list>--><expression>$SEPERATOR<expression-list>
	bool IntepretExpression(const Symbols& compact_symbol, std::vector<Symbols>& result);
	bool IntepretSExpression(const Symbols& s_exp_symbol, std::vector<Symbols>& result);
};


#endif /* WLL2INTEPRETERCompactLL1IMPL_H_ */
