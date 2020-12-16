#include "Calculate.h"
#include "LanguageSymbols.h"
#include "WllString.h"
#include <cassert>
#include <algorithm>
using namespace Wll::Util;

//CHAR_SYMBOL => Symbols::CHAR
Symbols ToRemarkType(SymbolTypes type)
{
	const Symbols* targetType;
	switch(type)
	{
	case CHAR_SYMBOL:
		targetType = &Symbols::CHAR;
		break;
	case INTEGER_SYMBOL:
		targetType = &Symbols::INTEGER;
		break;
	case LONG_SYMBOL:
		targetType = &Symbols::LONG;
		break;		
	case FLOAT_SYMBOL:
		targetType = &Symbols::FLOAT;
		break;
	case DOUBLE_SYMBOL:
		targetType = &Symbols::DOUBLE;
		break;
	case STRING_SYMBOL:
		targetType = &Symbols::STRING;
		break;
	default:
		break;
	}
	return *targetType;
}

//Symbols::CHAR => CHAR_SYMBOL
SymbolTypes ToEnumType(Symbols type)
{
	if(type == Symbols::CHAR)
	{
		return CHAR_SYMBOL;
	}
	else if(type == Symbols::INTEGER)
	{
		return INTEGER_SYMBOL;
	}
	else if(type == Symbols::LONG)
	{
		return LONG_SYMBOL;
	}
	else if(type == Symbols::FLOAT)
	{
		return FLOAT_SYMBOL;
	}
	else if(type == Symbols::DOUBLE)
	{
		return DOUBLE_SYMBOL;
	}
	else if(type == Symbols::STRING)
	{
		return STRING_SYMBOL;
	}
	else
	{
		return VOID_SYMBOL;
	}
}

//根據字符常量內容轉換爲對應類型的數值型變量值
Symbols CastToNumber(const vector<Symbols>& constant_symbols)
{
	string number_string;
	ToString(number_string, constant_symbols);
	SymbolTypes type = DetectNumType(number_string);
	return CastTo(type, number_string); 
}

//根據字符串內容檢測能轉換的數值類型
SymbolTypes DetectNumType(const string& number_string)
{
	bool have_see_point = false;
	int integer_part_len = 0;
	int float_part_len = 0;
	for(const char* c = number_string.c_str(); *c != '\0'; c++)
	{
		if(*c == '.')
		{
			if(!have_see_point)
			{
				have_see_point = true;
			}
			else
			{
				return STRING_SYMBOL;
			}
		}
		else if(isdigit(*c))
		{
			if(!have_see_point)
			{
				integer_part_len++;
			}
			else
			{
				float_part_len++;
			}
		}
		else
		{
			return STRING_SYMBOL;
		}
	}
	
	return (have_see_point?(float_part_len>=6?DOUBLE_SYMBOL:FLOAT_SYMBOL):(integer_part_len>=6?LONG_SYMBOL:INTEGER_SYMBOL));
}

//把字符常量串轉換爲對應的數值類型變量值
Symbols CastTo(SymbolTypes type, string s)
{
	assert(type == CHAR_SYMBOL
		|| type == INTEGER_SYMBOL
		|| type == LONG_SYMBOL
		|| type == FLOAT_SYMBOL
		|| type == DOUBLE_SYMBOL
		|| type == STRING_SYMBOL
	);
	
	switch(type)
	{
	case INTEGER_SYMBOL:
		{
			int i;
			String2Int(s,i);
			return(Symbols(i));
		}
	case LONG_SYMBOL:
		{
			long l;
			String2Long(s,l);
			return(Symbols(l));		
		}
	case FLOAT_SYMBOL:
		{
			float f;
			String2Float(s,f);
			return(Symbols(f));
		}
	case DOUBLE_SYMBOL:
		{
			double d;
			String2Double(s,d);
			return(Symbols(d));		
		}
	case STRING_SYMBOL:
		{
			return(Symbols(STRING_SYMBOL, s.c_str()));		
		}
	default:
		return Symbols();
	}	
}

Symbols CastTo(SymbolTypes type, const Symbols& value)
{
	assert(type == CHAR_SYMBOL
		|| type == INTEGER_SYMBOL
		|| type == LONG_SYMBOL
		|| type == FLOAT_SYMBOL
		|| type == DOUBLE_SYMBOL
		|| type == STRING_SYMBOL
	);
	
	return CastTo(ToRemarkType(type), value);
}

Symbols CastTo(const Symbols& targetType, const Symbols& value)
{
	assert(targetType.IsRemark());
	
	assert(targetType == Symbols::CHAR
		|| targetType == Symbols::INTEGER
		|| targetType == Symbols::LONG
		|| targetType == Symbols::FLOAT
		|| targetType == Symbols::DOUBLE
		|| targetType == Symbols::STRING
	);
	
	if(targetType == Symbols::CHAR)
	{
		//convert to char type
		assert(value.type == INTEGER_SYMBOL || value.type == CONSTANT_SYMBOL || value.type == CHAR_SYMBOL);
		if (value.type == INTEGER_SYMBOL)
		{
			return Symbols(CHAR_SYMBOL, char(value.i));
		}
		else if (value.type == CONSTANT_SYMBOL)
		{
			return Symbols(CHAR_SYMBOL, char(value.value));
		}
		else if (value.type == CHAR_SYMBOL)
		{
			return value;
		}
	} 
	else if(targetType == Symbols::INTEGER)
	{
		//convert to integer type
		assert(value.type == INTEGER_SYMBOL 
			|| value.type == CHAR_SYMBOL
			|| value.type == LONG_SYMBOL
			|| value.type == FLOAT_SYMBOL
			|| value.type == DOUBLE_SYMBOL
			|| value.type == STRING_SYMBOL
		);
		switch(value.type)
		{
		case INTEGER_SYMBOL:
			return(value);
			break;
		case CHAR_SYMBOL:
			return(Symbols(int(value.c)));
			break;
		case LONG_SYMBOL:
			return(Symbols(int(value.l)));
			break;
		case FLOAT_SYMBOL:
			return(Symbols(int(value.f)));
			break;
		case DOUBLE_SYMBOL:
			return(Symbols(int(value.d)));
			break;
		case STRING_SYMBOL:
			{
				int i;
				String2Int(*(*(value.s)),i);
				return(Symbols(i));
			}
			break;
		default:
			break;
		}
	}
	else if(targetType == Symbols::LONG)
	{
		//convert to long type
		assert(value.type == LONG_SYMBOL 
			|| value.type == CHAR_SYMBOL
			|| value.type == INTEGER_SYMBOL
			|| value.type == FLOAT_SYMBOL
			|| value.type == DOUBLE_SYMBOL
			|| value.type == STRING_SYMBOL
		);
		switch(value.type)
		{
		case LONG_SYMBOL:
			return(value);
			break;
		case CHAR_SYMBOL:
			return(Symbols(long(value.c)));
			break;
		case INTEGER_SYMBOL:
			return(Symbols(long(value.i)));
			break;
		case FLOAT_SYMBOL:
			return(Symbols(long(value.f)));
			break;
		case DOUBLE_SYMBOL:
			return(Symbols(long(value.d)));
			break;
		case STRING_SYMBOL:
			{
				long l;
				String2Long(*(*(value.s)),l);
				return(Symbols(l));
			}
			break;
		default:
			break;
		}
	}
	else if(targetType == Symbols::FLOAT)
	{
		//convert to float type
		assert(value.type == FLOAT_SYMBOL 
			|| value.type == CHAR_SYMBOL
			|| value.type == INTEGER_SYMBOL
			|| value.type == LONG_SYMBOL
			|| value.type == DOUBLE_SYMBOL
			|| value.type == STRING_SYMBOL
		);
		switch(value.type)
		{
		case FLOAT_SYMBOL:
			return(value);
			break;
		case CHAR_SYMBOL:
			return(Symbols(float(value.c)));
			break;
		case INTEGER_SYMBOL:
			return(Symbols(float(value.i)));
			break;
		case LONG_SYMBOL:
			return(Symbols(float(value.l)));
			break;
		case DOUBLE_SYMBOL:
			return(Symbols(float(value.d)));
			break;
		case STRING_SYMBOL:
			{
				float f;
				String2Float(*(*(value.s)),f);
				return(Symbols(f));
			}
			break;
		default:
			break;
		}		
	}
	else if(targetType == Symbols::DOUBLE)
	{
		//convert to double type
		assert(value.type == DOUBLE_SYMBOL 
			|| value.type == CHAR_SYMBOL
			|| value.type == INTEGER_SYMBOL
			|| value.type == LONG_SYMBOL
			|| value.type == FLOAT_SYMBOL
			|| value.type == STRING_SYMBOL
		);
		switch(value.type)
		{
		case DOUBLE_SYMBOL:
			return(value);
			break;
		case CHAR_SYMBOL:
			return(Symbols(double(value.c)));
			break;
		case INTEGER_SYMBOL:
			return(Symbols(double(value.i)));
			break;
		case LONG_SYMBOL:
			return(Symbols(double(value.l)));
			break;
		case FLOAT_SYMBOL:
			return(Symbols(double(value.f)));
			break;
		case STRING_SYMBOL:
			{
				double d;
				String2Double(*(*(value.s)),d);
				return(Symbols(d));
			}
			break;
		default:
			break;
		}		
	}
	else if(targetType == Symbols::STRING)
	{
		//convert to string type
		if(value.type == STRING_SYMBOL)
		{
			return(value);
		}
		else
		{
			return(Symbols(STRING_SYMBOL, value.ToString().c_str()));
		}
	}
	return Symbols();
}

Symbols CastTo(const Symbols& targetType, std::vector<Symbols>& values)
{
	assert(targetType.IsRemark());
	Symbols value(values[0]);
	
	assert(targetType == Symbols::CHAR
		|| targetType == Symbols::INTEGER
		|| targetType == Symbols::LONG
		|| targetType == Symbols::FLOAT
		|| targetType == Symbols::DOUBLE
		|| targetType == Symbols::STRING
	);
	
	if(value.type == CONSTANT_SYMBOL && !(targetType == Symbols::CHAR))
	{
		string s;
		ToString(s, values);
		return CastTo(ToEnumType(targetType), s);
	}
	else
	{
		return CastTo(targetType, value);
	}
}

Symbols CastTo(std::vector<Symbols>& values, bool treatConstantsAsNumber)
{
	assert(!values.empty());
	Symbols firstSymbol(values[0]);
	if (firstSymbol.IsConstant())
	{
		if(treatConstantsAsNumber)
		{
			return CastToNumber(values);
		}
		else
		{
			return CastTo(Symbols::STRING, values);
		}
	}
	else
	{
		return firstSymbol;
	}
}

Symbols Add(const Symbols& a, const Symbols& b)
{
	Symbols c(a);
	return AddTo(c,b);
}

const Symbols& AddTo(Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
		|| a.type == STRING_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
		|| b.type == STRING_SYMBOL
	);
	
	Symbols c(b);
	if (a.type < c.type)
	{
		a = CastTo(c.type, a);
	}
	else if (a.type > c.type)
	{
		c = CastTo(a.type, c);
	}
	
	//a.type == c.type
	switch(a.type)
	{
	case CHAR_SYMBOL:
		a.c += c.c;
		break;
	case INTEGER_SYMBOL:
		a.i += c.i;
		break;
	case LONG_SYMBOL:
		a.l += c.l;
		break;
	case FLOAT_SYMBOL:
		a.f += c.f;
		break;
	case DOUBLE_SYMBOL:
		a.d += c.d;
		break;
	case STRING_SYMBOL:
		(*(*a.s)) = (*(*a.s)) + (*(*c.s));
		break;
	default:
		break;
	}
	
	return a;
}

Symbols operator-(const Symbols& a, const Symbols& b)
{
	Symbols c(a);
	return (c -= b);
}

const Symbols& operator-=(Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
	);
	
	Symbols c(b);
	if (a.type < c.type)
	{
		a = CastTo(c.type, a);
	}
	else if (a.type > c.type)
	{
		c = CastTo(a.type, c);
	}
	
	//a.type == c.type
	switch(a.type)
	{
	case CHAR_SYMBOL:
		a.c -= c.c;
		break;
	case INTEGER_SYMBOL:
		a.i -= c.i;
		break;
	case LONG_SYMBOL:
		a.l -= c.l;
		break;
	case FLOAT_SYMBOL:
		a.f -= c.f;
		break;
	case DOUBLE_SYMBOL:
		a.d -= c.d;
		break;
	default:
		break;
	}
	
	return a;
}

Symbols operator*(const Symbols& a, const Symbols& b)
{
	Symbols c(a);
	return (c *= b);
}

const Symbols& operator*=(Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
	);
	
	Symbols c(b);
	if (a.type < c.type)
	{
		a = CastTo(c.type, a);
	}
	else if (a.type > c.type)
	{
		c = CastTo(a.type, c);
	}
	
	//a.type == c.type
	switch(a.type)
	{
	case CHAR_SYMBOL:
		a.c *= c.c;
		break;
	case INTEGER_SYMBOL:
		a.i *= c.i;
		break;
	case LONG_SYMBOL:
		a.l *= c.l;
		break;
	case FLOAT_SYMBOL:
		a.f *= c.f;
		break;
	case DOUBLE_SYMBOL:
		a.d *= c.d;
		break;
	default:
		break;
	}
	
	return a;
}

Symbols operator/(const Symbols& a, const Symbols& b)
{
	Symbols c(a);
	return (c /= b);
}

const Symbols& operator/=(Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
	);
	
	Symbols c(b);
	if (a.type < c.type)
	{
		a = CastTo(c.type, a);
	}
	else if (a.type > c.type)
	{
		c = CastTo(a.type, c);
	}
	
	//a.type == c.type
	switch(a.type)
	{
	case CHAR_SYMBOL:
		a.c /= c.c;
		break;
	case INTEGER_SYMBOL:
		a.i /= c.i;
		break;
	case LONG_SYMBOL:
		a.l /= c.l;
		break;
	case FLOAT_SYMBOL:
		a.f /= c.f;
		break;
	case DOUBLE_SYMBOL:
		a.d /= c.d;
		break;
	default:
		break;
	}
	
	return a;
}

Symbols operator%(const Symbols& a, const Symbols& b)
{
	Symbols c(a);
	return (c %= b);
}

const Symbols& operator%=(Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
	);
	
	Symbols c(b);
	if (a.type < c.type)
	{
		a = CastTo(c.type, a);
	}
	else if (a.type > c.type)
	{
		c = CastTo(a.type, c);
	}
	
	//a.type == c.type
	switch(a.type)
	{
	case CHAR_SYMBOL:
		a.c %= c.c;
		break;
	case INTEGER_SYMBOL:
		a.i %= c.i;
		break;
	case LONG_SYMBOL:
		a.l %= c.l;
		break;
	default:
		break;
	}
	
	return a;
}

bool Equal(const Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
	);
	
	Symbols c;
	if (a.type < b.type)
	{
		c = CastTo(b.type, a);
		return (c == b);
	}
	else if (a.type > b.type)
	{
		c = CastTo(a.type, b);
		return (a == c);
	}
	else
	{
		return (a == b);
	}
	
}

bool LessThan(const Symbols& a, const Symbols& b)
{
	assert(a.type == CHAR_SYMBOL
		|| a.type == INTEGER_SYMBOL
		|| a.type == LONG_SYMBOL
		|| a.type == FLOAT_SYMBOL
		|| a.type == DOUBLE_SYMBOL
	);
	
	assert(b.type == CHAR_SYMBOL
		|| b.type == INTEGER_SYMBOL
		|| b.type == LONG_SYMBOL
		|| b.type == FLOAT_SYMBOL
		|| b.type == DOUBLE_SYMBOL
	);
	
	Symbols c;
	if (a.type < b.type)
	{
		c = CastTo(b.type, a);
		return (c < b);
	}
	else if (a.type > b.type)
	{
		c = CastTo(a.type, b);
		return (a < c);
	}
	else
	{
		return (a < b);
	}
	
}

Symbols Encode(vector<Symbols>& value)
{
	Symbols symbol;
	if(value.size() == 1)
	{
		symbol = value[0];
	}
	else
	{
		symbol = Symbols(COMPACT_SYMBOL);
		//symbol.GetList() = value;
		for(vector<Symbols>::const_iterator i = value.begin(); i != value.end(); i++)
		{
			symbol.GetList().push_back(*i);
		}
	}
	return symbol;
}

vector<Symbols> Decode(Symbols& symbol)
{
	vector<Symbols> value;
	if(symbol.type == COMPACT_SYMBOL)
	{
		//value = symbol.GetList();
		for(vector<Symbols>::const_iterator i = symbol.GetList().begin(); i != symbol.GetList().end(); i++)
		{
			value.push_back(*i);
		}
	}
	else
	{
		value.push_back(symbol);
	}
	return value;
}

