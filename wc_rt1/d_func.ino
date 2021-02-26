#include "fonts.h"

bool scroll_String(int8_t, int8_t, String, int&, int&, byte*, const byte*, uint8_t, uint8_t, uint8_t);


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
bool scroll_String(int8_t x1, int8_t x2, String in, int &icp, int &cbp, byte *out, const byte *font, uint8_t font_wdt, uint8_t spacer_wdt, uint8_t qbs)
{
  byte inbyte[qbs]; //источник байтов

  unsigned char character = 0; // дергаем входящую сроку по символам

  if (icp < in.length() + x2 - x1 + 1)
  {
    character = in[icp]; // достаем очередной символ

    if (cbp < font_wdt) // Потрошим строку Переходим к очередному символу входящей строки
    {
      memcpy (out + x2 + 1,                      // цель
              font + character * font_wdt + cbp, // источник
              qbs);                              // объем
      cbp += qbs;
    }
    else // символ "закончился" - вставляем пустой столбик-разделитель
    {
      icp++; // переходим к следующему символу в строке
      if (icp < in.length())
      {
        cbp = 0;   // Пока сидим внутри строки сбрасываем указатель на байт в шрифте
        for (uint8_t i = 0; i < spacer_wdt; i++) out[i + x2 + spacer_wdt] = 0; // вставляем пустой столбик-разделитель
      }
      else for (uint8_t i = 0; i < qbs; i++) out[i + x2 + 1] = 0; // очищаем экран по концу строки
    }
    if (!(cbp == 0 && spacer_wdt == 0))
    {
      memmove (out + x1,       // цель
               out + x1 + qbs, // источник
               x2 - x1 + 1);   // объем
    }
  }
  else
  {
    cbp = 0; //сбрасываем указатель на байт в шрифте
    icp = 0; //cбрасываем указатель на байт во входной строке
    return true; //end of scrolling
  }
  return false;
}
