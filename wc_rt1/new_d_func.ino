#include "fonts.h"


void CLS(void);
void cleanPos(uint8_t);
void printCharacter_S7(uint8_t, uint8_t);
void printCharacter_S14(uint8_t, uint8_t);
void printDot(uint8_t);
void printDisplay_S(String, uint8_t, uint8_t);
void utf714(unsigned char&, unsigned char);
void scrollString(String, uint8_t, uint8_t, uint16_t);


uint16_t countD;
uint8_t stringIndex;


void printCharacter_m32_8(unsigned char, uint8_t, byte);

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

void printCharacter_S7(uint8_t ch, uint8_t pos)
{
  if (ch >= 32 && ch < 256) //characters must remain within ASCII printable characters defined in array
  {
    screen[pos] = FourteenSegmentASCII[ch - 32] & 0xFF;
    f_dsp.mir_seg(screen[pos]);
  }
}

/*
   Displays an ASCII character on a given display
*/
void printCharacter_S14(uint8_t ch, uint8_t pos)
{
  if (ch >= 32 && ch < 256) //characters must remain within ASCII printable characters defined in array
  {
    byte _data1 = FourteenSegmentASCII[ch - 32] & 0xFF;
    byte _data2 = FourteenSegmentASCII[ch - 32] >> 8;
    if (pos > 9)
    {
      screen[pos] = _data1;
      screen[pos + 3] = _data2;
    }
    else screen[pos] = _data1;
  }
}


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

void printDisplay_S(String string, uint8_t f_pos, uint8_t l_pos)
{
  uint8_t sym_pos_idx = 0; // number of byte in inbox string
  unsigned char curr = 0;
  unsigned char next = 0;

  for (uint8_t i = f_pos; i <= l_pos; i++) //position on the display
  {
    if (sym_pos_idx > string.length()) curr = 0;
    else curr = string[sym_pos_idx];

    if (sym_pos_idx + 1 > string.length()) next = 0;
    else next = string[sym_pos_idx + 1];

    if (curr == 0xD0 || curr == 0xD1)   // Cyrillic symbol detected
    {
      utf714(curr, next);
      sym_pos_idx++;
    }

    if (ram_data.type_vdrv == 2) printCharacter_S7(curr, i);
    else printCharacter_S14 (curr, i);
    if (next == 0x2E)
    {
      printDot(i);
      sym_pos_idx++;
    }
    sym_pos_idx++;
  }
}

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
void scrollString(String string, uint8_t f_pos, uint8_t l_pos)
{
  uint8_t q_pos = l_pos - f_pos + 1;
  if (stringIndex > string.length() + q_pos)
  {
    stringIndex = 0;
  }

  for (uint8_t posIdx = 0; posIdx < q_pos; posIdx++)
  {
    uint8_t i = stringIndex + posIdx - q_pos;
    unsigned char uch = string[i];
    printCharacter_S14 ((i >= 0 && i < string.length()) ? uch : ' ', posIdx + f_pos); //front
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void printCharacter_m32_8(unsigned char character, uint8_t x, byte *in)
{
  if (character >= 0 && character < 256) //characters must remain within ASCII printable characters defined in array
  {
    for (uint8_t i = 0; i < 5; i++)
    {
      in[x + i] = font5x7[character * 5 + i];
    }
  }
}

void print_m32_8(char *in,  uint8_t size_in, byte *buff)
{
  unsigned char character = in[0];
  uint8_t ch_pos = 0, ch_byte_pos = 0, font_wdt = 5;

  while (ch_pos < size_in)
  {
    if (ch_byte_pos > font_wdt - 1)
    {
      if (ch_byte_pos == font_wdt) buff[ch_pos * (font_wdt + 1) + ch_byte_pos] = 0; // символ "закончился" - вставляем пустой столбик
      if (ch_byte_pos > font_wdt)
      {
        ch_byte_pos = 0;
        ch_pos ++;
        character = in[ch_pos]; // дергаем входящую сроку по символам
        buff[ch_pos * (font_wdt + 1) + ch_byte_pos] = font5x7[character * font_wdt + ch_byte_pos];
      }
    }
    else  buff[ch_pos * (font_wdt + 1) + ch_byte_pos] = font5x7[character * font_wdt + ch_byte_pos];
    ch_byte_pos++;
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
      out[x] = inn[x] >> 7;
    }
    else
    {
      if (r_s) inn[x] |= in[x] << 7;
      else     inn[x] >>= 1;
      out[x] = inn[x] & 0xFF;
    }
  }
}
