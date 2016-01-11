#ifndef LR1_PARSERS_H
#define LR1_PARSERS_H

#include "LanguageTypes.h"
#include "LanguageParsers.h"
#include "StateTransformTable.h"
#include "States.h"


//定义LR1分析器类LR1Parsers
class LR1Parsers : public LanguageParsers
{
public:
	virtual void DisplayStates();
//重载LRPARSERS支撑算法
protected:
	//判定文法是否存在移近规约冲突（文法二义性判断）
	virtual bool IsAmbiguous(const vector< vector< TransformEdge > >& state_transform_table, const vector< StateSets<LR1States> >& state_sets);

//重载(继承)数据结构
protected:
	virtual bool AnalyzeLanguage();
	//根据文法规则对输入符号流进行文法结构的分析,分析结果用文法分析树表示
	virtual bool Parse();
	virtual bool IsXyzLanguage(const vector<Symbols>& symbols);

private:
	StateTransformTable state_transform_table;
	vector< StateSets<LR1States> > state_sets;
};//End of class LR1Parsers

#endif  //LR1_PARSERS_H

