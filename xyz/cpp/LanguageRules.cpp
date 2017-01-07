#include "LanguageRules.h"

LanguageRules::LanguageRules()
{ 

}

LanguageRules::LanguageRules(const Symbols& symbol,const LanguageExpressions& expression)
{
	this->symbol=symbol;
	this->expression=expression;
}

LanguageRules::LanguageRules(const LanguageRules& that)
{
	this->symbol=that.symbol;
	this->expression=that.expression;
}

const LanguageRules& LanguageRules::operator= (const LanguageRules& that)
{
	this->symbol=that.symbol;
	this->expression=that.expression;
	return (*this);
}

bool LanguageRules::operator== (const LanguageRules& that) const
{
	return (this->symbol==that.symbol && this->expression==that.expression);
}

ostream& operator<< (ostream& o,const LanguageRules& lr)
{
	o<<lr.symbol<<"-->";
	lr.expression.Dump(o);
	return o;
}


