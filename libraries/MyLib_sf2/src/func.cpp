#include "Sysf2.h"

const unsigned char utf_recode[] PROGMEM =
{ 0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4, 0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f,
  0xa8, 0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab, 0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
  0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f,
  0xbe, 0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0, 0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

String SF::Serial_Read() {
  unsigned char c; // переменная для чтения сериал порта
  String Serial_string = ""; // Формируемая из символов строка
  while (DBG_OUT_PORT.available() > 0) { // Если в сериал порту есть символы
    c = DBG_OUT_PORT.read(); // Читаем символ
    //DBG_OUT_PORT.print(c,HEX); DBG_OUT_PORT.print(" "); DBG_OUT_PORT.print(c);
    if (c == '\n') {  // Если это конец строки
      return Serial_string; // Возвращаем строку
    }
    Serial_string += String(char(c)); //Добавить символ в строку
  }
  return Serial_string;
}

byte SF::roll_seg (byte in) //переворот сегмента
/*      a              d        a b c
  -              -     заменить на
  f| g |b        c| g |e     d e f
  -      -->     -
  e|   |c        b|   |f
  _              _
  d              a
*/

{
  byte b = (in & B00000111) << 3;
  b |= (in & B00111000) >> 3;
  b |= in & B11000000;
  return b;
}

byte SF::mir_seg (byte in) //адаптация для мах7219
/*
  для мах последовательность бит в байте для 7ми сегментника обратная
  т.е если для тм и нт 0 байт -> сегмент А .. 7 байт -> сегмент G
  то           для мах 0 байт -> сегмент G .. 7 байт -> сегмент А
*/
{
  byte b  = (in & B00000001) << 6; //биты c нулевого по второй попарно меняются местами с четвертого по шестой
  b |= (in & B00000010) << 4;
  b |= (in & B00000100) << 2;
  b |=  in & B10001000;      //3 и 7 биты остаются неизменными
  b |= (in & B00010000) >> 2;
  b |= (in & B00100000) >> 4;
  b |= (in & B01000000) >> 6;
  return b;
}


String SF::lcd_rus(String source)
{
  uint16_t i, k;
  String target = String();
  unsigned char n = 0x0;
  char m = 0x0;
  uint8_t utf_hi_char;

  k = source.length();
  i = 0;

  while (i < k)
  {
    n = source[i];

    if (n >= 0x80 && n <= 0xD1)   // UTF-8 handling
    {
      if (n >= 0xc0) utf_hi_char = n - 0xd0;
      else
      {
        n &= 0x3f;
        if (!utf_hi_char && (n == 1)) m = (0xa2); // ё
        else if ((utf_hi_char == 1) && (n == 0x11)) m = (0xb5); // Ё
        else m = pgm_read_byte_near(utf_recode + n + (utf_hi_char << 6) - 0x10);
      }
    }
    else m = n;

    if (n != 0xD0 && n != 0xD1) target += m;
    
    i++;
  }
  return target;
}

/* Recode russian fonts from UTF-8 to Windows-1251 */
String SF::utf8rus(String source)
{
  uint16_t i, k;
  String target;
  unsigned char n = 0x0;
  char m = 0x0;

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
            n = source[i]; i++;
            if (n == 0x81) {
              n = 0xA8;
              break;
            }
            if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
            break;
          }
        case 0xD1: {
            n = source[i]; i++;
            if (n == 0x91) {
              n = 0xB8;
              break;
            }
            if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
            break;
          }
      }
    }
    m = n; 
    target += m;
  }
  return target;
}

char* SF::utf8rus(char* source)
{
  uint16_t i, k;
  char* target;
  unsigned char n = 0x0;
  char m = 0x0;

  k = strlen(source); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
            n = source[i]; i++;
            if (n == 0x81) {
              n = 0xA8;
              break;
            }
            if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
            break;
          }
        case 0xD1: {
            n = source[i]; i++;
            if (n == 0x91) {
              n = 0xB8;
              break;
            }
            if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
            break;
          }
      }
    }
    m = n; 
    target += m;
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
