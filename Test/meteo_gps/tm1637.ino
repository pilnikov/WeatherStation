
void tm_setup()
{
  tm1637.set();
  tm1637.init();
}

void tm_loop()
{
  mod_sel();
  if (disp_mode > 9 || disp_mode < 0) disp_mode = 0;
  scr_buff_t buf2 = ss_prep();

  buf2.dig[1] |= colon << 7;
  s7_write_all(buf2);
}


scr_buff_t ss_prep()
{
  String bstr = String("    ");

  switch (disp_mode)
  {
    case 1: //температура канал №0
      if (abs(st[0]) < 99)
        bstr = "t" + (st[0] < 0 ? String("-") : String(" ")) + (abs(st[0]) < 10 ? String(" ") : String("")) + String(abs(st[0]));
      else
      {
        disp_mode ++;
      }
      break;

    case 2: //температура  канал №1
      if (abs(st[1]) < 99)
        bstr = "t" + (st[1] < 0 ? String(":") : String("^")) + (abs(st[1]) < 10 ? String(" ") : String("")) + String(abs(st[1]));
      else
      {
        disp_mode ++;
      }
      break;

    case 3: //температура  канал №2
      if (abs(st[2]) < 99)
        bstr = "t" + (st[2] < 0 ? String(";") : String("_")) + (abs(st[2]) < 10 ? String(" ") : String("")) + String(abs(st[2]));
      else
      {
        disp_mode ++;
      }
      break;

    case 4: //влажность канал №0
      if (sh[0] > 0)
        bstr = "H " + (sh[0] < 10 ? String(" ") : String("")) + String(sh[0]);
      else
      {
        disp_mode ++;
      }
      break;

    case 5: //влажность канал №1
      if (sh[1] > 0)
        bstr = "H^" + (sh[1] < 10 ? String(" ") : String("")) + String(sh[1]);
      else
      {
        disp_mode ++;
      }
      break;

    case 6: //влажность канал №2
      if (sh[2] > 0)
        bstr = "H_" + (sh[2] < 10 ? String(" ") : String("")) + String(sh[2]);
      else
      {
        disp_mode ++;
      }
      break;

    case 7: //Давление
      if (p1 > 700)
        bstr = "p" + String(p1);
      else
      {
        disp_mode ++;
      }
      break;

    case 8: //День недели, дата
      bstr = dow_sel(weekday()) + (day() < 10 ? String("0") : String("")) + String(day());
      break;

    case 9: //Месяц, год
      bstr = (month() < 10 ? String("0") : String("")) + String(month()) + String(year() % 100);
      break;

    case 10: //Секунды
      bstr = "c " + (second() < 10 ? String(" ") : String("")) + String(second());
      break;
    /*
        case 11: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) bstr = "AL--";
          else bstr = (rtc_data.a_hour < 10 ? String(" ") : String("")) + String(rtc_data.a_hour) + (rtc_data.a_min < 10 ? String("0") : String(""))
                        + String(rtc_data.a_min);
          break;

        case 12: //Текущая яркость
          bstr = "L " + (cur_br < 10 ? String(" ") : String("")) + String(cur_br);
          break;
    */
    default://Текущее время
      uint8_t h = hour();
      // Do 24 hour to 12 hour format conversion when required.
      //if (conf_data.use_pm && hour() > 12) h = hour() - 12;
      bstr = (h < 10 ? String(" ") : String("")) + String(h) + (minute() < 10 ? String("0") : String("")) + String(minute());
      break;
  }
  scr_buff_t dig1 = prn7(bstr);
  return dig1;
}

void mod_sel()
{
  if (disp_mode != 0 & (millis() - irq_end[0] > 5000) || (disp_mode == 0 & (millis() - irq_end[0] > 35000)))
  {
    disp_mode++;
    //Serial.print("mod..."); Serial.println(mod);
    irq_end[0] = millis();
  }
}


String dow_sel(uint8_t _dow)
{
  const String ndow[7] = {"Вc", "пН", "Вt", "сP", "Чt", "пt", "сБ"};

  String sbuf = ndow[_dow - 1];

  return sbuf;
}


void s7_write_all(scr_buff_t buff)
{
  byte dig2[4];

  dig2[0] = buff.dig[0];
  dig2[1] = buff.dig[1];
  dig2[2] = buff.dig[2];
  dig2[3] = buff.dig[3];

  tm1637.display(0, dig2[0]);
  tm1637.display(1, dig2[1]);
  tm1637.display(2, dig2[2]);
  tm1637.display(3, dig2[3]);
}

scr_buff_t prn7(String source)
{
  uint16_t i = 0, j = 0;
  uint16_t k = source.length();

  scr_buff_t buf_dig;

  byte n;

  while (i < k)
  {
    n = source[i]; i++;

    if (n == 0xD0 || n == 0xD1)   // UTF-8 handling
    {
      switch (n)
      {
        /*
          Позиция символа в массиве charArray вычисляется по формуле:
          Для символов от А до п, с кодами от 0x90 по 0xBF, с позициями от сто девяносто второй до двести тридцать девятой (0xC0 по 0xEF)
          pos = (код символа + 0х30). 0xEF - 0xBF = 0x30
           Для символов от р до я, с кодами от 0x80 по 0x8F, с позициями от двести сороковой     по двести пятьдесят  пятую (0xF0 по 0xFF)
           pos = (код символа + 0х70). 0xFF - 0x8F = 0x70
           ё и Ё стоят особняком (выбиваются из общего строя) поэтому для них отдельные строки.
        */

        case 0xD0:
          n = source[i]; i++;
          if (n == 0x81)
          {
            buf_dig.dig[j] = 0xA8; // Ё
            j++;
          }
          else if (n >= 0x90 && n <= 0xBF)
          {
            buf_dig.dig[j] = pgm_read_byte_near(charArray + n + 0x30); // от А до п
            j++;
          }
          break;
        case 0xD1:
          n = source[i]; i++;
          if (n == 0x91)
          {
            buf_dig.dig[j] = 0xB8; // ё
            j++;
          }
          else if (n >= 0x80 && n <= 0x8F)
          {
            buf_dig.dig[j] = pgm_read_byte_near(charArray + n + 0x70); // от р до я
            j++;
          }
          break;
      }
    }
    else
    {
      if (n != 0x2E)
      {
        buf_dig.dig[j] = pgm_read_byte_near(charArray + n);
        j++;
      }
      else buf_dig.dig[j - 1] |= 0x80;
    }
    if (j > 7) j = 0;
  }
  return buf_dig;
}
