#ifndef	LANGUAGE_SYMBOLS_H
#define	LANGUAGE_SYMBOLS_H

#include "StringTable.h"
#include <memory>
#include <iostream>
#include <vector>
#include <map>
using namespace std;
using namespace Wll;

//Symbols
enum SymbolTypes 
{
	VOID_SYMBOL,		//VOID类型，作为空类型
	VARIABLE_SYMBOL,	//文法变量类型
	REMARK_SYMBOL,		//文法标记类型
	CONSTANT_SYMBOL,	//文法常量类型
	//自動類型轉換的順序是由低向高
	CHAR_SYMBOL,            //单个字符类型
	INTEGER_SYMBOL,		//整数类型
	LONG_SYMBOL,		//长整数类型
	FLOAT_SYMBOL,		//浮点数类型
	DOUBLE_SYMBOL,		//双精度浮点数类型
	STRING_SYMBOL,		//字符串类型
	
	COMPACT_SYMBOL,		//S表达式类型(同之前的内部打包解包类型)	
	LIST_SYMBOL,		//数组类型
	MAP_SYMBOL		//字典类型
};

//when type =
//	VARIABLE_SYMBOL: value = index of variable in names table
//	CONSTANT_SYMBOL: value = code of character
//	REMARK_SYMBOL: 	 value = remark define value 
struct Symbols
{
public:
	union {
		int value;
		char c;
		int i;
		long l;
		float f;
		double d;
		shared_ptr<string>* s;
		shared_ptr< vector<Symbols> >* list;
		shared_ptr< map<string, Symbols> >* m;		
	};
	SymbolTypes type;
private:
	void Init();
	void Destroy();
	void Copy(const Symbols& that);
public:
	Symbols();
	Symbols(const Symbols& that);
	Symbols& operator= (const Symbols& that);
	~Symbols();
	Symbols(const char* variable);
	Symbols(char constant);
	Symbols(SymbolTypes type,const char* remark);
	Symbols(SymbolTypes symbol_type,char c);
	Symbols(int i);
	Symbols(long l);
	Symbols(float f);
	Symbols(double d);
	Symbols(SymbolTypes symbol_type, vector<Symbols> symbols);
	Symbols(SymbolTypes type);
	bool operator== (const Symbols& that) const;
	bool operator< (const Symbols& that) const;
public:
	vector<Symbols>& GetList();
	const vector<Symbols>& GetList() const;
	map<string, Symbols>& GetMap();
	const map<string, Symbols>& GetMap() const;
public:
	bool IsVariable()const;
	bool IsConstant()const;
	bool IsRemark()const;
	string ToString()const;
	void Display(ostream& o) const;
	void Dump(ostream& o) const;
public:
	static StringTable variable_table;
	static StringTable remark_table;
	
	static const Symbols VOID;
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
	static const Symbols EVAL;				//执行求值动作,受外部$IGNORE制约
	static const Symbols EXEC;				//执行求值动作,不受外部$IGNORE制约
	static const Symbols LEFT_QUOTE;		//S表达式的左括号,用来界定前缀表达式
	static const Symbols RIGHT_QUOTE;		//S表达式的右括号,用来界定前缀表达式
	static const Symbols SEPERATOR;			//S表达式的参数分隔符号,用来分隔前缀表达式参数
	static const Symbols LOAD_TRANSLATIONS;	//调用Wll0Loader加载文法规则
	static const Symbols ADD_TRANSLATIONS;	//调用Wll0Loader加载文法规则,和LOAD_TRANSLATIONS的区别是ADD_TRANSLATIONS不会清空之前的文法规则	
	//支持Ｓ表达式求值
	static const Symbols LIST;
	static const Symbols APPEND;
	static const Symbols CAR;
	static const Symbols CDR;
	//支持运算控制
	static const Symbols ADD;				//整数加,1..n个参数
	static const Symbols SUB;				//整数减,1..n个参数
	static const Symbols MUL;				//整数乘,1..n个参数
	static const Symbols DIV;				//整数除,1..n个参数
	static const Symbols SUB_STR;		//取子串,from,size,1或2個參數
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
	static const Symbols CALL;			//执行call命令
	//支持变量存储
    static const Symbols DEF;				//变量定义
	static const Symbols SET;				//变量赋值,如果变量之前没有定义则自动定义
	static const Symbols GET;				//变量引用,如果变量没有定义,返回空值
	static const Symbols PUSH_DATA;			//初始化变量空间并压栈
	static const Symbols POP_DATA;			//销毁本层变量空间并出栈
	static const Symbols PUSH;				//表达式值压入参数栈顶
	static const Symbols POP;				//参数栈顶值出栈到变量
	//支持数据结构
	static const Symbols ARRAY;				//创建Symbols(LIST_SYMBOL)数值变量
	static const Symbols MAP;				//创建Symbols(MAP_SYMBOL)字典变量
	static const Symbols INDEX;				//ARRAY/MAP寻址指令，返回REF_SYMBOL类型变量(地址类型)
	
	static const Symbols CAT;				//从文件中加载符号,支持INCLUDE特性
	//類型常量
	static const Symbols CHAR;
	static const Symbols INTEGER;
	static const Symbols LONG;
	static const Symbols FLOAT;
	static const Symbols DOUBLE;
	static const Symbols STRING;
	
	static const Symbols CAST;			//類型轉換
};

bool operator== (const vector<Symbols>& a, const vector<Symbols>& b);
bool operator< (const vector<Symbols>& a, const vector<Symbols>& b);
bool operator== (const map<string,Symbols>& a, const map<string,Symbols>& b);
bool operator< (const map<string,Symbols>& a, const map<string,Symbols>& b);

istream& operator>> (istream& i,  vector<Symbols>& symbols);
ostream& operator<< (ostream& o, const Symbols& symbol);
ostream& operator<< (ostream& o, const vector<Symbols>& symbols);
int Split(const vector<Symbols>&symbols, Symbols seperator, vector< vector<Symbols> >& fields);
int SplitParameters(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields);
int SplitParameters(vector<Symbols>::const_iterator begin, vector<Symbols>::const_iterator end, vector< vector<Symbols> >& fields);
//分解S表达式为符号表
//<expression>--><symbol>;
//<expression>--><symbol><expression>;
//<symbol>--><variable>;
//<symbol>--><constant>;
//<symbol>--><remark>;
//<symbol>--><s_expression>;
//<s_expression>-->$LEFT_QUOTE<expression_list>$RIGHT_QUOTE;
//<expression_list>--><expression>;
//<expression_list>--><expression>$SEPERATOR<expression_list>;
int SplitSList(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields);
//分解表达式为符号列表
int SplitExpression(const vector<Symbols>&symbols, vector< vector<Symbols> >& fields);

void ComposeSList(vector<vector<Symbols> >::const_iterator begin, vector<vector<Symbols> >::const_iterator end, vector<Symbols>& slist);
std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::vector<Symbols>& b);
std::vector<Symbols>& operator+= (std::vector<Symbols>& a, const std::string& b);
const std::string& ToString (std::string& str, const std::vector<Symbols>& symbols);

void AssertSymbolsType(vector<Symbols> symbols, SymbolTypes symbol_type);

#endif	//LANGUAGE_SYMBOLS_H

