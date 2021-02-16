//-------------------------------------------------------------- Установка яркости
uint16_t auto_br(uint16_t lt, uint16_t lev[4])
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

//-------------------------------------------------------------- Выбор дня недели
void dow_sel(uint8_t _dow)
{
  buf[22] &= 0x3;
  buf[23] = 0x0;
  buf[24] &= 0xE;

  switch (_dow)
  {
    case 2:
      buf[23] |= 0x1;
      break;
    case 3:
      buf[23] |= 0x2;
      break;
    case 4:
      buf[23] |= 0x4;
      break;
    case 5:
      buf[23] |= 0x8;
      break;
    case 6:
      buf[22] |= 0x8;
      break;
    case 7:
      buf[22] |= 0x4;
      break;
    case 1:
      buf[24] |= 0x1;
      break;
  }
}

inline void bat(uint8_t num) //Батарейка
{
  buf[22] &= 0xC;
  buf[21] = 0x0;

  buf[21] |= (batt[num] & 0xF0) >> 4;
  buf[22] |= batt[num] & 0x0F;
}

inline void digit(uint8_t place, uint8_t num) // Большие цифры
{
  uint8_t place2 = place + 1;
  if (place == 7) place2 = 31;

  buf[place] = 0x0;
  buf[place2] &= 0x8;

  buf[place] |= (dig1[num] & 0xF0) >> 4;
  buf[place2] |= dig1[num] & 0x0F;
}

inline void mon_day(uint8_t mon, uint8_t _day) //Маленькие цифры (Месяц, число, год)
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

inline void ala(uint8_t num) //Будильник
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

void  matrix32x8_time()
{
  for (uint8_t i = 0; i < num; i++) digoldig[i] = screen[i]; // перезапись предыдущих значений в буфер

  //----------------------------------------------------------------- заполнение массива
  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (conf_data.use_pm && hour() > 12) h = hour() - 12;
  if (h > 9)  screen[0] = h / 10 + '0';
  else screen[0] = 0;
  screen[1] = h % 10 + '0';
  screen[2] = minute() / 10 + '0';
  screen[3] = minute() % 10 + '0';
  screen[4] = second() / 10 + '\x80';
  screen[5] = second() % 10 + '\x80';

  //---------------------------------------------------------------------
  for (uint8_t i = 0; i < num; i++)
  {
    digPos_y_[i] = (screen[i] == digoldig[i]) ? 7 : 0; //при изменении буфера рисует набегающий символ сверху
    /*  DBG_OUT_PORT.print("pos ");
      DBG_OUT_PORT.print(i);
      DBG_OUT_PORT.print(" - ");
      DBG_OUT_PORT.println(digPos_y_[i]);
    */
  }
}



//-------------------------------------------------------------- Отображение бегущей строки
bool mov_str(uint8_t dtype, uint8_t dsp_wdt, String tape, uint8_t nline, int cur_sym_pos)
{
  if (dtype > 19)f_dsp.utf8rus(tape);

  if (dtype == 19) f_dsp.lcd_rus(tape);
#if defined(ESP32)
  if (dtype == 24)
  {
    m3264->setTextWrap(false);
    m3264->setTextSize(TextSize);
  }
#endif
 
/*  DBG_OUT_PORT.println(tape);
  DBG_OUT_PORT.print("input string length..");
  DBG_OUT_PORT.println(tape.length());
*/
  int colors[3];
  uint8_t sym_wdt = 5 + spacer; // Ширина занимаемая символом в пикселях (5 ширина шрифта + 1 линия разделитель = 6)
  if (dtype == 19)
  {
    sym_wdt = 1;
    spacer = 0;
  }

#if defined(ESP32)
  if (dtype == 24)
  {
    sym_wdt *= TextSize;
    spacer = TextSize;
  }
#endif

  if (cur_sym_pos < sym_wdt * tape.length() + dsp_wdt - spacer)  //текущая позиция < (длина строки + ширина дисплея)
  {
    if (dtype > 19)
    {
      int16_t letter = cur_sym_pos / sym_wdt;           //        номер крайнего правого отображаемого символа
      int8_t x = (dsp_wdt - 1) - cur_sym_pos % sym_wdt; // координата х крайнего правого отображаемого символа
      uint8_t y = nline * 8;                            // координата y строки
      uint8_t bg = 0;                                   // цвет фона
      if (dtype == 22) bg = 3;

      while (x + sym_wdt - spacer >= 0 && letter >= 0)
      {
        if (letter < tape.length())
        {
          if (dtype == 20) m7219->drawChar(x, y, tape[letter], 1, bg, 1); //вывод части строки посимвольно, справа налево
          if (dtype == 22) m1632->drawChar(x, y, tape[letter], 1, bg, 1); //вывод части строки посимвольно, справа налево
#if defined(ESP32)
          if (dtype == 24)
          {
            getRGB(abs(cur_sym_pos / 4) % 255, 255, 255, colors);
            m3264->drawChar(x, y, tape[letter], m3264->AdafruitColor(colors[0], colors[1], colors[2]), 0, TextSize);
          }
#endif
        }
        letter--;     // смещение на символ влево по строке
        x -= sym_wdt; // смещение на ширину символа влево по х
      }
      if (dtype == 20) m7219->write();  // Send bitmap to display
      if (dtype == 22) m1632->render(); // Send bitmap to display
      //if (dtype == 24) m3216 -> update(); // Send bitmap to display
    }

    if (dtype == 19)
    {
      int16_t letter = cur_sym_pos; //        номер крайнего правого отображаемого символа
      int8_t x = dsp_wdt - 1;       // координата х крайнего правого отображаемого символа

      while (x >= 0 && letter >= 0)
      {

        if (letter < tape.length())
        {
          lcd->setCursor(x, nline);
          lcd->print(tape[letter]); //Draw char in lcd
        }
        letter--;   // смещение на символ влево по строке
        x--;
      }
    }
  }
  else return true; //end of scrolling
  return false;
}
