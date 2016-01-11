#ifndef	LANGUAGE_TREE_H
#define	LANGUAGE_TREE_H

#include "LanguageSymbols.h"

static const size_t NULL_RULE_NO = -1;

//language tree : to describe the language parse tree(source) and generate tree(destination)
struct LanguageTree
{
public:
	Symbols symbol;      
	size_t rule_no;
	vector<LanguageTree*> symbols;
public:
	LanguageTree()
	{
		this->rule_no=NULL_RULE_NO;
	}
	bool IsLeaf() const { return this->rule_no==NULL_RULE_NO && this->symbols.empty(); }
};

//destroy tree nodes
void DestroyTree(LanguageTree *root);
//for debug and output
void DisplayTree(ostream& outf,const LanguageTree *root,int n);
//for debug and output
void DisplayTreeLeaves(ostream& outf,const LanguageTree *root);
inline ostream& operator<< (ostream& outf, const LanguageTree *root)
{
	DisplayTreeLeaves(outf,root);
	return outf;
}
//traverse the symbols of tree leaves, it is a recurve call, symbols should be empty list before call
void DisplayTreeLeaves(vector<Symbols>& symbols,const LanguageTree* root);
//search symbol in tree, left first search, if not found, return 0, root should not be null
LanguageTree* LeftFirstSearchTree(LanguageTree* root,const Symbols symbol);
LanguageTree* RightFirstSearchTree(LanguageTree* root, const Symbols symbol);

#endif	//LANGUAGE_TREE_H

