#ifndef	STATE_TRANSFORM_TABLE_H
#define STATE_TRANSFORM_TABLE_H

#include <iostream>
#include <map>
#include <vector>
#include "LanguageSymbols.h"
#include "LanguageRules.h"
#include "States.h"

enum ActionTypes
{
	MOVE_ACTION,
	REDUCE_ACTION
};

struct Actions
{
	ActionTypes	type;
	size_t		value;
	union
	{
		const LanguageRules* rule;
		const StateSets<LR1States>* state;
	};
};

std::ostream& operator<< (std::ostream& o, const Actions& action);

struct TransformEdge
{
	Symbols symbol;
	Actions action;
};

std::ostream& operator<< (std::ostream& o, const TransformEdge& edge);

typedef std::vector< std::map<Symbols,Actions> > StateTransformTable;

std::ostream& operator<< (std::ostream& o, const StateTransformTable& table);

#endif	//STATE_TRANSFORM_TABLE_H

