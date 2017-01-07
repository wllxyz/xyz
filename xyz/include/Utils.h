#ifndef	UTILS_H
#define	UTILS_H

#include "LanguageSymbols.h"
#include <vector>

bool GetLineColumn(const std::vector<Symbols>& symbols,size_t position,size_t& line,size_t& column);
bool GetCurrentLine(const std::vector<Symbols>& symbols, size_t position, std::vector<Symbols>& current_line);

#endif

