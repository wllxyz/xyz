/*
 * Wll1Loader.cpp
 *
 *  Created on: 2015-12-13
 *      Author: wll
 */


#include "Wll1Loader.h"
#include <assert.h>
#include "WllTrace.h"


Wll1Loader::Wll1Loader(const vector<Symbols>& input_symbols) : WllLoader(input_symbols)
{
	this->grammar_file_name = "wll1.xyz";
	this->input_pos = 0;
	INFO("input_symbols="<<this->input_symbols);
}

/*
void Wll1Loader::SetInputSymbols(const char* text)
{
	this->input_symbols.clear();
	while(*text)
	{
		this->input_symbols.push_back(*text);
		text++;
	}
	this->input_pos = 0;
}
*/

bool Wll1Loader::LoadWll(vector<LanguageTranslations>& translations)
{
	//<wll0>--><translation>";"<spaces>==><wll0>--><translation>;
	//<wll0>--><translation>";"<spaces><wll0>==><wll0>--><translation><wll0>;
	if(this->Encount(Symbols::END_SYMBOL)) return true;
	LanguageTranslations translation;
	return
		this->LoadTranslation(translation) &&
		this->Accept(';') &&
		this->SkipSpaces() &&
		(translations.push_back(translation),true) &&
		this->LoadWll(translations);
}

bool Wll1Loader::LoadTranslation(LanguageTranslations& translation)
{
	//
	//<translation>--><source_rule>"==>"<destination_rule>==><translation>--><source_rule><destination_rule>$TRANSLATION;
	LanguageRules source_rule, destination_rule;
	return
		this->LoadSourceRule(source_rule) &&
		this->Accept(LanguageExpressions("==>")) &&
		this->LoadDestinationRule(destination_rule) &&
		(translation=LanguageTranslations(source_rule,destination_rule),true);
}

bool Wll1Loader::LoadSourceRule(LanguageRules& source_rule)
{
	//<source_rule>--><rule>==><source_rule>--><rule>$SOURCE-RULE;
	return
		this->LoadRule(source_rule);
}

bool Wll1Loader::LoadDestinationRule(LanguageRules& destination_rule)
{
	//<destination_rule>--><rule>==><destination_rule>--><rule>$DESTINATION-RULE;
	return
		this->LoadRule(destination_rule);
}

bool Wll1Loader::LoadRule(LanguageRules& rule)
{
	//<rule>--><variable>"-->"<expression>==><rule>--><variable>$ROOT-SYMBOL<expression>$EXPRESSION$RULE;
	Symbols root_symbol;
	LanguageExpressions expression;
	return
		this->LoadRootSymbol(root_symbol) &&
		this->Accept(LanguageExpressions("-->")) &&
		this->LoadExpression(expression) &&
		(rule = LanguageRules(root_symbol,expression),true);
}

bool Wll1Loader::LoadRootSymbol(Symbols& root_symbol)
{
	return
		this->LoadVariable(root_symbol);
}

bool Wll1Loader::LoadExpression(LanguageExpressions& expression)
{
	//<expression>--><symbol>==><expression>--><symbol>;
	//<expression>--><symbol><expression>==><expression>--><symbol><expression>;
	Symbols no_used;
	if(!(this->Encount('<') || this->Encount('"') || this->Encount('\'') || this->Encount('$') || this->EncountRemark(no_used))) return true;
	LanguageExpressions symbol;

	return
		this->LoadSymbol(symbol) &&
		(expression += symbol, true) &&
		this->LoadExpression(expression);

}

bool Wll1Loader::LoadSymbol(LanguageExpressions& symbol)
{
	//
	//<symbol>--><variable>==><symbol>--><variable>$SUB-SYMBOL;
	//<symbol>--><constant>==><symbol>--><constant>$SUB-SYMBOL;
	//<symbol>-->"$"<remark>==><symbol>-->$REMARK<remark>$SUB-SYMBOL;
	Symbols variable;
	LanguageExpressions constant;
	Symbols remark;

	return
		(this->LoadVariable(variable) && (symbol = variable, true)) ||
		(this->LoadConstant(constant) && (symbol = constant, true)) ||
		(this->LoadRemark(remark) && (symbol = remark, true));
}

bool Wll1Loader::LoadVariable(Symbols& variable)
{
	//
	//<variable>-->"<"<identifier>">"==><variable>-->"$"<identifier>$VARIABLE;
	string ident;
	return
		this->Accept('<') &&
		this->LoadIdent(ident) &&
		this->Accept('>') &&
		(variable = Symbols(("<"+ident+">").c_str()),true);
}

bool Wll1Loader::LoadConstant(LanguageExpressions& constant)
{
	//<constant>-->"\""<string>"\""==><constant>--><string>$CONSTANT;
	//<constant>-->"'"<string>"'"==><constant>--><string>$CONSTANT;
	return
		(this->Accept('"') && this->LoadString(constant) && this->Accept('"')) ||
		(this->Accept('\'') && this->LoadOriginalString(constant) && this->Accept('\''));
}

bool Wll1Loader::LoadRemark(Symbols& remark)
{
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
	//<remark>-->"*"==><remark>-->$*;
	//
	string ident;
	//<remark>-->$*==><remark>-->$*;
	//<remark>-->"$"<identifier>==><remark>-->"$"<identifier>;
	return
		this->ExpectRemark(remark) ||
		this->Accept('$') &&
		this->LoadIdent(ident) &&
		(remark = Symbols(REMARK_SYMBOL,("$"+ident).c_str()),true);
}

bool Wll1Loader::LoadIdent(string& ident)
{
	//<identifier>--><letter>==><identifier>--><letter>;
	//<identifier>--><letter><letter_digit_string>==><identifier>--><letter><letter_digit_string>;
	//<letter_digit_string>--><letter>==><letter_digit_string>--><letter>;
	//<letter_digit_string>--><digit>==><letter_digit_string>--><digit>;
	//<letter_digit_string>--><letter><letter_digit_string>==><letter_digit_string>--><letter><letter_digit_string>;
	//<letter_digit_string>--><digit><letter_digit_string>==><letter_digit_string>--><digit><letter_digit_string>;
	//
	char c;
	if(this->ExpectLetter(c))
		ident += c;
	else
		return false;
	while(this->ExpectLetter(c) || this->ExpectDigit(c))
		ident += c;
	return true;
}

bool Wll1Loader::LoadString(LanguageExpressions& str)
{
	//<string>-->""==><string>-->"";
	//<string>--><character><string>==><string>--><character><string>;
	//<character>--><spacial>==><character>--><spacial>;
	//<character>--><letter>==><character>--><letter>;
	//<character>--><digit>==><character>--><digit>;
	//<character>--><operator>==><character>--><operator>;
	//<character>--><space>==><character>--><space>;
	//<spacial>-->"\\\""==><spacial>-->"\"";
	//<spacial>-->"\\\\"==><spacial>-->"\\";
	//<spacial>-->"\\t"==><spacial>-->"\t";
	//<spacial>-->"\\n"==><spacial>-->"\n";
	//<spacial>-->"\\r"==><spacial>-->"\r";
	Symbols symbol;
	while(!(this->Encount('"') || this->Encount(Symbols::END_SYMBOL)))
	{
		bool convert = this->Accept('\\');
		if(this->ExpectCharacter(symbol))
		{
			if(convert)
			{
				switch(symbol.value)
				{
				case 'n':
					symbol = '\n';
					break;
				case 'r':
					symbol = '\r';
					break;
				case 't':
					symbol = '\t';
					break;
				default:
					break;
				}
			}
			str += symbol;
		}
	}
	if(this->Encount('"'))
		return true;
	else
		return false;
}

//''单引号包括的字符串原样输出,不需要对\进行转义处理
bool Wll1Loader::LoadOriginalString(LanguageExpressions& str)
{
	//<string>-->''==><string>-->'';
	//<string>--><character><string>==><string>--><character><string>;
	//<character>-->除'之外的所有其他字符
	Symbols symbol;
	while(!(this->Encount('\'') || this->Encount(Symbols::END_SYMBOL)))
	{
		assert(this->ExpectCharacter(symbol));
		str += symbol;
	}
	if(this->Encount('\''))
		return true;
	else
		return false;
}

bool Wll1Loader::EncountRemark(Symbols& remark)
{
        if(this->input_pos >= this->input_symbols.size()) return false;
        Symbols symbol = this->GetSymbol();
        if(symbol.IsRemark())
        {
                remark = symbol;
                return true;
        }
        else
        {
                return false;
        }
}

bool Wll1Loader::ExpectRemark(Symbols& remark)
{
	//<remark>-->$*==><remark>-->$*;
	if(this->EncountRemark(remark))
	{
		this->input_pos++;
		return true;
	}
	else
	{
		return false;
	}
}

bool Wll1Loader::ExpectLetter(char& c)
{
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
	if(this->input_pos >= this->input_symbols.size()) return false;

	c = this->GetSymbol().value;
	if('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_')
	{
		this->input_pos++;
		return true;
	}
	else
	{
		return false;
	}
}
bool Wll1Loader::ExpectDigit(char& c)
{
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
	if(this->input_pos >= this->input_symbols.size()) return false;

	c = this->GetSymbol().value;
	if(isdigit(c))
	{
		this->input_pos++;
		return true;
	}
	else
	{
		return false;
	}
}

bool Wll1Loader::ExpectCharacter(Symbols& symbol)
{
	if(this->input_pos >= this->input_symbols.size())
	{
		symbol = Symbols::END_SYMBOL;
		return false;
	}
	else
	{
		symbol = this->input_symbols[this->input_pos++];
		return true;
	}
}

bool Wll1Loader::ExpectSpace(char& c)
{
	if(this->input_pos >= this->input_symbols.size()) return false;

	c = this->GetSymbol().value;
	if(isspace(c))
	{
		this->input_pos++;
		return true;
	}
	else
	{
		return false;
	}
}

bool Wll1Loader::SkipSpaces()
{
	//<spaces>--><space>==><spaces>--><space>;
	//<spaces>--><space><spaces>==><spaces>--><space><spaces>;
	char c;
	while(this->ExpectSpace(c));
	return true;
}

bool Wll1Loader::Accept(const Symbols& symbol)
{
	if(this->Encount(symbol))
	{
		this->input_pos++;
		return true;
	}
	else
	{
		return false;
	}
}

bool Wll1Loader::Accept(const LanguageExpressions& expression)
{
	int position = this->input_pos;
	for(vector<Symbols>::const_iterator i = expression.symbols.begin(); i != expression.symbols.end(); ++i)
	{
		if(!this->Accept(*i))
		{
			this->input_pos = position;
			return false;
		}
	}
	return true;
}

bool Wll1Loader::Encount(const Symbols& symbol)
{
	return this->GetSymbol()==symbol;
}

const Symbols& Wll1Loader::GetSymbol()
{
	return (this->input_pos >= this->input_symbols.size()) ? Symbols::END_SYMBOL : this->input_symbols[this->input_pos];
}

void Wll1Loader::ShowErrorMessage()
{
	TERM_ERROR("已经接受的输入符号:");
	for(int i = 0; i<this->input_symbols.size(); i++)
	{
		if(i == this->input_pos)
			cerr<<"["<<this->input_symbols[i]<<"]"<<endl;
		else
			cerr<<this->input_symbols[i];
	}
}

bool Wll1Loader::TestLanguage()
{
	static LanguageGrammar wll_xyz_languages;
	static bool is_wll_xyz_loaded = false;

	return WllLoader::TestLanguage(wll_xyz_languages, is_wll_xyz_loaded);
}
