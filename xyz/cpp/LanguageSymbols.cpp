#include "LanguageSymbols.h"
#include <cassert>
using namespace std;

StringTable Symbols::variable_table;
StringTable Symbols::remark_table;
//支持文法解析
const Symbols Symbols::NULL_SYMBOL(REMARK_SYMBOL,"$NULL");
const Symbols Symbols::END_SYMBOL(REMARK_SYMBOL,"$END");
//支持WLL0文法自解释
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
//支持S表达式求解(WLL1文法自解释)
const Symbols Symbols::EVAL(REMARK_SYMBOL,"$EVAL");
const Symbols Symbols::LEFT_QUOTE(REMARK_SYMBOL,"$LEFT_QUOTE");
const Symbols Symbols::RIGHT_QUOTE(REMARK_SYMBOL,"$RIGHT_QUOTE");
const Symbols Symbols::SEPERATOR(REMARK_SYMBOL,"$SEPERATOR");
const Symbols Symbols::LOAD_TRANSLATIONS(REMARK_SYMBOL,"$LOAD_TRANSLATIONS");
const Symbols Symbols::ADD_TRANSLATIONS(REMARK_SYMBOL,"$ADD_TRANSLATIONS");
//支持Ｓ表达式求值
const Symbols Symbols::LIST(REMARK_SYMBOL,"$LIST");
const Symbols Symbols::CAR(REMARK_SYMBOL,"$CAR");
const Symbols Symbols::CDR(REMARK_SYMBOL,"$CDR");
//支持运算控制
const Symbols Symbols::ADD(REMARK_SYMBOL,"$ADD");
const Symbols Symbols::SUB(REMARK_SYMBOL,"$SUB");
const Symbols Symbols::MUL(REMARK_SYMBOL,"$MUL");
const Symbols Symbols::DIV(REMARK_SYMBOL,"$DIV");
const Symbols Symbols::TRUE(REMARK_SYMBOL,"$TRUE");
const Symbols Symbols::FALSE(REMARK_SYMBOL,"$FALSE");
const Symbols Symbols::COND(REMARK_SYMBOL,"$COND");
const Symbols Symbols::LOOP(REMARK_SYMBOL,"$LOOP");
const Symbols Symbols::EQ(REMARK_SYMBOL,"$EQ");
const Symbols Symbols::LT(REMARK_SYMBOL,"$LT");
const Symbols Symbols::AND(REMARK_SYMBOL,"$AND");
const Symbols Symbols::OR(REMARK_SYMBOL,"$OR");
const Symbols Symbols::NOT(REMARK_SYMBOL,"$NOT");
const Symbols Symbols::SHELL(REMARK_SYMBOL,"$SHELL");
const Symbols Symbols::CALL(REMARK_SYMBOL,"$CALL");
//支持变量存储
const Symbols Symbols::DEF(REMARK_SYMBOL,"$DEF");
const Symbols Symbols::SET(REMARK_SYMBOL,"$SET");
const Symbols Symbols::GET(REMARK_SYMBOL,"$GET");
const Symbols Symbols::PUSH_DATA(REMARK_SYMBOL,"$PUSH_DATA");
const Symbols Symbols::POP_DATA(REMARK_SYMBOL,"$POP_DATA");
const Symbols Symbols::PUSH(REMARK_SYMBOL,"$PUSH");
const Symbols Symbols::POP(REMARK_SYMBOL,"$POP");

const Symbols Symbols::CAT(REMARK_SYMBOL,"$CAT");


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

istream& operator>> (istream&ins, vector<Symbols>& symbols)
{
	//symbols.clear();

	char c=ins.get();
	if(!ins.eof())
	{
		do
		{
			symbols.push_back(Symbols(c));
			c=ins.get();
		}while(!ins.eof());
	}

	return ins;
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

int Split(const vector<Symbols>&symbols, Symbols seperator, vector< vector<Symbols> >& fields)
{
	fields.clear();
	if(symbols.empty()) return 0;

	vector<Symbols> field;
	for(vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		if(*i == seperator)
		{
			fields.push_back(field);
			field.clear();
		}
		else
		{
			field.push_back(*i);
		}
	}
	fields.push_back(field);
	return fields.size();
}

int SplitParameters(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields)
{
	return SplitParameters(symbols.begin(), symbols.end(), fields);
}

int SplitParameters(vector<Symbols>::const_iterator begin, vector<Symbols>::const_iterator end, vector< vector<Symbols> >& fields)
{
	fields.clear();
	if(begin>=end) return 0;

	vector<Symbols> field;
	int depth = 0;
	for(vector<Symbols>::const_iterator i = begin; i != end; ++i)
	{
		if(*i == Symbols::SEPERATOR && depth == 0)
		{
			fields.push_back(field);
			field.clear();
		}
		else
		{
			if(*i == Symbols::LEFT_QUOTE)
			{
				depth++;
			}
			else if(*i == Symbols::RIGHT_QUOTE)
			{
				depth--;
			}
			field.push_back(*i);
		}
	}
	fields.push_back(field);
	return fields.size();
}

void ComposeSList(vector<vector<Symbols> >::const_iterator begin, vector<vector<Symbols> >::const_iterator end, vector<Symbols>& slist)
{
	slist.push_back(Symbols::LEFT_QUOTE);
	for(vector<vector<Symbols> >::const_iterator i = begin; i != end; ++i)
	{
		slist += *i;
		slist.push_back(Symbols::SEPERATOR);
	}
	if(slist.back() == Symbols::SEPERATOR) slist.pop_back();
	slist.push_back(Symbols::RIGHT_QUOTE);
}

vector<Symbols>& operator+= (vector<Symbols>& a, const vector<Symbols>& b)
{
	for(vector<Symbols>::const_iterator i = b.begin(); i != b.end(); ++i)
	{
		a.push_back(*i);
	}
	return a;
}

std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::string& b)
{
	for(string::const_iterator i = b.begin(); i != b.end(); ++i)
	{
		a.push_back(*i);
	}
	return a;
}

const std::string& ToString (std::string& str, const std::vector<Symbols>& symbols)
{
	str.clear();
	for(std::vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		str.push_back(char(i->value));
	}
	return str;
}

