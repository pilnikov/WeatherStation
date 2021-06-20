
void CLS(byte*);
void cleanPos(uint8_t, byte*);
void printDot(uint8_t, byte*);
void printCharacter(unsigned char, uint8_t, byte*, const byte*, uint8_t);
void print_(char*, uint8_t, byte*, uint8_t, const byte*, uint8_t, uint8_t);
void compressor7(byte*, uint8_t, uint8_t);
void shift_ud(bool, bool, byte*, byte*, byte*, int8_t, int8_t);

/*
   Erases all display
*/

void CLS(byte *out) // Clean screen buffer
{
  memset(out, 0, 64);
}

/*
   Erases one pos on display
*/

void cleanPos(uint8_t pos, byte *out)
{
  memset(out + pos, 0, 1);
}

/*
   Displays an dot  on a given display
*/
void printDot(uint8_t pos, byte *out)
{
  out[pos] |= 0x80;
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
        buff[x] |= in[x] << 8;
      }
      else     buff[x] >>= 1;
      out[x] = buff[x] & 0xFF;
    }
  }
}

void compressor7(byte *in, uint8_t x1, uint8_t x2) // Адаптация дисплейного буфера под семисегментники
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
