//-------------------------------------------------------------- Установка яркости
uint16_t auto_br(uint16_t lt, uint16_t lev[4])
{
  uint16_t c_br = 0;

  int a = constrain(conf_data.br_level[2] - conf_data.br_level[1], 1 , conf_data.br_level[2]);
  int b = constrain(conf_data.br_level[3] - conf_data.br_level[0], 1 , conf_data.br_level[3]);

  c_br = constrain((float)(lt - conf_data.br_level[0]) * a / b + conf_data.br_level[1], conf_data.br_level[1], conf_data.br_level[2]);
  return c_br;
}

//-------------------------------------------------------------- Смена содержимого экрана для 7 сегментников
uint8_t mod_sel()
{
  if (millis() - irq_end[0] > 5000)
  {
    disp_mode++;
    if (nm_is_on || disp_mode > 6 || second() > 35) disp_mode = 0;
    irq_end[0] = millis();
  }
  return disp_mode;
}

//-------------------------------------------------------------- Прочерк
inline void procherk (uint8_t pos, uint8_t q)
{
  for (uint8_t i = pos; i < pos + q; i++)  dig[i] = ( numbertable[23]);
}

//-------------------------------------------------------------- Выбор дня недели
inline void dow_sel(uint8_t _dow)
{
  buf[22] &= 0x3;
  buf[23] =  0x0;
  buf[24] &= 0xE;

  switch (_dow)
  {
    case 2:
      dig[0] = numbertable[22];
      dig[1] = numbertable[16]; //ПН
      buf[23] |= 0x1;
      break;
    case 3:
      dig[0] = numbertable[ 8];
      dig[1] = numbertable[19]; //Bt
      buf[23] |= 0x2;
      break;
    case 4:
      dig[0] = numbertable[12];
      dig[1] = numbertable[18]; //CP
      buf[23] |= 0x4;
      break;
    case 5:
      dig[0] = numbertable[ 4];
      dig[1] = numbertable[19]; //Чt
      buf[23] |= 0x8;
      break;
    case 6:
      dig[0] = numbertable[22];
      dig[1] = numbertable[19]; //Пt
      buf[22] |= 0x8;
      break;
    case 7:
      dig[0] = numbertable[12];
      dig[1] = numbertable[ 6]; //СБ
      buf[22] |= 0x4;
      break;
    case 1:
      dig[0] = numbertable[ 8];
      dig[1] = numbertable[12]; //BС
      buf[24] |= 0x1;
      break;
  }
  //  return dow;
}

inline void bat (uint8_t num) //Батарейка
{
  buf[22] &= 0xC;
  buf[21] = 0x0;

  buf[21] |= (batt[num] & 0xF0) >> 4;
  buf[22] |= batt[num] & 0x0F;
}

inline void digit (uint8_t place, uint8_t num) // Большие цифры
{
  uint8_t place2 = place + 1;
  if (place == 7) place2 = 31;

  buf[place] = 0x0;
  buf[place2] &= 0x8;

  buf[place] |= (dig1[num] & 0xF0) >> 4;
  buf[place2] |= dig1[num] & 0x0F;
}

inline void mon_day (uint8_t mon, uint8_t _day) //Маленькие цифры (Месяц, число, год)
{
  buf[25] = 0x0;
  buf[26] = 0x0;
  buf[27] &= 0x8;
  buf[28] = 0x0;
  buf[29] = 0x0;

  buf[25] |= (dig2[_day % 10] & 0xF0) >> 4;
  buf[26] |= (dig2[_day % 10] & 0x0F);

  if (_day > 9) buf[26] |= 0x1;
  buf[27] |= int(_day / 10);
  if (int(_day / 10) == 2) buf[27] |= 0x6;

  buf[28] |= (dig2[mon % 10] & 0xF0) >> 4;
  buf[29] |= (dig2[mon % 10] & 0x0F) + (mon / 10);
}

inline void ala (uint8_t num) //Будильник
{
  buf[0] &= 0x1;
  buf[6] &= 0x7;

  switch (num)
  {
    case 0:
      buf[6] |= 0x8;
      break;
    case 1:
      buf[0] |= 0x8;
      break;
    case 2:
      buf[0] |= 0x4;
      break;
    case 3:
      buf[0] |= 0x2;
      break;
  }
}

//-------------------------------------------------------------- Размещение в определенном месте экрана
inline void render_number_in_place(int number, int _colum, int _row)
{
  byte digit = 1; // Кол-во разрядов в number

  if (number > 9999) digit = 5;
  else if (number > 999 ) digit = 4;
  else if (number > 99  ) digit = 3;
  else if (number > 9   ) digit = 2;

  char msg[digit + 1];
  sprintf(msg, "%1u", number);

  m7219 -> setCursor(_colum - (digit - 1) * 6, _row);
  m7219 -> print(msg);
  if (debug_level == 5) DBG_OUT_PORT.print(msg);
}

inline void render_00number_in_place(int number, int _colum, int _row)
{
  char msg[2];
  sprintf(msg, "%02u", number);
  m7219 -> setCursor(_colum, _row);
  m7219 -> print(msg);
  if (debug_level == 5) DBG_OUT_PORT.print(msg);
}

void  matrix_refresh()
{
  for (uint8_t i = 0; i < num; i++) digoldig[i] = d[i]; // перезапись предыдущих значений в буфер

  //----------------------------------------------------------------- заполнение массива
  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (conf_data.use_pm && hour() > 12) h = hour() - 12;
  if (h > 9)  d[0] = h / 10 + '0';
  else d[0] = 0;
  d[1] = h % 10 + '0';
  d[2] = minute() / 10 + '0';
  d[3] = minute() % 10 + '0';
  d[4] = second() / 10 + '\x80';
  d[5] = second() % 10 + '\x80';

  //---------------------------------------------------------------------
  for (uint8_t i = 0; i < num; i++)
  {
    digtrans[i] = (d[i] == digoldig[i]) ? 7 : 0; //при изменении буфера рисует набегающий символ сверху
    /*  DBG_OUT_PORT.print("pos ");
        DBG_OUT_PORT.print(i);
        DBG_OUT_PORT.print(" - ");
        DBG_OUT_PORT.println(digtrans[i]);
    */
  }
}

//-------------------------------------------------------------- Отображение бегущей строки
bool mov_str(uint8_t dtype, uint8_t dsp_wdt, String tape, uint8_t nline, int cur_sym_pos)
{
 /*
  DBG_OUT_PORT.print("cur_sym_pos..");
  DBG_OUT_PORT.println(cur_sym_pos);
  DBG_OUT_PORT.print("tape length..");
  DBG_OUT_PORT.println(tape.length());
*/
  String instr = f_dsp.utf8rus(tape);
  if (dtype == 1) instr = f_dsp.lcd_rus(tape);
  
  uint8_t sym_wdt = 5 + spacer; // Ширина занимаемая символом в пикселях (5 ширина шрифта + 1 линия разделитель = 6)
  bool e_run_st;
  if (dtype == 1)
  {
    sym_wdt = 1;
    spacer = 0;
  }

  if (cur_sym_pos < sym_wdt * instr.length() + dsp_wdt - spacer)  //текущая позиция < (длина строки + ширина дисплея)
  {
    e_run_st = false; // флаг работы бегущей строки

    if (dtype == 4 || dtype == 8)
    {
      int16_t letter = cur_sym_pos / sym_wdt;           //        номер крайнего правого отображаемого символа
      int8_t x = (dsp_wdt - 1) - cur_sym_pos % sym_wdt; // координата х крайнего правого отображаемого символа
      uint8_t y = nline * 8;                            // координата y строки
      uint8_t bg = 0;                                   // цвет фона
      if (dtype == 8) bg = 3;

      while ( x + sym_wdt - spacer >= 0 && letter >= 0 )
      {
        if (letter < instr.length())
        {
          if (dtype == 4) m7219 -> drawChar(x, y, instr[letter], 1, bg, 1); //вывод части строки посимвольно, справа налево
          if (dtype == 8) m1632 -> drawChar(x, y, instr[letter], 1, bg, 1); //вывод части строки посимвольно, справа налево
        }
        letter--;     // смещение на символ влево по строке
        x -= sym_wdt; // смещение на ширину символа влево по х
      }
      if (dtype == 4) m7219 -> write();  // Send bitmap to display
      if (dtype == 8) m1632 -> render(); // Send bitmap to display
    }

    if (dtype == 1)
    {
      int16_t letter = cur_sym_pos; //        номер крайнего правого отображаемого символа
      int8_t x = dsp_wdt - 1;       // координата х крайнего правого отображаемого символа

      while ( x >= 0 && letter >= 0 )
      {

        if (letter < instr.length())
        {
          lcd -> setCursor(x, nline);
          lcd -> print(instr[letter]); //Draw char in lcd
        }
        letter--;   // смещение на символ влево по строке
        x--;
      }
    }
  }
  else e_run_st = true; //end of scrolling
  return e_run_st;
}
