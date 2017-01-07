#ifndef	LANGUAGE_RULE_H
#define	LANGUAGE_RULE_H

#include "LanguageSymbols.h"
#include "LanguageExpressions.h"

//<LanguageRule>::=<root-symbol><LanguageExpression>
struct LanguageRules
{
public:
	Symbols symbol;      			//root-symbol(<0)
	LanguageExpressions expression;  	//rule right expand expression
public:
	LanguageRules();
	LanguageRules(const Symbols& symbol,const LanguageExpressions& expression);
	LanguageRules(const LanguageRules& that);
	const LanguageRules& operator= (const LanguageRules& that);
	bool operator== (const LanguageRules& that) const;
};//end of LanguageRules
ostream& operator<< (ostream& o,const LanguageRules& lr);

#endif	//LANGUAGE_RULE_H

