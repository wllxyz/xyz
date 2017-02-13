#include "FirstSet.h"

//<FIRST算法>
//  <参数>e-文法符号表达式,s-已经访问过的文法符号的集合的引用(初始调用为空)</参数>
//  设e=(E1,E2,...,Ei,...,En)
//  对Ei(i=1...n)循环
//  <循环初始化>
//      FIRST(e)为空集
//  </循环初始化>
//  <循环体>
//      如果Ei是非空终结符,则把Ei加入FIRST(e)中,并结束循环
//      如果Ei是空终结符,则把控制传递给E(i+1)
//      如果Ei是非终结符,则首先判断Ei是否被访问过,如果没有访问过则继续下续步骤
//          首先标记Ei被访问过了(Ei-->s)
//          对Ei的所有产生式规则的右边表达式ei,循环
//              FIRST(e)=FIRST(e) U FIRST(ei)   [递归调用] 参数为ei,s
//      如果FIRST(Ei)中有空符号,则把控制传递给E(i+1),否则结束循环
//  </循环体>
//算法思想:FIRST(e)=FIRST(E1)[U FIRST(E2) (NULL 属于 FIRST(E1)) ]...
//</FIRST算法>

bool First(const LanguageIndex& language, Symbols e, set<Symbols>& result)
{
	set<Symbols> visited;
	result.clear();
	return First(language,e,visited,result);
}

//if e-->NULL then return true (would be null), first elements will added into result set(not clear result in algorith, you should make sure it's empty or not when call it)
bool First(const LanguageIndex& language, Symbols e,set<Symbols>& visited,set<Symbols>& result)
{
	if(!e.IsVariable()) 
	{
		result.insert(e);
		return false;
	}
	//is variable
	if(visited.find(e) != visited.end()) 
	{
		return false;
	}
	visited.insert(e);

	vector<LanguageRules*> rules_list;
	language.GetRulesBySymbol(e,rules_list);

	bool flag = false;
	for(vector<LanguageRules*>::iterator it = rules_list.begin(); it != rules_list.end(); ++it)
	{
		if((*it)->expression.symbols.empty())
		{
			flag = true;
		}
		else
		{
			bool tmp = First(language,(*it)->expression.symbols,0,visited,result);
			flag = flag || tmp;
		}
	}

	return flag;
}

bool First(const LanguageIndex& language, const vector<Symbols>& e,size_t i,set<Symbols>& visited,set<Symbols>& result)
{
	if(i>=e.size()) return true;	//empty set
	bool flag = First(language,e[i],visited,result);
	if(flag)
		return First(language,e,i+1,visited,result);
	else
		return false;
}

void CalculateAllFirstSet(const LanguageIndex& language, map<Symbols,set<Symbols> >& first_table)
{
	first_table.clear();

	set<Symbols> variables;
	for(multimap<Symbols,size_t>::const_iterator i = language.rule_index.begin(); i != language.rule_index.end(); ++i)
	{
		variables.insert(i->first);
	}
	set<Symbols> result;
	for(set<Symbols>::iterator i = variables.begin(); i != variables.end(); ++i)
	{
		result.clear();
		if(First(language,*i,result))
		{
			result.insert(Symbols::NULL_SYMBOL);
		}
		first_table.insert(pair<Symbols,set<Symbols> >(*i,result));
	}
}

bool LookupFirst(const map<Symbols, set<Symbols> >& first_table, Symbols e,set<Symbols>& result,bool& null_exist)
{
	result.clear();

	if(!e.IsVariable())
	{
		result.insert(e);
		null_exist = false;
		return true;
	}

	map<Symbols,set<Symbols> >::const_iterator it = first_table.find(e);
	if(it == first_table.end())
	{
		null_exist = false;
		return false;
	}

	result = it->second;
	set<Symbols>::iterator it2 = result.find(Symbols::NULL_SYMBOL);
	if( it2 != result.end())
	{
		result.erase(it2);
		null_exist = true;
	}
	else
	{
		null_exist = false;
	}
	return true;
}

FirstCalculator::FirstCalculator(const LanguageIndex& lang):
language(lang)
{
	this->initialized = false;
}

void FirstCalculator::Initialize()
{
	CalculateAllFirstSet(this->language,this->first_table);
	this->initialized = true;
}

bool FirstCalculator::First(Symbols e, set<Symbols>& result)
{
	if(this->initialized)
	{
		bool null_exist;
		LookupFirst(this->first_table, e, result, null_exist);
		return null_exist;
	}
	else
	{
		return ::First(this->language, e, result);
	}
}

bool FirstCalculator::Select(Symbols e, Symbols input_symbol)
{
	set<Symbols> result;
	bool null_able = this->First(e,result);
	return (null_able || result.find(input_symbol) != result.end());
}
