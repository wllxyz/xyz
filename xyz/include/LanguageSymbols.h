#ifndef	LANGUAGE_SYMBOLS_H
#define	LANGUAGE_SYMBOLS_H

#include "StringTable.h"
#include <iostream>
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
	//支持文法解析
	static const Symbols NULL_SYMBOL;	//在文法表示中用空expression表示
	static const Symbols END_SYMBOL;
	//支持WLL0文法自解释
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
	//支持S表达式求解(WLL1文法自解释)
	static const Symbols REMARK_IGNORE;		//忽略$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE括号内的符号的求值
	static const Symbols EVAL;				//执行求值动作
	static const Symbols LEFT_QUOTE;		//S表达式的左括号,用来界定前缀表达式
	static const Symbols RIGHT_QUOTE;		//S表达式的右括号,用来界定前缀表达式
	static const Symbols SEPERATOR;			//S表达式的参数分隔符号,用来分隔前缀表达式参数
	static const Symbols LOAD_TRANSLATIONS;	//调用Wll0Loader加载文法规则
	static const Symbols ADD_TRANSLATIONS;	//调用Wll0Loader加载文法规则,和LOAD_TRANSLATIONS的区别是ADD_TRANSLATIONS不会清空之前的文法规则
	//支持运算控制
	static const Symbols ADD;				//整数加,1..n个参数
	static const Symbols SUB;				//整数减,1..n个参数
	static const Symbols MUL;				//整数乘,1..n个参数
	static const Symbols DIV;				//整数除,1..n个参数
	static const Symbols TRUE;				//逻辑真
	static const Symbols FALSE;				//逻辑假
	static const Symbols COND;				//条件控制
	static const Symbols LOOP;				//循环控制
	static const Symbols EQ;				//等于
	static const Symbols LT;				//小于(字典序)
	static const Symbols AND;				//与
	static const Symbols OR;				//或
	static const Symbols NOT;				//非
	static const Symbols SHELL;				//执行shell命令
	//支持变量存储
    static const Symbols DEF;				//变量定义
	static const Symbols SET;				//变量赋值,如果变量之前没有定义则自动定义
	static const Symbols GET;				//变量引用,如果变量没有定义,返回空值
	static const Symbols PUSH_DATA;			//初始化变量空间并压栈
	static const Symbols POP_DATA;			//销毁本层变量空间并出栈
	static const Symbols PUSH;				//表达式值压入参数栈顶
	static const Symbols POP;				//参数栈顶值出栈到变量
};

istream& operator>> (istream& i,  vector<Symbols>& symbols);
ostream& operator<< (ostream& o, const Symbols& symbol);
ostream& operator<< (ostream& o, const vector<Symbols>& symbols);
int Split(const vector<Symbols>&symbols, Symbols seperator, vector< vector<Symbols> >& fields);
std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::vector<Symbols>& b);
std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::string& b);
const std::string& ToString (std::string& str, const std::vector<Symbols>& symbols);

#endif	//LANGUAGE_SYMBOLS_H

