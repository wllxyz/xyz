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

bool WllLoader::TestLanguage()
{
	static LR1Parsers wll_xyz;
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

		//LanguageParser instance will call Wll1Loader.LoadWll to LoadLanguage, it must NOT call TestLanguage()
		if( !wll_xyz.LoadLanguage(wll_xyz_grammar_file) )
		{
			cerr<<"load "<<wll_xyz_grammar_filename<<" failed"<<endl;
			return false;
		}

		is_wll_xyz_loaded = true;
	}

	wll_xyz.SetInput(this->input_symbols);
	wll_xyz.SetStartSymbol(wll_xyz.GetDefaultStartSymbol());

	if(!wll_xyz.Parse())
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


