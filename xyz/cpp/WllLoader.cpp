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
		string wll_xyz_grammar_filename = string(dir) + "/../data/" + this->grammar_file_name;

		ifstream wll_xyz_grammar_file(wll_xyz_grammar_filename.c_str());
		if(wll_xyz_grammar_file.fail())
		{
			cerr<<"open grammar file "<<wll_xyz_grammar_filename<<" failed"<<endl;
			return false;
		}

		vector<Symbols> symbols;
		wll_xyz_grammar_file >> symbols;

		//LanguageParser instance will call Wll1Loader.LoadWll to LoadLanguage, it must NOT call TestLanguage()
		Wll1Loader loader(symbols);
		if(!loader.Load(wll_xyz_languages))
		{
			cerr<<"LL(1) parser load language failed"<<endl;
			loader.ShowErrorMessage();
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
	return true;
}

WllLoader* WllLoaderFactory::CreateWllLoader(const std::vector<Symbols>& input_symbols)
{
	if( !input_symbols.empty() && input_symbols.front()==Symbols::REMARK_WLL0 )
	{
		return new Wll0Loader(input_symbols);
	}
	else
	{
		return new Wll1Loader(input_symbols);
	}
}

