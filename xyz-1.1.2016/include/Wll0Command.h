/*
 * Wll0Command.h
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#ifndef WLL0_COMMAND_H_
#define WLL0_COMMAND_H_

#include "Wll0Intepreter.h"

class Wll0Command
{
public:
	Wll0Command(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result) = 0;
protected:
	Symbols command;
	std::vector< std::vector<Symbols> >& parameters;
	Wll0Intepreter* intepreter;
};

class AddCommand : public Wll0Command
{
public:
	AddCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};


class SubCommand : public Wll0Command
{
public:
	SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class MulCommand : public Wll0Command
{
public:
	MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class DivCommand : public Wll0Command
{
public:
	DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LoadTranslationsCommand : public Wll0Command
{
public:
	LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AddTranslationsCommand : public Wll0Command
{
public:
	AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, Wll0Intepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

#endif /* WLL0_COMMAND_H_ */
