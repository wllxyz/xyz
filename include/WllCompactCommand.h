/*
 * WllCompactCommand.h
 *
 *  Created on: 2016-1-1
 *      Author: wll
 */

#ifndef WLL_COMPACT_COMMAND_H_
#define WLL_COMPACT_COMMAND_H_

#include "WllIntepreter.h"

//$LOAD_TRANSLATION(COMPACT_SYMBOL) => VOID_SYMBOL
bool IntepretLoadTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$ADD_TRANSLATION(COMPACT_SYMBOL) => VOID_SYMBOL
bool IntepretAddTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$WLL0(COMPACT_SYMBOL) => VOID_SYMBOL
bool IntepretWll0Command(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$TRANSLATION(MAP_SYMBOL,MAP_SYMBOL) => MAP_SYMBOL
bool IntepretTranslationCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$RULE(VARIABLE_SYMBOL,COMPACT_SYMBOL) => MAP_SYMBOL
bool IntepretRuleCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$VARIABLE(COMPACT_SYMBOL) => VARIABLE_SYMBOL
bool IntepretVariableCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CONSTANT(COMPACT_SYMBOL) => COMPACT_SYMBOL
bool IntepretConstantCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$REMARK(COMPACT_SYMBOL) => VARIABLE_SYMBOL
bool IntepretRemarkCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$EVAL(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
bool IntepretEvalCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$EXEC(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
bool IntepretExecCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$IGNORE(ANY_SYMBOL) => ANY_SYMBOL
bool IntepretIgnoreCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$COMPACT(ANY_SYMBOL,ANY_SYMBOL) => COMPACT_SYMBOL
bool IntepretCompactCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LIST ($LIST, A, B, ... , N, n) = (A, B, ... , N)
//$LIST(ANY_SYMBOL, ... , ANY_SYMBOL, n) = S_EXP_SYMBOL
bool IntepretListCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$APPEND(S_EXP_SYMBOL, S_EXP_SYMBOL) = S_EXP_SYMBOL
//($APPEND, (A,B), (C,D)) = (A, B, C, D)
bool IntepretAppendCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CAR, (A,B,C)) = A
bool IntepretCarCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CDR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CDR, (A,B,C)) = (B, C)
bool IntepretCdrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$COND(TRUE|FALSE,COMPACT|S_EXP,COMPACT|S_EXP)
//($COND, CONDITION, THEN, ELSE)
bool IntepretCondCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LOOP(COMPACT,COMPACT)
//($LOOP, CONDITION, REPEAT)
bool IntepretLoopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CALL(VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($CALL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$FCALL(STRING_SYMBOL,VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($FCALL, STRING_SYMBOL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
bool IntepretFCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LREF(REFERENCE,STRING)
bool IntepretLRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$RREF(REFERENCE,STRING)
bool IntepretRRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$DEF(STRING, ANY_SYMBOL)
bool IntepretDefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SET(STRING|REFERENCE, ANY_SYMBOL)
bool IntepretSetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$GET(STRING|REFERENCE, ANY_SYMBOL)
bool IntepretGetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$PUSHDATA()
//($PUSHDATA)
bool IntepretPushDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$POPDATA()
//($POPDATA)
bool IntepretPopDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$PUSH(ANY_SYMBOL)
bool IntepretPushCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$POP(STRING_SYMBOL|REFERENCE)
bool IntepretPopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$ARRAY()
//($ARRAY)
bool IntepretArrayCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$MAP()
//($MAP)
bool IntepretMapCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAST(REMARK_SYMBOL,SYMBOL)
bool IntepretCastCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($ADD, SYMBOLS, SYMBOLS) = SYMBOLS + SYMBOLS
bool IntepretAddCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($SUB, SYMBOLS, SYMBOLS) = SYMBOLS - SYMBOLS
bool IntepretSubCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($MUL, SYMBOLS, SYMBOLS) = SYMBOLS * SYMBOLS
bool IntepretMulCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($DIV, SYMBOLS, SYMBOLS) = SYMBOLS / SYMBOLS
bool IntepretDivCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($EQ, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
bool IntepretEqCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($LT, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
bool IntepretLtCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$AND(TRUE|FALSE, TRUE|FALSE)
bool IntepretAndCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$OR(TRUE|FALSE, TRUE|FALSE)
bool IntepretOrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$NOT(TRUE|FALSE, TRUE|FALSE)
bool IntepretNotCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SHELL(STRING_SYMBOL) => STRING_SYMBOL
bool IntepretShellCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAT(STRING_SYMBOL) => STRING_SYMBOL
bool IntepretCatCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SUBSTR(STRING) => STRING
bool IntepretSubStrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

bool IntepretCommand(const Symbols& command, std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

#endif /* WLL_COMPACT_COMMAND_H_ */
