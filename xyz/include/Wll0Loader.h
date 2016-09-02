/*
 * Wll0Loader.h
 *
 *  Created on: 2016-1-2
 *      Author: wll
 */

#ifndef WLL0LOADER_H_
#define WLL0LOADER_H_

#include "WllLoader.h"

class Wll0Loader : public WllLoader
{
public:
	Wll0Loader(const std::vector<Symbols>& input_symbols);
	virtual bool LoadWll(std::vector<LanguageTranslations>& translations);
	virtual void ShowErrorMessage();
	//Wll0Loader和Wll1Loader的TestLanguage的实现完全一样，但是静态变量缓存的文法不同
	virtual bool TestLanguage();
};

#endif /* WLL0LOADER_H_ */
