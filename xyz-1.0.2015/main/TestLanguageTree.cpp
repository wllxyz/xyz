#include "LanguageTree.h"
#include <iostream>
#include <cassert>
#include <sstream>
using namespace std;

int main()
{
	LanguageTree *t;
	t = new LanguageTree;
	t->symbol = Symbols("start");
	t->rule_no = 0;
	LanguageTree *t1 = new LanguageTree;
	t1->symbol = Symbols("expression");
	t1->rule_no = -1;
	t->symbols.push_back(t1);
	assert(LeftFirstSearchTree(t,Symbols("expression"))==t1);
	assert(RightFirstSearchTree(t,Symbols("expression"))==t1);
	t1->rule_no = 1;
	assert(LeftFirstSearchTree(t,Symbols("expression"))==NULL);
	assert(RightFirstSearchTree(t,Symbols("expression"))==NULL);
	LanguageTree *t2 = new LanguageTree;
	t2->symbol = Symbols("term");
	t2->rule_no = 2;
	t1->symbols.push_back(t2);
	t2->rule_no = -1;
	assert(LeftFirstSearchTree(t,Symbols("term"))==t2);
	assert(RightFirstSearchTree(t,Symbols("term"))==t2);
	t2->rule_no = 2;
	assert(LeftFirstSearchTree(t,Symbols("term"))==NULL);
	assert(RightFirstSearchTree(t,Symbols("term"))==NULL);
	LanguageTree *t3 = new LanguageTree;
	t3->symbol = Symbols("factor");
	t3->rule_no = 3;
	LanguageTree *t4 = new LanguageTree;
	t4->symbol = Symbols('+');
	t4->rule_no = -1;
	LanguageTree *t5 = new LanguageTree;
	t5->symbol = Symbols("term");
	t5->rule_no = 4;
	t2->symbols.push_back(t3);
	t2->symbols.push_back(t4);
	t2->symbols.push_back(t5);
	LanguageTree *t6 = new LanguageTree;
	t6->symbol = Symbols('1');
	t6->rule_no = -1;
	t3->symbols.push_back(t6);
	LanguageTree *t7 = new LanguageTree;
	t7->symbol = Symbols("factor");
	t7->rule_no = 3;
	t5->symbols.push_back(t7);
	t7->rule_no = -1;
	assert(LeftFirstSearchTree(t,Symbols("factor"))==t7);
	assert(RightFirstSearchTree(t,Symbols("factor"))==t7);
	t7->rule_no = 3;
	assert(LeftFirstSearchTree(t,Symbols("factor"))==NULL);
	assert(RightFirstSearchTree(t,Symbols("factor"))==NULL);
	LanguageTree *t8 = new LanguageTree;
	t8->symbol = Symbols('2');
	t8->rule_no = -1;
	t7->symbols.push_back(t8);

	stringstream s;
	s<<t;
	assert(s.str()=="1+2");

	DestroyTree(t);
	cout<<"PASS"<<endl;	
	return 0;	
}
