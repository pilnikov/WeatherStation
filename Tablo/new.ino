void m3216_init()
{
  m3216 = new RGBmatrixPanel(A, B, C, CLK, LAT, OE, true);
  m3216 -> begin();
  m3216 -> cp437(true);
  m3216 -> setTextSize(1);
  m3216 -> setTextWrap(false); // Allow text to run off right edge
}

void m3216_ramFormer(byte *in)
{
  for (uint8_t x = 0; x < 32; x++)
  {
    uint8_t dt = 0b1;
    for (uint8_t y = 0; y < 8; y++)
    {
      m3216 -> drawPixel(x, y, (in[x] & dt << y) ?  m3216 -> ColorHSV(700, 255, cur_br, true) : 0);
      m3216 -> drawPixel(x, y + 8, (in[x + 32] & dt << y) ?  m3216 -> ColorHSV(400, 255, cur_br, true) : 0);
    }
  }
}

// функция копирования из PROGMEM
void copyFromPGM(const char* const* charMap, byte *_buf, uint8_t _size)
{
  uint16_t _ptr = pgm_read_word(charMap); // получаем адрес из таблицы ссылок
  for (uint8_t i = 0; i < _size; i++) _buf[i] = pgm_read_byte(_ptr++); // прочитать символ из PGM в ячейку буфера, подвинуть указатель
//  DBG_OUT_PORT.print(F("buffer.."));
//  DBG_OUT_PORT.println(_buf);
}

void(* resetFunc) (void) = 0; //Programm reset

bool time_m32_8(byte *in, uint8_t pos, unsigned char *old, const uint8_t *dposx, bool *ch, uint16_t *buff, bool pm)
{
  //----------------------------------------------------------------- заполнение массива
  unsigned char d[q_dig];
  uint8_t font_wdt = 5;
  byte nbuf[64];

  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (pm && hour() > 12) h = hour() - 12;

  d[0] = ' ';
  if (h > 9) d[0] = h / 10 + '0';
  d[1] = h % 10 + '0';
  d[2] = minute() / 10 + '0';
  d[3] = minute() % 10 + '0';
  d[4] = second() / 10 + '\x80';
  d[5] = second() % 10 + '\x80';

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (i > 3)font_wdt = 3;

    ch[i] = false;
    if (d[i] != old[i])
    {
      printCharacter(d[i], dposx[i], nbuf + pos, font5x7, 5); //запись символа в nbuf для изменившихся позиций
      shift_ud(true, true, nbuf + pos, in + pos,  buff + pos, dposx[i],  dposx[i] + font_wdt); //запись символа в вертушок для изменившихся позиций

      old[i] = d[i]; // перезапись предыдущих значений в буфер
      ch[i] = true;
    }
    else
    {
      printCharacter(old[i], dposx[i], in + pos, font5x7, 5); //отображение символов
    }
  }
  //DBG_OUT_PORT.println(F("time_m32_8"));
  return true;
}

//-------------------------------------------------------------- Отображение бегущей строки
bool scroll_String(int8_t x1, int8_t x2, String in, int &icp, int &cbp, byte * out, const byte * font, uint8_t font_wdt, uint8_t spacer_wdt, uint8_t qbs)
{
  unsigned char character = 0; // дергаем входящую сроку по символам

  memmove (out + x1,/* цель */out + x1 + qbs,/* источник */x2 - x1 - qbs + 1/* объем */);

  if (icp < (int)in.length() + x2 - x1)
  {
    if (icp < (int)in.length())
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
  //DBG_OUT_PORT.println(F("scroll string"));
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printCharacter(unsigned char character, uint8_t x, byte *out, const byte* font, uint8_t font_wdt)
{
  if (character >= 0 && character < 256) //characters must remain within ASCII printable characters defined in array
  {
    memcpy_P (out + x,                     // цель
              font + character * font_wdt, // источник
              font_wdt);                   // объем
  }
}

void print_(char *in, uint8_t size_in, byte *out, uint8_t _offset, const byte* font, uint8_t font_wdt, uint8_t spacer_wdt)
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

void shift_ud(bool dwn, bool r_s, byte *in, byte *out, uint16_t *buff, int8_t x1, int8_t x2)
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
        buff[x] |= (in[x] << 8);
      }
      else     buff[x] >>= 1;
      out[x] = buff[x] & 0xFF;
    }
  }
}

//-------------------------------------------------------------- Установка яркости
uint8_t auto_br(uint16_t lt)
{
  // у = кх + в

  uint8_t c_br = conf_data.br_level[2]; // b
  float br = conf_data.br_level[3];

  float dx = (float)conf_data.br_level[1] - conf_data.br_level[0]; //диапазон освещенности (dх)
  float dy = (float)conf_data.br_level[3] - conf_data.br_level[2]; //уставки яркости (dу)
  float ltt = (float)lt;

  br = dy / dx * ltt + (float)conf_data.br_level[2];
  if (dx < 0)   br = dy / dx * ltt + (float)conf_data.br_level[3];

  c_br = constrain(br, conf_data.br_level[2], conf_data.br_level[3]);
  /*
    DBG_OUT_PORT.print(F("brightness before..."));
    DBG_OUT_PORT.println(br);

    DBG_OUT_PORT.print(F("brightness ..."));
    DBG_OUT_PORT.println(c_br);
  */
  return c_br;
}

uint16_t ft_read(bool snr_pres)
{
  uint16_t ft = 7;
  if (snr_pres) ft = lightMeter.readLightLevel();
  else
  {
    ft = analogRead(ANA_SNR);
  }

  //  DBG_OUT_PORT.print(F("level from sensor..."));
  //  DBG_OUT_PORT.println(ft);

  return ft;
}
