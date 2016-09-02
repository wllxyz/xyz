#define TOPDOWN_PARSER  0
#define LR0_PARSER  1
#define LR1_PARSER  2

//#define PARSER  LR1_PARSER

#if PARSER==LR0_PARSER
#include "LR0Parsers.h"
#elif   PARSER==TOPDOWN_PARSER
#include "TopDownParsers.h"
#elif   PARSER==LR1_PARSER
#include "LR1Parsers.h"
#endif  //PARSER

#include <fstream>
#include <iostream>
#include <cstring>
#include "Wll1Intepreter.h"
using namespace std;

int main(int argc,char** argv)
{
	Compiler compiler;
#if PARSER==LR0_PARSER
	LR0Parsers* parser = new LR0Parsers(&compiler);
#elif   PARSER==TOPDOWN_PARSER
	TopDownParsers* parser = new TopDownParsers(&compiler);
#elif   PARSER==LR1_PARSER
	LR1Parsers* parser = new LR1Parsers(&compiler);
#endif  //PARSER
	compiler.parser_strategy.Set(parser);
	compiler.intepreter_strategy.Set(new Wll1Intepreter(&compiler));

	if(argc==1)
	{
		//cout<<argv[0]<<"<grammar_file> <input_file>"<<endl;
		cout<<"WLL0 language auto loaded ..."<<endl;
		do
		{
			string cmd;
			cin>>cmd;
			if(cmd=="quit")
			{
				break;
			}
			if(cmd=="load")
			{
				string file_name;
				cin>>file_name;
				ifstream input(file_name.c_str());
				if(input.fail())
				{
					cout<<"open file ["<<file_name<<"] failed!"<<endl;
					continue;
				}
				cout<<"loading ["<<file_name<<"] ..."<<endl;
				if(!compiler.Process(input,cout))
				{
					cout<<"process file ["<<file_name<<"] failed!"<<endl;
					continue;
				}
				cout<<"["<<file_name<<"] loaded."<<endl;
			}
			else if(cmd=="grammar")
			{
				cout<<"current loaded language rules:"<<endl;
				compiler.DisplayLanguage();
			}
		}while(true);

		return 0;
	}

	for(int i=1; i<argc; i++)
	{
		istream* input = NULL;
		ifstream input_file;
		if(strcmp(argv[i],"-")==0)
		{
			input = &cin;
		}
		else
		{
			input_file.open(argv[i]);
			if(input_file.fail())
			{
				cerr<<"open input file ["<<argv[i]<<"] failed!"<<endl;
				return -1;
			}
			input = &input_file;
		}
		if(!compiler.Process(*input,cout))
		{
			cerr<<"process input file ["<<argv[i]<<"] failed!"<<endl;
			return -1;
		}
	}

	return 0;
}
