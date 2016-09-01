#include "LanguageParsers.h"
#include "LanguageAlgorithm.h"
#include "Wll1Loader.h"
#include "WllUtil.h"
#include "WllTrace.h"
#include <assert.h>
#include <fstream>
#include <string.h>
using namespace std;

//LanguageParsers
LanguageParsers::LanguageParsers()
{
	//加载默认的WLL0分析器规则
	//this->LoadLanguage();
}

LanguageParsers::LanguageParsers(const LanguageParsers& that)
: languages(that.languages)
{

}

//析构函数
LanguageParsers::~LanguageParsers()
{

}

Symbols LanguageParsers::GetDefaultStartSymbol()
{
	if(this->languages.translation_rules.empty())
	{
		return Symbols::NULL_SYMBOL;
	}
	return this->languages.translation_rules.front().source_rule.symbol;
}

bool LanguageParsers::Process(istream& inf,ostream& outf)
{
	Symbols start_symbol = this->GetDefaultStartSymbol();
	assert(!(start_symbol == Symbols::NULL_SYMBOL));
	vector<Symbols> input_symbols, output_symbols;
	inf >> input_symbols;
	bool retval = this->Process(input_symbols, output_symbols, start_symbol);
	outf<<output_symbols;

	return retval;
}

bool LanguageParsers::Process(const vector<Symbols>& input_symbols, vector<Symbols>& output_symbols, Symbols start_symbol)
{
	LanguageTree* source_tree = NULL;
	LanguageTree* destination_tree = NULL;

	if(!this->Parse(input_symbols, source_tree, start_symbol))
	{
		DestroyTree(source_tree);
		//DestroyTree(destination_tree);
		return false;
	}

	if(!TranslateTree(source_tree, destination_tree, this->languages.destination_rules.rules))
	{
		DestroyTree(source_tree);
		DestroyTree(destination_tree);
		return false;
	}

	vector<Symbols> translate_output_symbols;
	DisplayTreeLeaves(translate_output_symbols, destination_tree);

	INFO("translate_output_symbols="<<translate_output_symbols);

	if(!SelfExplain(translate_output_symbols, this->languages.translation_rules, output_symbols))
	{
		cerr<<"SelfExplain failed"<<endl;
		return false;
	}

	DestroyTree(source_tree);
	DestroyTree(destination_tree);

	return true;
}

//calculate first sets of symbols
bool LanguageParsers::AnalyzeLanguage()
{
	this->languages.Initialize();

	return true;
}


//从文法模板加载语言
bool LanguageParsers::LoadLanguage(istream& ins)
{
	vector<Symbols> input_symbols;

	ins >> input_symbols;

	Wll1Loader loader(input_symbols);
	if(!loader.LoadWll(this->languages.translation_rules))
	{
		cerr<<"LL(1) parser load language failed"<<endl;
		loader.ShowErrorMessage();
		return false;
	}

	return true;
}

bool LanguageParsers::LoadLanguage()
{
	char grammar_filename[Wll::Tool::LogInnerTypes::MAX_FILE_NAME_LENGTH];
	Wll::Util::GetProcessDir(grammar_filename, sizeof(grammar_filename));
	strcat(grammar_filename,"/../");
	strcat(grammar_filename, GRAMMAR_FILE_NAME);
	ifstream grammar_file(grammar_filename);
	if(grammar_file.fail())
	{
		cerr<<"open grammar file["<<grammar_filename<<"] failed"<<endl;
		return false;
	}

	return this->LoadLanguage(grammar_file);
}

//for debug
void LanguageParsers::DisplayLanguage()
{
	cout<<this->languages<<endl;
}

void LanguageParsers::DisplayStates()
{

}
