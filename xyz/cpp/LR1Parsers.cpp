#include "LR1Parsers.h"
#include "LanguageAlgorithm.h"
#include <fstream>
#include <cctype>
#include <cassert>
#include <WllTrace.h>
#include "WllUtil.h"
#include <string.h>
using namespace std;
using namespace Wll::Util;

//判定文法是否存在移近规约冲突（文法二义性判断）
bool LR1Parsers::IsAmbiguous(const vector< vector< TransformEdge > >& state_transform_table, const vector< StateSets<LR1States> >& state_sets)
{
	assert(state_transform_table.size() == state_sets.size());

	bool flag = false;
	int n = 0;
	for(vector< vector< TransformEdge > >::const_iterator i = state_transform_table.begin(); i != state_transform_table.end(); ++i)
	{
		map<Symbols,int> stat_map;
		for(vector<TransformEdge>::const_iterator j = i->begin(); j != i->end(); ++j)
		{
			Symbols symbol = j->symbol;
			if(stat_map.find(symbol)==stat_map.end())
				stat_map[symbol]=1;
			else
				stat_map[symbol]++;
		}

		//check & output conflict
		if(stat_map.size() != i->size())
		{
			flag = true;
			//#ifdef	DEBUG
			cerr<<"conflict in state transform table (state no = "<<n<<") :"<<endl;
			cerr<<state_sets[n]<<endl;

			for(map<Symbols,int>::iterator it = stat_map.begin(); it != stat_map.end(); ++it)
			{
				if(it->second > 1)	//conflict item
				{
					for(vector<TransformEdge>::const_iterator j = i->begin(); j != i->end(); ++j)
					{
						if(j->symbol == it->first)
						{
							cerr<<*j<<endl;
						}
					}
					for(set<LR1States>::const_iterator k=state_sets[n].states.begin(); k!=state_sets[n].states.end(); ++k)
					{
						if(k->IsReduceState())
						{
							if(k->follow.find(it->first)!=k->follow.end())
							{
								cerr<<*k<<endl;
							}
						}
						else if(it->first == k->rule->expression.symbols[k->position])
						{
							cerr<<*k<<endl;
						}
					}
				}
			}
			//#endif	//DEBUG 
		}

		++n;
	}
	return flag;
}

//根据文法规则对输入符号流进行文法结构的分析,分析结果用文法分析树表示
bool LR1Parsers::Parse(const LanguageGrammar& languages, const std::vector<Symbols>& input_symbols, LanguageTree*& source_tree, Symbols start_symbol)
{
  	//根据文法自动生成文法预测分析表
	vector< vector<TransformEdge> > state_transform_table;
	GenerateStateTransformTable(languages.source_rules,state_transform_table,this->state_sets, start_symbol);
  	if(this->IsAmbiguous(state_transform_table, this->state_sets)) return false;

	this->state_transform_table.clear();
	ConvertStateTransformTable(state_transform_table,this->state_transform_table);
	DEBUG_LOG("state_transform_table=\n"<<this->state_transform_table);

	//根据文法预测分析表分析文法,得到文法分析树
  	return LRParse(input_symbols, source_tree, this->state_transform_table, languages.source_rules.rules, this->state_sets, start_symbol);
}

void LR1Parsers::DisplayStates()
{
	int n = 0;
	for(vector< StateSets<LR1States> >::const_iterator i = this->state_sets.begin(); i != this->state_sets.end(); ++i)
	{
		cout<<"State "<<n<<" : "<<*i<<endl;
		++n;
	}
}

