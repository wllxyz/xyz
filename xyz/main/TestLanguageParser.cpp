#define TOPDOWN_PARSER  0
#define LR0_PARSER  1
#define LR1_PARSER  2

#define PARSER  LR1_PARSER

#if PARSER==LR0_PARSER
#include "LR0Parsers.h"
#elif   PARSER==TOPDOWN_PARSER
#include "TopDownParsers.h"
#elif   PARSER==LR1_PARSER
#include "LR1Parsers.h"
#endif  //PARSER

#include <fstream>
#include <iostream>
using namespace std;

int main(int argc,char** argv)
{
#if PARSER==LR0_PARSER
	LR0Parsers parser;
#elif   PARSER==TOPDOWN_PARSER
	TopDownParsers parser;
#elif   PARSER==LR1_PARSER
	LR1Parsers parser;
#endif  //PARSER

	if(argc != 3)
	{
		cout<<"Usage : "<<argv[0]<<" <grammar_file> <input_file> "<<endl;
		return -1;
	}
	
	ifstream grammar_file(argv[1]);
	if(grammar_file.fail())
	{
		cout<<"open grammar file ["<<argv[1]<<"] failed"<<endl;
		return -1;
	}
	parser.LoadLanguage(grammar_file);

	ifstream input_file(argv[2]);
	if(input_file.fail())
	{
		cout<<"open input file ["<<argv[2]<<"] failed"<<endl;
		return -1;	
	}

	if(!parser.Process(input_file,cout))
	{
		cout<<"process file ["<<argv[2]<<"] failed!"<<endl;
		return -1;
	}	

	return 0;
}

