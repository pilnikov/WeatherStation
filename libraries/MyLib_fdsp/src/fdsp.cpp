
#include "Fdsp.h"

byte Segments[162] PROGMEM = {
  //GFEDCBA  Segments      7-segment map:
  0b0000000, // 32  ' '  NO DISPLAY       AAA
  0b0110000, // 33  '!'                  F   B
  0b0100010, // 34  '"'                  F   B
  0b0000000, // 35  '#'  NO DISPLAY       GGG
  0b0000000, // 36  '$'  NO DISPLAY      E   C
  0b0000000, // 37  '%'  NO DISPLAY      E   C
  0b0000000, // 38  '&'  NO DISPLAY       DDD
  0b0000010, // 39  '''
  0b0111001, // 40  '('
  0b0001111, // 41  ')'
  0b0000000, // 42  '*'  NO DISPLAY
  0b0000000, // 43  '+'  NO DISPLAY
  0b0010000, // 44  ','
  0b1000000, // 45  '-'
  0b10000000, //46  '.'  
  0b0000000, // 47  '/'  NO DISPLAY
  0b0111111, // 48  "0"
  0b0000110, // 48  "1"
  0b1011011, // 50  "2"
  0b1001111, // 51  "3"
  0b1100110, // 52  "4"
  0b1101101, // 53  "5"
  0b1111101, // 54  "6"
  0b0000111, // 55  "7"
  0b1111111, // 56  "8"
  0b1101111, // 57  "9"
  0b0000000, // 58  ':'  NO DISPLAY
  0b0000000, // 59  ';'  NO DISPLAY
  0b1000110, // 60  '<'  
  0b0000000, // 61  '='  NO DISPLAY
  0b1110000, // 62  '>'  
  0b0000000, // 63  '?'  NO DISPLAY
  0b0000000, // 64  '@'  NO DISPLAY
  0b1110111, // 65  "A"
  0b1111100, // 66  "b"
  0b0111001, // 67  "C"
  0b1011110, // 68  "d"
  0b1111001, // 69  "E"
  0b1110001, // 70  "F"
  0b1101111, // 71  'G'
  0b1110110, // 72  'H'
  0b0000110, // 73  'I'
  0b0011110, // 74  'J'
  0b1110110, // 75  'K'  NO DISPLAY
  0b0111000, // 76  'L'
  0b0010101, // 77  'M'  NO DISPLAY
  0b1010100, // 78  'n'
  0b0111111, // 79  'O'
  0b1110011, // 80  'P'
  0b1100111, // 81  'q'
  0b1010000, // 82  'r'
  0b1101101, // 83  'S'
  0b1111000, // 84  't'
  0b0111110, // 85  'U'
  0b0011100, // 86  'V'  NO DISPLAY
  0b0101010, // 87  'W'  NO DISPLAY
  0b1110110, // 88  'X'  NO DISPLAY
  0b1101110, // 89  'y'
  0b1011011, // 90  'Z'  
  0b0111001, // 91  '['
  0b0000000, // 92  '\'  NO DISPLAY
  0b0001111, // 93  ']'
  0b0000000, // 94  '^'  NO DISPLAY
  0b0001000, // 95  '_'
  0b0000010, // 96  '`'
  0b1110111, // 97  'a' SAME AS CAP
  0b1111100, // 98  'b' SAME AS CAP
  0b1011000, // 99  'c'
  0b1011110, // 100 'd' SAME AS CAP
  0b1111011, // 101 'e'
  0b1110001, // 102 'F' SAME AS CAP
  0b0111101, // 103 'G' SAME AS CAP
  0b1110100, // 104 'h'
  0b0000100, // 105 'i'
  0b0011110, // 106 'j' SAME AS CAP
  0b0000000, // 107 'k'  NO DISPLAY
  0b0000110, // 108 'l'
  0b0000000, // 109 'm'  NO DISPLAY
  0b1010100, // 110 'n' SAME AS CAP
  0b1011100, // 111 'o'
  0b1110011, // 112 'p' SAME AS CAP
  0b1100111, // 113 'q' SAME AS CAP
  0b1010000, // 114 'r' SAME AS CAP
  0b1101101, // 115 'S' SAME AS CAP
  0b1111000, // 116 't' SAME AS CAP
  0b0011100, // 117 'u'
  0b0000000, // 118 'b'  NO DISPLAY
  0b0000000, // 119 'w'  NO DISPLAY
  0b0000000, // 120 'x'  NO DISPLAY
  0b0000000, // 121 'y'  NO DISPLAY
  0b0000000, // 122 'z'  NO DISPLAY
  0b0000000, // 123 ''   NO DISPLAY
  0b1100011, // 124 GRADUS
  0b1111001, // 125 'Ё'
  0b1111011, // 126 'ё'  
  0b0000000, // 127 ''   NO DISPLAY
  0b1110111, // 128 "A"
  0b1111101, // 129 "Б"
  0b1111111, // 130 "В"
  0b0110001, // 131 "Г"
  0b1011111, // 132 "Д"
  0b1111001, // 133 "Е"
  0b0110110, // 134 'Ж'
  0b1001111, // 135 '3'
  0b0000000, // 136 'И
  0b0000000, // 137 'Й'
  0b0000000, // 138 'К'  NO DISPLAY
  0b0000000, // 139 'Л'
  0b0000000, // 140 'M'  NO DISPLAY
  0b1110110, // 141 'Н'
  0b0111111, // 142 'O'
  0b0110111, // 143 'П'
  0b1110011, // 144 'Р'
  0b0111001, // 145 'С'
  0b1101101, // 146 'Т'
  0b1101110, // 147 'У'
  0b0000000, // 148 'Ф'
  0b0000000, // 149 'Х'  NO DISPLAY
  0b0111110, // 150 'Ц'  NO DISPLAY
  0b1100110, // 151 'Ч'  NO DISPLAY
  0b0000000, // 152 'Ш'
  0b0000000, // 153 'Щ'  NO DISPLAY
  0b1111100, // 154 'Ь'
  0b1111100, // 155 'Ы'  NO DISPLAY
  0b1111100, // 156 'Ъ'
  0b1001111, // 157 'Э'  NO DISPLAY
  0b0000000, // 158 'Ю'
  0b1110111, // 159 'Я'
  0b1110111, // 160 'a' SAME AS CAP
  0b1111101, // 161 'б' SAME AS CAP
  0b1111111, // 162 'в'
  0b1010000, // 163 'г' SAME AS CAP
  0b0000000, // 164 'д'
  0b1111011, // 165 'е' SAME AS CAP
  0b0000000, // 166 'ж' SAME AS CAP
  0b1001111, // 167 'з'
  0b0000000, // 168 'и'
  0b0000000, // 169 'й' SAME AS CAP
  0b0000000, // 170 'к'  NO DISPLAY
  0b0000000, // 171 'л'
  0b0000000, // 172 'м'  NO DISPLAY
  0b0110110, // 173 'н' SAME AS CAP
  0b1011100, // 174 'o'
  0b1010100, // 175 'п' SAME AS CAP
  0b1110011, // 176 'р' SAME AS CAP
  0b1011000, // 177 'с' SAME AS CAP
  0b0000000, // 178 'т' SAME AS CAP
  0b1101110, // 179 'у' SAME AS CAP
  0b0000000, // 180 'ф'
  0b0000000, // 181 'х'  NO DISPLAY
  0b0011100, // 182 'ц'  NO DISPLAY
  0b1100110, // 183 'ч'  NO DISPLAY
  0b0000000, // 184 'ш'  NO DISPLAY
  0b0000000, // 185 'щ'  NO DISPLAY
  0b1111100, // 186 'ь'
  0b0000000, // 187 'ы'  NO DISPLAY
  0b0000000, // 188 'ъ'  NO DISPLAY
  0b0000000, // 189 'э'  NO DISPLAY
  0b0000000, // 190 'ю'  NO DISPLAY
  0b0000000  // 191 'я'  NO DISPLAY
};

byte utf_recode[64] PROGMEM =
{ 0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4, 0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f,
  0xa8, 0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab, 0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
  0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f,
  0xbe, 0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0, 0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

void FD::roll_seg (byte &in) //переворот сегмента
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

void FD::mir_seg (byte &in) //адаптация для мах7219
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
  char m;
  
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
            if (n == 0x81) m = 0xA2; // Ё
			else if (n >= 0x90 && n <= 0xBF) m = utf_recode[n - 0x90]; // от А до п
   		break;
        case 0xD1: 
			i++;
            n = source[i];
            if (n == 0x91) m = 0xB5; // ё
            else if (n >= 0x80 && n <= 0x8F) m = utf_recode[n - 0x50]; // от р до я
		break;
      }
 	}
    else m = (char)n;
    
	target += m;
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

const String stdr[4] = {" ночь ", " yтро ", " день ", " вечер "};

String FD::td_name_r(uint8_t tod ) // Время суток по русски
{
  String td = stdr[tod];
  return td;
}

const String swnr[8] = {" северный ", " северо-восточный ", " восточный ", " юго-восточный ",
						" южный ", " юго-западный ", " западный ", " северо-западный "};


String FD::wind_dir_r(uint16_t wind_dir) // Направление ветра по русски
{
  String wn = swnr[wind_dir];
  return wn;
}

const String sprcr[6] = {"дождь ", "ливень ", "снегопад ", "сильный снегопад ", "гроза ", "без осадков "};
	
String FD::prc_type_r(uint8_t prec, bool sp, bool rp) // Тип осадков по русски
{
  String prc =  sprcr[5];
  if (prec > 3 && prec < 8) 
	{
		prc = (!rp ? "возможен " : "");
		prc += sprcr[prec - 4];
	} 
  if (prec == 8) 
	{
		prc = (!sp ? "возможна " : "");
        prc += sprcr[prec - 4];
	}

  return prc;
}

const String sdnr[7] = {" воскресенье ", " понедельник ", " вторник ", " среда ", 
" четверг ", " пятница ", " суббота "};
	
String FD::dow_name_r(uint8_t wd) // Название Дня недели по русски
{
  String sDOW = sdnr[wd - 1];
  return sDOW;
}

const String smnr[12] = {" января ", " февраля ", " марта ", " апреля ", " мая ", " июня ",
" июля ", " августа ", " сентября ", " октября ", " ноября ", " декабря "};
				
String FD::month_name_r(uint8_t mn) // Название Месяца по русски
{
  String sMON = smnr[mn - 1];
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

//------------------------------------------------------------------- Формирование строки с текущим временем для LCD
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

void FD::prn7(String &source)
{
/* 
 * Позиция символа в массиве Segments вычисляется по формуле: 
 * Для символов от А до п, с кодами от 0x90 по 0xBF, с позициями от сто двадцать восьмой до сто семьдесят пятой (0x80 по 0xAF)
 * pos = код символа - 0х10
 * Для символов от р до я, с кодами от 0x80 по 0x8F, с позициями от сто семьдесят шестой до сто девяносто первой (0xB0 по 0xBF)
 * pos = код символа + 0х30
 * ё и Ё  и символ градуса стоят особняком (выбиваются из общего строя) поэтому для них отдельные строки.
 */ 

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
            else if (n >= 0x90 && n <= 0xBF) n -= 0x10;
            break;
        case 0xD1: 
            i++;
			n = source[i]; 
            if (n == 0x91) n = 0xB8; // ё
            else if (n >= 0x80 && n <= 0x8F) n += 0x30;
            break;
      }
    }
    target += Segments[n - 32];
	i++;
  }
  source = target;
}
