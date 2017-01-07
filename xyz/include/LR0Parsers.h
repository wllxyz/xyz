#ifndef LR0_PARSER_H
#define LR0_PARSER_H

#include "LRParsers.h"

//定义LR0Parsers类
class LR0Parsers : public LRParsers
{
//重载LRPARSERS支撑算法
protected:
 //根据文法自动生成文法预测分析表(供LR1文法分析器重载)
 virtual void GenerateStateTransformTable(List<LanguageRules>& languages,List< List<StateTransformEdges> >& state_transform_table);
};//end of class LR0Parsers

#endif //LR0_PARSER_H
