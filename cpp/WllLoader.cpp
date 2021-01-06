/*
 * WllLoader.cpp
 *
 *  Created on: 2016-1-2
 *      Author: wll
 */

#include "WllLoader.h"
#include "Wll0Loader.h"
#include "Wll1Loader.h"
#include "LR1Parsers.h"
#include "LanguageAlgorithm.h"
#include <string>
#include <fstream>
#include "WllTrace.h"
using namespace std;

WllLoader::WllLoader(const vector<Symbols>& input_symbols) : input_symbols(input_symbols)
{

}

bool WllLoader::Load(LanguageGrammar& languages)
{
	if(!this->LoadWll(languages.translation_rules)) return false;
	languages.Initialize();
	return true;
}

bool WllLoader::TestLanguage()
{
	static LanguageGrammar wll_xyz_languages;
	static bool is_wll_xyz_loaded = false;

	if(is_wll_xyz_loaded == false)
	{
		char dir[Wll::Tool::LogInnerTypes::MAX_FILE_NAME_LENGTH];
		Wll::Util::GetProcessDir(dir,sizeof(dir));
		string wll_xyz_grammar_filename = string(dir) + "/../data/wll.xyz";

		//LanguageParser instance will call Wll1Loader.LoadWll to LoadLanguage, it must NOT call TestLanguage()
		if(!LoadLanguage(wll_xyz_grammar_filename.c_str(), wll_xyz_languages, false, false))
		{
			ERROR("Load wll_xyz_grammar_filename["<<wll_xyz_grammar_filename<<"] failed");
			return false;
		}

		is_wll_xyz_loaded = true;
	}

	LanguageTree* source_tree = NULL;
	LR1Parsers wll_xyz(NULL);
	if(!wll_xyz.Parse(wll_xyz_languages, this->input_symbols, source_tree, wll_xyz_languages.GetDefaultStartSymbol()))
	{
		ERROR("Test WLL language failed");
		TERM_ERROR("Test WLL language failed");
		return false;
	}
	DestroyTree(source_tree);
	return true;
}

WllLoader* WllLoaderFactory::CreateWllLoader(const std::vector<Symbols>& input_symbols)
{
	return new Wll1Loader(input_symbols);
}

