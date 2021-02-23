
#include "Fdsp.h"

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

const static char* stdr[4] = {" ночь ", " yтро ", " день ", " вечер "};

 char* FD::td_name_r(uint8_t tod ) // Время суток по русски
{
   char *td = stdr[tod];
  return td;
}

const static char* swnr[8] = {" северный ", " северо-восточный ", " восточный ", " юго-восточный ",
						" южный ", " юго-западный ", " западный ", " северо-западный "};


 char* FD::wind_dir_r(uint16_t wind_dir) // Направление ветра по русски
{
   char *wn = swnr[wind_dir];
  return wn;
}

const static char* sprcr[6] = {"дождь ", "ливень ", "снегопад ", "сильный снегопад ", "гроза ", "без осадков "};
	
 char* FD::prc_type_r(uint8_t prec, bool sp, bool rp) // Тип осадков по русски
{
   char prc[] 
  
  prc = sprcr[5];
  
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

const static char* sdnr[7] = {" воскресенье ", " понедельник ", " вторник ", " среда ", 
" четверг ", " пятница ", " суббота "};
	
 char*  FD::dow_name_r(uint8_t wd) // Название Дня недели по русски
{
  char* sDOW = sdnr[wd - 1];
  return sDOW;
}

const static char* smnr[12] = {" января ", " февраля ", " марта ", " апреля ", " мая ", " июня ",
" июля ", " августа ", " сентября ", " октября ", " ноября ", " декабря "};
				
char* FD::month_name_r(uint8_t mn) // Название Месяца по русски
{
  char* sMON = smnr[mn - 1];
  return sMON;
}

//-------------------------------------------------------------- Форматирование в одно знакоместо
String FD::render_number(int number)
{
  char msg[2];
  snprintf(msg, 1, "%1u", number);
  return String(msg);
}

//-------------------------------------------------------------- Форматирование в  два знакоместа
String FD::render_00number(int number)
{
  char msg[2];
  snprintf(msg, 2, "%02u", number);
  return String(msg);
}

//------------------------------------------------------------------- Формирование строки с текущим временем для LCD
String FD::form_time_string (uint8_t hr, uint8_t mn, uint8_t sec, uint8_t a_hour, uint8_t a_min, bool alm, bool rus)
{
  char msg[16];
  snprintf(msg, 16, " %2u:%02u:%02u  -:-  ",       hr, mn, sec);
  if (alm)
  {
    snprintf(msg, 16, " %2u:%02u:%02u %2u:%02u",     hr, mn, sec, a_hour, a_min);
	if (rus) snprintf(msg, 16, " %2u:%02u:%02u %2u:%02u\355", hr, mn, sec, a_hour, a_min);
  }

  return String(msg);
}

//-------------------------------------------------------------- Формирование строки IP адреса
String FD::IP_to_str(byte ad0, byte ad1, byte ad2, byte ad3)
{
  char msg[15];
  snprintf(msg, 15, "%d.%d.%d.%d", ad0, ad1, ad2, ad3);
  return String (msg);
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
