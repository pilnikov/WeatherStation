
void sevenseg_init()
{
  uint8_t delaytime = 250;
  String bstr = String();
  switch (ram_data.type_disp)
  {
    case 2:
      ht33 = new Adafruit_7segment;
      ht33 -> begin(ram_data.ht_addr);
      ht33 -> setBrightness(0xf);
      bstr = "1633";
      s7dig = f_dsp.prn7(bstr);
      s7_write_all(ram_data.type_disp, s7dig);
      break;

    case 3:
      tm1637 = new TM1637(CLK_PIN, DIO_PIN);
      tm1637 -> init();
      tm1637 -> set(7);
      bstr = "1637";
      s7dig = f_dsp.prn7(bstr);
      s7_write_all(ram_data.type_disp, s7dig);
      break;

    case 5:
      max7 = new LedControl(DIO_PIN, CLK_PIN, CS_PIN, 1);
      max7 -> shutdown(0, false);
      max7 -> setIntensity(0, 8);
      max7 -> clearDisplay(0);

      max7 -> setDigit(0, 0, 7, false);
      delay(delaytime);
      max7 -> setDigit(0, 1, 2, false);
      delay(delaytime);
      max7 -> setDigit(0, 2, 1, false);
      delay(delaytime);
      max7 -> setDigit(0, 3, 9, false);
      delay(delaytime);
      max7 -> setDigit(0, 0, 7, false);
      delay(delaytime);
      max7 -> setDigit(0, 1, 5, false);
      delay(delaytime);
      max7 -> setDigit(0, 2, 4, false);
      delay(delaytime);
      max7 -> setChar (0, 3, 'd', false);
      delay(delaytime);
      max7 -> clearDisplay(0);
      break;

    case 6:
      max7 = new LedControl(DIO_PIN, CLK_PIN, CS_PIN, 1);
      max7 -> shutdown(0, false);
      max7 -> setIntensity(0, 8);
      max7 -> clearDisplay(0);

      max7 -> setDigit(0, 0, 7, false);
      delay(delaytime);
      max7 -> setDigit(0, 1, 2, false);
      delay(delaytime);
      max7 -> setDigit(0, 2, 1, false);
      delay(delaytime);
      max7 -> setDigit(0, 3, 9, false);
      delay(delaytime);
      max7 -> setDigit(0, 4, 7, false);
      delay(delaytime);
      max7 -> setDigit(0, 5, 5, false);
      delay(delaytime);
      max7 -> setDigit(0, 6, 8, false);
      delay(delaytime);
      max7 -> setChar (0, 7, 'd', false);
      delay(delaytime);
      max7 -> clearDisplay(0);
      break;

    case 7:
      ht21 = new HT1621(CS_PIN, CLK_PIN, DIO_PIN); // ss, rw, data
      ht21 -> begin();

      ht21 -> sendCommand(HT1621::RC256K);
      ht21 -> sendCommand(HT1621::BIAS_THIRD_4_COM);
      ht21 -> sendCommand(HT1621::SYS_EN);
      ht21 -> sendCommand(HT1621::LCD_ON);

      uint8_t i = 0;
      // clear memory
      ht21 -> clear_all();
      for (i = 0; i < 32; i++) buf[i] = 0x0;

      digit(1, 1); /* 1 */
      digit(3, 2); /* 2 */
      digit(5, 6); /* 6 */
      digit(7, 1); /* 1 */

      for (i = 1; i < 8; i++)
      {
        ht21 -> write(i, buf[i]);

        bat(i - 1);
        ht21 -> write(21, buf, 2);
        delay(delaytime);
      }
      break;
  }
}

scr_buff_t ss_prep(uint8_t mod)
{
  String bstr = String();

  switch (mod)
  {
    case 1: //Внутренняя температура
      bstr = "t" + (snr_data.t1 < 0 ? String("-") : String(" ")) + (abs(snr_data.t1) < 10 ? String(" ") : String("")) + (abs(snr_data.t1) < 99 ? String(abs(snr_data.t1)) : String("--"));
      if (ram_data.type_disp == 6) bstr += "t." + (snr_data.t2 < 0 ? String("-") : String(" ")) + (abs(snr_data.t2) < 10 ? String(" ") : String("")) + (abs(snr_data.t2) < 99 ? String(abs(snr_data.t2)) : String("--"));
      break;

    case 2: //Внутренняя влажность
      bstr = "H " + (snr_data.h1 > 0 ? (abs(snr_data.h1) < 10 ? String(" ") : String("")) + String(snr_data.h1) : String("--"));
      if (ram_data.type_disp == 6) bstr += "H. " + (snr_data.h2 > 0 ? (abs(snr_data.h2) < 10 ? String(" ") : String("")) + String(snr_data.h2) : String("--"));
      break;

    case 3: //Внешняя температура
      bstr = "t." + (snr_data.t2 < 0 ? String("-") : String(" ")) + (abs(snr_data.t2) < 10 ? String(" ") : String("")) + (abs(snr_data.t2) < 99 ? String(abs(snr_data.t2)) : String("--"));
      if (ram_data.type_disp == 6) bstr = "p" + (snr_data.p > 700 ? String(snr_data.p) : String("---")) + String("    ");
      break;

    case 4: //Внешняя влажность
      bstr = "H. " + (snr_data.h2 > 0 ? (abs(snr_data.h2) < 10 ? String(" ") : String("")) + String(snr_data.h2) : String("--"));
      if (ram_data.type_disp == 6) bstr = dow_sel(weekday()) + (day() < 10 ? String("0") : String("")) +
                                            String(day()) + (month() < 10 ? String("0") : String("")) + String(month()) + String(year() % 100);
      break;

    case 5: //Давление
      bstr = "p" + (snr_data.p > 700 ? String(snr_data.p) : String("---"));
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        bstr = (h < 10 ? String(" ") : String("")) + String(h) + String("-")
               + (minute() < 10 ? String("0") : String("")) + String(minute()) + String("-")
               + (second() < 10 ? String("0") : String("")) + String(second());
      }
      break;

    case 6: //День недели, дата
      bstr = dow_sel(weekday()) + (day() < 10 ? String("0") : String("")) + String(day());
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        bstr = (h < 10 ? String(" ") : String("")) + String(h) + String("-")
               + (minute() < 10 ? String("0") : String("")) + String(minute()) + String("-")
               + (second() < 10 ? String("0") : String("")) + String(second());
      }
      break;

    case 7: //Месяц, год
      bstr = (month() < 10 ? String("0") : String("")) + String(month()) + String(year() % 100);
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        bstr = (h < 10 ? String(" ") : String("")) + String(h) + String("-")
               + (minute() < 10 ? String("0") : String("")) + String(minute()) + String("-")
               + (second() < 10 ? String("0") : String("")) + String(second());
      }
      break;

    case 8: //Секунды
      bstr = "c " + (second() < 10 ? String(" ") : String("")) + String(second());
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        bstr = (h < 10 ? String(" ") : String("")) + String(h) + String("-")
               + (minute() < 10 ? String("0") : String("")) + String(minute()) + String("-")
               + (second() < 10 ? String("0") : String("")) + String(second());
      }
      break;

    case 9: //Актуальный будильник
      if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) bstr = "AL--";
      else bstr = (rtc_data.a_hour < 10 ? String(" ") : String("")) + String(rtc_data.a_hour) + (rtc_data.a_min < 10 ? String("0") : String(""))
                    + String(rtc_data.a_min);
      break;

    case 10: //Текущая яркость
      bstr = "L " + (cur_br < 10 ? String(" ") : String("")) + String(cur_br);
      break;

    default://Текущее время
      uint8_t h = hour();
      // Do 24 hour to 12 hour format conversion when required.
      if (conf_data.use_pm && hour() > 12) h = hour() - 12;
      bstr = (h < 10 ? String(" ") : String("")) + String(h) + (minute() < 10 ? String("0") : String("")) + String(minute());
      if (ram_data.type_disp == 6) bstr = (h < 10 ? String(" ") : String("")) + String(h) + String("-")
                                            + (minute() < 10 ? String("0") : String("")) + String(minute()) + String("-")
                                            + (second() < 10 ? String("0") : String("")) + String(second());
      break;
  }
  scr_buff_t dig1 = f_dsp.prn7(bstr);
  return dig1;
}

void sevenseg()
{
  disp_mode = mod_sel(disp_mode);
  scr_buff_t buf2 = ss_prep(disp_mode);

  //---------------------------------------------------- Двоеточие
  if (ram_data.type_disp != 6)
  {
    buf2.dig[1] |= blinkColon << 7;
    buf2.dig[3] |= (web_cli || web_ap) << 7;
  }
  else buf2.dig[7] |= (web_cli || web_ap) << 7;
  //-----------------------------------------------------

  switch (ram_data.type_disp)
  {
    case 2:
      ht33 -> setBrightness(cur_br);
      s7_write_all(ram_data.type_disp, buf2);
      break;

    case 3:
      tm1637 -> set(cur_br);
      s7_write_all(ram_data.type_disp, buf2);
      break;

    case 5:
      max7 -> setIntensity(0, cur_br);
      s7_write_all(ram_data.type_disp, buf2);
      break;

    case 6:
      max7 -> setIntensity(0, cur_br);
      s7_write_all(ram_data.type_disp, buf2);
      break;

    case 7:
      for (byte i = 0; i < 32; i++) buf[i] = 0x0;

      digit(1, minute() % 10);
      digit(3, minute() / 10);

      if (blinkColon) buf[4] |= 0x8; //colon

      digit(5, hour() % 10);
      if (hour() > 9) digit (7, hour() / 10);

      dow_sel(weekday());

      mon_day(month(), day());
      buf[24] |= 0x8; //frame
      buf[27] |= 0x8; //slash

      if (rtc_data.n_cur_alm < 4) ala(rtc_data.n_cur_alm);

      ht21 -> write_all(buf);

      break;
  }
}
