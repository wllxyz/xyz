﻿#include "LanguageParsers.h"
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
: first_calculator(languages.source_rules)
{
        this->source_tree=this->destination_tree=0;
	this->is_analyzed = false;
	//加载默认的WLL0分析器规则
	//this->LoadLanguage();
}

LanguageParsers::LanguageParsers(const LanguageParsers& that)
: languages(that.languages), 
first_calculator(languages.source_rules)
{
	this->source_tree=this->destination_tree=0;
	this->is_analyzed = false;
}

//析构函数
LanguageParsers::~LanguageParsers()
{
	//销毁源语言文法分析树
	if(this->source_tree)
	    DestroyTree(this->source_tree);
	this->source_tree=0;

	//销毁目标语言文法分析树
	if(this->destination_tree)
		DestroyTree(this->destination_tree);
	this->destination_tree=0;
}

//加载输入流
bool LanguageParsers::LoadInput(istream& ins, vector<Symbols>& input_symbols)
{
	if(ins.fail())
	{
		return false;
	}

	input_symbols.clear();

	char c=ins.get();
	if(!ins.eof())
	{
		do
		{
			input_symbols.push_back(Symbols(c));
			c=ins.get();
		}while(!ins.eof());
	}

	return true;
}

void LanguageParsers::SetInput(const vector<Symbols>& symbols)
{
	this->input_symbols = symbols;
}

void LanguageParsers::SetStartSymbol(Symbols start_symbol)
{
	this->start_symbol = start_symbol;
}

Symbols LanguageParsers::GetDefaultStartSymbol()
{
	if(this->languages.translation_rules.empty())
	{
		return Symbols::NULL_SYMBOL;
	}
	return this->languages.translation_rules.front().source_rule.symbol;
}

bool LanguageParsers::Translate()
{
	FOOT();
	if(this->destination_tree) 
	{
		DestroyTree(this->destination_tree);
		this->destination_tree = NULL;
	}
	//通过文法模板的分析树结构来构造规则库:源语言产生式,目标语言产生式,规则之间的对应.
	if(!TranslateTree(this->source_tree,this->destination_tree,this->languages.destination_rules.rules)) return false;
	//生成规则的内部表示
	this->output_symbols.clear();
	DisplayTreeLeaves(this->output_symbols,this->destination_tree);
	return true;
}

//实现文法模板结构的翻译
bool LanguageParsers::ParseAndTranslate()
{
	FOOT();
	if(!this->Parse()) return false;
	if(!this->Translate()) return false;
	return true;
}

bool LanguageParsers::Process(istream& inf,ostream& outf)
{
	Symbols start_symbol = this->GetDefaultStartSymbol();
	assert(!(start_symbol == Symbols::NULL_SYMBOL));
	vector<Symbols> input_symbols, output_symbols;
	this->LoadInput(inf, input_symbols);
	bool retval = this->Process(input_symbols, output_symbols, start_symbol);
	outf<<output_symbols;

	return retval;
}

bool LanguageParsers::Process(const vector<Symbols>& input_symbols, vector<Symbols>& output_symbols, Symbols start_symbol)
{
	this->SetStartSymbol(start_symbol);
	this->SetInput(input_symbols);

	if(!this->AnalyzeLanguage())
	{
			cerr<<"analyzed language failed"<<endl;
			return false;
	}

	if(!this->ParseAndTranslate())
	{
		cerr<<"ParseAndTranslate failed"<<endl;
		return false;
	}

	INFO("output_symbols="<<this->output_symbols);

	if(!SelfExplain(this->output_symbols,this->languages.translation_rules, output_symbols))
	{
		cerr<<"SelfExplain failed"<<endl;
		return false;
	}

	return true;
}

//calculate first sets of symbols
bool LanguageParsers::AnalyzeLanguage()
{
	this->languages.Initialize();
	this->first_calculator.Initialize();

	return true;
}


//从文法模板加载语言
bool LanguageParsers::LoadLanguage(istream& ins)
{
	if(!this->LoadInput(ins,this->input_symbols))
	{
		cerr<<"load input failed"<<endl;
		return false;
	}

	Wll1Loader loader(this->input_symbols);
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
