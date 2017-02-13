#ifndef	TOPDOWN_PARSERS_H
#define	TOPDOWN_PARSERS_H

#include "LanguageParsers.h"

//状态
struct SearchStates
{
	LanguageExpressions expression;		//文法表达式
	size_t position;			//当前词语的位置
	vector<size_t> rules;			//用来记下生成当前状态(表达式)所用到的文法规则号
	//默认构造函数
	SearchStates(){}
	//带参数的构造函数
	SearchStates(const LanguageExpressions& expression,size_t position,const vector<size_t>& rules=vector<size_t>())
	{
		this->expression=expression;
		this->position=position;
		this->rules=rules;
	}
	//拷贝构造函数
	SearchStates(const SearchStates& that)
	{
		this->expression=that.expression;
		this->position=that.position;
		this->rules=that.rules;
	}
	//拷贝函数
	const SearchStates& operator= (const SearchStates& that)
	{
		this->expression=that.expression;
		this->position=that.position;
		this->rules=that.rules;
		return (*this);
	}
	//比较函数(顺序表搜索接口)
	bool operator== (const SearchStates& that)const
	{
		return (this->expression==that.expression && this->position==that.position);
	}

};//状态


class TopDownParsers: public LanguageParsers
{
protected:
	//调用算法分析句法
	virtual bool Parse();
};

#endif	//TOPDOWN_PARSERS_H

