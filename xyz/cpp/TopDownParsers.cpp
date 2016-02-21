#include "TopDownParsers.h"
#include "LanguageAlgorithm.h"
#include "WllTrace.h"
#include <stack>
#include <cassert>
using namespace std;

//输出状态到输出流中
ostream& operator<< (ostream& o,const SearchStates& s)
{
	o<<"("<<s.expression<<","<<s.position<<")"<<endl;
	o<<"(";
	if(!s.rules.empty())
	{
		for(vector<size_t>::const_iterator i=s.rules.begin(); i<s.rules.end(); ++i)
			o<<*i<<',';
	}
	o<<")";
	o<<endl;
	return o;
}

//调用算法分析句法
bool TopDownParsers::Parse()
{
//输入字符流==>查阅词典==>单词流==>句法分析器的输入
//<句法分析算法>
//	构建词典
//	构建符号表(包括终结符号和非终结符号)
//	构建文法产生式表
//	初始化状态栈为空,初始状态((S)1)压栈
//	<循环><结束条件>栈为空,(搜索了全部状态都没有匹配)算法失败</结束条件>
//		栈顶状态出栈到当前状态C
//		如果当前状态C是一个空符号链表,而且当前词语的位置恰好处于句子末尾,那么算法成功返回
//		如果当前状态C是一个空符号链表,但是当前词语的位置不是处于句子末尾
//		或者当前状态C不是一个空符号链表,当前词语的位置已经处于句子末尾
//		则跳过这次循环,(抛弃这个状态)分析下一个状态
//		如果C中符号表的第一个符号和输入符号匹配
//			则从符号表中删除第一个符号,更新当前词语的位置(+1),把它构成一个新状态加入状态栈中
//		否则(C中符号表的第一个符号和输入符号不匹配),
//			如果C中符号表的第一个符号是非终结符
//				则根据第一个非终结符号查找文法产生式表,把所有能够由此非终结符号产生的文法表达式替换非终结符号,构成新的状态集,加入到状态搜索栈中
//			否则,抛弃这个状态,分析下一个状态.(什么也不做)
//	</循环>
//<句法分析算法>
	INFO("start top-down parse");
	vector<SearchStates> success_states;
	stack<SearchStates> state_stack;					//状态栈
	SearchStates state(LanguageExpressions(this->start_symbol),0);		//当前状态
	bool flag=false;							//成功匹配标志
	unsigned long int steps=0;						//记录分析的步数
	int max_matched_length=0;						//for error analyze
	SearchStates max_matched_state;	//for error analyze
//	初始化状态栈为空,初始状态((S)1)压栈
	state_stack.push(state);
//	<循环><结束条件>栈为空,(搜索了全部状态都没有匹配)算法失败</结束条件>
	while(!state_stack.empty())
	{
//		栈顶状态出栈到当前状态C
		state = state_stack.top();
		state_stack.pop();
		//cout<<state<<endl;
		steps++;

//		如果当前状态C是一个空符号链表,而且当前词语的位置恰好处于句子末尾,那么算法成功返回
		if(state.position==this->input_symbols.size() && state.expression.symbols.empty())
		{
			//成功匹配！
			success_states.push_back(state);
			flag=true;
#ifdef	SEARCH_ALL_SOLUTIONS
			continue;
#else
			break;
#endif	//SEARCH_ALL_SOULUTIONS
		}
//		如果当前状态C是一个空符号链表,但是当前词语的位置不是处于句子末尾
//		或者当前状态C不是一个空符号链表,当前词语的位置已经处于句子末尾
//		则跳过这次循环,(抛弃这个状态)分析下一个状态
		if(state.expression.symbols.empty() || state.position==this->input_symbols.size()) continue;
		Symbols current_symbol = state.expression.symbols.front();
		Symbols current_input_symbol = this->input_symbols[state.position];
//		如果C中符号表的第一个符号和输入符号匹配
		if(current_input_symbol == current_symbol)
		{
//			则从符号表中删除第一个符号,更新当前词语的位置(+1),把它构成一个新状态加入状态栈中
			state.expression.symbols.erase(state.expression.symbols.begin());
			state.position++;
			state_stack.push(state);
		}
//		否则(C中符号表的第一个符号和输入符号不匹配),
		else if(current_symbol.IsVariable())
		{
//			如果C中符号表的第一个符号是非终结符
//				则根据第一个非终结符号查找文法产生式表,把所有能够由此非终结符号产生的文法表达式替换非终结符号,构成新的状态集,加入到状态搜索栈中
//			否则,抛弃这个状态,分析下一个状态.(什么也不做)

			//下一个输入符号应该在当前扩展的变量的first集合中,否则跳过(终止)这个分析状态
			if(this->first_calculator.Select(current_symbol, current_input_symbol))
			{
				vector<size_t> rule_nos;
				if(this->languages.source_rules.GetRuleNosBySymbol(current_symbol, rule_nos))
				{
					state.expression.symbols.erase(state.expression.symbols.begin());
					for(vector<size_t>::reverse_iterator i = rule_nos.rbegin(); i < rule_nos.rend(); ++i)
					{
						LanguageExpressions& entend_expression = this->languages.source_rules.rules[*i]->expression;
						//根据first集合过滤一些规则
						if(entend_expression.symbols.empty())
						{
							SearchStates new_state;
							new_state.expression=state.expression;
							new_state.position=state.position;
							new_state.rules=state.rules;
							new_state.rules.push_back(*i);
							state_stack.push(new_state);
						}
						else	//not empty rule
						{
							Symbols entend_first_symbol = entend_expression.symbols.front();

							if(this->first_calculator.Select(entend_first_symbol, current_input_symbol))
							{
								SearchStates new_state;
								new_state.expression=entend_expression+state.expression;
								new_state.position=state.position;
								new_state.rules=state.rules;
								new_state.rules.push_back(*i);
								state_stack.push(new_state);
							}
						}
					}
				}
				else
				{
					//save the longest matched search state(for error analyze)
					if(state.position>max_matched_length)
					{
						max_matched_length=state.position;
						max_matched_state=state;
					}	
				}
			}//if(this->first_calculator.Select(current_symbol, current_input_symbol))
		}
		else	//字符常量和输入符号没有匹配
		{
			if(state.position>max_matched_length)
			{
				max_matched_length=state.position;
				max_matched_state=state;
			}
		}
	}//end of while
//	</循环>
	INFO("Total steps:"<<steps);
	if(flag)
	{
		SearchStates state;
		if(success_states.size()==1)
		{
			state = success_states.front();
		}
		else
		{
			for(size_t i =0; i<success_states.size(); ++i)
			{
				cout<<"solution "<<i<<endl;
				cout<<success_states[i]<<endl;
				ConstructTreeByRules(this->source_tree,success_states[i].rules,this->languages.source_rules.rules,LEFT_REDUCE);
				DisplayTree(cout,this->source_tree,0);
				cout<<endl;
				cout<<this->source_tree<<endl;
			}
			int choose;
			cout<<"please choose a solution:";
			cin>>choose;
			assert(0<=choose && choose<success_states.size());
			state = success_states[choose];
		}
		//根据使用的规则构造文法分析树
		INFO(">>>ConstructTreeByRules");
		ConstructTreeByRules(this->source_tree,state.rules,this->languages.source_rules.rules,LEFT_REDUCE);
		INFO("<<<ConstructTreeByRules");
	}
	else
	{	
		//output error analyze info
		cout<<"parse failed"<<endl;
		cout<<"the max matched state:"<<endl;
		cout<<max_matched_state<<endl;
		cout<<"the current input symbol = "<<this->input_symbols[max_matched_state.position]<<endl;
		for(size_t i=0; i<this->input_symbols.size(); i++)
		{
			if(i==max_matched_state.position)
			{
				cout<<"["<<this->input_symbols[i]<<"]";
			}
			else
			{
				cout<<this->input_symbols[i];
			}
		}
		cout<<endl;
	}
	return flag;
}//Parse

