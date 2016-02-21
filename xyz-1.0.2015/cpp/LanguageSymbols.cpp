#include "LanguageSymbols.h"
#include <cassert>
using namespace std;

StringTable Symbols::variable_table;
StringTable Symbols::remark_table;
const Symbols Symbols::REMARK_WLL0(REMARK_SYMBOL,"$WLL0");
const Symbols Symbols::REMARK_TRANSLATION(REMARK_SYMBOL,"$TRANSLATION");
const Symbols Symbols::REMARK_SOURCE_RULE(REMARK_SYMBOL,"$SOURCE_RULE");
const Symbols Symbols::REMARK_DESTINATION_RULE(REMARK_SYMBOL,"$DESTINATION_RULE");
const Symbols Symbols::REMARK_RULE(REMARK_SYMBOL,"$RULE");
const Symbols Symbols::REMARK_ROOT_SYMBOL(REMARK_SYMBOL,"$ROOT_SYMBOL");
const Symbols Symbols::REMARK_EXPRESSION(REMARK_SYMBOL,"$EXPRESSION");
const Symbols Symbols::REMARK_SUB_SYMBOL(REMARK_SYMBOL,"$SUB_SYMBOL");
const Symbols Symbols::REMARK_VARIABLE(REMARK_SYMBOL,"$VARIABLE");
const Symbols Symbols::REMARK_CONSTANT(REMARK_SYMBOL,"$CONSTANT");
const Symbols Symbols::REMARK_REMARK(REMARK_SYMBOL,"$REMARK");
const Symbols Symbols::REMARK_IGNORE(REMARK_SYMBOL,"$IGNORE");
const Symbols Symbols::NULL_SYMBOL(REMARK_SYMBOL,"$NULL");
const Symbols Symbols::END_SYMBOL(REMARK_SYMBOL,"$END");

Symbols::Symbols()
{
	this->type = UNKNOW_SYMBOL;
	this->value = 0;
}

Symbols::Symbols(const char* variable)
{
	this->type = VARIABLE_SYMBOL;
	this->value = Symbols::variable_table.GetIndexByName(variable);
}

Symbols::Symbols(char constant)
{
	this->type = CONSTANT_SYMBOL;
	this->value = constant;
}

Symbols::Symbols(SymbolTypes type,const char* remark)
{
	assert(type==REMARK_SYMBOL);
	this->type = REMARK_SYMBOL;
	this->value = Symbols::remark_table.GetIndexByName(remark);
}

bool Symbols::operator== (const Symbols& that) const
{
	return ((this->type == that.type) && (this->value == that.value));
}

bool Symbols::operator< (const Symbols& that) const
{
	//assert(this->type == that.type);
	return (this->type < that.type || (this->type == that.type && this->value < that.value));
}

bool Symbols::IsVariable() const
{
	return this->type==VARIABLE_SYMBOL;
}

bool Symbols::IsConstant() const
{
	return this->type==CONSTANT_SYMBOL;
}

bool Symbols::IsRemark() const
{
	return this->type==REMARK_SYMBOL;
}

void Symbols::Display(ostream& o) const
{
	switch(this->type)
	{
	case VARIABLE_SYMBOL:
		o<<Symbols::variable_table.GetNameByIndex(this->value);
		break;
	case REMARK_SYMBOL:
		o<<Symbols::remark_table.GetNameByIndex(this->value);
		break;
	case CONSTANT_SYMBOL:
		o<<char(this->value);
		break;
	default:
		o<<"Unknow Symbols!!!";
		break;
	}
}

void Symbols::Dump(ostream& o) const
{
	switch(this->type)
	{
	case VARIABLE_SYMBOL:
		o<<Symbols::variable_table.GetNameByIndex(this->value);
		break;
	case REMARK_SYMBOL:
		o<<Symbols::remark_table.GetNameByIndex(this->value);
		break;
	case CONSTANT_SYMBOL:
		switch(this->value)
		{
		case '"':
			o<<"\\\"";
			break;
		case '\\':
			o<<"\\\\";
			break;
		case '\t':
			o<<"\\t";
			break;
		case '\r':
			o<<"\\r";
			break;
		case '\n':
			o<<"\\n";
			break;
		default:
			o<<(char)this->value;
			break;
		}
		break;
	default:
		o<<"Unknow Symbols!!!";
		break;
	}
}

ostream& operator<< (ostream& o, const Symbols& symbol)
{
	symbol.Display(o);
	return o;
}

ostream& operator<< (ostream& o, const vector<Symbols>& symbols)
{
	for(vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		o<<*i;
	}
	return o;
}

