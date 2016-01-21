#include "LanguageAlgorithm.h"
#include "States.h"
#include "FirstSet.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>
#include <stack>
#include <queue>
#include "WllTrace.h"
#include "Wll0Intepreter.h"
#include "Wll1Intepreter.h"
#include "Wll2Intepreter.h"
using namespace std;


bool Expend(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules,ReduceType type)
{
	switch(type)
	{
	case LEFT_REDUCE:
		return LeftExpend(expression,used_rules);
		break;
	case REVERSE_RIGHT_REDUCE:
		return ReverseRightExpend(expression,used_rules);
		break;
	default:
		assert(false);
		break;
	}
	return false;
}

bool LeftExpend(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules)
{
	bool have_noused_rule = false;
	for(vector<LanguageRules*>::const_iterator i = used_rules.begin(); i != used_rules.end(); ++i)			//<<<
	{
		const LanguageRules& rule=**i;

		//在文法表达式中定位将要替代的文法符号(规则开始符号)
		vector<Symbols>::iterator j= find(expression.symbols.begin(),expression.symbols.end(),rule.symbol);	//<<<
		if(j != expression.symbols.end())				//<<<
		{
			//用规则中的文法表达式替代它
			expression.symbols.erase(j);
			copy(rule.expression.symbols.begin(),rule.expression.symbols.end(),inserter(expression.symbols,j));
		}
		else have_noused_rule = true;
	}
	return !have_noused_rule;
}

bool ReverseRightExpend(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules)
{
	bool have_noused_rule = false;
	for(vector<LanguageRules*>::const_reverse_iterator i = used_rules.rbegin(); i != used_rules.rend(); ++i)	//>>>
	{
		const LanguageRules& rule=**i;

		//在文法表达式中定位将要替代的文法符号(规则开始符号)
		//vector<Symbols>::reverse_iterator j= find(expression.symbols.rbegin(),expression.symbols.rend(),rule.symbol);
		vector<Symbols>::iterator j = expression.symbols.end();		//>>>
		for(--j; j > expression.symbols.begin(); --j)			//>>>
		{								//>>>
			if(rule.symbol == *j) break;				//>>>
		}								//>>>

		if(*j == rule.symbol)						//>>>
		{
			//用规则中的文法表达式替代它
			expression.symbols.erase(j);
			copy(rule.expression.symbols.begin(),rule.expression.symbols.end(),inserter(expression.symbols,j));
		}
		else have_noused_rule = true;
	}
	return !have_noused_rule;	
}

//依次使用规则来推导语法树
bool Reduce(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules,ReduceType type)
{
	switch(type)
	{
	case LEFT_REDUCE:
		return LeftReduce(expression,used_rules);
		break;
	case REVERSE_RIGHT_REDUCE:
		return ReverseRightReduce(expression,used_rules);
		break;
	default:
		assert(false);
		break;
	}
	return false;
}

bool LeftReduce(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules)
{
	expression.symbols.clear();
	if(used_rules.empty())
	{
		return true;
	}
	//从开始符号开始推导
	expression += used_rules.front()->symbol;				//<<<

	return LeftExpend(expression,used_rules);
}

bool ReverseRightReduce(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules)
{
	expression.symbols.clear();
	if(used_rules.empty())
	{
		return true;
	}
	//从开始符号开始推导
	expression += used_rules.back()->symbol;				//>>>

	return ReverseRightExpend(expression,used_rules);	
}

bool ConstructTreeByRules(LanguageTree*& source_tree,const vector<size_t>& rules_no,const vector<LanguageRules*>& source_languages,ReduceType type)
{
	switch(type)
	{
	case LEFT_REDUCE:
		return ConstructTreeByLeftReduceRules(source_tree,rules_no,source_languages);
		break;
	case REVERSE_RIGHT_REDUCE:
		return ConstructTreeByReverseRightReduceRules(source_tree,rules_no,source_languages);
		break;
	default:
		assert(false);
		break;
	}
	return false;
}

//根据依次调用的文法规则构造源语言文法分析树
bool ConstructTreeByLeftReduceRules(LanguageTree*& source_tree,const vector<size_t>& rules_no,const vector<LanguageRules*>& source_languages)
{
	if(source_tree!=NULL)
	{
		DestroyTree(source_tree);
		source_tree = NULL;
	}
	
	if(rules_no.empty())
	{
		source_tree=NULL;
		return true;
	}

	assert(rules_no.front()!=NULL_RULE_NO && rules_no.front()<source_languages.size());

	//先构造一棵以第一条文法规则的开始符号为结点树
	source_tree = new LanguageTree();
	source_tree->symbol = source_languages[rules_no.front()]->symbol;	//<<<
	//对每一条规则,插入其生成成分
	for(size_t i = 0; i < rules_no.size(); ++i)					//<<<
	{
		size_t rule_no = rules_no[i];
		assert(rule_no != NULL_RULE_NO && rule_no < source_languages.size());
		LanguageRules* rule=source_languages[rule_no]; //规则
		//根据规则左边符号查找语法分析树,用先根遍历的顺序找到第一个匹配的结点
		LanguageTree *p=LeftFirstSearchTree(source_tree,rule->symbol);	//<<<
		if(p)
		{
			//根据规则插入语法成分
			//设置规则号
			p->rule_no=rule_no;
			//为文法规则的表达式的每一个符号建立一个结点
			for(vector<Symbols>::iterator j=rule->expression.symbols.begin(); j!=rule->expression.symbols.end(); ++j)
			{
				//为文法符号分配新结点
				LanguageTree *q=new LanguageTree();
				q->symbol = *j;
				q->rule_no=NULL_RULE_NO;   //叶子结点无产生式规则
				//链接该结点
				p->symbols.push_back(q);
			}
		}
		else
		{
			cout<<"Unknow Language Rule!"<<*rule<<endl;
		}
	}
	return true;
}

bool ConstructTreeByReverseRightReduceRules(LanguageTree*& source_tree,const vector<size_t>& rules_no,const vector<LanguageRules*>& source_languages)
{
	if(source_tree!=NULL)
	{
		DestroyTree(source_tree);
		source_tree = NULL;
	}
	
	if(rules_no.empty())
	{
		source_tree=NULL;
		return true;
	}

	assert(rules_no.back()!=NULL_RULE_NO && rules_no.back()<source_languages.size());

	//先构造一棵以第一条文法规则的开始符号为结点树
	source_tree = new LanguageTree();
	source_tree->symbol = source_languages[rules_no.back()]->symbol;	//>>>
	//对每一条规则,插入其生成成分
	for(long i = rules_no.size()-1; i >=0; --i)					//>>>
	{
		size_t rule_no = rules_no[i];
		assert(rule_no != NULL_RULE_NO && rule_no < source_languages.size());
		LanguageRules* rule=source_languages[rule_no]; //规则
		//根据规则左边符号查找语法分析树,用先根遍历的顺序找到第一个匹配的结点
		LanguageTree *p=RightFirstSearchTree(source_tree,rule->symbol);	//>>>
		if(p)
		{
			//根据规则插入语法成分
			//设置规则号
			p->rule_no=rule_no;
			//为文法规则的表达式的每一个符号建立一个结点
			for(vector<Symbols>::iterator j=rule->expression.symbols.begin(); j!=rule->expression.symbols.end(); ++j)
			{
				//为文法符号分配新结点
				LanguageTree *q=new LanguageTree();
				q->symbol = *j;
				q->rule_no=NULL_RULE_NO;   //叶子结点无产生式规则
				//链接该结点
				p->symbols.push_back(q);
			}
		}
		else
		{
			cout<<"Unknow Language Rule!"<<*rule<<endl;
		}
	}
	return true;
}

//通过源语言文法分析树建立目标语言文法分析树
bool TranslateTree(LanguageTree* source,LanguageTree*& dest,const vector<LanguageRules*>& destination_languages)
{
	//如果源语言树规则到目标语言树没有规则相对应,则把dest设置为空
	//在这里目标规则总是和源语言规则的规则号相同(也就是总是存在)
	//可以通过一个规则变换表来建立规则之间的对应关系,对于没有对应的规则可以返回一个0值表示对应的规则不存在

	assert(source!=NULL);
	assert(source->rule_no != NULL_RULE_NO && source->rule_no<destination_languages.size());

	//要确保调用时,source不是叶子结点,也不为空
	size_t rule_no=source->rule_no;
	LanguageRules* rule=destination_languages[rule_no];

	dest=new LanguageTree();
	dest->symbol=rule->symbol;
	dest->rule_no=rule_no;

	for(vector<Symbols>::iterator i=rule->expression.symbols.begin(); i != rule->expression.symbols.end(); ++i)
	{
		//查找对应源语言子树的序号
		Symbols symbol=*i;

		if(symbol.IsConstant())
		{
			LanguageTree *p=new LanguageTree();
			p->symbol=symbol;
			p->rule_no=NULL_RULE_NO;

			dest->symbols.push_back(p);
			continue;
		}

		vector<LanguageTree*>::iterator j;
		for(j = source->symbols.begin(); j != source->symbols.end(); ++j)
		{
    			//找不是叶子结点的源语言对应的子树,跳过叶子结点
			if(!(*j)->IsLeaf() && (*j)->symbol.IsVariable())
			{
				size_t rule_no = (*j)->rule_no;
				if(rule_no != NULL_RULE_NO && rule_no<destination_languages.size())
					if(symbol==destination_languages[rule_no]->symbol) break;	//同一条转换子树中不能有同名扩展标识符
			}
		}

		//找到的对应源语言文法子树不为叶子结点
		if(j != source->symbols.end())
		{
			LanguageTree *p;
			TranslateTree(*j,p,destination_languages);	//递归调用
			dest->symbols.push_back(p);
		}
		else //没有找到从源语言转换过来的对应结构,或者源语言对应结构为叶子结点
		{
			//自己建立一个叶子结点
			LanguageTree *p=new LanguageTree();
			p->symbol=symbol;
			p->rule_no=NULL_RULE_NO;

			dest->symbols.push_back(p);
		}
	}//end of for
	return true;
}

//<start>--><wll0>==><start>-->$WLL0<wll0>;
//<wll0>--><translation>";"<spaces>==><wll0>--><translation>;
//<wll0>--><translation>";"<spaces><wll0>==><wll0>--><translation><wll0>;
//<translation>--><source_rule>"==>"<destination_rule>==><translation>--><source_rule><destination_rule>$TRANSLATION;
//<source_rule>--><rule>==><source_rule>--><rule>$SOURCE-RULE;
//<destination_rule>--><rule>==><destination_rule>--><rule>$DESTINATION-RULE;
//<rule>--><root>"-->"<expression>==><rule>--><root><expression>$EXPRESSION$RULE;
//<root>--><variable>==><root>--><variable>$ROOT-SYMBOL;
//<expression>--><symbol>==><expression>--><symbol>;
//<expression>--><symbol><expression>==><expression>--><symbol><expression>;
//<symbol>--><variable>==><symbol>--><variable>$SUB-SYMBOL;
//<symbol>--><constant>==><symbol>--><constant>$SUB-SYMBOL;
//<symbol>--><remark>==><symbol>--><remark>$SUB-SYMBOL;
//<variable>-->"<"<identifier>">"==><variable>--><identifier>$VARIABLE;
//<constant>-->"\""<string>"\""==><constant>--><string>$CONSTANT;
//<remark>-->"$"<identifier>==><remark>--><identifier>$REMARK;
bool SelfExplain(const vector<Symbols>& input_symbols,vector<LanguageTranslations>& languages)
{
	vector<Symbols> output_symbols;
#ifndef	WLL1LOADER
	Wll0Intepreter intepreter;
#else
	Wll1Intepreter intepreter;
	//Wll2Intepreter intepreter();
#endif
	bool retval = intepreter.IntepretWll(input_symbols, output_symbols, &languages);
	cout<<output_symbols;
	return retval;
}

//<预测分析表的自动生成算法>

//<SLR(1)><UPDATE>
//初始化:   I={[S'->S]} {(0,0)}     //S'->S这条规则永远不会得到规约
//</SLR(1)><AS>
//初始化:   I={[S'->S,#]} {((0,0),0)}     //S'->S这条规则永远不会得到规约
//</SLR(1)></UPDATE>
//          I=CLOSURE(I)
//          项目集合列表state_sets初始化为空
//          I加入state_sets中
//          I加入待处理列表stack中
//          (为了节省数据存储空间,可以只把I在state_sets中的标号加入待处理列表stack中.则stack是一个整型标号栈)
//          我们可以通过反查state_sets来获取I
//<循环>:
//  <循环条件>若待处理列表不空</循环条件>
//  <循环体>
//      从待处理列表中取出一个元素(项目集合)-->I

//      <循环>获取出发边列表内容
//          <循环初始化>
//              发出边列表(一个发出边的结构由发出边的符号标记和终止项目集合组成)设置为空
//          <循环初始化>
//          <循环条件>对项目集合I中的每一个项目</循环条件>
//          <循环体>
//              在发出边列表中查找与I中当前子项目的移进边符号匹配的记录
//              若找到,则将该子项目移进一个符号,并将其加入到相应发出边列表项中的终止项目集合中去
//              若未找到,则新建一个以该子项目移进符号为发出边的列表项,并将其加入到相应发出边列表项中的终止项目集合中去
//          </循环体>
//      </循环>

//      <循环>为出发边列表中的终止状态项目集合获取状态号,并把新的状态加入待处理列表中去
//          <循环初始化>
//              状态转换表的当前状态行的转换边列表初始化为空
//          </循环初始化>
//          <循环条件>对发出边列表中的每一项</循环条件>
//          <循环体>
//              首先对当前发出边列表项的终止状态项目集合求闭包e-CLOSURE
//              若当前发出边列表项的终止状态项目集合不在state_sets列表中,则把它加入state_sets中,并返回其标号.同时把它加入待处理列表中
//              若当前发出边列表项的终止状态项目集合在state_sets列表中,则返回其在列表中的标号(状态号)
//              将(当前发出边(符号),目标状态号)转换边加入当前状态行的转换边列表中
//          </循环体>
//      </循环>

//<SLR(1)><DELETE>
//      为当前状态的规约状态生成规约状态
//          如果当前状态项目集合中存在规约子项目A->B*,则为每一个规约子项目,循环
//              对当前规约子项目A->B*,求A的follow集
//              对Follow(A)中的每一个符号,循环
//                  以其符号为边,相应规约规则A->B*的规则号的负数为目标状态建立一条状态转换边,加入当前状态行转换边列表中
//</SLR(1)></DELETE>

//      将当前状态行转换列表插入状态转换表数组中的相应位置中(状态号I)
//  </循环体>
//</循环>

//</预测分析表的自动生成算法>

//根据文法自动生成文法预测分析表(供LR1文法分析器重载)
void GenerateStateTransformTable(const LanguageIndex& languages,vector< vector<TransformEdge> >& state_transform_table, vector< StateSets<LR1States> >& state_sets)
{
	state_transform_table.clear();
	state_sets.clear();

//<预测分析表的自动生成算法>
	stack<int> stack;    							//待处理列表stack
	StateSets<LR1States> I;     					//当前处理的子项目集合

//<SLR(1)><UPDATE>
//初始化:   I={[S'->S]} {(0,0)}     //S'->S这条规则永远不会得到规约
//</SLR(1)><AS>
//初始化:   I={[S'->S,#]} {((0,0),0)}     //S'->S这条规则永远不会得到规约
	set<Symbols> symbol_set;
	symbol_set.insert(Symbols::END_SYMBOL);
	Symbols start_symbol = languages.rules[0]->symbol;
	vector<size_t> start_rules;
	languages.GetRuleNosBySymbol(start_symbol, start_rules);
	for(vector<size_t>::const_iterator i = start_rules.begin(); i != start_rules.end(); ++i)
		I.states.insert(LR1States(languages.GetRule(*i),*i,0,symbol_set));
//</SLR(1)></UPDATE>
//          I=CLOSURE(I)
	Closure(languages,I);
//          项目集合列表state_sets初始化为空
//          I加入state_sets中
	state_sets.push_back(I);
//          I加入待处理列表stack中
	stack.push(0);
//          (为了节省数据存储空间,可以只把I在state_sets中的标号加入待处理列表stack中.则stack是一个整型标号栈)
//          我们可以通过反查state_sets来获取I
//<循环>:
//  <循环条件>若待处理列表不空</循环条件>
	while(!stack.empty())
	{
		vector<TransformEdge> trans_list;
		map< Symbols,StateSets<LR1States> > edges_list;
		int m;
//  <循环体>
//      从待处理列表中取出一个元素(项目集合)-->I
		m = stack.top();
		stack.pop();
		I=state_sets[m];

		DEBUG_LOG("current state set I="<<I);

//      <循环>获取出发边列表内容
//          <循环初始化>
//              发出边列表(一个发出边的结构由发出边的符号标记和终止项目集合组成)设置为空
//          <循环初始化>

//          <循环条件>对项目集合I中的每一个项目</循环条件>
//          <循环体>
		for(set<LR1States>::iterator k = I.states.begin(); k != I.states.end(); ++k)
		{   
			DEBUG_LOG("handle state k="<<*k);
//    如果该项目是规约子项目
			if(k->position==languages.rules[k->rule_no]->expression.symbols.size())
			{
				DEBUG_LOG("k is a reduce item, add transform edges into its transform list");
//      对Follow(A)中的每一个符号,循环
				for(set<Symbols>::iterator j = k->follow.begin(); j != k->follow.end(); ++j)
				{
//       以其符号为边,相应规约规则A->B*的规则号的负数为目标状态建立一条状态转换边,加入当前状态行转换边列表中
					TransformEdge e;
					e.symbol = *j;
					e.action.type = REDUCE_ACTION;
					e.action.value = k->rule_no;
					trans_list.push_back(e);
					DEBUG_LOG("e="<<e);
				}
			}
//              如果该项目是移进子项目，在发出边列表中查找与I中当前子项目的移进边符号匹配的记录
//     若找到,则将该子项目移进一个符号,并将其加入到相应发出边列表项中的终止项目集合中去
//     若未找到,则新建一个以该子项目移进符号为发出边的列表项,并将其加入到相应发出边列表项中的终止项目集合中去
			else
			{
				DEBUG_LOG("k is a move on item, add state into edges list's states set");
				Symbols symbol=languages.rules[k->rule_no]->expression.symbols[k->position];
				LR1States state = *k;
				state.position++;
				//自然合并相同symbol的多条移进子项目
				set<LR1States>::iterator l = edges_list[symbol].states.find(state);
				if(l == edges_list[symbol].states.end())
				{
					edges_list[symbol].states.insert(state);
				}
				else
				{
					//合并follow集合
					l->follow.insert(state.follow.begin(),state.follow.end());
				}
				DEBUG_LOG("state="<<state);
			}
//          </循环体>
		}

//      </循环>
   
//      <循环>为出发边列表中的终止状态项目集合获取状态号,并把新的状态加入待处理列表中去
//          <循环初始化>
//              状态转换表的当前状态行的转换边列表初始化为空
//          </循环初始化>
//          <循环条件>对发出边列表中的每一项</循环条件>
		DEBUG_LOG("process edges list ...");
		for(map< Symbols,StateSets<LR1States> >::iterator i = edges_list.begin(); i != edges_list.end(); ++i)
		{
//          <循环体>
			StateSets<LR1States> J = i->second;
			int n = -1;
//              首先对当前发出边列表项的终止状态项目集合求闭包e-CLOSURE
			Closure(languages,J);
			DEBUG_LOG("edge="<<i->first<<" ==> "<<J);
			vector< StateSets<LR1States> >::iterator index = find(state_sets.begin(),state_sets.end(),J);
//              若当前发出边列表项的终止状态项目集合不在state_sets列表中,则把它加入state_sets中,并返回其标号.同时把它加入待处理列表中
			if(index == state_sets.end())
			{
				DEBUG_LOG("state J="<<J<<" is NOT found in state_sets");
				state_sets.push_back(J);
				n=state_sets.size()-1;
				stack.push(n);
				DEBUG_LOG("assign state no="<<n<<" for the new state");
			}
//              若当前发出边列表项的终止状态项目集合在state_sets列表中,则返回其在列表中的标号(状态号)
			else
			{
				n = index - state_sets.begin();
				DEBUG_LOG("state J="<<J<<" is found in state sets of index ="<<n);
			}
//              将(当前发出边(符号),目标状态号)转换边加入当前状态行的转换边列表中
			TransformEdge e;
			e.symbol = i->first;
			e.action.type = MOVE_ACTION;
			e.action.value = n;
			trans_list.push_back(e);
			DEBUG_LOG("add transform edge e="<<e<<" into transform list");
//          </循环体>
		}//end of for
//      </循环>

//<SLR(1)><DELETE>
//      为当前状态的规约状态生成规约状态
//          如果当前状态项目集合中存在规约子项目A->B*,则为每一个规约子项目,循环
//              对当前规约子项目A->B*,求A的follow集
//              对Follow(A)中的每一个符号,循环
//                  以其符号为边,相应规约规则A->B*的规则号的负数为目标状态建立一条状态转换边,加入当前状态行转换边列表中
//</SLR(1)></DELETE>

//      将当前状态行转换列表插入状态转换表数组中的相应位置中(状态号I)
		if(state_transform_table.size()<state_sets.size())
			state_transform_table.resize(state_sets.size());
		assert(m<state_transform_table.size());
		state_transform_table[m]=trans_list;
		DEBUG_LOG("add transform list into state transform table");
//  </循环体>
	}//end of while
//</循环>
//</预测分析表的自动生成算法>
}

//<e-CLOSURE算法>
//  <参数>I--LR(1)项目集合</参数>
//LR(1)子项目的表示:(项目的心(规则号,移进位置0-length of expression),向后搜索的符号集合)
//  <算法步骤>
//      <循环>对待处理项目集合列表中每一项
//          <循环初始化>
//              R--结果项目集合初始化为I
//              s--待处理项目集合列表初始化为I
//          </循环初始化>
//          <循环条件>待处理列表s不为空</循环条件>
//          <循环体>
//              取出下一个要处理的子项目i(A->B*XC,D)
//              如果X(可以通过规则号+当前移进位置从规则表中获取)是非终结符号
//                  <循环>对X中的每一条规则(X->E)
//                      若子项目(X->*E,FIRST(CD))不在R中
//                          则将子项目(X->*E,FIRST(CD))加入R和s中
//                  </循环>
//          </循环体>
//      </循环>
//  </算法步骤>
//</e-CLOSURE算法>

void Closure(const LanguageIndex& languages, StateSets<LR1States>& I)
{
//<e-CLOSURE算法>
//  <参数>I--LR(1)项目集合</参数>
//LR(1)子项目的表示:(项目的心(规则号,移进位置0-length of expression),向后搜索的符号集合)

	DEBUG_LOG("calculating Closure( I="<<I<<" ) ...");

	queue<LR1States> s;
	for(set<LR1States>::iterator i=I.states.begin(); i != I.states.end(); ++i)
		s.push(*i);
//  <算法步骤>
	while(!s.empty())
//      <循环>对待处理项目集合列表中每一项
	{
//          <循环初始化>
//              R--结果项目集合初始化为I
//              s--待处理项目集合列表初始化为I
//          </循环初始化>
//          <循环条件>待处理列表s不为空</循环条件>
		LR1States i = s.front();
//          <循环体>
//              取出下一个要处理的子项目i(A->B*XC,D)
		s.pop();
		DEBUG_LOG("handle state i = "<<i);
//          如果子项目是待规约项目，则直接取下一个项目
		if(i.position==languages.rules[i.rule_no]->expression.symbols.size())
		{
			DEBUG_LOG("state i ="<<i<<" is a reduce item, skip");
			continue;
		}

		DEBUG_LOG("state i ="<<i<<" is a move on item");
//          获取项目的下一个移进符号
		Symbols X = languages.rules[i.rule_no]->expression.symbols[i.position];
		DEBUG_LOG("rule="<<*languages.rules[i.rule_no]);
		DEBUG_LOG("X="<<X);

//              如果X(可以通过规则号+当前移进位置从规则表中获取)是非终结符号
		if(X.IsVariable())
		{
			DEBUG_LOG("X="<<X<<" is a variable");

			vector<size_t> rule_no_list;
			languages.GetRuleNosBySymbol(X,rule_no_list);

			LR1States state;                //暂时存放要插入的子项目
                    //所有扩展子项目的都是X->*E形式，（即开始位置为0)
			state.position=0;
                     //如果子项目i(A->B*XC,D)中C为空，则用X->E规则规约时的后跟字符集合为D（子项目i的后跟字符集合）
			if(languages.rules[i.rule_no]->expression.symbols.size()-1==i.position)
			{
				DEBUG_LOG("X="<<X<<" is the end symbol of rule="<<*languages.rules[i.rule_no]);

				state.follow=i.follow;
			}
                    //如果子项目i(A->B*XC,D)中C非空，则用X->E规则规约时的后跟字符集合为FIRST(C)
	     	else
	      	{
	     		DEBUG_LOG("X="<<X<<" is NOT the end symbol of rule="<<*languages.rules[i.rule_no]);

				set<Symbols> visited;
				state.follow.clear();
				bool flag = First(languages,languages.rules[i.rule_no]->expression.symbols,i.position+1,visited,state.follow);
				if(flag)	//would be null, if C would -->null, then add i's follow set into X->E 's follow set
				{
					state.follow.insert(i.follow.begin(),i.follow.end()); 
				}
	      	}
                    //设置扩展子项目的规则号，并尝试插入
			for(vector<size_t>::iterator k=rule_no_list.begin(); k != rule_no_list.end(); ++k)
			{
//                  <循环>对X中的每一条规则(X->E)
				state.rule_no = *k;
				state.rule = languages.GetRule(*k);
				DEBUG_LOG("state="<<state);
//                      若子项目(X->*E,FIRST(CD))不在R中
				set<LR1States>::iterator j = I.Find(state);
				if(j==I.states.end())
				{
//                          则将子项目(X->*E,FIRST(CD))加入R和s中
					DEBUG_LOG("state["<<state<<"] are NOT find in states["<<I<<"], INSERT into it");
					I.states.insert(state);
					s.push(state);
				}
				else
				{
					DEBUG_LOG("state["<<state<<"] are find in states["<<I<<"], compare follow set");
					bool update_flag = false;
					//set<Symbols> update_follow = j->follow;
					for(set<Symbols>::const_iterator m = state.follow.begin(); m != state.follow.end(); ++m)
					{
						if(j->follow.find(*m) == j->follow.end())
						{
							//update_follow.insert(*m);
							j->follow.insert(*m);
							update_flag = true;
						}
					}
					if(update_flag)
					{
						DEBUG_LOG("state["<<state<<"]'s follow set are updated, push into queue to recalculate Closure");
						//I.states.erase(j);
						//state.follow = update_follow;
						//I.states.insert(state);
						s.push(state);
					}
					else
					{
						DEBUG_LOG("state["<<state<<"] 's following set is subset of state["<<*j<<"]");
					}
				}
			}
//                  </循环>
		}//END OF IF
		DEBUG_LOG("I="<<I);
	}
//          </循环体>
//      </循环>

//  </算法步骤>
//</e-CLOSURE算法>
}

void ConvertStateTransformTable(const vector< vector<TransformEdge> >& state_transform_table, StateTransformTable& table)
{
	table.clear();
	for(vector<vector<TransformEdge> >::const_iterator i = state_transform_table.begin(); i != state_transform_table.end(); ++i)
	{
		std::map<Symbols,Actions> line;
		for(vector<TransformEdge>::const_iterator j = i->begin(); j != i->end(); ++j)
		{
			line[j->symbol] = j->action;
		}
		table.push_back(line);
	}
}

//根据文法预测分析表分析文法,得到文法分析树(供LR1重载)
bool LRParse(const vector<Symbols>& symbols,LanguageTree*& tree,const StateTransformTable& state_transform_table,const vector<LanguageRules*>& languages, const vector< StateSets<LR1States> >& state_sets)
{
	stack<size_t> state_stack;  		//状态分析栈
	size_t S;     						//当前分析状态号
  	Symbols symbol; 					//当前分析符号
	LanguageTree* symbol_node = NULL;			//当前分析符号生成的语法树node
  	bool read_flag=true; 				//从输入流中读符号标志
  	size_t pos=0;  						//读指针
  	vector<size_t> used_rules; 			//用来记录依次使用的规则号
	vector<LanguageTree*> analyzed_symbols;		//用来记录已经分析过的符号(辅助分析和错误定位) 
	Symbols start_symbol = languages[0]->symbol;	//文法开始符号

  	//初始化
	state_stack.push(0); //第一个入口状态
  
	//分析循环
	do
	{
		//获取当前分析状态
		S=state_stack.top();
		DEBUG_LOG("current state = "<<S);

		//获取输入符号
		if(read_flag)
		{
			//当读取指针指向末尾时，返回输入结束字符0
			symbol=(pos<symbols.size())?symbols[pos]:Symbols::END_SYMBOL;
			DEBUG_LOG("get input as current symbol");
		}
		else
		{
			//自动设置下次从输入流中读取分析字符
			read_flag=true;
			DEBUG_LOG("reduced symbol as current symbol");
		}
		DEBUG_LOG("symbol="<<symbol);

		//在状态转换表中查找目的状态
		map<Symbols,Actions>::const_iterator index = state_transform_table[S].find(symbol);
		
		//若没有找到目的状态号
		if(index==state_transform_table[S].end())
		{
			DEBUG_LOG("NOT find entry for symbol["<<symbol<<"] of state["<<S<<"]");
			//定义出错位置信息
			size_t line=0,column=0;
			vector<Symbols> current_line;

			GetLineColumn(symbols,pos,line,column);
			cerr<<endl;
			cerr<<"current position : "<<pos<<"(line:"<<line<<",column:"<<column<<")"<<endl;

			cerr<<"current_line:";
			GetCurrentLine(symbols,pos,current_line);
			for(size_t i=0; i<current_line.size(); i++)
			{
				if(i==column)
				{
					cerr<<"("<<current_line[i]<<")";
				}
				else
					cerr<<current_line[i];
			}
			cerr<<endl;

			cerr<<"current symbol : "<<symbol;
			symbol.Dump(cerr);
			cerr<<endl;
			cerr<<"current state : "<< state_sets[S] <<endl;
			for(map<Symbols,Actions>::const_iterator i=state_transform_table[S].begin(); i!=state_transform_table[S].end(); ++i)
				cerr << i->first << " ==> " << i->second << endl;
			cerr<<endl;

			cerr<<"accpeted part:"<<endl;
			for(vector<LanguageTree*>::iterator i = analyzed_symbols.begin(); i != analyzed_symbols.end(); ++i)
			{
				cerr<<(*i)->symbol;
				delete (*i);
			}
			cerr<<endl;

#ifdef	DEBUG
			cerr<<"used rules :"<<endl;
			for(vector<size_t>::const_iterator i = used_rules.begin(); i != used_rules.end(); ++i)
			{
				cerr<<(*languages[*i])<<endl;
			}
#endif

			//出错处理,句法分析失败结束
			return false;
		}

		Actions action = index->second;
		DEBUG_LOG("entry find in transform table, action="<<action);

		//找到目标状态号,进行移进和规约处理
		if(action.type == MOVE_ACTION) //移进（状态转换）
		{
			DEBUG_LOG("action ["<<action<<"] is a move on action");
			//状态压栈
			state_stack.push(action.value);
			DEBUG_LOG("state ["<<action.value<<"] is pushed into state stack");
			//对于规约的非终结符号移进，已经读取的输入终结符号保留
			if(!symbol.IsVariable())		//we accept constant and remark(for self explain grammar wll.xyz check) now.(except variable)
			{
				//生成语法节点
				symbol_node = new LanguageTree();
				symbol_node->symbol = symbol;
				//读取指针前移
				if(pos<symbols.size()) ++pos;
				DEBUG_LOG("symbol["<<symbol<<"] is constant, advance input position");
			}
			analyzed_symbols.push_back(symbol_node);
			DEBUG_LOG("symbol["<<symbol<<"] is pushed into analyzed symbols stack");
		}

		else if(action.type == REDUCE_ACTION) //规约
		{
			DEBUG_LOG("action ["<<action<<"] is a reduce action");
			//记录使用的规则号,以便在分析成功时生成文法分析树
			used_rules.push_back(action.value);
			DEBUG_LOG("use rule ["<<(*languages[action.value])<<"] to reduce");

			//把规约后的文法符号(非终结符号)作为下一次的文法符号输入
			symbol=languages[action.value]->symbol;
			symbol_node = new LanguageTree();
			symbol_node->symbol = symbol;
			symbol_node->rule_no = action.value;
			read_flag=false;
			//analyzed_symbols.Push(symbol);  //don't push here, for it will push in the above branch (S>0 : march[移进])
			DEBUG_LOG("reduced symbol["<<symbol<<"] is used as next analyze symbol");

			//弹出这条规则的移进状态(回退到调用这条规则的状态)
			vector<LanguageTree*> tmp_revert_symbols;
			for(size_t i=0; i<languages[action.value]->expression.symbols.size(); i++)
			{
				DEBUG_LOG("state["<<state_stack.top()<<"] is pop from state stack");
				state_stack.pop();

				LanguageTree* tree_node = analyzed_symbols.back();
				DEBUG_LOG("analyzed symbol["<<tree_node->symbol<<"] is pop from analyzed_symbols stack");	
				analyzed_symbols.pop_back();
				tmp_revert_symbols.push_back(tree_node);
			}

			for(vector<LanguageTree*>::reverse_iterator i=tmp_revert_symbols.rbegin(); i != tmp_revert_symbols.rend(); ++i)
			{
				symbol_node->symbols.push_back(*i);
			}
			
			//<Insert>
			if(state_stack.top()==0 && symbol==start_symbol && pos>=symbols.size())//规约的规则是扩展文法的开始符号
			{
				DEBUG_LOG("analyzing finish when reduce used the first rule ["<<(*languages[0])<<"] of language, and the input symbols are all consumed");
#ifdef	DEBUG
				DEBUG_LOG("used rules:");
				for(vector<size_t>::const_iterator i = used_rules.begin(); i != used_rules.end(); ++i)
				{
					DEBUG_LOG(*languages[*i]);
				}
#endif	//DEBUG
			    	//文法分析成功
			    	//根据分析过程中依次规约的规则建立文法分析树
			   	//ConstructTreeByRules(tree,used_rules,languages,REVERSE_RIGHT_REDUCE);
				tree = symbol_node;
			   	return true;
			}
			//</Insert>
		}
	}while(!state_stack.empty()); //这两个条件正常情况下永远成立

	return false;
}


