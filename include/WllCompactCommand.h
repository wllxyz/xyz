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
inline bool IntepretLoadTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$ADD_TRANSLATION(COMPACT_SYMBOL) => VOID_SYMBOL
inline bool IntepretAddTranslationsCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$WLL0(COMPACT_SYMBOL) => VOID_SYMBOL
inline bool IntepretWll0Command(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$TRANSLATION(MAP_SYMBOL,MAP_SYMBOL) => MAP_SYMBOL
inline bool IntepretTranslationCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$RULE(VARIABLE_SYMBOL,COMPACT_SYMBOL) => MAP_SYMBOL
inline bool IntepretRuleCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$VARIABLE(COMPACT_SYMBOL) => VARIABLE_SYMBOL
inline bool IntepretVariableCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CONSTANT(COMPACT_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretConstantCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$REMARK(COMPACT_SYMBOL) => VARIABLE_SYMBOL
inline bool IntepretRemarkCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$EVAL(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
inline bool IntepretEvalCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$EXEC(ANY_SYMBOL) => SYMBOLS(AUTO COMPACT TO COMPACT_SYMBOL)
inline bool IntepretExecCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$IGNORE(ANY_SYMBOL) => ANY_SYMBOL
inline bool IntepretIgnoreCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$COMPACT(ANY_SYMBOL,ANY_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCompactCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LIST ($LIST, A, B, ... , N, n) = (A, B, ... , N)
//$LIST(ANY_SYMBOL, ... , ANY_SYMBOL, n) = S_EXP_SYMBOL
inline bool IntepretListCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$APPEND(S_EXP_SYMBOL, S_EXP_SYMBOL) = S_EXP_SYMBOL
//($APPEND, (A,B), (C,D)) = (A, B, C, D)
inline bool IntepretAppendCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CAR, (A,B,C)) = A
inline bool IntepretCarCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CDR(ANY_SYMBOL) = S_EXP_SYMBOL|NIL
//($CDR, (A,B,C)) = (B, C)
inline bool IntepretCdrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$COND(TRUE|FALSE,COMPACT|S_EXP,COMPACT|S_EXP)
//($COND, CONDITION, THEN, ELSE)
inline bool IntepretCondCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LOOP(COMPACT,COMPACT)
//($LOOP, CONDITION, REPEAT)
inline bool IntepretLoopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CALL(VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($CALL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$FCALL(STRING_SYMBOL,VARIABLE_SYMBOL,COMPACT_SYMBOL)
//($FCALL, STRING_SYMBOL, VARIABLE_SYMBOL, COMPACT_SYMBOL) => COMPACT_SYMBOL
inline bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LREF(REFERENCE,STRING)
inline bool IntepretLRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$RREF(REFERENCE,STRING)
inline bool IntepretRRefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$DEF(STRING, ANY_SYMBOL)
inline bool IntepretDefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SET(STRING|REFERENCE, ANY_SYMBOL)
inline bool IntepretSetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$GET(STRING|REFERENCE, ANY_SYMBOL)
inline bool IntepretGetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$PUSHDATA()
//($PUSHDATA)
inline bool IntepretPushDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$POPDATA()
//($POPDATA)
inline bool IntepretPopDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$PUSH(ANY_SYMBOL)
inline bool IntepretPushCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$POP(STRING_SYMBOL|REFERENCE)
inline bool IntepretPopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$ARRAY()
//($ARRAY)
inline bool IntepretArrayCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$MAP()
//($MAP)
inline bool IntepretMapCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAST(REMARK_SYMBOL,SYMBOL)
inline bool IntepretCastCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($ADD, SYMBOLS, SYMBOLS) = SYMBOLS + SYMBOLS
inline bool IntepretAddCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($SUB, SYMBOLS, SYMBOLS) = SYMBOLS - SYMBOLS
inline bool IntepretSubCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($MUL, SYMBOLS, SYMBOLS) = SYMBOLS * SYMBOLS
inline bool IntepretMulCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($DIV, SYMBOLS, SYMBOLS) = SYMBOLS / SYMBOLS
inline bool IntepretDivCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($EQ, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
inline bool IntepretEqCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//($LT, CHAR_SYMBOL|INTEGER_SYMBOL|LONG_SYMBOL|FLOAT_SYMBOL|DOUBLE_SYMBOL|COMPACT_SYMBOL, SYMBOL)
inline bool IntepretLtCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$AND(TRUE|FALSE, TRUE|FALSE)
inline bool IntepretAndCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$OR(TRUE|FALSE, TRUE|FALSE)
inline bool IntepretOrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$NOT(TRUE|FALSE, TRUE|FALSE)
inline bool IntepretNotCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SHELL
inline bool IntepretShellCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAT
inline bool IntepretCatCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SUBSTR
inline bool IntepretSubStrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);


#endif /* WLL_COMPACT_COMMAND_H_ */
