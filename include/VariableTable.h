/*
 * VariableTable.h
 *
 *  Created on: 2016-1-21
 *      Author: wll
 */

#ifndef VARIABLETABLE_H_
#define VARIABLETABLE_H_

#include "LanguageExpressions.h"
#include <map>
#include <string>
#include "WllString.h"
#include <ostream>
#include <assert.h>
#include <algorithm>

struct VariableNode
{
	typedef std::string KeyType;
	typedef LanguageExpressions ValType;
	typedef std::map<KeyType, VariableNode> MapType;
	MapType sub_nodes;
	ValType value;

	VariableNode& operator [] (const std::string& sub_node_name)
	{
		return this->sub_nodes[sub_node_name];
	}

	VariableNode(){}
	VariableNode(const VariableNode& that)
	{
		this->sub_nodes = that.sub_nodes;
		this->value = that.value;
	}

	const VariableNode& operator = (const VariableNode& that)
	{
		this->sub_nodes = that.sub_nodes;
		this->value = that.value;
		return (*this);
	}

	const VariableNode& operator = (const ValType& value)
	{
		this->value = value;
		return (*this);
	}

	const ValType& operator() ()
	{
		return this->value;
	}
};
inline std::ostream& operator<< (std::ostream& o, const VariableNode& variable_node)
{
	for(std::map<std::string, VariableNode>::const_iterator i = variable_node.sub_nodes.begin(); i != variable_node.sub_nodes.end(); ++i)
	{
		o << i->first << "==>" << i->second.value << std::endl;
		o << i->second;
	}
	return o;
}

class VariableTable
{
public:
	bool Has(const std::string& variable_name)
	{
		std::vector<std::string> name_fields;
		Wll::Util::split(variable_name, '.', name_fields);
		VariableNode* node = &variable_table;
		for(std::vector<std::string>::iterator i = name_fields.begin(); i != name_fields.end(); ++i)
		{
			if(node->sub_nodes.find(*i) != node->sub_nodes.end())
			{
				node = &(node->sub_nodes[*i]);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	VariableNode::ValType& operator[] (const std::string& variable_name)
	{
		std::vector<std::string> name_fields;
		Wll::Util::split(variable_name, '.', name_fields);
		VariableNode* node = &variable_table;
		for(std::vector<std::string>::iterator i = name_fields.begin(); i != name_fields.end(); ++i)
		{
			node = &(node->sub_nodes[*i]);
		}
		return node->value;
	}
private:
	VariableNode variable_table;
};


#endif /* VARIABLETABLE_H_ */
