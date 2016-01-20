#ifndef	LANGUAGE_SYMBOLS_H
#define	LANGUAGE_SYMBOLS_H

#include "StringTable.h"
#include <ostream>
#include <vector>
using namespace std;
using namespace Wll;

//Symbols
enum SymbolTypes 
{
	UNKNOW_SYMBOL,
	VARIABLE_SYMBOL,
	CONSTANT_SYMBOL,
	REMARK_SYMBOL
};

//when type =
//	VARIABLE_SYMBOL: value = index of variable in names table
//	CONSTANT_SYMBOL: value = code of character
//	REMARK_SYMBOL: 	 value = remark define value 
struct Symbols
{
public:
	SymbolTypes type;
	int value;
public:
	Symbols();
	Symbols(const char* variable);
	Symbols(char constant);
	Symbols(SymbolTypes type,const char* remark);
	bool operator== (const Symbols& that) const;
	bool operator< (const Symbols& that) const;
public:
	bool IsVariable()const;
	bool IsConstant()const;
	bool IsRemark()const;
	virtual void Display(ostream& o) const;
	void Dump(ostream& o) const;
public:
	static StringTable variable_table;
	static StringTable remark_table;
	static const Symbols REMARK_WLL0;
	static const Symbols REMARK_TRANSLATION;
	static const Symbols REMARK_SOURCE_RULE;
	static const Symbols REMARK_DESTINATION_RULE;
	static const Symbols REMARK_RULE;
	static const Symbols REMARK_ROOT_SYMBOL;
	static const Symbols REMARK_EXPRESSION;
	static const Symbols REMARK_SUB_SYMBOL;
	static const Symbols REMARK_VARIABLE;
	static const Symbols REMARK_CONSTANT;
	static const Symbols REMARK_REMARK;
	static const Symbols REMARK_IGNORE;	//
	static const Symbols NULL_SYMBOL;	//在文法表示中用空expression表示
	static const Symbols END_SYMBOL;
	static const Symbols LEFT_QUOTE;		//S表达式的左括号,用来界定前缀表达式
	static const Symbols RIGHT_QUOTE;		//S表达式的右括号,用来界定前缀表达式
	static const Symbols SEPERATOR;			//S表达式的参数分隔符号,用来分隔前缀表达式参数
	static const Symbols LOAD_TRANSLATIONS;	//调用Wll0Loader加载文法规则
	static const Symbols ADD_TRANSLATIONS;	//调用Wll0Loader加载文法规则,和LOAD_TRANSLATIONS的区别是ADD_TRANSLATIONS不会清空之前的文法规则

	static const Symbols ADD;
	static const Symbols SUB;
	static const Symbols MUL;
	static const Symbols DIV;
	static const Symbols TRUE;
	static const Symbols FALSE;
	static const Symbols COND;
	static const Symbols EQ;	
	static const Symbols LT;
	static const Symbols AND;
	static const Symbols OR;
	static const Symbols NOT;
	static const Symbols SHELL;
	//支持变量存储
	static const Symbols SET;
	static const Symbols GET;
	static const Symbols PUSH_DATA;
	static const Symbols POP_DATA;
};

ostream& operator<< (ostream& o, const Symbols& symbol);
ostream& operator<< (ostream& o, const vector<Symbols>& symbols);
int Split(const vector<Symbols>&symbols, Symbols seperator, vector< vector<Symbols> >& fields);
std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::vector<Symbols>& b);
std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::string& b);
const std::string& ToString (std::string& str, const std::vector<Symbols>& symbols);

#endif	//LANGUAGE_SYMBOLS_H

