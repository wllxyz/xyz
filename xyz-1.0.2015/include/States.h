#ifndef	LR_STATES_H
#define	LR_STATES_H

#include <set>
#include <vector>
#include <algorithm>
using namespace std;
#include "LanguageSymbols.h"
#include "LanguageRules.h"

//状态
struct States
{
	const LanguageRules* rule;		//
	size_t	rule_no;     //文法规则号
	size_t	position;     //当前词语的位置(0-length)

	//默认构造函数
	States(){ this->rule = NULL; this->rule_no=0; size_t position=0; }
	States(const LanguageRules* rule, size_t rule_no, size_t position)
	{
		this->rule = rule;
		this->rule_no = rule_no;
		this->position = position;
	}
	//比较函数(顺序表搜索接口)
	virtual bool operator== (const States& that) const
	{
		return (this->rule_no==that.rule_no && this->position==that.position);
	}
	virtual bool operator< (const States& that) const
	{
		return (this->rule_no<that.rule_no || this->rule_no==that.rule_no && this->position<that.position);
	}

	bool IsReduceState() const
	{
		return this->rule->expression.symbols.size() == this->position;
	}

	virtual void Display(ostream& o)const
	{
	    o<<"(";
	    o<<this->rule_no;
	    o<<",";
	    o<<this->position;
	    o<<",";
	    o<<this->rule->symbol;
	    o<<"-->";
	    size_t i=0;
	    for(vector<Symbols>::const_iterator j = this->rule->expression.symbols.begin(); j!=this->rule->expression.symbols.end(); ++j)
	    {
	    	if(i==this->position) o<<"*";
	    	o<<*j;
	    	++i;
	    }
	    if(i==this->position) o<<"*";
	    o<<")";
	}
};//状态

inline ostream& operator<< (ostream& o, const States& state)
{
	state.Display(o);
	return o;
}

//LR1子项目
struct LR1States: public States
{
	//增加的数据成员
	mutable set<Symbols> follow;   //后跟终结符号的集合(引入集合数据结构?还是直接用有序线性表来实现?)

	LR1States(){}
	LR1States(const LanguageRules* rule, size_t rule_no, size_t position, set<Symbols>& that_follow):
		States(rule,rule_no,position),
		follow(that_follow)
	{
	}

	//比较函数(顺序表搜索接口)
	virtual bool operator== (const LR1States& that) const
	{
		return (this->rule_no==that.rule_no && this->position==that.position && this->follow==that.follow);
	}
	virtual bool operator< (const LR1States& that) const
	{
		if(this->rule_no<that.rule_no) return true;
		if(this->rule_no>that.rule_no) return false;
		if(this->position<that.position) return true;
		if(this->position>that.position) return false;
		if(this->follow<that.follow) return true;
		if(this->follow>that.follow) return false;

		return false;
	}

	virtual void Display(ostream& o)const
	{
		o<<"(";
		States::Display(o);
		o<<",";
		o<<"(";
		for(set<Symbols>::const_iterator i=this->follow.begin(); i != this->follow.end(); ++i)
		{
			o<<*i;
		}
	    o<<")";
		o<<")";
	}
};//end of LR1States

/*
inline ostream& operator<< (ostream& o, const LR1States& s)
{
	s.Display(o);
	return o;
}
*/

//状态集
template<typename T>
struct StateSets
{
	typedef typename set<T>::iterator iterator_type;
	set<T> states;

	//比较函数（匹配函数）
	bool operator== (const StateSets& that)const
	{
		if(this->states.size()!=that.states.size()) return false;
		return equal(this->states.begin(),this->states.end(),that.states.begin());
	}

	iterator_type Find(const T& data)
	{
		for(iterator_type i=this->states.begin(); i!=this->states.end(); ++i)
		{
			if(i->rule_no == data.rule_no && i->position == data.position) return i;
		}
		return this->states.end();
	}
};

inline ostream& operator<<(ostream& o, const StateSets<LR1States>& ss)
{
	o<<"(";

	if (!ss.states.empty())
	{
		set<LR1States>::const_iterator i=ss.states.begin();
		o<<*i;
		for ( ++i; i != ss.states.end(); ++i )
		{
			o<<","<<*i;
		}
	}
	o<<")";

	return o;
}

#endif	//LR_STATES_H

