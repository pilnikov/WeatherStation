
#include "Fdsp.h"
#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#elif defined(__xtensa__)
#include <pgmspace.h>
#endif


# if defined(__xtensa__)

byte utf_recode[64] PROGMEM =
{ 0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4, 0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f,
  0xa8, 0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab, 0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
  0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f,
  0xbe, 0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0, 0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

#endif

# if defined(__AVR_ATmega2560__)

const byte utf_recode[64] PROGMEM =
{ 0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4, 0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f,
  0xa8, 0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab, 0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
  0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f,
  0xbe, 0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0, 0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

#endif

void FD::roll_seg(byte &in) //переворот сегмента
/*      a              d        a b c
        -              -     заменить на
     f|   |b        c|   |e     d e f
        -      -->     -
     e| g |c        b| g |f
        _              _
        d              a
*/

{
  byte b  = (in & B00000111) << 3;
       b |= (in & B00111000) >> 3;
       b |=  in & B11000000;
  in = b;
}

void FD::mir_seg(byte &in) //адаптация для мах7219
/*
*  для мах последовательность бит в байте для 7ми сегментника обратная,
*  т.е если для тм и нт 0 бит -> сегмент А .. 6 бит -> сегмент G 
*  то           для мах 0 бит -> сегмент G .. 6 бит -> сегмент А 
* (это у него регистр внутренний так устроен)
*  бит 7 это DP (точка) и там и там.
*/
{
  byte b  = (in & B00000001) << 6; //биты c нулевого по второй попарно меняются местами с четвертого по шестой
       b |= (in & B00000010) << 4;
       b |= (in & B00000100) << 2;
       b |=  in & B10001000;       //3 и 7 биты остаются неизменными
       b |= (in & B00010000) >> 2;
       b |= (in & B00100000) >> 4;
       b |= (in & B01000000) >> 6;
  in = b;
}


void FD::lcd_rus(String &source)
{
 String target;

  uint16_t i = 0; // source symbol pos index
  uint16_t k = source.length(); // source length

  byte n = 0x0;
  char _m = ' ';
  
  while (i < k) 
  {
    n = source[i];

    if (n == 0xD0 || n == 0xD1)   // Cyrillic symbol detected
	{
      switch (n) 
	  {
        // Позиция символа в массиве utf_recode вычисляется по формуле pos = код символа - 0х90 для символов от А до п, с кодами от 0x90 по 0xBF, с позициями с нулевой по сорок седьмую (0x2F в HEX)
        // Для символов от р до я, с кодами от 0x80 по 0x8F, с позициями с сорок восьмой (0x30 в HEX) по шестьдесят третью (0x3F в HEX) pos = код символа - 0х50
		// ё (код 0xD1 91) и Ё (код 0xD0 81) стоят особняком (выбиваются из общего строя) поэтому для них отдельные строки.

        case 0xD0: 
            i++;
			n = source[i];
            if (n == 0x81) _m = 0xA2; // Ё
			else if (n >= 0x90 && n <= 0xBF) _m = utf_recode[n - 0x90]; // от А до п
   		break;
        case 0xD1: 
			i++;
            n = source[i];
            if (n == 0x91) _m = 0xB5; // ё
            else if (n >= 0x80 && n <= 0x8F) _m = utf_recode[n - 0x50]; // от р до я
		break;
      }
 	}
    else _m = (char)n;
    
	target += _m;
    i++;
  }
  source = target;
}

/* Recode russian fonts from UTF-8 to Windows-1251 */
void FD::utf8rus(String &source)
{
  String target;

  uint16_t i = 0;
  uint16_t k = source.length();

  byte n = 0x0;

  while (i < k) 
  {
    n = source[i];

    if (n == 0xD0 || n == 0xD1)   // Cyrillic symbol detected
	{
      switch (n) 
	  {
        case 0xD0: 
			i++;
            n = source[i];
            if (n == 0x81) n = 0xA8; // Ё
            else if (n >= 0x90 && n <= 0xBF) n += 0x30;
            break;
        case 0xD1: 
			i++;
			n = source[i];
            if (n == 0x91) n = 0xB8; // ё
            else if (n >= 0x80 && n <= 0x8F) n += 0x70;
            break;
      }
    }

    target += (char)n;
	i++;
  }
  source = target;
}

void FD::utf8rus(char *source)
{
  char * target;
  
  uint16_t i = 0;
  uint16_t k = strlen(source);

  byte n = 0x0;
 
  while (i < k) 
  {
    n = source[i];

    if (n == 0xD0 || n == 0xD1)   // Cyrillic symbol detected
	{
      switch (n) 
	  {
        case 0xD0: 
            i++;
			n = source[i];
            if (n == 0x81) n = 0xA8; // Ё
            else if (n >= 0x90 && n <= 0xBF) n += 0x30;
            break;
        case 0xD1: 
            i++;
			n = source[i]; 
            if (n == 0x91) n = 0xB8; // ё
            else if (n >= 0x80 && n <= 0x8F) n += 0x70;
            break;
      }
    }
    target += (char)n;
	i++;
  }
  source = target;
}
