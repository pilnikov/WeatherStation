
void CLS(void);
void cleanPos(uint8_t);
void printDot(uint8_t);
void utf714(unsigned char&, unsigned char);
void printCharacter(unsigned char, uint8_t, byte*, const byte*, uint8_t);
void compressor7(byte*, uint8_t);

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


void print_(char *in, uint8_t size_in, byte *out, const byte* font, uint8_t font_wdt, uint8_t spacer_wdt)
{
  unsigned char character = 0;
  uint8_t icp = 0, cbp = 0, obp = 0;

  while (icp < size_in)
  {
    if (cbp > font_wdt - 1 + spacer_wdt) // Переходим к очередному символу входящей строки
    {
      cbp = 0;
      icp ++;
    }

    character = in[icp]; // достаем очередной символ
    if (cbp < font_wdt) out[obp] = font[character * font_wdt + cbp]; //потрошим символ на байты
    else out[obp] = 0; // символ "закончился" - вставляем пустой столбик-разделитель
    cbp++;
    obp++;
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

void compressor7(byte *in, uint8_t in_size) // Адаптация дисплейного буфера под семисегментники
{
  uint8_t y = 0;
  for (uint8_t i = 0; i < in_size / 2; i++)
  {
    if (in[y * 2 + 1] == 0x80)
    {
      in[i - 1] |= 0x80;
      y++;
    }
    in[i] = in[y * 2 + 1]; //position on the display
    y++;
  }
}
