#ifndef	WLL_LOADER_H
#define	WLL_LOADER_H

#include "LanguageSymbols.h"
#include "LanguageGrammar.h"
#include <vector>

class WllLoader
{
public:
	WllLoader(const std::vector<Symbols>& input_symbols);
	//TODO: 校验Wll0和Wll1文法输入是否有效:参考IsXyzLanguage
	//Wll0Loader和Wll1Loader的TestLanguage的实现完全一样，但是静态变量缓存的文法不同
	virtual bool TestLanguage()=0;
	bool TestLanguage(LanguageGrammar& wll_xyz_languages, bool& is_wll_xyz_loaded);
	virtual bool LoadWll(std::vector<LanguageTranslations>& translations) = 0;
	virtual bool Load(LanguageGrammar& languages);
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
