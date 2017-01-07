#include "Utils.h"

bool GetLineColumn(const vector<Symbols>& symbols,size_t position,size_t& line,size_t& column)
{
	if(symbols.size()<=position) return false;
	
	line = 0;
	column = 0;
	bool newline_flag = true;
	for(size_t i=0; i<position; i++)
	{
		if(newline_flag) 
		{
			line++;
			column=0;
			newline_flag = false;
		}

		if(symbols[i]=='\n')
		{
			newline_flag = true;
		}
		column++;
	}
	return true;
}

bool GetCurrentLine(const vector<Symbols>& symbols, size_t position, vector<Symbols>& current_line)
{
	if(symbols.size()<=position) return false;

	current_line.clear();

	//seek to the last line end
	size_t last_line_end=position;
	for(; last_line_end!=0; last_line_end--)
	{
		if(symbols[last_line_end]=='\n') break;
	}

	if(symbols[last_line_end]=='\n')
		last_line_end++;
	//copy current line content
	for(size_t i=last_line_end; i<symbols.size(); i++)
	{
		current_line.push_back(symbols[i]);
		if(symbols[i]=='\n') break;
	}

	return true;
}

