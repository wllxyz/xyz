#ifndef	LANGUAGE_EXPRESSIONS_H
#define LANGUAGE_EXPRESSIONS_H

#include <vector>
#include "LanguageSymbols.h"
using namespace std;

//<LanguageExpression>::=<symbol><LanguageExpression>
struct LanguageExpressions
{
public:
	LanguageExpressions();
	LanguageExpressions(const Symbols& c);
	LanguageExpressions(const char* constants);
	LanguageExpressions(const LanguageExpressions& that);
	const LanguageExpressions& operator= (const LanguageExpressions& that);
	//LanguageExpressions(int len,...);
	const LanguageExpressions& operator+= (const LanguageExpressions& that);
	LanguageExpressions operator+ (const LanguageExpressions& that);
	bool operator== (const LanguageExpressions& that)const;
	virtual void Display(ostream& o) const;
	virtual void Dump(ostream& o) const;
public: 
	vector<Symbols> symbols;
};//end of LanguageExpressions

ostream& operator<< (ostream& o,const LanguageExpressions& exp);

LanguageExpressions operator+ (const Symbols& a, const Symbols& b);

#endif	//LANGUAGE_EXPRESSIONS_H

