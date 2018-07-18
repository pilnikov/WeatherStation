
void sevenseg_init()
{
  uint8_t delaytime = 250;
  switch (ram_data.type_disp)
  {
    case 2:
      ht33 = new Adafruit_7segment;
      ht33 -> begin(ram_data.ht_addr);
      dig[0] = 0x06; /* 1 */
      dig[1] = 0x7D; /* 6 */
      dig[2] = 0x4F; /* 3 */
      dig[3] = 0x4F; /* 3 */

      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);
      ht33 -> setBrightness(0xf);
      ht33 -> writeDisplay();
      break;

    case 3:
      dig[0] = 0x06; /* 1 */
      dig[1] = 0x7D; /* 6 */
      dig[2] = 0x4F; /* 3 */
      dig[3] = 0x07; /* 7 */
      //if (conf_data.rus_disp) dig[2] = roll_seg (dig[2]);

      tm1637 = new TM1637Display(TM1637_CLK, TM1637_DIO);
      tm1637 -> setBrightness(0xf);
      tm1637 -> setSegments(dig);
      break;

    case 5:
      max7 = new LedControl(kDiPin, kClPin, pinCS, 1);
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
      max7 = new LedControl(kDiPin, kClPin, pinCS, 1);
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
      ht21 = new HT1621(kCePin, kClPin, kDiPin); // ss, rw, data
      ht21 -> begin();

      ht21 -> sendCommand(HT1621::RC256K);
      ht21 -> sendCommand(HT1621::BIAS_THIRD_4_COM);
      ht21 -> sendCommand(HT1621::SYS_EN);
      ht21 -> sendCommand(HT1621::LCD_ON);

      uint8_t i = 0;
      // clear memory
      ht21 -> clear_all();
      for (i = 0; i < 32; i++) buf[i] = 0x0;

      digit (1, 1); /* 1 */
      digit (3, 2); /* 2 */
      digit (5, 6); /* 6 */
      digit (7, 1); /* 1 */

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

void inline ss_prep()
{
  switch (mod_sel())
  {
    case 1: //Внутренняя температура
      dig[0] = ( numbertable[19]); // "t"
      dig[1] = ( snr_data.t1 < 0 ? numbertable[23] : numbertable[24]); // "-"/" "
      if ( snr_data.t1 < 99 )
      {
        dig[2] = ( numbertable[abs(snr_data.t1) / 10]);
        dig[3] = ( numbertable[abs(snr_data.t1) % 10]);
      }
      else procherk(2, 2);
      if (ram_data.type_disp == 6)
      {
        dig[4] = ( numbertable[19] | (true << 7)); // "t."
        dig[5] = ( snr_data.t2 < 0 ? numbertable[23] : numbertable[24]); // "-"/" "
        if ( snr_data.t2 < 99 )
        {
          dig[6] = ( numbertable[abs(snr_data.t2) / 10]);
          dig[7] = ( numbertable[abs(snr_data.t2) % 10]);
        }
        else procherk(6, 2);
      }

      break;

    case 2: //Внутренняя влажность
      dig[0] = ( numbertable[16]); // "H"
      dig[1] = ( numbertable[24]); // " "
      if ( snr_data.h1 > 0 )
      {
        dig[2] = ( numbertable[snr_data.h1 / 10]);
        dig[3] = ( numbertable[snr_data.h1 % 10]);
      }
      else procherk(2, 2);
      if (ram_data.type_disp == 6)
      {
        dig[4] = ( numbertable[16] | (true << 7)); // "H."
        dig[5] = ( numbertable[24]); // " "
        if ( snr_data.h2 > 0 ) {
          dig[6] = ( numbertable[snr_data.h2 / 10]);
          dig[7] = ( numbertable[snr_data.h2 % 10]);
        }
        else procherk(6, 2);

      }
      break;

    case 3: //Внешняя температура
      if (ram_data.type_disp == 6)
      {
        dig[0] = ( numbertable[18]); // "P"
        if ( snr_data.p > 0 ) {
          dig[1] = ( numbertable[snr_data.p / 100]);
          dig[2] = ( numbertable[(snr_data.p / 10) % 10]);
          dig[3] = ( numbertable[snr_data.p % 10]);
          dig[4] = ( numbertable[24]);
          dig[5] = ( numbertable[24]);
          dig[6] = ( numbertable[24]);
          dig[7] = ( numbertable[24]);
        }
        else procherk(1, 7);
      }
      else
      {
        dig[0] = ( numbertable[19] | (true << 7)); // "t."
        dig[1] = ( snr_data.t2 < 0 ? numbertable[23] : numbertable[24]); // "-"/" "
        if ( snr_data.t2 < 99 )
        {
          dig[2] = ( numbertable[abs(snr_data.t2) / 10]);
          dig[3] = ( numbertable[abs(snr_data.t2) % 10]);
        }
        else procherk(2, 2);
      }
      break;

    case 4: //Внешняя влажность
      if (ram_data.type_disp == 6)
      {
        dow_sel(weekday());
        dig[2] = ( numbertable[day()   / 10]);
        dig[3] = ( numbertable[day()   % 10]);
        dig[4] = ( numbertable[month() / 10]);
        dig[5] = ( numbertable[month() % 10]);
        dig[6] = ( numbertable[year()  / 10 % 10]);
        dig[7] = ( numbertable[year()  % 10]);
      }
      else
      {
        dig[0] = ( numbertable[16] | (true << 7)); // "H."
        dig[1] = ( numbertable[24]); // " "
        if ( snr_data.h2 > 0 ) {
          dig[2] = ( numbertable[snr_data.h2 / 10]);
          dig[3] = ( numbertable[snr_data.h2 % 10]);
        }
        else procherk(2, 2);
      }
      break;

    case 5: //Давление
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        dig[0] = ( h < 10 ? numbertable[24] : numbertable[h / 10]);
        dig[1] = ( numbertable[h % 10]);
        dig[2] = ( numbertable[23]);
        dig[3] = ( numbertable[minute() / 10]);
        dig[4] = ( numbertable[minute() % 10]);
        dig[5] = ( numbertable[23]);
        dig[6] = ( numbertable[second() / 10]);
        dig[7] = ( numbertable[second() % 10]);
      }
      else
      {
        dig[0] = ( numbertable[18]); // "P"
        if ( snr_data.p > 0 ) {
          dig[1] = ( numbertable[snr_data.p / 100]);
          dig[2] = ( numbertable[(snr_data.p / 10) % 10]);
          dig[3] = ( numbertable[snr_data.p % 10]);
        }
        else procherk(1, 7);
      }
      break;

    case 6: //День недели, дата
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        dig[0] = ( h < 10 ? numbertable[24] : numbertable[h / 10]);
        dig[1] = ( numbertable[h % 10]);
        dig[2] = ( numbertable[23]);
        dig[3] = ( numbertable[minute() / 10]);
        dig[4] = ( numbertable[minute() % 10]);
        dig[5] = ( numbertable[23]);
        dig[6] = ( numbertable[second() / 10]);
        dig[7] = ( numbertable[second() % 10]);
      }
      else
      {
        dow_sel(weekday());
        dig[2] = ( numbertable[day() / 10]);
        dig[3] = ( numbertable[day() % 10]);
      }
      break;

    case 7: //Месяц, год
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        dig[0] = ( h < 10 ? numbertable[24] : numbertable[h / 10]);
        dig[1] = ( numbertable[h % 10]);
        dig[2] = ( numbertable[23]);
        dig[3] = ( numbertable[minute() / 10]);
        dig[4] = ( numbertable[minute() % 10]);
        dig[5] = ( numbertable[23]);
        dig[6] = ( numbertable[second() / 10]);
        dig[7] = ( numbertable[second() % 10]);
      }
      else
      {
        dig[0] = ( numbertable[month() / 10]);
        dig[1] = ( numbertable[month() % 10]);
        dig[2] = ( numbertable[year() / 10 % 10]);
        dig[3] = ( numbertable[year() % 10]);
      }
      break;

    case 8: //Секунды
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        dig[0] = ( h < 10 ? numbertable[24] : numbertable[h / 10]);
        dig[1] = ( numbertable[h % 10]);
        dig[2] = ( numbertable[23]);
        dig[3] = ( numbertable[minute() / 10]);
        dig[4] = ( numbertable[minute() % 10]);
        dig[5] = ( numbertable[23]);
        dig[6] = ( numbertable[second() / 10]);
        dig[7] = ( numbertable[second() % 10]);
      }
      else
      {
        dig[0] = ( numbertable[12]);
        dig[1] = ( numbertable[24]);
        dig[2] = ( numbertable[second() / 10]);
        dig[3] = ( numbertable[second() % 10]);
      }
      break;

    case 9: //Актуальный будильник
      if (rtc_data.a_hour == 62) procherk(0, 2);
      else
      {
        dig[0] = ( numbertable[rtc_data.a_hour / 10]);
        dig[1] = ( numbertable[rtc_data.a_hour % 10]);
      }
      if (rtc_data.a_min == 62) procherk(2, 2);
      else
      {
        dig[2] = ( numbertable[rtc_data.a_min / 10]);
        dig[3] = ( numbertable[rtc_data.a_min % 10]);
      }
      break;

    case 10: //Текущая яркость
      dig[0] = ( numbertable[17]);
      dig[1] = ( numbertable[24]);
      dig[2] = ( numbertable[cur_br / 10]);
      dig[3] = ( numbertable[cur_br % 10]);
      break;

    default://Текущее время
      if (ram_data.type_disp == 6)
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;
        dig[0] = ( h < 10 ? numbertable[24] : numbertable[h / 10]);
        dig[1] = ( numbertable[h % 10]);
        dig[2] = ( numbertable[23]);
        dig[3] = ( numbertable[minute() / 10]);
        dig[4] = ( numbertable[minute() % 10]);
        dig[5] = ( numbertable[23]);
        dig[6] = ( numbertable[second() / 10]);
        dig[7] = ( numbertable[second() % 10]);
      }
      else
      {
        uint8_t h = hour();
        // Do 24 hour to 12 hour format conversion when required.
        if (conf_data.use_pm && hour() > 12) h = hour() - 12;

        dig[0] = ( h < 10 ? numbertable[24] : numbertable[h / 10]);
        dig[1] = ( numbertable[h % 10]);
        dig[2] = ( numbertable[minute() / 10]);
        dig[3] = ( numbertable[minute() % 10]);
      }
      break;
  }
  //  return d;
}

void sevenseg()
{
  ss_prep();

  if (ram_data.type_disp != 6)
  {
    dig[1] |= blinkColon << 7;
    dig[3] |= (web_cli || web_ap) << 7;
  }
  else dig[7] |= (web_cli || web_ap) << 7;

  //  if (conf_data.rus_disp) dig[2] = roll_seg (dig[2]);

  switch (ram_data.type_disp)
  {
    case 2:
      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);

      ht33 -> drawColon(blinkColon);

      ht33 -> setBrightness(cur_br);
      ht33 -> writeDisplay();
      break;

    case 3:
      tm1637 -> setBrightness(cur_br);
      tm1637 -> setSegments(dig);
      break;

    case 5:
      max7 -> setIntensity(0, cur_br);
      max7 -> setRow(0, 0, f_dsp.mir_seg(dig[0]));
      max7 -> setRow(0, 1, f_dsp.mir_seg(dig[1]));
      max7 -> setRow(0, 2, f_dsp.mir_seg(dig[2]));
      max7 -> setRow(0, 3, f_dsp.mir_seg(dig[3]));
      break;

    case 6:
      max7 -> setIntensity(0, cur_br);
      max7 -> setRow(0, 0, f_dsp.mir_seg(dig[0]));
      max7 -> setRow(0, 1, f_dsp.mir_seg(dig[1]));
      max7 -> setRow(0, 2, f_dsp.mir_seg(dig[2]));
      max7 -> setRow(0, 3, f_dsp.mir_seg(dig[3]));
      max7 -> setRow(0, 4, f_dsp.mir_seg(dig[4]));
      max7 -> setRow(0, 5, f_dsp.mir_seg(dig[5]));
      max7 -> setRow(0, 6, f_dsp.mir_seg(dig[6]));
      max7 -> setRow(0, 7, f_dsp.mir_seg(dig[7]));
      break;

    case 7:
      for (byte i = 0; i < 32; i++) buf[i] = 0x0;

      digit (1, minute() % 10);
      digit (3, minute() / 10);

      if (blinkColon) buf[4] |= 0x8; //colon

      digit (5, hour() % 10);
      if (hour() > 9) digit (7, hour() / 10);

      dow_sel (weekday());

      mon_day (month(), day());
      buf[24] |= 0x8; //frame
      buf[27] |= 0x8; //slash

      if (rtc_data.n_cur_alm < 4) ala(rtc_data.n_cur_alm);

      ht21 -> write_all(buf);

      break;
  }
}


