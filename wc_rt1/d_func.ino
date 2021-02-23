#include "fonts.h"

//-------------------------------------------------------------- Установка яркости
uint16_t auto_br(uint16_t lt)
{
  uint16_t c_br = 0;

  int a = constrain(conf_data.br_level[2] - conf_data.br_level[1], 1, conf_data.br_level[2]);
  int b = constrain(conf_data.br_level[3] - conf_data.br_level[0], 1, conf_data.br_level[3]);

  c_br = constrain((float)(lt - conf_data.br_level[0]) * a / b + conf_data.br_level[1], conf_data.br_level[1], conf_data.br_level[2]);
  return c_br;
}

uint16_t ft_read(bool snr_pres)
{
  uint16_t ft = 7;
  if (snr_pres) ft = lightMeter.readLightLevel();
  else
  {
#if defined(ESP8266)
    ft = analogRead(A0);
#endif
  }
  return ft;
}


void  time_m32_8()
{
  //----------------------------------------------------------------- заполнение массива
  unsigned char d[q_dig];
  uint8_t font_wdt = 5;

  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (conf_data.use_pm && hour() > 12) h = hour() - 12;

  d[0] = ' ';
  if (h > 9) d[0] = h / 10 + '0';
  d[1] = h % 10 + '0';
  d[2] = minute() / 10 + '0';
  d[3] = minute() % 10 + '0';
  d[4] = second() / 10 + '\x80';
  d[5] = second() % 10 + '\x80';

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (i > 3) font_wdt = 3;
    d_notequal[i] = d[i] != oldDigit[i]; // проверка изменений в буфере отображаемых символов
    if (d_notequal[i])
    {
      printCharacter(d[i], digPos_x[i], buff1, font5x7, 5); // запись символа в вертушок для изменившихся позиций
      shift_ud(true,  true, buff1, screen,  digPos_x[i],  digPos_x[i] + font_wdt); // запись символа в вертушок для изменившихся позиций
    }
    else printCharacter(oldDigit[i], digPos_x[i], screen, font5x7, 5); // отображение символов
    oldDigit[i] = d[i]; // перезапись предыдущих значений в буфер
  }
}

//-------------------------------------------------------------- Отображение бегущей строки
bool scroll_String(int8_t x1, int8_t x2, String in,  int &cp, int &cbp, byte *out, const byte *font, uint8_t font_wdt)
{
  byte inbyte = 0; //источник байтов

  unsigned char character = 0; // дергаем входящую сроку по символам

  if (cp < in.length() + x2)
  {
    if (cp < in.length()) // Пока сидим внутри строки
    {
      if (cbp > font_wdt) // Переходим к очередному символу входящей строки
      {
        cbp = 0;
        cp ++;
      }
      character = in[cp]; // достаем очередной символ
      inbyte = 0; // символ "закончился" - вставляем пустой столбик
      if (cbp < font_wdt) inbyte = font[character * font_wdt + cbp]; //потрошим символ на байты
      cbp++;
    }
    else
    {
      inbyte = 0; // символ "закончился" - вставляем пустой столбик
      cp++;
    }

    out[x2] = inbyte; // запись крайнего правого байта (координата х2) в буфер экрана
    //сдвиг в буфере экрана на одну позицию (колонку) (на один байт) влево
    memmove (out + x1,                   // цель
             out + x1 + 1,               // источник
             x2 - x1);                   // объем
  }
  else
  {
    cp = 0;
    cbp = 0;
    return true; //end of scrolling
  }
  return false;
}
