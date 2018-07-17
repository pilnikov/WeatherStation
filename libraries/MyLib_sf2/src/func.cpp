#include "Sysf2.h"

const byte utf_recode[] PROGMEM =
{ 0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4, 0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f,
  0xa8, 0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab, 0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
  0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f,
  0xbe, 0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0, 0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

byte SF::roll_seg (byte in) //переворот сегмента
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
  return b;
}

byte SF::mir_seg (byte in) //адаптация для мах7219
/*
  для мах последовательность бит в байте для 7ми сегментника обратная,
  т.е если для тм и нт 0 бит -> сегмент А .. 7 бит -> сегмент G
  то           для мах 0 бит -> сегмент G .. 7 бит -> сегмент А
*/
{
  byte b  = (in & B00000001) << 6; //биты c нулевого по второй попарно меняются местами с четвертого по шестой
       b |= (in & B00000010) << 4;
       b |= (in & B00000100) << 2;
       b |=  in & B10001000;       //3 и 7 биты остаются неизменными
       b |= (in & B00010000) >> 2;
       b |= (in & B00100000) >> 4;
       b |= (in & B01000000) >> 6;
  return b;
}


String SF::lcd_rus(String source)
{
 String target;

  uint16_t i = 0;
  uint16_t k = source.length();

  unsigned char n = 0x0;
  char m;
  
  while (i < k) 
  {
    n = source[i]; i++;

    if (n == 0xD0 || n == 0xD1)   // UTF-8 handling
	{
      switch (n) 
	  {
        // Позиция символа в массиве utf_recode вычисляется по формуле pos = код символа - 0х90 для символов от А до п, с кодами от 0x90 по 0xBF,
		// с позициями с нулевой по сорок седьмую (0x2F в HEX)
        // Для символов от р до я, с кодами от 0x80 по 0x8F, с позициями с сорок восьмой (0x30 в HEX) по шестьдесят третью (0x3F в HEX)
        // - формула pos = (код символа - 0х80) + 0x30 = код символа - 0х50
		// ё и Ё стоят особняком (выбиваются из общего строя) поэтому для них отдельные строки.

        case 0xD0: 
            n = source[i]; i++;
            if (n == 0x81) m = 0xA2; // Ё
			else if (n >= 0x90 && n <= 0xBF) m = pgm_read_byte_near(utf_recode + n - 0x90); // от А до п
            break;
        case 0xD1: 
            n = source[i]; i++;
            if (n == 0x91) m = 0xB5; // ё
            else if (n >= 0x80 && n <= 0x8F) m = pgm_read_byte_near(utf_recode + n - 0x50); // от р до я
            break;
      }
    }
    else m = n;
    
	target += m;
  }
  return target;
}

/* Recode russian fonts from UTF-8 to Windows-1251 */
String SF::utf8rus(String source)
{
  String target;

  uint16_t i = 0;
  uint16_t k = source.length();

  unsigned char n = 0x0;

  while (i < k) 
  {
    n = source[i]; i++;

    if (n == 0xD0 || n == 0xD1)   // UTF-8 handling
	{
      switch (n) 
	  {
        case 0xD0: 
            n = source[i]; i++;
            if (n == 0x81) n = 0xA8; // Ё
            else if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
            break;
        case 0xD1: 
            n = source[i]; i++;
            if (n == 0x91) n = 0xB8; // ё
            else if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
            break;
      }
    }

    target += (char)n;
  }
  return target;
}

char* SF::utf8rus(char* source)
{
  char* target;
  
  uint16_t i = 0;
  uint16_t k = strlen(source);

  unsigned char n = 0x0;
 
  while (i < k) 
  {
    n = source[i]; i++;

    if (n == 0xD0 || n == 0xD1)   // UTF-8 handling
	{
      switch (n) 
	  {
        case 0xD0: 
            n = source[i]; i++;
            if (n == 0x81) n = 0xA8; // Ё
            else if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
            break;
        case 0xD1: 
            n = source[i]; i++;
            if (n == 0x91) n = 0xB8; // ё
            else if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
            break;
      }
    }
    target += (char)n;
  }
  return target;
}

String SF::formatBytes(size_t bytes) 
{
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}
