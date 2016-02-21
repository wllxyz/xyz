#include "StateTransformTable.h"
#include <cassert>
using namespace std;

std::ostream& operator<< (std::ostream& o, const Actions& action)
{
	o<<"(";
	switch(action.type)
	{
	case MOVE_ACTION:
		o<<"MOVE";
		break;
	case REDUCE_ACTION:
		o<<"REDUCE";
		break;
	default:
		assert(false);
	}
	return o<<" "<<action.value<<")";
}

std::ostream& operator<< (std::ostream& o, const TransformEdge& edge)
{
	return o<<"("<<edge.symbol<<","<<edge.action<<")";
}

std::ostream& operator<< (std::ostream& o, const StateTransformTable& state_transform_table)
{
	int state = 0;
	for(StateTransformTable::const_iterator i = state_transform_table.begin(); i != state_transform_table.end(); ++i)
	{
		o<<state++<<"\t";
		for(map<Symbols,Actions>::const_iterator j = i->begin(); j != i->end(); ++j)
		{
			o<<"("<<j->first<<","<<j->second<<") ";
		}
		o<<endl;
	}
	return o;
}

