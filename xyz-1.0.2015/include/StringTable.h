//<FILENAME>StringTable.h</FILENAME>
//<AUTHOR>WangLiLiang</AUTHOR>
//<DATE>2012.04.04</DATE>
//<TYPE>CPP PROGRAM CLASS</TYPE>

#ifndef	STRING_TABLE_H
#define	STRING_TABLE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
//#include <hash_map>

namespace Wll
{

class IndexOutOfBoundaryException
{
};

class StringTable
{
public:
	typedef	std::map<std::string,int> MapType;
public:
	//获取字符串注册的索引，如果字符串不存在，首先注册，然后返回新注册的索引值
	int GetIndexByName(const std::string& name);
	//根据先前注册时分配的索引值获取字符串
	const std::string& GetNameByIndex(int index);
private:
	MapType				name2index_table;
	std::vector<std::string>	index2name_table;
};

};//end of namespace Wll

#endif	//STRING_TABLE_H

