/*
 * WllCompactCommand.h
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#ifndef WLL_COMPACT_COMMAND_H_
#define WLL_COMPACT_COMMAND_H_

#include "WllIntepreter.h"

class WllCompactCommand
{
public:
	WllCompactCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result) = 0;
protected:
	Symbols command;
	WllIntepreter* intepreter;
};

class LoadTranslationsCommand : public WllCompactCommand
{
public:
	LoadTranslationsCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AddTranslationsCommand : public WllCompactCommand
{
public:
	AddTranslationsCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class Wll0Command : public WllCompactCommand
{
public:
	Wll0Command(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class TranslationCommand : public WllCompactCommand
{
public:
	TranslationCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class RuleCommand : public WllCompactCommand
{
public:
	RuleCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class VariableCommand : public WllCompactCommand
{
public:
	VariableCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ConstantCommand : public WllCompactCommand
{
public:
	ConstantCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class RemarkCommand : public WllCompactCommand
{
public:
	RemarkCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class EvalCommand : public WllCompactCommand
{
public:
	EvalCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ExecCommand : public WllCompactCommand
{
public:
	ExecCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class IgnoreCommand : public WllCompactCommand
{
public:
	IgnoreCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ListCommand : public WllCompactCommand
{
public:
	ListCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AppendCommand : public WllCompactCommand
{
public:
	AppendCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CarCommand : public WllCompactCommand
{
public:
	CarCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CdrCommand : public WllCompactCommand
{
public:
	CdrCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CondCommand : public WllCompactCommand
{
public:
	CondCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LoopCommand : public WllCompactCommand
{
public:
	LoopCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class DefCommand : public WllCompactCommand
{
public:
	DefCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class SetCommand : public WllCompactCommand
{
public:
	SetCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class GetCommand : public WllCompactCommand
{
public:
	GetCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class PushDataCommand : public WllCompactCommand
{
public:
	PushDataCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class PopDataCommand : public WllCompactCommand
{
public:
	PopDataCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class PushCommand : public WllCompactCommand
{
public:
	PushCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class PopCommand : public WllCompactCommand
{
public:
	PopCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CallCommand : public WllCompactCommand
{
public:
	CallCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ArrayCommand : public WllCompactCommand
{
public:
	ArrayCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class MapCommand : public WllCompactCommand
{
public:
	MapCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class SubStrCommand : public WllCompactCommand
{
public:
	SubStrCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CastCommand : public WllCompactCommand
{
public:
	CastCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AddCommand : public WllCompactCommand
{
public:
	AddCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class SubCommand : public WllCompactCommand
{
public:
	SubCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class MulCommand : public WllCompactCommand
{
public:
	MulCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class DivCommand : public WllCompactCommand
{
public:
	DivCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class EqCommand : public WllCompactCommand
{
public:
	EqCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class LtCommand : public WllCompactCommand
{
public:
	LtCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class AndCommand : public WllCompactCommand
{
public:
	AndCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};


class OrCommand : public WllCompactCommand
{
public:
	OrCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class NotCommand : public WllCompactCommand
{
public:
	NotCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class ShellCommand : public WllCompactCommand
{
public:
	ShellCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class CatCommand : public WllCompactCommand
{
public:
	CatCommand(Symbols cmd, WllIntepreter* intepreter);
	virtual bool Intepret(std::vector<Symbols>& result);
};

class WllCommandFactory
{
public:
	static WllCompactCommand* CreateCommand(Symbols cmd, WllIntepreter* intepreter);
};

#endif /* WLL_COMPACT_COMMAND_H_ */
