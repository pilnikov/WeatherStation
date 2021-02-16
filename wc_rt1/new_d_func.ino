
void cleanPos(uint8_t);
void printCharacter(uint8_t, uint8_t);
void printDot(uint8_t);
void printDisplay(String, uint8_t, uint8_t);
void scrollString(String, uint8_t, uint8_t, uint16_t);


uint16_t countD;
uint8_t stringIndex;


/*
   Erases one pos on display
*/

void cleanPos(uint8_t pos)
{
  screen[pos] = 0;
}

void printCharacter_m7(uint8_t ch, uint8_t pos)
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
void printCharacter(uint8_t ch, uint8_t pos)
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

void printDisplay(String string, uint8_t f_pos, uint8_t l_pos)
{
  uint8_t sym_pos_idx = 0; // number of byte in inbox string
  uint8_t curr = 0;
  uint8_t next = 0;

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
    
    if (ram_data.type_vdrv == 2) printCharacter_m7(curr, i);
    else printCharacter (curr, i);
    if (next == 0x2E)
    {
      printDot(i);
      sym_pos_idx++;
    }
    sym_pos_idx++;
  }
}

void utf714(byte & n, byte m)
{
  /*
     Позиция символа в массиве Segments вычисляется по формуле:
     Для символов от А до п, с кодами от 0x90 по 0xBF, с позициями от сто двенадцатой до сто шестьдесят первой (0x70 по 0xA1)
     pos = код символа
     Для символов от р до я, с кодами от 0x80 по 0x8F, с позициями от сто шестьдесят второй до сто семьдесят седьмой (0xA2 по 0xC1)
     pos = код символа + 0х40
     ё и Ё  и символ градуса стоят особняком (выбиваются из общего строя) поэтому для них отдельные строки.
  */
  byte target;
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
    printCharacter ((i >= 0 && i < string.length()) ? uch : ' ', posIdx + f_pos); //front
  }
}
