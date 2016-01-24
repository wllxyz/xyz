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
	WllCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result) = 0;
protected:
	Symbols command;
	std::vector< std::vector<Symbols> >& parameters;
	WllIntepreter* intepreter;
	std::vector<LanguageTranslations>* translations;
};

class AddCommand : public WllCommand
{
public:
	AddCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};


class SubCommand : public WllCommand
{
public:
	SubCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class MulCommand : public WllCommand
{
public:
	MulCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class DivCommand : public WllCommand
{
public:
	DivCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LoadTranslationsCommand : public WllCommand
{
public:
	LoadTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AddTranslationsCommand : public WllCommand
{
public:
	AddTranslationsCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CondCommand : public WllCommand
{
public:
	CondCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class EqCommand : public WllCommand
{
public:
	EqCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LtCommand : public WllCommand
{
public:
	LtCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AndCommand : public WllCommand
{
public:
	AndCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};


class OrCommand : public WllCommand
{
public:
	OrCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class NotCommand : public WllCommand
{
public:
	NotCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ShellCommand : public WllCommand
{
public:
	ShellCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class SetCommand : public WllCommand
{
public:
	SetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class GetCommand : public WllCommand
{
public:
	GetCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class PushDataCommand : public WllCommand
{
public:
	PushDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class PopDataCommand : public WllCommand
{
public:
	PopDataCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class EvalCommand : public WllCommand
{
public:
	EvalCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LoopCommand : public WllCommand
{
public:
	LoopCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class WllCommandFactory
{
public:
	static WllCommand* CreateCommand(Symbols cmd, std::vector< std::vector<Symbols> >& parameter_fields, WllIntepreter* intepreter, std::vector<LanguageTranslations>* translations);
};

#endif /* WLL_COMMAND_H_ */
