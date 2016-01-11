#include "LanguageExpressions.h"
#include <ctype.h>

LanguageExpressions::LanguageExpressions()
{
}

LanguageExpressions::LanguageExpressions(const LanguageExpressions& that)
{
	this->symbols=that.symbols;
}

LanguageExpressions::LanguageExpressions(const Symbols& c)
{
	this->symbols.push_back(c);
}

LanguageExpressions::LanguageExpressions(const char* constants)
{
	while(*constants != '\0')
	{
		this->symbols.push_back(*constants);
		constants++;
	}
}

const LanguageExpressions& LanguageExpressions::operator= (const LanguageExpressions& that)
{
	this->symbols=that.symbols;
	return (*this);
}

/*
LanguageExpressions::LanguageExpressions(int len,...)
{
	va_list parameters;
	va_start(parameters,len);
	for(int i=0; i<len; i++)
	{
		Symbols symbol=va_arg(parameters,Symbols);
		this->symbols.Insert(symbol);
	}
	va_end(parameters);
}
*/

const LanguageExpressions& LanguageExpressions::operator+= (const LanguageExpressions& that)
{
	for(vector<Symbols>::const_iterator i = that.symbols.begin(); i != that.symbols.end(); ++i)
	{
		this->symbols.push_back(*i);
	}
	return *this;
}

LanguageExpressions LanguageExpressions::operator+ (const LanguageExpressions& that)
{
	LanguageExpressions result(*this);
	return result+=that;
}

bool LanguageExpressions::operator== (const LanguageExpressions& that)const
{
	if(this->symbols.size() != that.symbols.size()) return false;
	vector<Symbols>::const_iterator a = this->symbols.begin();
	vector<Symbols>::const_iterator b = that.symbols.begin();
	while(a != this->symbols.end() && b != that.symbols.end())
	{
		if(!(*a == *b)) return false;
		++a;
		++b;
	}
	return true;
}


void LanguageExpressions::Dump(ostream& o) const
{
	if(this->symbols.empty())
	{
		o<<"\"\"";
		return;
	}

	//对连续的CONSTANT_SYMBOL前后添加""
	bool constant_state = false;
	for(vector<Symbols>::const_iterator i = this->symbols.begin(); i != this->symbols.end(); ++i)
	{
		if(i->IsConstant())
		{
			if(!constant_state)
			{
				o<<"\"";
			}
			constant_state = true;
		}
		else
		{
			if(constant_state)
			{
				o<<"\"";
			}
			constant_state = false;
		}
		i->Dump(o);
	}
	if(constant_state)
	{
		o<<"\"";
	}
}

void LanguageExpressions::Display(ostream& o) const
{
	for(vector<Symbols>::const_iterator i = this->symbols.begin(); i != this->symbols.end(); ++i)
	{
		i->Display(o);
	}
}


ostream& operator<< (ostream& o,const LanguageExpressions& exp)
{
	exp.Display(o);
	return o;
}

LanguageExpressions operator+ (const Symbols& a, const Symbols& b)
{
	LanguageExpressions result(a);
	result+=b;
	return result;
}

