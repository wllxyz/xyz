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
};

#endif /* WLL0LOADER_H_ */
