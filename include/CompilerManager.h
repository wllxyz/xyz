//
// Created by wll on 19-6-30.
//

#include "Compiler.h"
#include <string>
#include <map>

#ifndef XYZ_COMPILERMANAGER_H
#define XYZ_COMPILERMANAGER_H

class CompilerManager {
public:
    Compiler* CreateOrGetCompiler(std::string compiler_grammar_file_name);

private:
    map<string, Compiler*> compiler_map;
};

#endif //XYZ_COMPILERMANAGER_H
