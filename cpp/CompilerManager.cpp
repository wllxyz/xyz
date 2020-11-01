//
// Created by wll on 19-6-30.
//
#include "CompilerManager.h"
#include <fstream>
#include "WllTrace.h"

Compiler* CompilerManager::CreateOrGetCompiler(std::string compiler_grammar_file_name)
{
    map<string, Compiler*>::iterator i = this->compiler_map.find(compiler_grammar_file_name);
    if (i == this->compiler_map.end())
    {
        INFO("create compiler (" << compiler_grammar_file_name << ") instance");
        Compiler* compiler = new Compiler();
        ifstream input_grammar(compiler_grammar_file_name.c_str());
        if(!input_grammar)
        {
            ERROR("open gramar file["<<compiler_grammar_file_name<<"] failed");
            return nullptr;
        }
        if(!compiler->Process(input_grammar, cout))
        {
            ERROR("process grammar_file_name["<<compiler_grammar_file_name<<"] failed");
            return nullptr;
        }
        this->compiler_map.insert(std::pair<string,Compiler*>(compiler_grammar_file_name,compiler));
        return compiler;
    } else {
        INFO("get compiler (" << compiler_grammar_file_name << ") instance");
        return i->second;
    }
}