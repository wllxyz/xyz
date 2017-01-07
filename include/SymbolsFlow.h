/*
 * SymbolsFlow.h
 *
 *  Created on: 2016年8月31日
 *      Author: wll
 */

#ifndef INCLUDE_SYMBOLSFLOW_H_
#define INCLUDE_SYMBOLSFLOW_H_

#include "LanguageSymbols.h"
#include <iostream>

class SymbolsFlow
{
public:
	//get current position symbol, when is end return Symbols::END_SYMBOL
	virtual Symbols GetSymbol() { return this->symbol; }
	//is end of the symbols?
	virtual bool IsEnd()=0;
	//go ahead one position, failed when is end
	virtual bool Advance()=0;
	//get row no of symbols
	virtual int Row() { return this->row; }
	//get column no of symbols
	virtual int Column() { return this->column; }
	//get position of symbols
	virtual int Position() { return this->position; }
protected:
	SymbolsFlow()
	{
		this->position = 0;
		this->row = 1;
		this->column = 1;
		this->symbol = Symbols::END_SYMBOL;
	}
protected:
	int position;			//从0开始
	int row;					//从1开始
	int column;				//从1开始
	Symbols symbol;
};

class StreamSymbolsFlow  : public SymbolsFlow
{
public:
	StreamSymbolsFlow(std::istream& input) : input_stream(input)
	{
		//初始化为第一个符号
		char c = this->input_stream.get();
		this->symbol = this->input_stream.eof()?Symbols::END_SYMBOL:Symbols(c);
	}
	//is end of the symbols?
	bool IsEnd()
	{
		return this->input_stream.eof();
	}
	//go ahead one position, failed when is end
	bool Advance()
	{
		if(this->IsEnd())
		{
			this->symbol = Symbols::END_SYMBOL;
			return false;
		}
		else
		{
			//如果上一个符号是换行符号,则row++,column=0
			if(this->symbol == Symbols('\n'))
			{
				this->row++;
				this->column = 0;
			}
			//处理当前符号
			char c = this->input_stream.get();
			this->symbol = this->input_stream.eof()?Symbols::END_SYMBOL:Symbols(c);
			this->position++;
			this->column++;
			return true;
		}
	}

protected:
	std::istream& input_stream;
};

class ArraySymbolsFlow  : public SymbolsFlow
{
public:
	ArraySymbolsFlow(const std::vector<Symbols>& input) : input_symbols(input)
	{
		//初始化为第一个符号
		this->symbol = this->input_symbols.empty() ? Symbols::END_SYMBOL : this->input_symbols[this->position];
	}
	//is end of the symbols?
	bool IsEnd()
	{
		return (this->position >= this->input_symbols.size());
	}
	//go ahead one position, failed when is end
	bool Advance()
	{
		if(this->IsEnd())
		{
			this->symbol = Symbols::END_SYMBOL;
			return false;
		}
		else
		{
			//如果上一个符号是换行符号,则row++,column=0
			if(this->symbol == Symbols('\n'))
			{
				this->row++;
				this->column = 0;
			}
			this->position++;
			this->column++;
			this->symbol = this->IsEnd() ? Symbols::END_SYMBOL : this->input_symbols[this->position];
			return true;
		}
	}

protected:
	const std::vector<Symbols>& input_symbols;
};

#endif /* INCLUDE_SYMBOLSFLOW_H_ */
