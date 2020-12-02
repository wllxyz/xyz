#include "LanguageSymbols.h"
#include "WllString.h"
#include <memory>
#include <cassert>
#include <sstream>
#include <cstring>
#include <math.h>
//#include <algorithm>
using namespace std;
using namespace Wll::Util;

StringTable Symbols::variable_table;
StringTable Symbols::remark_table;

const Symbols Symbols::VOID;
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
const Symbols Symbols::EXEC(REMARK_SYMBOL,"$EXEC");
const Symbols Symbols::LEFT_QUOTE(REMARK_SYMBOL,"$LEFT_QUOTE");
const Symbols Symbols::RIGHT_QUOTE(REMARK_SYMBOL,"$RIGHT_QUOTE");
const Symbols Symbols::SEPERATOR(REMARK_SYMBOL,"$SEPERATOR");
const Symbols Symbols::LOAD_TRANSLATIONS(REMARK_SYMBOL,"$LOAD_TRANSLATIONS");
const Symbols Symbols::ADD_TRANSLATIONS(REMARK_SYMBOL,"$ADD_TRANSLATIONS");
//支持Ｓ表达式求值
const Symbols Symbols::LIST(REMARK_SYMBOL,"$LIST");
const Symbols Symbols::APPEND(REMARK_SYMBOL,"$APPEND");
const Symbols Symbols::CAR(REMARK_SYMBOL,"$CAR");
const Symbols Symbols::CDR(REMARK_SYMBOL,"$CDR");
//支持运算控制
const Symbols Symbols::ADD(REMARK_SYMBOL,"$ADD");
const Symbols Symbols::SUB(REMARK_SYMBOL,"$SUB");
const Symbols Symbols::MUL(REMARK_SYMBOL,"$MUL");
const Symbols Symbols::DIV(REMARK_SYMBOL,"$DIV");
const Symbols Symbols::SUB_STR(REMARK_SYMBOL,"$SUB_STR");
const Symbols Symbols::NEXT_CHAR(REMARK_SYMBOL,"$NEXT_CHAR");
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
//支持数据结构
const Symbols Symbols::MAP(REMARK_SYMBOL,"$MAP");
const Symbols Symbols::ARRAY(REMARK_SYMBOL,"$ARRAY");

const Symbols Symbols::CAT(REMARK_SYMBOL,"$CAT");

const Symbols Symbols::CHAR(REMARK_SYMBOL,"$CHAR");
const Symbols Symbols::INTEGER(REMARK_SYMBOL,"$INTEGER");
const Symbols Symbols::LONG(REMARK_SYMBOL,"$LONG");
const Symbols Symbols::FLOAT(REMARK_SYMBOL,"$FLOAT");
const Symbols Symbols::DOUBLE(REMARK_SYMBOL,"$DOUBLE");
const Symbols Symbols::STRING(REMARK_SYMBOL,"$STRING");

const Symbols Symbols::CAST(REMARK_SYMBOL,"$CAST");

void Symbols::Init()
{
	this->type = VOID_SYMBOL;
	this->value = 0;
	
	this->s = NULL;
	this->list = NULL;
	this->m = NULL;
}

void Symbols::Destroy()
{
	switch(this->type)
	{
	case COMPACT_SYMBOL:
	case LIST_SYMBOL:
		if (this->list != NULL) 
		{
			//(*this->list).reset();
			delete this->list;
			this->list = NULL;
		}
		break;
	case MAP_SYMBOL:
		if (this->m != NULL)
		{
			//(*this->m).reset();
			delete this->m;
			this->m = NULL;
		}
		break;
	case STRING_SYMBOL:
		if (this->s != NULL) 
		{ 
			delete this->s;
			this->s = NULL;
		}
		break;
	default:
		break;
	}
	
	this->Init();
}

void Symbols::Copy(const Symbols& that)
{
	this->type = that.type;
	switch(that.type)
	{
	case COMPACT_SYMBOL:
	case LIST_SYMBOL:
		this->list = new shared_ptr< vector<Symbols> >(*that.list);
		break;
	case MAP_SYMBOL:
		this->m = new shared_ptr< map<string, Symbols> >(*that.m);
		break;
	case CHAR_SYMBOL:
		this->c = that.c;
		break;
	case INTEGER_SYMBOL:
		this->i = that.i;
		break;
	case LONG_SYMBOL:
		this->l = that.l;
		break;
	case FLOAT_SYMBOL:
		this->f = that.f;
		break;
	case DOUBLE_SYMBOL:
		this->d = that.d;
		break;
	case STRING_SYMBOL:
		this->s = new shared_ptr<string>(*that.s);
		break;
	default:
		this->value = that.value;
		break;
	}
}

Symbols::Symbols()
{
	this->type = VOID_SYMBOL;
	this->value = 0;
	
	this->s = NULL;
	this->list = NULL;
	this->m = NULL;
}

Symbols::Symbols(const Symbols& that)
{
	this->Copy(that);
}

Symbols& Symbols::operator= (const Symbols& that)
{
	if (this == &that) return *this;
	this->Destroy();
	this->Copy(that);
	return *this;
}

Symbols::~Symbols()
{
	this->Destroy();
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
	assert(type==REMARK_SYMBOL || type==STRING_SYMBOL);
	this->type = type;
	switch(type)
	{
	case REMARK_SYMBOL:
		this->value = Symbols::remark_table.GetIndexByName(remark);
		break;
	case STRING_SYMBOL:
		this->s = new shared_ptr<string>(new string(remark));
		break;
	default:
		throw "unknow symbol type";
		break;
	}
}

Symbols::Symbols(SymbolTypes type, char c)
{
	assert(type == CHAR_SYMBOL || type == CONSTANT_SYMBOL);
	
	this->type = type;
	switch(type)
	{
	case CHAR_SYMBOL:
		this->c = c;
		break;
	case CONSTANT_SYMBOL:
		this->value = c;
		break;	
	default:
		throw "unknow symbol type";
		break;
	}
}

Symbols::Symbols(int i)
{
	this->type = INTEGER_SYMBOL;
	this->i = i;
}

Symbols::Symbols(long l)
{
	this->type = LONG_SYMBOL;
	this->l = l;
}

Symbols::Symbols(float f)
{
	this->type = FLOAT_SYMBOL;
	this->f = f;
}

Symbols::Symbols(double d)
{
	this->type = DOUBLE_SYMBOL;
	this->d = d;
}

Symbols::Symbols(SymbolTypes type, vector<Symbols> symbols)
{
	assert(type == COMPACT_SYMBOL || type == LIST_SYMBOL);
	this->type = type;
	this->list = new shared_ptr<vector<Symbols> >(new vector<Symbols>(symbols.size()));
	copy(symbols.begin(), symbols.end(), (*this->list)->begin());
}

Symbols::Symbols(SymbolTypes type)
{
	switch(type)
	{
	case LIST_SYMBOL:
	case COMPACT_SYMBOL:
		this->list = new shared_ptr<vector<Symbols> >(new vector<Symbols>());
		break;
	case MAP_SYMBOL:
		this->m = new shared_ptr<map<string,Symbols> >(new map<string,Symbols>());
		break;
	case STRING_SYMBOL:
		this->s = new shared_ptr<string>(new string());
		break;
	default:
		break;
	}
	this->type = type;
}

bool Symbols::operator== (const Symbols& that) const
{
	if (this->type != that.type) return false;
	switch(this->type)
	{
	case COMPACT_SYMBOL:
	case LIST_SYMBOL:
		return this->GetList() == that.GetList();
		break;
	case MAP_SYMBOL:
		return this->GetMap() == that.GetMap();
		break;
	case CHAR_SYMBOL:
		return (this->c == that.c);
		break;
	case INTEGER_SYMBOL:
		return (this->i == that.i);
		break;
	case LONG_SYMBOL:
		return (this->l == that.l);
		break;
	case FLOAT_SYMBOL:
		return fabs(this->f - that.f) < 1e-6;
		break;
	case DOUBLE_SYMBOL:
		return fabs(this->d - that.d) < 1e-6;
		break;
	case STRING_SYMBOL:
		return *(*this->s) == *(*that.s);
		break;
	default:
		return (this->value == that.value);
		break;
	}
}

bool Symbols::operator< (const Symbols& that) const
{
	if (this->type < that.type) return true;
	if (this->type > that.type) return false;
	switch(this->type)
	{
	case COMPACT_SYMBOL:
	case LIST_SYMBOL:
		return this->GetList() < that.GetList();
		break;
	case MAP_SYMBOL:
		return this->GetMap() < that.GetMap();
		break;
	case CHAR_SYMBOL:
		return (this->c < that.c);
		break;
	case INTEGER_SYMBOL:
		return (this->i < that.i);
		break;
	case LONG_SYMBOL:
		return (this->l < that.l);
		break;
	case FLOAT_SYMBOL:
		return this->f < that.f;
		break;
	case DOUBLE_SYMBOL:
		return this->d < that.d;
		break;
	case STRING_SYMBOL:
		return *(*this->s) < *(*that.s);
		break;
	default:
		return (this->value < that.value);
		break;
	}
}

bool operator== (const vector<Symbols>& a, const vector<Symbols>& b)
{
	vector<Symbols>::const_iterator ia = a.begin();
	vector<Symbols>::const_iterator ib = b.begin();
	for(; ia != a.end() && ib != b.end(); ia++, ib++)
	{
		if (!(*ia == *ib)) return false;
	}
	return (ia == a.end() && ib == b.end());	
}

bool operator< (const vector<Symbols>& a, const vector<Symbols>& b)
{
	vector<Symbols>::const_iterator ia = a.begin();
	vector<Symbols>::const_iterator ib = b.begin();
	for(; ia != a.end() && ib != b.end(); ia++, ib++)
	{
		if (*ia < *ib) return true;
		if (!(*ia == *ib)) return false;
	}
	if (ia == a.end() && ib == b.end()) return false;
	if (ia == a.end()) return true;
	if (ib == b.end()) return false;
	return false;	
}

bool operator== (const map<string,Symbols>& a, const map<string,Symbols>& b)
{
	map<string,Symbols>::const_iterator ia = a.begin();
	map<string,Symbols>::const_iterator ib = b.begin();
	for(; ia != a.end() && ib != b.end(); ia++, ib++)
	{
		if (!(ia->first == ib->first && ia->second == ib->second)) return false;
	}
	return (ia == a.end() && ib == b.end());
}

bool operator< (const map<string,Symbols>& a, const map<string,Symbols>& b)
{
	map<string,Symbols>::const_iterator ia = a.begin();
	map<string,Symbols>::const_iterator ib = b.begin();
	for(; ia != a.end() && ib != b.end(); ia++, ib++)
	{
		if (ia->first < ib->first || (ia->first == ib->first && ia->second < ib->second)) return true;
		if (!(ia->first == ib->first && ia->second == ib->second)) return false;
	}
	if (ia == a.end() && ib == b.end()) return false;
	if (ia == a.end()) return true;
	if (ib == b.end()) return false;
	return false;
}

vector<Symbols>& Symbols::GetList()
{
	assert(this->type == LIST_SYMBOL || this->type == COMPACT_SYMBOL);
	return(*(*(this->list)));
}

const vector<Symbols>& Symbols::GetList() const
{
	assert(this->type == LIST_SYMBOL || this->type == COMPACT_SYMBOL);
	return(*(*(this->list)));
}

map<string, Symbols>& Symbols::GetMap()
{
	assert(this->type == MAP_SYMBOL);
	return(*(*(this->m)));
}

const map<string, Symbols>& Symbols::GetMap() const
{
	assert(this->type == MAP_SYMBOL);
	return(*(*(this->m)));
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

string Symbols::ToString() const
{
	char tmp[2] = "";
	string result = "";
	switch(this->type)
	{
	case VARIABLE_SYMBOL:
		return(Symbols::variable_table.GetNameByIndex(this->value));
		break;
	case REMARK_SYMBOL:
		return(Symbols::remark_table.GetNameByIndex(this->value));
		break;
	case CONSTANT_SYMBOL:
		tmp[0] = char(this->value);
		return string(tmp);
		break;
	case CHAR_SYMBOL:
		tmp[0] = char(this->c);
		return string(tmp);
		break;
	case INTEGER_SYMBOL:
		Int2String(this->i, result);
		return result;
		break;
	case LONG_SYMBOL:
		Long2String(this->l, result);
		return result;
		break;
	case FLOAT_SYMBOL:
		Float2String(this->f, result);
		return result;
		break;
	case DOUBLE_SYMBOL:
		Double2String(this->d, result);
		return result;
		break;			
	case STRING_SYMBOL:
		return *(*this->s);
		break;
	case COMPACT_SYMBOL:
		{
			result += "(";
			const vector<Symbols>& l = this->GetList();
			if(!l.empty())
			{
				vector<Symbols>::const_iterator i = l.begin();
				result += i->ToString();
				for(++i; i != l.end(); ++i)
				{
					result += ",";
					result += i->ToString();
				}
			}
			result +=")";
			return result;
		}
		break;		
	case LIST_SYMBOL:
		{
			result += "[";
			const vector<Symbols>& l = this->GetList();
			if(!l.empty())
			{
				vector<Symbols>::const_iterator i = l.begin();
				result += i->ToString();
				for(++i; i != l.end(); ++i)
				{
					result += ",";
					result += i->ToString();
				}
			}
			result +="]";
			return result;
		}
		break;
	case MAP_SYMBOL:
		{
			result += "{";
			const map<string, Symbols>& l = this->GetMap();
			if(!l.empty())
			{
				map<string, Symbols>::const_iterator i = l.begin();
				result +=  i->first + "=>" + i->second.ToString();
				for(++i; i != l.end(); ++i)
				{
					result += ",";
					result +=  i->first + "=>" + i->second.ToString();
				}
			}
			result +="}";
			return result;
		}
	default:
		break;
	}
	return "";
}

void Symbols::Display(ostream& o) const
{
	o<<this->ToString();
}

void Symbols::Dump(ostream& o) const
{
	switch(this->type)
	{
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
		o<<this->ToString();
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

int SplitSList(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields)
{
	if(symbols.empty()) return 0;
	if(symbols.front() == Symbols::LEFT_QUOTE && symbols.back() == Symbols::RIGHT_QUOTE)
	{
		return SplitParameters(symbols.begin()+1, symbols.end()-1, fields);
	}
	else
	{
		return -1;
	}
}

//分解表达式为符号列表
int SplitExpression(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields)
{
	fields.clear();
	if(symbols.empty()) return 0;

	vector<Symbols> field;
	int depth = 0;
	for(vector<Symbols>::const_iterator i = symbols.begin(); i != symbols.end(); ++i)
	{
		field.push_back(*i);

		if(*i == Symbols::LEFT_QUOTE)
		{
			depth++;
		}
		else if(*i == Symbols::RIGHT_QUOTE)
		{
			depth--;
		}

		if(depth == 0)
		{
			fields.push_back(field);
			field.clear();
		}
	}

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

