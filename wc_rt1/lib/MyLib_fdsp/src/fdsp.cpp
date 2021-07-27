
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

#elif defined(__AVR_ATmega2560__) 
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

void FD::lcd_rus(char *source)
{
  uint16_t i = 0, j = 0; // source symbol pos index
  uint16_t k = strlen(source);// source length
  char target[k];

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
    
	target[j] = _m;
    i++; j++;
  }
  for (i = 0; i < j; i++) source[i] = target[i];
  for (i = j; i < k; i++) source[i] = 0;
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
   
  uint16_t i = 0, j = 0;
  uint16_t k = strlen(source);
 
  char target[k];

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
    target[j] = (char)n;
	i++; j++;
  }
  for (i = 0; i < j; i++) source[i] = target[i];
  for (i = j; i < k; i++) source[i] = 0;
}

bool FD::time_m32_8(byte *in, uint8_t pos, unsigned char *old, const uint8_t *dposx, bool *change, uint16_t *buff, const byte *font, bool pm, const uint8_t q_dig, rtc_data_t rt)
{
  //----------------------------------------------------------------- заполнение массива
  unsigned char d[q_dig];
  uint8_t font_wdt = 5;
  byte nbuf[256];

  uint8_t h = rt.hour;
  // Do 24 hour to 12 hour format conversion when required.
  if (pm & (h != 12)) h = h % 12;

  d[0] = ' ';
  if (h > 9) d[0] = h / 10 + '0';
  d[1] = h % 10 + '0';
  d[2] = rt.min / 10 + '0';
  d[3] = rt.min % 10 + '0';
  d[4] = rt.sec / 10 + '\x80';
  d[5] = rt.sec % 10 + '\x80';

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (i > 3) font_wdt = 3;
    change[i] = d[i] != old[i]; // проверка изменений в буфере отображаемых символов
    if (change[i])
    {
      FD::printCharacter(d[i], dposx[i], nbuf + pos, font, 5); // запись символа в вертушок для изменившихся позиций
      FD::shift_ud(true, true, nbuf + pos, in + pos,  buff + pos, dposx[i],  dposx[i] + font_wdt); // запись символа в вертушок для изменившихся позиций
    }
    else
    {
      FD::printCharacter(old[i], dposx[i], in + pos, font, 5); // отображение символов
    }
    old[i] = d[i]; // перезапись предыдущих значений в буфер
  }
  //DBG_OUT_PORT.println(F("time_m32_8");
  return true;
}

//-------------------------------------------------------------- Отображение бегущей строки
bool FD::scroll_String(int8_t x1, int8_t x2, char* in, uint8_t &icp, uint8_t &cbp, byte *out, const byte *font, uint8_t font_wdt, uint8_t spacer_wdt, uint8_t qbs)
{
  unsigned char character = 0; // дергаем входящую сроку по символам

  memmove (out + x1,/* цель */out + x1 + qbs,/* источник */x2 - x1 - qbs + 1/* объем */);

  if (icp < strlen(in) + x2 - x1)
  {
    if (icp < strlen(in))
    {
      character = in[icp]; // достаем очередной символ

      if (cbp < font_wdt) // Потрошим строку Переходим к очередному символу входящей строки
      {
        memcpy_P (out + x2 - qbs + 1,/* цель */font + character * font_wdt + cbp, /* источник */ qbs /* объем */);
        cbp += qbs;
      }
      else if (spacer_wdt > 0)
      {
        memset (out + x2 - spacer_wdt + 1, 0, spacer_wdt); // вставляем пустой столбик-разделитель
        cbp += spacer_wdt;
      }

      if (cbp >= font_wdt + spacer_wdt) // Символ "закончился"
      {
        icp++;     // переходим к следующему символу в строке
        cbp = 0;   // Пока сидим внутри строки сбрасываем указатель на байт в шрифте
      }
    }
    else
    {
      memset (out + x2 - qbs + 1, 0, qbs); // очищаем экран по концу строки
      icp += qbs;
    }
  }
  else
  {
    cbp = 0; //сбрасываем указатель на байт в шрифте
    icp = 0; //cбрасываем указатель на байт во входной строке
    return true; //end of scrolling
  }
  //DBG_OUT_PORT.println(F("scroll string");
  return false;
}

void FD::CLS(byte *out, size_t size) // Clean screen buffer
{
  memset(out, 0, size);
}

void FD::cleanPos(uint8_t pos, byte *out) // Erases one pos on display
{
  memset(out + pos, 0, 1);
}

void FD::printDot(uint8_t pos, byte *out) // Displays an dot  on a given display
{
  out[pos] |= 0x80;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::printCharacter(unsigned char character, uint8_t x, byte *out, const byte* font, uint8_t font_wdt)
{
  if (character >= 0 && character < 256) //characters must remain within ASCII printable characters defined in array
  {
    memcpy_P (out + x,                     // цель
            font + character * font_wdt, // источник
            font_wdt);                   // объем
  }
}

void FD::print_(char *in, uint8_t size_in, byte *out, uint8_t _offset, const byte* font, uint8_t font_wdt, uint8_t spacer_wdt)
{
  unsigned char character = 0;
  uint8_t icp = 0;

  while (icp < size_in)
  {
    character = in[icp]; // достаем очередной символ

    memcpy_P (out + _offset + icp * (font_wdt + spacer_wdt),/* цель */font + character * font_wdt, /* источник */ font_wdt /* объем */);
    if (spacer_wdt > 0) memset (out + _offset + ((icp + 1) * (font_wdt + spacer_wdt)), 0, spacer_wdt); // вставляем пустой столбик-разделитель
    icp++;     // переходим к следующему символу в строке
  }
}

void FD::shift_ud(bool dwn, bool r_s, byte *in, byte *out, uint16_t *buff, int8_t x1, int8_t x2)
{
  if (x1 < 0 || x2 > 63) return;
  for (uint8_t x = x1; x < x2; x++)
  {
    if (dwn)
    {
      if (r_s)
      {
        buff[x] &= 0xFF00;
        buff[x] |= in[x];
      }
      else     buff[x] <<= 1;
      out[x] = buff[x] >> 8;
    }
    else
    {
      if (r_s)
      {
        buff[x] &= 0xFF;
        buff[x] |= in[x] << 8;
      }
      else     buff[x] >>= 1;
      out[x] = buff[x] & 0xFF;
    }
  }
}

void FD::compressor7(byte *in, uint8_t x1, uint8_t x2) // Адаптация дисплейного буфера под семисегментники
{
  uint8_t _size = (x2 - x1);

  for (uint8_t i = x1, y = x1; i < _size; i++, y++)
  {
    if (in[y * 2 + 1] == 0x80)
    {
      in[i - 1] |= 0x80;
      in[y * 2 + 1] = 0;     //clean position on the display
      y ++;
    }
    in[i] = in[y * 2 + 1]; //position on the display
    in[y * 2 + 1] = 0;     //clean position on the display
  }
}

//-------------------------------------------------------------- Установка яркости
uint8_t FD::auto_br(uint16_t lt, conf_data_t cf)
{
  // у = кх + в

  uint8_t c_br = cf.br_level[2]; // b
  float br = cf.br_level[3];

  float dx = (float)cf.br_level[1] - cf.br_level[0]; //диапазон освещенности (dх)
  float dy = (float)cf.br_level[3] - cf.br_level[2]; //уставки яркости (dу)
  float ltt = (float)lt;

  br = dy / dx * ltt + (float)cf.br_level[2];
  if (dx < 0)   br = dy / dx * ltt + (float)cf.br_level[3];

  c_br = constrain(br, cf.br_level[2], cf.br_level[3]);
  /*
    DBG_OUT_PORT.print(F("brightness before..."));
    DBG_OUT_PORT.println(br);

    DBG_OUT_PORT.print(F("brightness ..."));
    DBG_OUT_PORT.println(c_br);
  */
  return c_br;
}

uint16_t FD::ft_read(bool snr_pres, uint16_t bh_lvl, const int in)
{
  uint16_t ft = 7;
  if (snr_pres) ft = bh_lvl;
  else
  {
    ft = analogRead(in);
  }

  //  DBG_OUT_PORT.print(F("level from sensor..."));
  //  DBG_OUT_PORT.println(ft);

  return ft;
}



//-------------------------------------------------------------- Выбор дня недели
void HT::dow_sel(uint8_t _dow, byte *in)
{
  in[22] &= 0x3;
  in[23] = 0x0;
  in[24] &= 0xE;

  switch (_dow)
  {
    case 2:
      in[23] |= 0x1;
      break;
    case 3:
      in[23] |= 0x2;
      break;
    case 4:
      in[23] |= 0x4;
      break;
    case 5:
      in[23] |= 0x8;
      break;
    case 6:
      in[22] |= 0x8;
      break;
    case 7:
      in[22] |= 0x4;
      break;
    case 1:
      in[24] |= 0x1;
      break;
  }
}

void HT::bat(uint8_t num, byte *in) //Батарейка
{
  //массив батарейка
  const uint8_t batt[6] = {0x02, 0x82, 0x92, 0xD2, 0xF2, 0xF3};
  
  in[22] &= 0xC;
  in[21] = 0x0;

  in[21] |= (batt[num] & 0xF0) >> 4;
  in[22] |= batt[num] & 0x0F;
}

void HT::digit(uint8_t place, uint8_t num, byte *in) // Большие цифры
{
  //массив больших цифр
  const uint8_t dig1[10] = {0xF5, 0x60, 0xB6, 0xF2, 0x63, 0xD3, 0xD7, 0x70, 0xF7, 0xF3};
  uint8_t place2 = place + 1;
  
  if (place == 7) place2 = 31;

  in[place] = 0x0;
  in[place2] &= 0x8;

  in[place] |= (dig1[num] & 0xF0) >> 4;
  in[place2] |= dig1[num] & 0x0F;
}

void HT::mon_day(uint8_t mon, uint8_t _day, byte *in) //Маленькие цифры (Месяц, число, год)
{
  //массив маленьких цифр
  const uint8_t dig2[10] = {0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC};
  
  in[25] = 0x0;
  in[26] = 0x0;
  in[27] &= 0x8;
  in[28] = 0x0;
  in[29] = 0x0;

  in[25] |= (dig2[_day % 10] & 0xF0) >> 4;
  in[26] |= (dig2[_day % 10] & 0x0F);

  if (_day > 9) in[26] |= 0x1;
  in[27] |= int(_day / 10);
  if (int(_day / 10) == 2) in[27] |= 0x6;

  in[28] |= (dig2[mon % 10] & 0xF0) >> 4;
  in[29] |= (dig2[mon % 10] & 0x0F) + (mon / 10);
}

void HT::ala(uint8_t num, byte *in) //Будильник
{
  in[0] &= 0x1;
  in[6] &= 0x7;

  switch (num)
  {
    case 0:
      in[6] |= 0x8;
      break;
    case 1:
      in[0] |= 0x8;
      break;
    case 2:
      in[0] |= 0x4;
      break;
    case 3:
      in[0] |= 0x2;
      break;
  }
}
