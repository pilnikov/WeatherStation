
//-------------------------------------------------------------- Установка яркости
uint8_t auto_br(uint16_t lt, conf_data_t cf)
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

uint16_t ft_read(bool snr_pres, uint16_t bh_lvl, const int in)
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


bool time_m32_8(byte *in, uint8_t pos, unsigned char *old, const uint8_t *dposx, bool *change, uint16_t *buff, bool pm, const uint8_t q_dig, rtc_data_t rt)
{
  //----------------------------------------------------------------- заполнение массива
  unsigned char d[q_dig];
  uint8_t font_wdt = 5;
  byte nbuf[64];

  uint8_t h = rt.hour;
  // Do 24 hour to 12 hour format conversion when required.
  if (pm && rt.hour > 12) h = rt.hour - 12;

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
      printCharacter(d[i], dposx[i], nbuf + pos, font5x7, 5); // запись символа в вертушок для изменившихся позиций
      shift_ud(true, true, nbuf + pos, in + pos,  buff + pos, dposx[i],  dposx[i] + font_wdt); // запись символа в вертушок для изменившихся позиций
    }
    else
    {
      printCharacter(old[i], dposx[i], in + pos, font5x7, 5); // отображение символов
    }
    old[i] = d[i]; // перезапись предыдущих значений в буфер
  }
  //DBG_OUT_PORT.println(F("time_m32_8");
  return true;
}

//-------------------------------------------------------------- Отображение бегущей строки
bool scroll_String(int8_t x1, int8_t x2, String in, int &icp, int &cbp, byte * out, const byte * font, uint8_t font_wdt, uint8_t spacer_wdt, uint8_t qbs)
{
  unsigned char character = 0; // дергаем входящую сроку по символам

  memmove (out + x1,/* цель */out + x1 + qbs,/* источник */x2 - x1 - qbs + 1/* объем */);

  if (icp < in.length() + x2 - x1)
  {
    if (icp < in.length())
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
