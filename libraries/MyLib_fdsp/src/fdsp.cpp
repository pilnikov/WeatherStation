
#include "Fdsp.h"

const byte charArray[] PROGMEM = {
//  ABCDEFG  Segments      7-segment map:
  0b1111110, // 0   "0"          AAA
  0b0110000, // 1   "1"         F   B
  0b1101101, // 2   "2"         F   B
  0b1111001, // 3   "3"          GGG
  0b0110011, // 4   "4"         E   C
  0b1011011, // 5   "5"         E   C
  0b1011111, // 6   "6"          DDD
  0b1110000, // 7   "7"
  0b1111111, // 8   "8"
  0b1111011, // 9   "9"
  0b1110111, // 10  "A"
  0b0011111, // 11  "b"
  0b1001110, // 12  "C"
  0b0111101, // 13  "d"
  0b1001111, // 14  "E"
  0b1000111, // 15  "F"
  0b0000000, // 16  NO DISPLAY
  0b0000000, // 17  NO DISPLAY
  0b0000000, // 18  NO DISPLAY
  0b0000000, // 19  NO DISPLAY
  0b0000000, // 20  NO DISPLAY
  0b0000000, // 21  NO DISPLAY
  0b0000000, // 22  NO DISPLAY
  0b0000000, // 23  NO DISPLAY
  0b0000000, // 24  NO DISPLAY
  0b0000000, // 25  NO DISPLAY
  0b0000000, // 26  NO DISPLAY
  0b0000000, // 27  NO DISPLAY
  0b0000000, // 28  NO DISPLAY
  0b0000000, // 29  NO DISPLAY
  0b0000000, // 30  NO DISPLAY
  0b0000000, // 31  NO DISPLAY
  0b0000000, // 32  ' '
  0b0000000, // 33  '!'  NO DISPLAY
  0b0100010, // 34  '"'
  0b0000000, // 35  '#'  NO DISPLAY
  0b0000000, // 36  '$'  NO DISPLAY
  0b0000000, // 37  '%'  NO DISPLAY
  0b0000000, // 38  '&'  NO DISPLAY
  0b0100000, // 39  '''
  0b1001110, // 40  '('
  0b1111000, // 41  ')'
  0b0000000, // 42  '*'  NO DISPLAY
  0b0000000, // 43  '+'  NO DISPLAY
  0b0000100, // 44  ','
  0b0000001, // 45  '-'
  0b0000000, // 46  '.'  NO DISPLAY
  0b0000000, // 47  '/'  NO DISPLAY
  0b1111110, // 48  '0'
  0b0110000, // 49  '1'
  0b1101101, // 50  '2'
  0b1111001, // 51  '3'
  0b0110011, // 52  '4'
  0b1011011, // 53  '5'
  0b1011111, // 54  '6'
  0b1110000, // 55  '7'
  0b1111111, // 56  '8'
  0b1111011, // 57  '9'
  0b0000000, // 58  ':'  NO DISPLAY
  0b0000000, // 59  ';'  NO DISPLAY
  0b0000000, // 60  '<'  NO DISPLAY
  0b0000000, // 61  '='  NO DISPLAY
  0b0000000, // 62  '>'  NO DISPLAY
  0b0000000, // 63  '?'  NO DISPLAY
  0b0000000, // 64  '@'  NO DISPLAY
  0b1110111, // 65  'A'
  0b0011111, // 66  'b'
  0b1001110, // 67  'C'
  0b0111101, // 68  'd'
  0b1001111, // 69  'E'
  0b1000111, // 70  'F'
  0b1011110, // 71  'G'
  0b0110111, // 72  'H'
  0b0110000, // 73  'I'
  0b0111000, // 74  'J'
  0b0000000, // 75  'K'  NO DISPLAY
  0b0001110, // 76  'L'
  0b0000000, // 77  'M'  NO DISPLAY
  0b0010101, // 78  'n'
  0b1111110, // 79  'O'
  0b1100111, // 80  'P'
  0b1110011, // 81  'q'
  0b0000101, // 82  'r'
  0b1011011, // 83  'S'
  0b0001111, // 84  't'
  0b0111110, // 85  'U'
  0b0000000, // 86  'V'  NO DISPLAY
  0b0000000, // 87  'W'  NO DISPLAY
  0b0000000, // 88  'X'  NO DISPLAY
  0b0111011, // 89  'y'
  0b0000000, // 90  'Z'  NO DISPLAY
  0b1001110, // 91  '['
  0b0000000, // 92  '\'  NO DISPLAY
  0b1111000, // 93  ']'
  0b0000000, // 94  '^'  NO DISPLAY
  0b0001000, // 95  '_'
  0b0000010, // 96  '`'
  0b1110111, // 97  'a' SAME AS CAP
  0b0011111, // 98  'b' SAME AS CAP
  0b0001101, // 99  'c'
  0b0111101, // 100 'd' SAME AS CAP
  0b1101111, // 101 'e'
  0b1000111, // 102 'F' SAME AS CAP
  0b1011110, // 103 'G' SAME AS CAP
  0b0010111, // 104 'h'
  0b0010000, // 105 'i'
  0b0111000, // 106 'j' SAME AS CAP
  0b0000000, // 107 'k'  NO DISPLAY
  0b0110000, // 108 'l'
  0b0000000, // 109 'm'  NO DISPLAY
  0b0010101, // 110 'n' SAME AS CAP
  0b0011101, // 111 'o'
  0b1100111, // 112 'p' SAME AS CAP
  0b1110011, // 113 'q' SAME AS CAP
  0b0000101, // 114 'r' SAME AS CAP
  0b1011011, // 115 'S' SAME AS CAP
  0b0001111, // 116 't' SAME AS CAP
  0b0011100, // 117 'u'
  0b0000000, // 118 'b'  NO DISPLAY
  0b0000000, // 119 'w'  NO DISPLAY
  0b0000000, // 120 'x'  NO DISPLAY
  0b0000000, // 121 'y'  NO DISPLAY
  0b0000000, // 122 'z'  NO DISPLAY
  0b0000000, // 123 '0b'  NO DISPLAY
  0b0000000, // 124 '|'  NO DISPLAY
  0b0000000, // 125 ','  NO DISPLAY
  0b0000000, // 126 '~'  NO DISPLAY
  0b0000000, // 127 'DEL'  NO DISPLAY
};

const byte utf_recode[] PROGMEM =
{ 0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4, 0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f,
  0xa8, 0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab, 0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
  0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f,
  0xbe, 0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0, 0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

byte FD::roll_seg (byte in) //переворот сегмента
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

byte FD::mir_seg (byte in) //адаптация для мах7219
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


String FD::lcd_rus(String source)
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
String FD::utf8rus(String source)
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

char* FD::utf8rus(char* source)
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

String FD::td_name_r(uint8_t tod ) // Время суток по русски
{
  String td = String();
  switch (tod)
  {
    case 0:
      td = " ночь ";
      break;
    case 1:
      td = " yтро ";
      break;
    case 2:
      td = " день ";
      break;
    case 3:
      td = " вечер ";
      break;
  }
  return td;
}

String FD::wind_dir_r(uint16_t wind_dir) // Направление ветра по русски
{
  String wn = String();

  switch (wind_dir)
  {
    case 0:
      wn = " северный ";
      break;
    case 1:
      wn = " северо-восточный ";
      break;
    case 2:
      wn = " восточный ";
      break;
    case 3:
      wn = " юго-восточный ";
      break;
    case 4:
      wn = " южный ";
      break;
    case 5:
      wn = " юго-западный ";
      break;
    case 6:
      wn = " западный ";
      break;
    case 7:
      wn = " северо-западный ";
      break;
  }
  return wn;
}

String FD::prc_type_r(uint8_t prec, bool sp, bool rp) // Тип осадков по русски
{
  String prc = String();
  switch (prec)
  {
    case 4:
      prc = (!rp ? "возможен " : "");
      prc += "дождь ";
      break;
    case 5:
      prc = (!rp ? "возможен " : "");
      prc += "ливень ";
      break;
    case 6:
      prc = (!rp ? "возможен " : "");
      prc += "снегопад ";
      break;
    case 7:
      prc = (!rp ? "возможен " : "");
      prc += "сильный снегопад ";
      break;
    case 8:
      prc = (!sp ? "возможна " : "");
      prc += "гроза ";
      break;
    case 10:
      prc = "без осадков ";
      break;
  }
  return prc;
}

String FD::dow_name_r(uint8_t wd) // Название Дня недели по русски
{
  String sDOW = String();
  switch (wd)
  {
    case 2:
      sDOW = " понедельник ";
      break;
    case 3:
      sDOW = " вторник ";
      break;
    case 4:
      sDOW = " среда ";
      break;
    case 5:
      sDOW = " четверг ";
      break;
    case 6:
      sDOW = " пятница ";
      break;
    case 7:
      sDOW = " суббота ";
      break;
    case 1:
      sDOW = " воскресенье ";
      break;
  }
  return sDOW;
}

String FD::month_name_r(uint8_t mn) // Название Месяца по русски
{
  String sMON = String();
  switch (mn)
  {
    case 1:
      sMON = " января ";
      break;
    case 2:
      sMON = " февраля ";
      break;
    case 3:
      sMON = " марта ";
      break;
    case 4:
      sMON = " апреля ";
      break;
    case 5:
      sMON = " мая ";
      break;
    case 6:
      sMON = " июня ";
      break;
    case 7:
      sMON = " июля ";
      break;
    case 8:
      sMON = " августа ";
      break;
    case 9:
      sMON = " сентября ";
      break;
    case 10:
      sMON = " октября ";
      break;
    case 11:
      sMON = " ноября ";
      break;
    case 12:
      sMON = " декабря ";
      break;
  }
  return sMON;
}

//-------------------------------------------------------------- Форматирование в одно знакоместо
String FD::render_number(int number)
{
  char msg[2];
  sprintf(msg, "%1u", number);
  return msg;
}

//-------------------------------------------------------------- Форматирование в  два знакоместа
String FD::render_00number(int number)
{
  char msg[2];
  sprintf(msg, "%02u", number);
  return msg;
}

//-------------------------------------------------------------- Формирование строки с текущим временем для LCD
String FD::form_time_string (uint8_t hr, uint8_t mn, uint8_t sec, uint8_t a_hour, uint8_t a_min, bool alm, bool rus)
{
  char msg[20];
  if (alm)
  {
    if (rus) sprintf(msg, " %2u:%02u:%02u %2u:%02u\355", hr, mn, sec, a_hour, a_min);
    else     sprintf(msg, " %2u:%02u:%02u %2u:%02u",     hr, mn, sec, a_hour, a_min);
  }
  else       sprintf(msg, " %2u:%02u:%02u  -:-  ",       hr, mn, sec);

  return msg;
}

//-------------------------------------------------------------- Формирование строки IP адреса
String FD::IP_to_str (byte ad0, byte ad1, byte ad2, byte ad3)
{
  char msg[15];
  sprintf(msg, "%d.%d.%d.%d", ad0, ad1, ad2, ad3);
  return msg;
}
