/*======================================================================
Имя файла:          add_ValueConv.h
Автор:
Версия файла:
Дата изменения:
Применяемость:      
Описание:
----------------------------------------------------------------------*/

#ifndef VALUE_CONV_
#define VALUE_CONV_

#include "std.h"
#include "peref_Clock.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__TMS320C28X__)
/*#define ValueToPU0(Val, Base)	(Uns)_IQ22rmpy(_IQ22div(Val, Base), 100)
#define ValueToPU1(Val, Base)	(Uns)_IQ25rmpy(_IQ25div(Val, Base), 1000)
#define ValueToPU2(Val, Base)	(Uns)_IQ29rmpy(_IQ29div(Val, Base), 10000)

#define PU0ToValue(Val, Base)	(Uns)_IQ22rmpy(_IQ22div(Val, 100),   Base)
#define PU1ToValue(Val, Base)	(Uns)_IQ25rmpy(_IQ25div(Val, 1000),  Base)
#define PU2ToValue(Val, Base)	(Uns)_IQ29rmpy(_IQ29div(Val, 10000), Base)

#define GainPU(Base, Max)			_IQ22div(Max, Base)
#define ValueToPU(Value, Gain)	_IQ22rmpy(Value, Gain)*/
#endif

#define InRange(Val, Min, Max)	(((Val) >= (Min)) && ((Val) <= (Max)))

#define MIN_VAL(Val1, Val2)		((Val1) < (Val2) ? (Val1) : (Val2))
#define MAX_VAL(Val1, Val2)		((Val1) > (Val2) ? (Val1) : (Val2))

#define SAT_VAL(Val,Min,Max){	if (Val <= Min) return Min;\
								else if (Val >= Max) return Max;\
								return Val;}

#define Min3Value(Type) \
  Type Min = 0; \
  Min = MIN_VAL(Min, Val1); \
  Min = MIN_VAL(Min, Val2); \
  Min = MIN_VAL(Min, Val3); \
  return Min

#define Mid3Value(Type) \
  return ((((LgInt)((Type)(Val1)) + (LgInt)((Type)(Val2)) + (LgInt)((Type)(Val3))) * 341L) >> 10)

#define Mid3ValueUns(Val1, Val2, Val3) \
	(Uint16)(0.33333333 * (Val1 + Val2 + Val3))

#define Max3Value(Type) \
  Type Max = 0; \
  Max = MAX_VAL(Max, Val1); \
  Max = MAX_VAL(Max, Val2); \
  Max = MAX_VAL(Max, Val3); \
  return Max

#if defined(__IAR_SYSTEMS_ICC__)

#ifndef F_SYSCLK
#define F_SYSCLK		8e6
#endif

#define DelayUs(A)  __delay_cycles((Extended)(F_SYSCLK / 1e6) * (Extended)(A))

#else
//extern void DelayUs(Uns Delay);
#endif

//extern LgInt SAT_VAL(LgInt Val, LgInt Min, LgInt Max);
Bool CheckRange(Uns Data, Uns Min, Uns Max);
extern Uns Min3UnsValue(Uns Val1, Uns Val2, Uns Val3);
extern Int Min3IntValue(Int Val1, Int Val2, Int Val3);
extern Uns Mid3UnsValue(Uns Val1, Uns Val2, Uns Val3);
extern Int Mid3IntValue(Int Val1, Int Val2, Int Val3);
extern Uns Max3UnsValue(Uns Val1, Uns Val2, Uns Val3);
extern LgUns Max3LgUnsValue(LgUns Val1, LgUns Val2, LgUns Val3);
extern Int Max3IntValue(Int Val1, Int Val2, Int Val3);
extern Uns SkewCalc(Uns, Uns, Uns, Uns);

__inline Int Sign(Int Value) {return ((Value < 0) ? -1 : 1);}

//extern Uns LIntersection(Uns P1_1, Uns P1_2, Uns P2_1, Uns P2_2);

// Переменные-разделители
/*extern Char FloatSeparator;
extern Char FloatPositive;
extern Char TimeSeparator;
extern Char DateSeparator;*/

/*union _TTimeVar;
union _TDateVar;*/

// Прототипы функции
//LgInt SAT_VAL(LgInt, LgInt, LgInt);
/*Byte DecToStr(Uns Value, String Str, Byte Prec, Byte RateMax, Bool Clear, Bool Sign);
void DecToStr4s(Uns Value, String Str);
Byte BinToStr(Uns Value, String Str, Byte RateMax, Bool BinChar);
void BinToStr4s(Uns Value, String Str, Byte Bit);
Byte HexToStr(Uns Value, String Str, Byte RateMax, Bool HexChar);
void HexToStr4s(Uns Value, String Str);
Byte TimeToStr(union _TTimeVar *Time, String Str);
void TimeToStr4s(union _TTimeVar *Time, String Str);
Byte DateToStr(union _TDateVar *Date, String Str);
void DateToStr4s(union _TDateVar *Date, String Str, Bool Flag);
Uns  StrToDec(String, Byte);
Uns  StrToBin(String, Byte);
Uns  StrToHex(String, Byte);
Uns  StrToTime(String);
Uns  StrToDate(String);*/



#ifdef __cplusplus
}
#endif // extern "C"

#endif

