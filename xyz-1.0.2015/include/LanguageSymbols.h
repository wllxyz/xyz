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
};

ostream& operator<< (ostream& o, const Symbols& symbol);
ostream& operator<< (ostream& o, const vector<Symbols>& symbols);

#endif	//LANGUAGE_SYMBOLS_H

