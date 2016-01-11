/*
 * Wll2Intepreter.h
 *
 *  Created on: 2016-1-5
 *      Author: wll
 */

#ifndef WLL2INTEPRETER_H_
#define WLL2INTEPRETER_H_

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include "WllIntepreter.h"
#include <vector>

class Wll2Intepreter : public WllIntepreter
{
public:
	Wll2Intepreter(const std::vector<Symbols>& input_symbols,std::vector<Symbols>& output_symbols, std::vector<LanguageTranslations>& translations);
	virtual bool IntepretWll();
	void ShowErrorMessage();
protected:
	int input_pos;
	//<e>--><i><e>
	//<i>--><s>
	//<i>--><a>
	//<s>-->(<el>)
	//<el>--><e>
	//<el>--><e> <el>
	bool IntepretExpression(std::vector<Symbols>& result);
	bool IntepretSExpression(std::vector<Symbols>& result);
	bool Accept(const Symbols& symbol);
	bool Encount(const Symbols& symbol);
	const Symbols& GetSymbol();
};


#endif /* WLL2INTEPRETER_H_ */
