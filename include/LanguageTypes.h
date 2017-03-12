//<FILENAME>LanguageTypes.h</FILENAME>
//<AUTHOR>WangLiLiang</AUTHOR>
//<DATE>2006.11.15</DATE>
//<TYPE>CPP PROGRAM CLASS</TYPE>

#ifndef LANGUAGE_TYPES_H
#define LANGUAGE_TYPES_H

#include "LanguageSymbols.h"
#include "LanguageExpressions.h"
#include "LanguageRules.h"
#include "LanguageTranslations.h"
#include "LanguageTree.h"
#include "LanguageGrammar.h"

#include <vector>
#include <iostream>
#include <sstream>

template <class T> ostream& operator<< (ostream& o, const vector<T>& r)
{
	typedef typename vector<T>::const_iterator iterator;
	for(iterator it = r.begin(); it != r.end(); ++it)
	{
		o<<(*it)<<endl;
	}
	return o;
}

template <class T>  std::string ToString(const vector<T>& r)
{
	stringstream o;
	o<<r;
	return o.str();
}

#endif	//LANGUAGE_TYPES_H

