#include "CommonTestTools.h"
#include <iostream>
using namespace std;

int main()
{
	LanguageGrammar lg;
	LoadExpression(lg);

	cout<<lg<<endl;
	return 0;
}

