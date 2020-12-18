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

//$COND
inline bool IntepretCondCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LOOP
inline bool IntepretLoopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$DEF
inline bool IntepretDefCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SET
inline bool IntepretSetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$GET
inline bool IntepretGetCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$PUSHDATA
inline bool IntepretPushDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$POPDATA
inline bool IntepretPopDataCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$PUSH
inline bool IntepretPushCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$POP
inline bool IntepretPopCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CALL
inline bool IntepretCallCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$ARRAY
inline bool IntepretArrayCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$MAP
inline bool IntepretMapCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SUBSTR
inline bool IntepretSubStrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAST
inline bool IntepretCastCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$ADD
inline bool IntepretAddCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SUB
inline bool IntepretSubCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$MUL
inline bool IntepretMulCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$DIV
inline bool IntepretDivCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$EQ
inline bool IntepretEqCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$LT
inline bool IntepretLtCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$AND
inline bool IntepretAndCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$OR
inline bool IntepretOrCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$NOT
inline bool IntepretNotCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$SHELL
inline bool IntepretShellCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);

//$CAT
inline bool IntepretCatCommand(std::vector<Symbols>& data_stack, WllIntepreter* intepreter);


#endif /* WLL_COMPACT_COMMAND_H_ */
