/*
 * WllCommand.h
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#ifndef WLL_COMMAND_H_
#define WLL_COMMAND_H_

#include "WllIntepreter.h"

class WllCommand
{
public:
	WllCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result) = 0;
protected:
	Symbols command;
	std::vector< std::vector<Symbols> >& parameters;
	WllIntepreter* intepreter;
};

class AddCommand : public WllCommand
{
public:
	AddCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};


class SubCommand : public WllCommand
{
public:
	SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class MulCommand : public WllCommand
{
public:
	MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class DivCommand : public WllCommand
{
public:
	DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LoadTranslationsCommand : public WllCommand
{
public:
	LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AddTranslationsCommand : public WllCommand
{
public:
	AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CondCommand : public WllCommand
{
public:
	CondCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class EqCommand : public WllCommand
{
public:
	EqCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LtCommand : public WllCommand
{
public:
	LtCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AndCommand : public WllCommand
{
public:
	AndCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};


class OrCommand : public WllCommand
{
public:
	OrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class NotCommand : public WllCommand
{
public:
	NotCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ShellCommand : public WllCommand
{
public:
	ShellCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class WllCommandFactory
{
public:
	static WllCommand* CreateCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter);
};

#endif /* WLL_COMMAND_H_ */
