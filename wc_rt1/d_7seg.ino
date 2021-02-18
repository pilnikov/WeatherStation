

void tm1637_init()
{
  tm1637 = new TM1637(CLK_PIN, DIO_PIN);
  tm1637->clear();
  tm1637->set_br(7);
}


void ht1621_init()
{
  ht21 = new HT1621(CS_PIN, CLK_PIN, DIO_PIN); // ss, rw, data
  ht21->begin();

  ht21->sendCommand(HT1621::RC256K);
  ht21->sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht21->sendCommand(HT1621::SYS_EN);
  ht21->sendCommand(HT1621::LCD_ON);

  uint8_t i = 0;

  ht21->clear_all();// clear memory
  CLS();

  digit(1, 1); /* 1 */
  digit(3, 2); /* 2 */
  digit(5, 6); /* 6 */
  digit(7, 1); /* 1 */

  for (i = 1; i < 8; i++)
  {
    ht21->write(i, screen[i]);

    bat(i - 1);
    ht21->write(21, screen, 2);
    delay(250);
  }

}

//-------------------------------------------------------------- Выбор дня недели
void dow_sel(uint8_t _dow)
{
  screen[22] &= 0x3;
  screen[23] = 0x0;
  screen[24] &= 0xE;

  switch (_dow)
  {
    case 2:
      screen[23] |= 0x1;
      break;
    case 3:
      screen[23] |= 0x2;
      break;
    case 4:
      screen[23] |= 0x4;
      break;
    case 5:
      screen[23] |= 0x8;
      break;
    case 6:
      screen[22] |= 0x8;
      break;
    case 7:
      screen[22] |= 0x4;
      break;
    case 1:
      screen[24] |= 0x1;
      break;
  }
}

inline void bat(uint8_t num) //Батарейка
{
  screen[22] &= 0xC;
  screen[21] = 0x0;

  screen[21] |= (batt[num] & 0xF0) >> 4;
  screen[22] |= batt[num] & 0x0F;
}

inline void digit(uint8_t place, uint8_t num) // Большие цифры
{
  uint8_t place2 = place + 1;
  if (place == 7) place2 = 31;

  screen[place] = 0x0;
  screen[place2] &= 0x8;

  screen[place] |= (dig1[num] & 0xF0) >> 4;
  screen[place2] |= dig1[num] & 0x0F;
}

inline void mon_day(uint8_t mon, uint8_t _day) //Маленькие цифры (Месяц, число, год)
{
  screen[25] = 0x0;
  screen[26] = 0x0;
  screen[27] &= 0x8;
  screen[28] = 0x0;
  screen[29] = 0x0;

  screen[25] |= (dig2[_day % 10] & 0xF0) >> 4;
  screen[26] |= (dig2[_day % 10] & 0x0F);

  if (_day > 9) screen[26] |= 0x1;
  screen[27] |= int(_day / 10);
  if (int(_day / 10) == 2) screen[27] |= 0x6;

  screen[28] |= (dig2[mon % 10] & 0xF0) >> 4;
  screen[29] |= (dig2[mon % 10] & 0x0F) + (mon / 10);
}

inline void ala(uint8_t num) //Будильник
{
  screen[0] &= 0x1;
  screen[6] &= 0x7;

  switch (num)
  {
    case 0:
      screen[6] |= 0x8;
      break;
    case 1:
      screen[0] |= 0x8;
      break;
    case 2:
      screen[0] |= 0x4;
      break;
    case 3:
      screen[0] |= 0x2;
      break;
  }
}
