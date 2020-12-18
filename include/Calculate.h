#ifndef CALCULATE_H
#define CALCULATE_H

#include "LanguageSymbols.h"

//CHAR_SYMBOL => Symbols::CHAR
Symbols ToRemarkType(SymbolTypes type);
//Symbols::CHAR => CHAR_SYMBOL
SymbolTypes ToEnumType(Symbols type);

//根據字符常量內容轉換爲對應類型的數值型變量值
Symbols CastToNumber(const vector<Symbols>& constant_symbols);
//根據字符串內容檢測能轉換的數值類型
SymbolTypes DetectNumType(const string& number_string);

//把字符常量串轉換爲對應的數值類型變量值
Symbols CastTo(SymbolTypes type, string number_string);

//強制類型轉換
//	type 		-- 目標類型(內部枚舉表示）
//	value 		-- 值
Symbols CastTo(SymbolTypes type, const Symbols& value);

//強制類型轉換
//	targetType 	-- 目標類型（外部Remark類型表示）
//	value 		-- 值
Symbols CastTo(const Symbols& targetType, const Symbols& value);

//強制類型轉換
//	targetType 	-- 目標類型（外部Remark類型表示）
//	value 		-- 值(list參數表示形式）
Symbols CastTo(const Symbols& targetType, std::vector<Symbols>& values);

//自动对参数进行类型统一转换
//	values		-- 参数Symbol列表
//	treatConstantsAsNumber	--　如果为true，则尝试把字符串转换为对应的Number类型
//				-- 如果为false,则默认把常量列表转换为STRING_SYMBOL类型
Symbols CastTo(std::vector<Symbols>& values, bool treatConstantsAsNumber=false);

//算術類型和STRING類型＋
//	自動根據 CHAR => INTEGER => LONG => FLOAT => DOUBLE => STRING 進行隱式類型轉換
Symbols Add(const Symbols& a, const Symbols& b);
const Symbols& AddTo(Symbols& a, const Symbols& b);

//算術類型-
//	自動根據 CHAR => INTEGER => LONG => FLOAT => DOUBLE => STRING 進行隱式類型轉換
Symbols operator-(const Symbols& a, const Symbols& b);
const Symbols& operator-=(Symbols& a, const Symbols& b);

//算術類型*
//	自動根據 CHAR => INTEGER => LONG => FLOAT => DOUBLE => STRING 進行隱式類型轉換
Symbols operator*(const Symbols& a, const Symbols& b);
const Symbols& operator*=(Symbols& a, const Symbols& b);

//算術類型/
//	自動根據 CHAR => INTEGER => LONG => FLOAT => DOUBLE => STRING 進行隱式類型轉換
Symbols operator/(const Symbols& a, const Symbols& b);
const Symbols& operator/=(Symbols& a, const Symbols& b);

//算術類型/
//	自動根據 CHAR => INTEGER => LONG => FLOAT => DOUBLE => STRING 進行隱式類型轉換
Symbols operator%(const Symbols& a, const Symbols& b);
const Symbols& operator%=(Symbols& a, const Symbols& b);

//算術類型比較 a==b
bool Equal(const Symbols& a, const Symbols& b);
//算術類型比較 a<b
bool LessThan(const Symbols& a, const Symbols& b);

//把长度大于1的Symbols自动打包成COMPACT_SYMBOL类型
Symbols Encode(vector<Symbols>& value, bool force = false);
//把COMPACT_SYMBOL类型的Symbols复原成vector<Symbols>数组
vector<Symbols> Decode(Symbols& symbol);

//递归展平COMPACT_SYMBOL
void Flat(const Symbols& compacted_symbol, /* ==> */ vector<Symbols>& flated_symbols);
//递归展平COMPACT_SYMBOL
void Flat(const vector<Symbols>& symbols, /* ==> */ vector<Symbols>& flated_symbols);

#endif //CALCULATE_H
