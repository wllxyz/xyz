#ifndef FIRST_SET_H
#define FIRST_SET_H

#include "LanguageGrammar.h"
#include <set>
#include <map>
using namespace std;

//get symbols first set(if include null, return true)
bool First(const LanguageIndex& language, Symbols e, set<Symbols>& result); 
bool First(const LanguageIndex& language, Symbols e,set<Symbols>& visited,set<Symbols>& result);
bool First(const LanguageIndex& language, const vector<Symbols>& e,size_t i,set<Symbols>& visited,set<Symbols>& result);
void CalculateAllFirstSet(const LanguageIndex& language, map<Symbols,set<Symbols> >& first_table);

bool LookupFirst(const map<Symbols, set<Symbols> >& first_table, Symbols e,set<Symbols>& result,bool& null_exist);

class FirstCalculator
{
public:
	FirstCalculator(const LanguageIndex& language);
	bool First(Symbols e, set<Symbols>& result);
	bool Select(Symbols e, Symbols input_symbol);
	void Initialize();
private:
	const LanguageIndex& language;
	map<Symbols, set<Symbols> > first_table;
	bool initialized;
};

#endif	//FIRST_SET_H