/*
 * TestWllString.cpp
 *
 *  Created on: Sep 2, 2015
 *      Author: wll
 */



#include "WllString.h"
#include <iostream>
using namespace std;
using namespace Wll::Util;

void traverse_fields(const char* str)
{
	char field[30];
	while(str=split(str, field, sizeof(field),','))
	{
		cout<<field<<"|";
	}
	cout<<field<<endl;
}

int main()
{
	char str1[] = "abc,def,afdkjf";
	char str2[] = "";
	char str3[] = ",,";
	char str4[] = "abc,";

	traverse_fields(str1);
	traverse_fields(str2);
	traverse_fields(str3);
	traverse_fields(str4);

	std::vector<std::string> fields;
	split(str1, ',', fields);

	cout<<fields.size()<<endl;
	for(std::vector<std::string>::iterator i = fields.begin(); i != fields.end(); ++i)
	{
		cout<<*i<<"|"<<endl;
	}

	return 0;
}
