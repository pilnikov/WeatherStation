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

  for (uint8_t i = 0; i < q_dig; i++) oldDigit[i] = d[i]; // перезапись предыдущих значений в буфер


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
    d_notequal[i] = d[i] != oldDigit[i]; // проверка изменений в буфере отображаемых символов
    if (d_notequal[i])
    {
      printCharacter_m32_8(d[i], digPos_x[i], buff1); // запись символа в вертушок для изменившихся позиций
      shift_ud(true, true, buff1, screen,  digPos_x[i],  digPos_x[i] + 5); // запись символа в вертушок для изменившихся позиций
    }
    printCharacter_m32_8(d[i], digPos_x[i], screen); // отображение символов
  }
}

//-------------------------------------------------------------- Отображение бегущей строки
bool scroll_String(int8_t x1, int8_t x2, char *in,  uint8_t size_in, int &cp, int &cbp, byte *buff, uint8_t font)
{

  byte inbyte = 0; //источник байтов

  unsigned char character = in[cp]; // дергаем входящую сроку по символам

  uint8_t font_wdt = 1; // "ширина" шрифта в байтах

  if (font == 1) // шрифт 14 seg
  {
    font_wdt = 2;
    cbp++;

    if (cbp > font_wdt - 1)
    {
      cbp = 0;
      if (cp < size_in)
      {
        cp ++;

        character = in[cp];
        buff[x2] = FourteenSegmentASCII[character - 32] & 0xFF;
        buff[x2 - 1] = FourteenSegmentASCII[character - 32] >> 8;

        for (uint8_t x = x2; x > x1; x -= 2)
        {
          buff[x - 2] = buff[x]; // сдвиг в буфере экрана на одну позицию (разряд) (на два байта) влево
        }
      }
      else return true; //end of scrolling
    }
  }

  if (font == 2) // шрифт 5x7
  {
    font_wdt = 5;

    inbyte = font5x7[character * font_wdt + cbp];

    if (cp < size_in + 32)
    {
      if (cp < size_in)
      {
        inbyte = 0; // символ "закончился" - вставляем пустой столбик
        if (cbp > font_wdt)
        {
          cbp = 0;
          cp ++;
          character = in[cp];
          inbyte = font5x7[character * font_wdt + cbp];
        }
        else
        {
          character = in[cp];
          inbyte = font5x7[character * font_wdt + cbp];
        }
        cbp++;
      }
      else 
      {
        inbyte = 0; // символ "закончился" - вставляем пустой столбик
        cp++;
      }

      buff[x2] = inbyte; // запись крайнего правого байта (координата х2) в буфер экрана
      for (uint8_t x = x1; x < x2; x++)
      {
        buff[x] = buff[x + 1]; // сдвиг в буфере экрана на одну позицию (колонку) (на один байт) влево
      }
    }
    else return true; //end of scrolling
  }
  return false;
}
