
void CLS(void);
void cleanPos(uint8_t);
void printDot(uint8_t);
void utf714(unsigned char&, unsigned char);
void printCharacter(unsigned char, uint8_t, byte*, const byte*, uint8_t);
void print_(char*, uint8_t, byte*, uint8_t, const byte*, uint8_t, uint8_t);
void compressor7(byte*, uint8_t, uint8_t);
void shift_ud(bool, bool, byte*, byte*, int8_t, int8_t);

/*
   Erases one pos on display
*/

void CLS() // Clean screen buffer
{
  for (uint8_t i = 0; i < 64; i++) screen[i] = 0; //position on the display
}


void cleanPos(uint8_t pos)
{
  screen[pos] = 0;
}


/*
   Displays an ASCII character on a given display
*/


/*
   Displays an dot  on a given display
*/
void printDot(uint8_t pos)
{
  if (pos > 9)
  {
    screen[pos + 3] |= 0x80;
  }
  else screen[pos] |= 0x80;
}

/*
   Displays all symbol on a given display
*/

void utf714(unsigned char &n, unsigned char m)
{
  /*
     Позиция символа в массиве Segments вычисляется по формуле:
     Для символов от А до п, с кодами от 0x90 по 0xBF, с позициями от сто двенадцатой до сто шестьдесят первой (0x70 по 0xA1)
     pos = код символа
     Для символов от р до я, с кодами от 0x80 по 0x8F, с позициями от сто шестьдесят второй до сто семьдесят седьмой (0xA2 по 0xC1)
     pos = код символа + 0х40
     ё и Ё  и символ градуса стоят особняком (выбиваются из общего строя) поэтому для них отдельные строки.
  */
  unsigned char target;
  switch (n)
  {
    case 0xD0:
      if (m == 0x81) target = D0; // Ё
      else if (m >= 0x90 && m <= 0xBF) target = m; //от А до п
      break;
    case 0xD1:
      if (m == 0x91) target = D1; // ё
      else if (m >= 0x80 && m <= 0x8F) target = m + 0x40; //от р до я
      break;
  }
  n = target;
}


/*
   Scroll a string across displays
   Includes fade in where string starts on right-most display preceded by blank space
   and scrolls left. This also includes a fade out where characters travel off the left
   display followed by blank space
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void printCharacter(unsigned char character, uint8_t x, byte *out, const byte* font, uint8_t font_wdt)
{
  if (character >= 0 && character < 256) //characters must remain within ASCII printable characters defined in array
  {
    memcpy (out + x,                     // цель
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

    memcpy (out + _offset + icp * (font_wdt + spacer_wdt),/* цель */font + character * font_wdt, /* источник */ font_wdt /* объем */);
    if (spacer_wdt > 0) memset (out + _offset + icp * (font_wdt + spacer_wdt) + spacer_wdt, 0, spacer_wdt); // вставляем пустой столбик-разделитель
    icp++;     // переходим к следующему символу в строке
  }
}


void shift_ud(bool dwn, bool r_s, byte * in, byte * out,  int8_t x1, int8_t x2)
{
  for (uint8_t x = x1; x < x2; x++)
  {
    if (dwn)
    {
      if (r_s) inn[x] |= in[x];
      else     inn[x] <<= 1;
      out[x] = inn[x] >> 8;
    }
    else
    {
      if (r_s) inn[x] |= in[x] << 8;
      else     inn[x] >>= 1;
      out[x] = inn[x] & 0xFF;
    }
  }
}

void compressor7(byte *in, uint8_t x1, uint8_t x2) // Адаптация дисплейного буфера под семисегментники
{
  uint8_t _size = (x2 - x1) * 2;

  for (uint8_t i = x1, y = x1; i < _size; i++, y++)
  {
    if (in[y * 2 + 1] == 0x80)
    {
      in[i - 1] |= 0x80;
      y ++;
    }
    in[i] = in[y * 2 + 1]; //position on the display
  }
}
