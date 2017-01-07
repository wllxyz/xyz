//<FILENAME>StringTable.cpp</FILENAME>
//<AUTHOR>WangLiLiang</AUTHOR>
//<DATE>2012.04.04</DATE>
//<TYPE>CPP PROGRAM CLASS</TYPE>

#include "StringTable.h"
using namespace Wll;
using namespace std;

//获取字符串注册的索引，如果字符串不存在，首先注册，然后返回新注册的索引值
int StringTable::GetIndexByName(const string& name)
{
	MapType::const_iterator i = this->name2index_table.find(name);
	if(i == this->name2index_table.end())
	{
		//没有注册，现在注册
		size_t index = this->index2name_table.size();
		this->index2name_table.push_back(name);
		this->name2index_table[name]=index;
		return index;
	}
	else
	{
		return i->second;
	}
}

//根据先前注册时分配的索引值获取字符串
const string& StringTable::GetNameByIndex(int index)
{
	if(index<0 || index >=this->index2name_table.size()) 
		throw IndexOutOfBoundaryException();

	return this->index2name_table[index];
}


