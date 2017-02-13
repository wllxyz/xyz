#include "LanguageParsers.h"
#include "LanguageAlgorithm.h"
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
	if(!this->Parse()) return false;
	if(!this->Translate()) return false;
	return true;
}

bool LanguageParsers::Process(istream& inf,ostream& outf)
{
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
#ifndef	WLL0LOADER
	if(LanguageParsers::IsXyzLanguage(this->output_symbols))
	{
		if(!SelfExplain(this->output_symbols,this->languages.translation_rules))
		{
			cerr<<"SelfExplain failed"<<endl;
			return false;
		}
		this->is_analyzed = false;
	}
	else
	{
		outf<<this->output_symbols;
	}
#else
		INFO("output_symbols="<<this->output_symbols);
		if(!SelfExplain(this->output_symbols,this->languages.translation_rules))
		{
			cerr<<"SelfExplain failed"<<endl;
			return false;
		}
		//当文法没有加载时不需要重新分析(加载文法后,调用文法分析)
		this->is_analyzed = false;
#endif

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

//FILE : data/null_grammar.xyz
//<start>--><wll0>==><start>-->$WLL0<wll0>;
//<wll0>--><translation>";"<spaces>==><wll0>--><translation>;
//<wll0>--><translation>";"<spaces><wll0>==><wll0>--><translation><wll0>;
//
//<translation>--><source_rule>"==>"<destination_rule>==><translation>--><source_rule><destination_rule>$TRANSLATION;
//<source_rule>--><rule>==><source_rule>--><rule>$SOURCE-RULE;
//<destination_rule>--><rule>==><destination_rule>--><rule>$DESTINATION-RULE;
//<rule>--><variable>"-->"<expression>==><rule>--><variable>$ROOT-SYMBOL<expression>$EXPRESSION$RULE;
//
//<expression>--><symbol>==><expression>--><symbol>;
//<expression>--><symbol><expression>==><expression>--><symbol><expression>;
//
//<symbol>--><variable>==><symbol>--><variable>$SUB-SYMBOL;
//<symbol>--><constant>==><symbol>--><constant>$SUB-SYMBOL;
//<symbol>-->"$"<remark>==><symbol>-->$REMARK<remark>$SUB-SYMBOL;
//
//<variable>-->"<"<identifier>">"==><variable>-->"$"<identifier>$VARIABLE;
//<constant>-->"\""<string>"\""==><constant>--><string>$CONSTANT;
//
//<remark>-->"WLL0"==><remark>-->$WLL0;
//<remark>-->"TRANSLATION"==><remark>-->$TRANSLATION;
//<remark>-->"SOURCE-RULE"==><remark>-->$SOURCE-RULE;
//<remark>-->"DESTINATION-RULE"==><remark>-->$DESTINATION-RULE;
//<remark>-->"RULE"==><remark>-->$RULE;
//<remark>-->"ROOT-SYMBOL"==><remark>-->$ROOT-SYMBOL;
//<remark>-->"EXPRESSION"==><remark>-->$EXPRESSION;
//<remark>-->"SUB-SYMBOL"==><remark>-->$SUB-SYMBOL;
//<remark>-->"VARIABLE"==><remark>-->$VARIABLE;
//<remark>-->"CONSTANT"==><remark>-->$CONSTANT;
//<remark>-->"REMARK"==><remark>-->$REMARK;
//
//<identifier>--><letter>==><identifier>--><letter>;
//<identifier>--><letter><letter_digit_string>==><identifier>--><letter><letter_digit_string>;
//<letter_digit_string>--><letter>==><letter_digit_string>--><letter>;
//<letter_digit_string>--><digit>==><letter_digit_string>--><digit>;
//<letter_digit_string>--><letter><letter_digit_string>==><letter_digit_string>--><letter><letter_digit_string>;
//<letter_digit_string>--><digit><letter_digit_string>==><letter_digit_string>--><digit><letter_digit_string>;
//
//<string>-->""==><string>-->"";
//<string>--><character>==><string>--><character>;
//<string>--><character><string>==><string>--><character><string>;
//<character>--><spacial>==><character>--><spacial>;
//<character>--><letter>==><character>--><letter>;
//<character>--><digit>==><character>--><digit>;
//<character>--><operator>==><character>--><operator>;
//<character>--><space>==><character>--><space>;
//
//<letter>-->"a"==><letter>-->"a";
//<letter>-->"b"==><letter>-->"b";
//<letter>-->"c"==><letter>-->"c";
//<letter>-->"d"==><letter>-->"d";
//<letter>-->"e"==><letter>-->"e";
//<letter>-->"f"==><letter>-->"f";
//<letter>-->"g"==><letter>-->"g";
//<letter>-->"h"==><letter>-->"h";
//<letter>-->"i"==><letter>-->"i";
//<letter>-->"j"==><letter>-->"j";
//<letter>-->"k"==><letter>-->"k";
//<letter>-->"l"==><letter>-->"l";
//<letter>-->"m"==><letter>-->"m";
//<letter>-->"n"==><letter>-->"n";
//<letter>-->"o"==><letter>-->"o";
//<letter>-->"p"==><letter>-->"p";
//<letter>-->"q"==><letter>-->"q";
//<letter>-->"r"==><letter>-->"r";
//<letter>-->"s"==><letter>-->"s";
//<letter>-->"t"==><letter>-->"t";
//<letter>-->"u"==><letter>-->"u";
//<letter>-->"v"==><letter>-->"v";
//<letter>-->"w"==><letter>-->"w";
//<letter>-->"x"==><letter>-->"x";
//<letter>-->"y"==><letter>-->"y";
//<letter>-->"z"==><letter>-->"z";
//<letter>-->"A"==><letter>-->"A";
//<letter>-->"B"==><letter>-->"B";
//<letter>-->"C"==><letter>-->"C";
//<letter>-->"D"==><letter>-->"D";
//<letter>-->"E"==><letter>-->"E";
//<letter>-->"F"==><letter>-->"F";
//<letter>-->"G"==><letter>-->"G";
//<letter>-->"H"==><letter>-->"H";
//<letter>-->"I"==><letter>-->"I";
//<letter>-->"J"==><letter>-->"J";
//<letter>-->"K"==><letter>-->"K";
//<letter>-->"L"==><letter>-->"L";
//<letter>-->"M"==><letter>-->"M";
//<letter>-->"N"==><letter>-->"N";
//<letter>-->"O"==><letter>-->"O";
//<letter>-->"P"==><letter>-->"P";
//<letter>-->"Q"==><letter>-->"Q";
//<letter>-->"R"==><letter>-->"R";
//<letter>-->"S"==><letter>-->"S";
//<letter>-->"T"==><letter>-->"T";
//<letter>-->"U"==><letter>-->"U";
//<letter>-->"V"==><letter>-->"V";
//<letter>-->"W"==><letter>-->"W";
//<letter>-->"X"==><letter>-->"X";
//<letter>-->"Y"==><letter>-->"Y";
//<letter>-->"Z"==><letter>-->"Z";
//<letter>-->"_"==><letter>-->"_";
//
//<digit>-->"0"==><digit>-->"0";
//<digit>-->"1"==><digit>-->"1";
//<digit>-->"2"==><digit>-->"2";
//<digit>-->"3"==><digit>-->"3";
//<digit>-->"4"==><digit>-->"4";
//<digit>-->"5"==><digit>-->"5";
//<digit>-->"6"==><digit>-->"6";
//<digit>-->"7"==><digit>-->"7";
//<digit>-->"8"==><digit>-->"8";
//<digit>-->"9"==><digit>-->"9";
//
//<operator>-->"+"==><operator>-->"+";
//<operator>-->"-"==><operator>-->"-";
//<operator>-->"*"==><operator>-->"*";
//<operator>-->"/"==><operator>-->"/";
//<operator>-->"%"==><operator>-->"%";
//<operator>-->"^"==><operator>-->"^";
//<operator>-->">"==><operator>-->">";
//<operator>-->"="==><operator>-->"=";
//<operator>-->"<"==><operator>-->"<";
//<operator>-->"#"==><operator>-->"#";
//<operator>-->","==><operator>-->",";
//<operator>-->":"==><operator>-->":";
//<operator>-->";"==><operator>-->";";
//<operator>-->"."==><operator>-->".";
//<operator>-->"?"==><operator>-->"?";
//<operator>-->"!"==><operator>-->"!";
//<operator>-->"'"==><operator>-->"'";
//<operator>-->"("==><operator>-->"(";
//<operator>-->")"==><operator>-->")";
//<operator>-->"["==><operator>-->"[";
//<operator>-->"]"==><operator>-->"]";
//<operator>-->"{"==><operator>-->"{";
//<operator>-->"}"==><operator>-->"}";
//<operator>-->"`"==><operator>-->"`";
//<operator>-->"~"==><operator>-->"~";
//<operator>-->"@"==><operator>-->"@";
//<operator>-->"$"==><operator>-->"$";
//<operator>-->"&"==><operator>-->"&";
//<operator>-->"|"==><operator>-->"|";
//
//<space>-->"\t"==><space>-->"\t";
//<space>-->"\n"==><space>-->"\n";
//<space>-->"\r"==><space>-->"\r";
//<space>-->" "==><space>-->" ";
//
//<spacial>-->"\\\""==><spacial>-->"\"";
//<spacial>-->"\\\\"==><spacial>-->"\\";
//<spacial>-->"\\t"==><spacial>-->"\t";
//<spacial>-->"\\n"==><spacial>-->"\n";
//<spacial>-->"\\r"==><spacial>-->"\r";
//
//<spaces>--><space>==><spaces>--><space>;
//<spaces>--><space><spaces>==><spaces>--><space><spaces>;

//从文法模板加载语言
bool LanguageParsers::LoadLanguage(istream& ins)
{
	if(!this->LoadInput(ins))
	{
		cerr<<"load input failed"<<endl;
		return false;
	}

	Wll0Loader loader(this->input_symbols);
	if(!loader.LoadWll0(this->languages.translation_rules))
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
