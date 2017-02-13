#ifndef LR_PARSERS_H
#define LR_PARSERS_H

#include <iostream>
using namespace std;
#include "LanguageParsers.h"

//定义LRParsers类
class LRParsers : public LanguageParsers
{
//重载算法
protected:
 virtual bool AnalyzeLanguage();
 //根据文法规则对输入符号流进行文法结构的分析,分析结果用文法分析树表示
 virtual bool ParseTree(const List<Symbols>& symbols,List<LanguageRules>& languages,LanguageTree*& tree);
//支撑算法
protected:
 //根据文法自动生成文法预测分析表(供LR1文法分析器重载)
 virtual void GenerateStateTransformTable(List<LanguageRules>& languages,List< List<StateTransformEdges> >& state_transform_table)=0;

//判定文法是否存在移近规约冲突（文法二义性判断）
virtual bool IsAmbiguous(const List< List<StateTransformEdges> >& state_transform_table);

 //根据文法预测分析表分析文法,得到文法分析树(供LR1重载)
 virtual bool LRParse(const List<Symbols>& symbols,List< List<StateTransformEdges> >& state_transform_table,List<LanguageRules>& languages,LanguageTree*& tree);

 //根据分析过程中依次规约的规则建立文法分析树(根据后跟遍历顺序）
 virtual void ConstructTreeByRules(List<SIZE_TYPE>& rules,List<LanguageRules>& languages,LanguageTree*& source_tree);

 //搜索树中与symbol匹配的叶子结点(搜索顺序：从右到左）
 virtual LanguageTree* SearchTree(LanguageTree* root,Symbols symbol);

void DisplayStateTransformEdge(const StateTransformEdges& edge,ostream& oo);

protected:
	List< List<StateTransformEdges> > state_transform_table;

};//end of class LRParser



#endif  //LR_PARSERS_H

