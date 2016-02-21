#ifndef	WLL_LOADER_H
#define	WLL_LOADER_H

#include "LanguageSymbols.h"
#include "LanguageTranslations.h"
#include <vector>

class WllLoader
{
public:
	WllLoader(const std::vector<Symbols>& input_symbols);
	//TODO: 校验Wll0和Wll1文法输入是否有效:参考IsXyzLanguage
	virtual bool TestLanguage();
	virtual bool LoadWll(std::vector<LanguageTranslations>& translations) = 0;
	virtual void ShowErrorMessage() = 0;
protected:
	const std::vector<Symbols>& input_symbols;
	const char* grammar_file_name;
};

class WllLoaderFactory
{
public:
	static WllLoader* CreateWllLoader(const std::vector<Symbols>& input_symbols);
};

#endif	//WLL_LOADER_H
