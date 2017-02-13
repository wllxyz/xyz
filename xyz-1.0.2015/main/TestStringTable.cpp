#include "StringTable.h"
#include <cassert>
#include <string>
#include <iostream>
using namespace Wll;
using namespace std;

int main()
{
	StringTable st;
	assert(st.GetIndexByName("test1")==0);
	assert(st.GetIndexByName("test2")==1);
	assert(st.GetIndexByName("test1")==0);
	assert(st.GetNameByIndex(1)=="test2");
	cout<<"PASS"<<endl;
}

