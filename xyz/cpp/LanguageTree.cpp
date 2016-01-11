#include "LanguageTree.h"
#include <iostream>
using namespace std;

//销毁语法分析树
void DestroyTree(LanguageTree *root)
{
	if(root==NULL) return;

	if(!root->symbols.empty())
	{
		for(vector<LanguageTree*>::iterator i = root->symbols.begin(); i != root->symbols.end(); ++i)
		{
			DestroyTree(*i);
		}
	}
	delete root;
}

//遍历输出语法分析树结构
void DisplayTree(ostream& outf,const LanguageTree *root,int n)
{
	if(root==NULL) return;

	for(int i=0; i<n; i++) outf<<"->";
	outf<<root->symbol<<endl;

	if(root->symbols.empty()) return;

	for(vector<LanguageTree*>::const_iterator i = root->symbols.begin(); i != root->symbols.end(); ++i)
	{
		DisplayTree(outf,*i,n+1);
	}
}

//遍历输出语法分析树叶子结点
void DisplayTreeLeaves(ostream& outf,const LanguageTree *root)
{
	if(root==NULL) return;

	if(root->IsLeaf())
	{
		//null表达式会导致非终结符-->null在分析树中表示和叶子节点相同
		outf<<root->symbol;
		return;
	}
	for(vector<LanguageTree*>::const_iterator i = root->symbols.begin(); i != root->symbols.end(); ++i)
	{
		DisplayTreeLeaves(outf,*i);
	}
}

void DisplayTreeLeaves(vector<Symbols>& symbols,const LanguageTree* root)
{
	if(root==NULL) return;

	if(root->IsLeaf())
	{
		//null表达式会导致非终结符-->null在分析树中表示和叶子节点相同
		symbols.push_back(root->symbol);
		return;
	}
	for(vector<LanguageTree*>::const_iterator i = root->symbols.begin(); i != root->symbols.end(); ++i)
	{
		DisplayTreeLeaves(symbols,*i);
	}
}

//搜索树中与symbol匹配的叶子结点(搜索顺序：从左到右）[在同一个文法规则中不要出现同名非终结符,可以方便的改写这类文法使之满足条件]
LanguageTree* LeftFirstSearchTree(LanguageTree* root,Symbols symbol)
{
	if(root==NULL) return NULL;

	//匹配叶子结点上的非终结符号
	if(symbol==root->symbol && root->IsLeaf()) return root;
	for(vector<LanguageTree*>::const_iterator i = root->symbols.begin(); i != root->symbols.end(); ++i)
	{
		LanguageTree *p;
		if((p=LeftFirstSearchTree(*i,symbol))!=NULL) return p;
	}
	return NULL;
}

LanguageTree* RightFirstSearchTree(LanguageTree* root,Symbols symbol)
{
	if(root==NULL) return NULL;

	//匹配叶子结点上的非终结符号
	if(symbol==root->symbol && root->IsLeaf()) return root;
	for(vector<LanguageTree*>::const_reverse_iterator i = root->symbols.rbegin(); i != root->symbols.rend(); ++i)
	{
		LanguageTree *p;
		if((p=RightFirstSearchTree(*i,symbol))!=NULL) return p;
	}
	return NULL;
}

