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
bool LanguageParsers::LoadInput(istream& ins)
{
	if(ins.fail())
	{
		return false;
	}

	this->input_symbols.clear();

	char c=ins.get();
	if(!ins.eof())
	{
		do
		{
			this->input_symbols.push_back(Symbols(c));
			c=ins.get();
		}while(!ins.eof());
	}

	return true;
}

void LanguageParsers::SetInput(const vector<Symbols>& symbols)
{
	this->input_symbols = symbols;
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
	FOOT();
	if(!this->is_analyzed)
	{
		if(this->AnalyzeLanguage())
			this->is_analyzed = true;
		else
		{
			cerr<<"analyzed language failed"<<endl;
			return false;
		}
	}
	if(!this->LoadInput(inf)) 
	{
		cerr<<"LoadInput failed"<<endl;
		return false;
	}
	if(!this->ParseAndTranslate()) 
	{
		cerr<<"ParseAndTranslate failed"<<endl;
		return false;
	}

	INFO("output_symbols="<<this->output_symbols);
	if(!SelfExplain(this->output_symbols,this->languages.translation_rules))
	{
		cerr<<"SelfExplain failed"<<endl;
		return false;
	}
	//当文法没有加载时不需要重新分析(加载文法后,调用文法分析)
	this->is_analyzed = false;

	return true;
}

bool LanguageParsers::IsXyzLanguage(const vector<Symbols>& symbols)
{
	//not check inner wll grammar now
	return ( !symbols.empty() && symbols.front()==Symbols::REMARK_WLL0 );
}

//calculate first sets of symbols
bool LanguageParsers::AnalyzeLanguage()
{
	this->languages.Initialize();
	this->first_calculator.Initialize();
	assert(!this->languages.source_rules.rules.empty());
	this->start_symbol = this->languages.source_rules.rules.front()->symbol;
	return true;
}


//从文法模板加载语言
bool LanguageParsers::LoadLanguage(istream& ins)
{
	if(!this->LoadInput(ins))
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

	if(!this->AnalyzeLanguage())
	{
		cerr<<"analyze language failed"<<endl;
		return false;
	}
	this->is_analyzed = true;
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
