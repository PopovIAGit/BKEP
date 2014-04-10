/*======================================================================
Имя файла:          add_ValueConv.c
Автор:
Версия файла:
Дата изменения:
Применяемость:      
======================================================================*/

#include "std.h"
#include "add_ValueConv.h"

Char FloatSeparator = '.';
Char FloatPositive  = ' ';
Char TimeSeparator  = ':';
Char DateSeparator  = '/';
//--------------------------------------------------------
// Ограничение значения
Int SAT_VAL_Int(Int Val, Int Min, Int Max) {SAT_VAL(Val,Min,Max);}
LgUns SAT_VAL_LgInt(LgUns Val, LgUns Min, LgUns Max) {SAT_VAL(Val,Min,Max);}

//--------------------------------------------------------
/*// Ограничение значения
LgInt SAT_VAL(LgInt Val, LgInt Min, LgInt Max)
{
	if (Val <= Min) return Min;
	else if (Val >= Max) return Max;
	return Val;
}*/
//--------------------------------------------------------
// Попадает ли значение в диапазон
Bool CheckRange(Uns Data, Uns Min, Uns Max)
{
	if (Max > Min) {if ((Data < Min) || (Data > Max)) return false;}
	else if (((Int)Data < (Int)Min) || ((Int)Data > (Int)Max)) return false;
	return true;
}
//--------------------------------------------------------
// Минимум
Uns Min3UnsValue(Uns Val1, Uns Val2, Uns Val3) {Min3Value(Uns);}
Int Min3IntValue(Int Val1, Int Val2, Int Val3) {Min3Value(Int);}
//--------------------------------------------------------
// Среднее
Uns Mid3UnsValue(Uns Val1, Uns Val2, Uns Val3) {Mid3Value(Uns);}
Int Mid3IntValue(Int Val1, Int Val2, Int Val3) {Mid3Value(Int);}
//--------------------------------------------------------
// Максимум
Uns Max3UnsValue(Uns Val1, Uns Val2, Uns Val3) {Max3Value(Uns);}
LgUns Max3LgUnsValue(LgUns Val1, LgUns Val2, LgUns Val3) {Max3Value(LgUns);}
Int Max3IntValue(Int Val1, Int Val2, Int Val3) {Max3Value(Int);}

Uns LIntersection(Uns P1_1, Uns P1_2, Uns P2_1, Uns P2_2)
{
	if (P2_1 > P1_1) P1_1 = P2_1;
	if (P2_2 < P1_2) P1_2 = P2_2;
	return (P1_2 - P1_1 + 1);
}
//--------------------------------------------------------
// Число в строку
Byte DecToStr(Uns Value, String Str, Byte Prec, Byte RateMax, Bool Clear, Bool Sign)
{
	register Byte i = 0;
	register Bool Flag = false;
	register Uns  Tmp;
	
	if (Sign)
	{
		if ((Int)Value < 0) Value = -Value;
		else Sign = false;
	}
	
	if (!Prec) Prec = 0xFF;
	else {RateMax++; Flag = true;}
	
	do
	{
		if (i == Prec)
		{
			*Str-- = FloatSeparator;
			Flag = false;
		}
		else
		{
			Tmp    = Value / 10;
			*Str-- = (Value - (Uns)Tmp * 10) | 0x30;
			Value  = Tmp;
			if (!Value && Clear && !Flag) break;
		}
	}
	while (++i <= RateMax);
	
	*Str = Sign ? '-' : FloatPositive;
  
	return(i + 1);
}
//--------------------------------------------------------
// Число в строку 4-х значную
void DecToStr4s(Uns Value, String Str)
{
	register Uns Tmp1 = Value, Tmp2;
	
	Tmp2 = Tmp1 / 10; Str[3] = (Char)(Tmp1 - Tmp2 * 10) | 0x30;
	Tmp1 = Tmp2 / 10; Str[2] = (Char)(Tmp2 - Tmp1 * 10) | 0x30;
	Tmp2 = Tmp1 / 10; Str[1] = (Char)(Tmp1 - Tmp2 * 10) | 0x30;
	Tmp1 = Tmp2 / 10; Str[0] = (Char)(Tmp2 - Tmp1 * 10) | 0x30;
}
//--------------------------------------------------------
// Байт в строку
__inline void ByteToStr(Byte Value, String Str)
{
	register Byte Tmp = Value / 10;
	Str[0] = Tmp | 0x30;
	Str[1] = (Value - Tmp * 10) | 0x30;
}
//--------------------------------------------------------
// Двоичное число в строку
Byte BinToStr(Uns Value, String Str, Byte RateMax, Bool BinChar)
{
  register Byte i;
	
  Str = Str + RateMax;
	if (BinChar) {*(Str+1) = ' '; *(Str+2) = 'b';}
	
  for (i=RateMax; i != 0; i--)
  {
     *Str-- = (Byte)(Value & 0x1) | 0x30;
     Value = Value >> 1;
  }

  return(RateMax + 1 + (BinChar ? 2 : 0));
}
//--------------------------------------------------------
// Двоичное число в строку 4-х значную
void BinToStr4s(Uns Value, String Str, Byte Bit)
{
	ByteToStr(Bit, &Str[1]);

  Str[0] = 'B';
  Str[3] = (Value & (1 << Bit)) ? 0x31 : 0x30;
}
//--------------------------------------------------------
// Шестнадцатеричное число в строку
Byte HexToStr(Uns Value, String Str, Byte RateMax, Bool HexChar)
{
  register Byte i, Tmp;
	
  Str = Str + RateMax;
  if (HexChar) {*(Str+1) = ' '; *(Str+2) = 'h';}
	
  for (i=RateMax; i != 0; i--)
  {
     Tmp = (Byte)(Value & 0xF);
     if (Tmp > 0x9) Tmp = (Tmp - 0x09) | 0x40;
     else Tmp = Tmp | 0x30;
     *Str-- = Tmp;
     Value = Value >> 4;
  }

  return(RateMax + 1 + (HexChar ? 2 : 0));
}
//--------------------------------------------------------
// Шестнадцатеричное число в строку 4-х значную
void HexToStr4s(Uns Value, String Str)
{
  register Byte Tmp1;
  register Uns  Tmp2 = Value;
	
	Tmp1 = Tmp2 & 0xF; Str[3] = (Tmp1 > 0x9) ? (Tmp1 + 0x37) : (Tmp1 | 0x30); Tmp2 = Tmp2 >> 4;
	Tmp1 = Tmp2 & 0xF; Str[2] = (Tmp1 > 0x9) ? (Tmp1 + 0x37) : (Tmp1 | 0x30); Tmp2 = Tmp2 >> 4;
	Tmp1 = Tmp2 & 0xF; Str[1] = (Tmp1 > 0x9) ? (Tmp1 + 0x37) : (Tmp1 | 0x30); Tmp2 = Tmp2 >> 4;
	Tmp1 = Tmp2 & 0xF; Str[0] = (Tmp1 > 0x9) ? (Tmp1 + 0x37) : (Tmp1 | 0x30);
}
//--------------------------------------------------------
// Время в строку
Byte TimeToStr(TTimeVar *Time, String Str)
{
	ByteToStr(Time->bit.Hour, &Str[0]);
	ByteToStr(Time->bit.Min,  &Str[3]);
	
	Str[2] = TimeSeparator;
  
  return(5);
}
//--------------------------------------------------------
// Время в строку 4-х значную
void TimeToStr4s(TTimeVar *Time, String Str)
{
	ByteToStr(Time->bit.Hour, &Str[0]);
	ByteToStr(Time->bit.Min,  &Str[2]);
}
//--------------------------------------------------------
// Дату в строку
Byte DateToStr(TDateVar *Date, String Str)
{
	ByteToStr(Date->bit.Day,   &Str[0]);
	ByteToStr(Date->bit.Month, &Str[3]);
	ByteToStr(Date->bit.Year,  &Str[6]);
	
	Str[2] = DateSeparator;
	Str[5] = DateSeparator;
 
  return(8);
}
//--------------------------------------------------------
// Дату в строку 4-х значную
void DateToStr4s(TDateVar *Date, String Str, Bool Flag)
{
	if (Flag)
	{
		ByteToStr(Date->bit.Day,   &Str[0]);
		ByteToStr(Date->bit.Month, &Str[2]);
	}
	else
	{
		ByteToStr(Date->bit.Month, &Str[0]);
		ByteToStr(Date->bit.Year,  &Str[2]);
	}
}
//--------------------------------------------------------
// Строку в число
Uns StrToDec(String Str, Byte RateMax)
{
	register LgUns Value = 0;
	
	do	{
		if (*Str == FloatSeparator) Str++;
		Value = Value * 10;
		Value = Value + (*Str++ & 0xF);
	}
	while (RateMax--);
	if (Value > 0xFFFF) Value = 0xFFFF;
  
	return (Uns)Value;
}
//--------------------------------------------------------
// Строку в двоичное число
Uns StrToBin(String Str, Byte RateMax)
{
	register Uns Value = 0;

	do	{
		Value = Value << 1;
		Value = Value + (*Str++ & 0x1);
	}
	while (RateMax--);
	
	return Value;
}
//--------------------------------------------------------
// Строку в шестнадцатеричное число
Uns StrToHex(String Str, Byte RateMax)
{
  register Uns  Value = 0;
  register Char Data;

	do	{
		Value = Value << 4;
		Data = *Str++;
		if (Data > 0x39) Data = (Data - 0x37);
		else Data = Data & 0xF;
		Value = Value + (Data & 0xF);
	}
	while (RateMax--);
	
	return Value;
}
//--------------------------------------------------------
// Строку во время
Uns StrToTime(String Str)
{
	TTimeVar Time;
	
	Time.bit.Hour = ((Uns)(Str[0] & 0xF) * 10) + (Uns)(Str[1] & 0xF);
	Time.bit.Min  = ((Uns)(Str[3] & 0xF) * 10) + (Uns)(Str[4] & 0xF);
	
	return Time.all;
}
//--------------------------------------------------------
// Строку в дату
Uns StrToDate(String Str)
{
	TDateVar Date;
	
	Date.bit.Day   = ((Uns)(Str[0] & 0xF) * 10) + (Uns)(Str[1] & 0xF);
	Date.bit.Month = ((Uns)(Str[3] & 0xF) * 10) + (Uns)(Str[4] & 0xF);
	Date.bit.Year  = ((Uns)(Str[6] & 0xF) * 10) + (Uns)(Str[7] & 0xF);
	
	return Date.all;
}
//--------------------------------------------------------



