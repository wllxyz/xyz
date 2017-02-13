#include "CommonTestTools.h"
#include "LanguageAlgorithm.h"
#include <iostream>
using namespace std;
#include <cassert>
#include <sstream>

int main()
{
	LanguageGrammar lg;
	LoadExpression(lg);

	cout<<lg<<endl;

	//0+1*0
	vector<size_t> left_reduce_rules_no,reverse_right_reduce_rules_no;
	vector<LanguageRules*> left_reduce_rules, reverse_right_reduce_rules;

	left_reduce_rules_no.push_back(0);
	left_reduce_rules_no.push_back(1);
	left_reduce_rules_no.push_back(4);
	left_reduce_rules_no.push_back(5);
	left_reduce_rules_no.push_back(6);
	left_reduce_rules_no.push_back(2);
	left_reduce_rules_no.push_back(3);
	//left_reduce_rules_no.push_back(3);
	left_reduce_rules_no.push_back(5);
	left_reduce_rules_no.push_back(7);
	left_reduce_rules_no.push_back(4);
	left_reduce_rules_no.push_back(5);
	left_reduce_rules_no.push_back(6);

	for(vector<size_t>::iterator i = left_reduce_rules_no.begin(); i != left_reduce_rules_no.end(); ++i)
	{
		left_reduce_rules.push_back(lg.source_rules.rules[*i]);
		cout<<*lg.source_rules.rules[*i]<<endl;
	}
	
	cout<<endl;

	reverse_right_reduce_rules_no.push_back(6);
	reverse_right_reduce_rules_no.push_back(5);
	reverse_right_reduce_rules_no.push_back(4);
	reverse_right_reduce_rules_no.push_back(7);
	reverse_right_reduce_rules_no.push_back(5);
	reverse_right_reduce_rules_no.push_back(3);
	//reverse_right_reduce_rules_no.push_back(3);
	reverse_right_reduce_rules_no.push_back(2);
	reverse_right_reduce_rules_no.push_back(6);
	reverse_right_reduce_rules_no.push_back(5);
	reverse_right_reduce_rules_no.push_back(4);
	reverse_right_reduce_rules_no.push_back(1);
	reverse_right_reduce_rules_no.push_back(0);
	
	for(vector<size_t>::iterator i = reverse_right_reduce_rules_no.begin(); i != reverse_right_reduce_rules_no.end(); ++i)
	{
		reverse_right_reduce_rules.push_back(lg.source_rules.rules[*i]);
		cout<<*lg.source_rules.rules[*i]<<endl;
	}	

	//bool Reduce(LanguageExpressions& expression,const vector<LanguageRules*>& used_rules,ReduceType type);
	LanguageExpressions e1(Symbols("start")),e2(Symbols("start"));
	assert(Expend(e1,left_reduce_rules,LEFT_REDUCE));
	cout<<e1<<endl;
	assert(Expend(e2,reverse_right_reduce_rules,REVERSE_RIGHT_REDUCE));
	cout<<e2<<endl;
	assert(e1==e2);
	assert(Reduce(e1,left_reduce_rules,LEFT_REDUCE));
	cout<<e1<<endl;
	assert(Reduce(e2,reverse_right_reduce_rules,REVERSE_RIGHT_REDUCE));
	cout<<e2<<endl;
	assert(e1==e2);
	stringstream str0;
	str0<<e1;

	//bool ConstructTreeByRules(LanguageTree*& source_tree,const vector<size_t>& rules_no,const vector<LanguageRules*>& source_languages,ReduceType type);
	LanguageTree* source_tree1 = NULL;
	LanguageTree* source_tree2 = NULL;
	LanguageTree* dest_tree1=NULL;
	LanguageTree* dest_tree2=NULL;
	assert(ConstructTreeByRules(source_tree1,left_reduce_rules_no,lg.source_rules.rules,LEFT_REDUCE));
	cout<<source_tree1<<endl;
	assert(ConstructTreeByRules(source_tree2,reverse_right_reduce_rules_no,lg.source_rules.rules,REVERSE_RIGHT_REDUCE));
	cout<<source_tree2<<endl;
	stringstream str1;
	str1<<source_tree1;
	stringstream str2;
	str2<<source_tree2;
	assert(str1.str()==str2.str());

	assert(str1.str()==str0.str());

	assert(ConstructTreeByRules(dest_tree1,left_reduce_rules_no,lg.destination_rules.rules,LEFT_REDUCE));
	cout<<dest_tree1<<endl;
	assert(ConstructTreeByRules(dest_tree2,reverse_right_reduce_rules_no,lg.destination_rules.rules,REVERSE_RIGHT_REDUCE));
	cout<<dest_tree2<<endl;
	stringstream str3;
	str3<<dest_tree1;
	stringstream str4;
	str4<<dest_tree2;
	assert(str3.str()==str4.str());

	//bool TranslateTree(LanguageTree* source,LanguageTree*& dest,const vector<LanguageRules*>& destination_languages);
	LanguageTree* dest_tree3 = NULL;
	assert(TranslateTree(source_tree1,dest_tree3,lg.destination_rules.rules));
	stringstream str5;
	DisplayTree(cout,dest_tree3,0);
	str5<<dest_tree3;
	cout<<str5.str()<<endl;
	assert(str5.str()==str3.str());

	//bool SelfExplain(const vector<Symbols>& symbols,vector<LanguageTranslations>& languages);
	//<e>--><t>+<e>==><e>--><t><e>+
	//<e>--><t>==><e>--><t>
	vector<Symbols> symbols;
	vector<LanguageTranslations> languages;

	symbols.push_back(Symbols::REMARK_WLL0);
	symbols.push_back('e');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_ROOT_SYMBOL);
	symbols.push_back('t');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back('+');
	symbols.push_back(Symbols::REMARK_CONSTANT);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back('e');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back(Symbols::REMARK_EXPRESSION);
	symbols.push_back(Symbols::REMARK_RULE);
	symbols.push_back(Symbols::REMARK_SOURCE_RULE);

	symbols.push_back('e');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_ROOT_SYMBOL);
	symbols.push_back('t');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back('e');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back('+');
	symbols.push_back(Symbols::REMARK_CONSTANT);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back(Symbols::REMARK_EXPRESSION);
	symbols.push_back(Symbols::REMARK_RULE);
	symbols.push_back(Symbols::REMARK_DESTINATION_RULE);
	symbols.push_back(Symbols::REMARK_TRANSLATION);

	symbols.push_back('e');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_ROOT_SYMBOL);
	symbols.push_back('t');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back(Symbols::REMARK_EXPRESSION);
	symbols.push_back(Symbols::REMARK_RULE);
	symbols.push_back(Symbols::REMARK_SOURCE_RULE);

	symbols.push_back('e');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_ROOT_SYMBOL);
	symbols.push_back('t');
	symbols.push_back(Symbols::REMARK_VARIABLE);
	symbols.push_back(Symbols::REMARK_SUB_SYMBOL);
	symbols.push_back(Symbols::REMARK_EXPRESSION);
	symbols.push_back(Symbols::REMARK_RULE);
	symbols.push_back(Symbols::REMARK_DESTINATION_RULE);
	symbols.push_back(Symbols::REMARK_TRANSLATION);	

	/*
	vector<Symbols> temp_result;	
	assert(SelfExplain(symbols,languages,temp_result));
	stringstream lang_str;
	for(vector<LanguageTranslations>::iterator i = languages.begin(); i != languages.end(); ++i)
	{
		lang_str<<*i<<"\n";
	}
	cout<<lang_str.str();
	assert(lang_str.str()=="<e>--><t>+<e>==><e>--><t><e>+\n<e>--><t>==><e>--><t>\n");
	*/

	//根据文法自动生成文法预测分析表(供LR1文法分析器重载)
	//void GenerateStateTransformTable(const LanguageIndex& languages,vector< vector<TransformEdge> >& state_transform_table);
	vector< vector<TransformEdge> > state_transform_table;
	vector< StateSets<LR1States> > state_sets;
	StateTransformTable table;
	GenerateStateTransformTable(lg.source_rules,state_transform_table,state_sets,lg.source_rules.rules[0]->symbol);
	int state = 0;
	for(vector<vector<TransformEdge> >::const_iterator i = state_transform_table.begin(); i != state_transform_table.end(); ++i)
	{
		std::map<Symbols,Actions> line;
		cout<<state++<<"\t";
		for(vector<TransformEdge>::const_iterator j = i->begin(); j != i->end(); ++j)
		{
			cout<<*j<<" ";
			line[j->symbol] = j->action;
		}
		cout<<endl;
		table.push_back(line);
	}

	//void Closure(const LanguageIndex& languages, StateSets<LR1States>& I);

	set<Symbols> follow;
	follow.insert(Symbols::END_SYMBOL);
	StateSets<LR1States> state_set;
	state_set.states.insert(LR1States(lg.source_rules.GetRule(0),0,0,follow));
	Closure(lg.source_rules,state_set);
	cout<<state_set<<endl;
	stringstream closure_output;
	closure_output<<state_set;
	cout<<"======================================="<<endl;
	cout<<closure_output.str()<<endl;
	cout<<"======================================="<<endl;
	assert(closure_output.str()=="(((0,0),($END)),((1,0),($END)),((2,0),($END)),((3,0),(+$END)),((4,0),(+$END)),((5,0),(*+$END)),((6,0),(*+$END)),((7,0),(*+$END)))");

	//根据文法预测分析表分析文法,得到文法分析树(供LR1重载)
	//bool LRParse(const vector<Symbols>& symbols,LanguageTree*& tree,const StateTransformTable& state_transform_table,const vector<LanguageRules*>& languages);

	const char * input = "0+1*0";
	vector<Symbols> input_symbols;
	for(const char* p = input; *p != '\0'; ++p)
	{
		input_symbols.push_back(Symbols(*p));
	}

	LanguageTree* source_tree = NULL;
	assert(LRParse(input_symbols, source_tree, table, lg.source_rules.rules, state_sets,lg.source_rules.rules[0]->symbol));

	stringstream output;
	output<<source_tree;
	cout<<output.str()<<endl;
	cout<<input<<endl;
	assert(output.str()==string(input));

	cout<<"===================================test parse expression2========================================"<<endl;
	LoadExpression2(lg);
	cout<<"Expression2 Language:"<<endl;
	cout<<lg<<endl;
	GenerateStateTransformTable(lg.source_rules,state_transform_table,state_sets,lg.source_rules.rules[0]->symbol);
	ConvertStateTransformTable(state_transform_table, table);
	cout<<"state_transfrom_table:"<<endl;
	cout<<table<<endl;
	LRParse(input_symbols, source_tree, table, lg.source_rules.rules, state_sets,lg.source_rules.rules[0]->symbol);
	stringstream output2;
	output2<<source_tree;
	cout<<"analyze source tree:"<<endl;
	cout<<output2.str()<<endl;
	assert(input==output2.str());

	cout<<"===================================test parse expression3========================================"<<endl;
	LoadExpression3(lg);
	cout<<"Expression3 Language:"<<endl;
	cout<<lg<<endl;
	GenerateStateTransformTable(lg.source_rules,state_transform_table,state_sets,lg.source_rules.rules[0]->symbol);
	ConvertStateTransformTable(state_transform_table, table);
	cout<<"state_transfrom_table:"<<endl;
	cout<<table<<endl;
	LRParse(input_symbols, source_tree, table, lg.source_rules.rules, state_sets,lg.source_rules.rules[0]->symbol);
	stringstream output3;
	output3<<source_tree;
	cout<<"analyze source tree:"<<endl;
	cout<<output3.str()<<endl;
	assert(input==output3.str());

	cout<<"PASS"<<endl;		
	return 0;
}

