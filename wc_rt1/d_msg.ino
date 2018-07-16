void ts_msg(byte disp, bool dir)
{
  switch (disp) // Отображаем время на выбранном дисплее
  {
    case 0:
      if (dir) DBG_OUT_PORT.println("True get data from TS. Please wait!");
      else DBG_OUT_PORT.println("True put data to TS. Please wait!");
      break;
    case 1:
      lcd -> clear();
      lcd -> setCursor(0, 0);
      if (dir)
      {
        if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Берем инфу c TS"));
        else lcd -> print("Get data from TS");
      }
      else
      {
        if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Шлем инфу на TS"));
        else lcd -> print("Put data to TS");
      }
      lcd -> setCursor(2, 1);
      if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Минуточку!"));
      else lcd -> print("Please wait!");
      break;
    case 2:
      dig[0] = 0x78; /* t */
      dig[1] = 0x6D; /* 5 */
      procherk(2, 2);
      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);
      ht33 -> writeDisplay();
      break;
    case 3:
      dig[0] = 0x78; /* t */
      dig[1] = 0x6D; /* 5 */
      procherk(2, 2);

      //if (conf_data.rus_disp) dig[2] = roll_seg (dig[2]);
      tm1637 -> setSegments(dig);
      break;
    case 4:
      m7219 -> fillScreen(LOW);
      m7219 -> setCursor(3, 0);
      m7219 -> print( "TS--");
      m7219 -> write(); // Send bitmap to display
      break;
    case 5:
      dig[0] = 0x78; /* t */
      dig[1] = 0x6D; /* 5 */
      procherk(2, 2);
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;
    case 6:
      dig[0] = 0x78; /* t */
      dig[1] = 0x6D; /* 5 */
      procherk(2, 2);
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;
    case 7:
      digit (0x1, dig[0]);
      digit (0x3, dig[1]);
      digit (0x5, dig[2]);
      digit (0x7, dig[3]);
      break;
    case 8:
      m1632 -> clear();
      m1632 -> setTextColor(0, 3);
      m1632 -> setCursor(3, 0);
      m1632 -> print( "TS--");
      m1632 -> render(); // Send bitmap to display
      break;

    default:
      DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
      break;
  }

}

void ntp_msg(byte disp)
{
  uint8_t            dig[4] = {0};
  switch (disp) // Отображаем время на выбранном дисплее
  {
    case 0:
      DBG_OUT_PORT.println("True sync with NTP server. Please wait!");
      break;
    case 1:
      lcd -> clear();
      lcd -> setCursor(0, 0);
      if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Берем инфу c NTP"));
      else lcd -> print("Sync with NTP");
      lcd -> setCursor(2, 1);
      if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Минуточку!"));
      else lcd -> print("Please wait!");
      break;
    case 2:
      dig[0] = ( numbertable[16]); // "H"
      dig[1] = ( numbertable[19]); // "t"
      dig[2] = ( numbertable[22]); // "П"
      dig[3] = ( numbertable[23]); // "-"

      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);
      ht33 -> writeDisplay();
      break;
    case 3:
      dig[0] = ( numbertable[16]); // "H"
      dig[1] = ( numbertable[19]); // "t"
      dig[2] = ( numbertable[22]); // "П"
      dig[3] = ( numbertable[23]); // "-"

      //if (conf_data.rus_disp) dig[2] = roll_seg (dig[2]);
      tm1637 -> setSegments(dig);
      break;
    case 4:
      m7219 -> fillScreen(LOW);
      m7219 -> setCursor(1, 0);
      if (conf_data.rus_disp) m7219 -> print(fsys.utf8rus("-НТП-"));
      else m7219 -> print( "-NTP-");
      m7219 -> write(); // Send bitmap to display
      break;
    case 5:
      dig[0] = ( numbertable[16]); // "H"
      dig[1] = ( numbertable[19]); // "t"
      dig[2] = ( numbertable[22]); // "П"
      dig[3] = ( numbertable[23]); // "-"
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;

    case 6:
      dig[0] = ( numbertable[16]); // "H"
      dig[1] = ( numbertable[19]); // "t"
      dig[2] = ( numbertable[22]); // "П"
      dig[3] = ( numbertable[23]); // "-"
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;

    case 7:
      digit (0x1, dig[0]);
      digit (0x3, dig[1]);
      digit (0x5, dig[2]);
      digit (0x7, dig[3]);
      break;

    case 8:
      m1632 -> clear ();
      m1632 -> setTextColor(0, 3);
      m1632 -> setCursor(0, 1);
      if (conf_data.rus_disp) m1632 -> print(fsys.utf8rus("-НТП-"));
      else m1632 -> print( "-NTP-");
      m1632 -> render(); // Send bitmap to display
      break;

    default:
      DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
      break;
  }
}

void sta_msg(byte disp, uint8_t _row, uint8_t _colum, bool sta, uint16_t _delay)
{
  //  uint8_t            dig[4] = {0};
  switch (disp) // Отображаем инфу на выбранном дисплее
  {
    case 0:
      if (sta) DBG_OUT_PORT.println("Sucsess!");
      else DBG_OUT_PORT.println("Failed!");
      break;
    case 1:
      lcd -> setCursor(_row, _colum);
      if (sta)
      {
        if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("    Успешно!    "));
        else lcd ->                                 print("    Sucsess!    ");
      }
      else
      {
        if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("    Косяк!      "));
        else lcd ->                                 print("    Failed!     ");
      }
      delay(_delay);
      lcd -> clear();
      break;
    case 2:
      if (sta) dig[3] = 0x62; /* u */
      else     dig[3] = 0x54; /* п */
      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);
      ht33 -> writeDisplay();
      delay(_delay);
      break;
    case 3:
      if (sta) dig[3] = 0x62; /* u */
      else     dig[3] = 0x54; /* п */
      //if (conf_data.rus_disp && _row == 0) dig[2] = roll_seg (dig[2]);
      tm1637 -> setSegments(dig);
      delay(_delay);
      break;
    case 4:
      m7219 -> fillScreen(LOW);
      m7219 -> setCursor(_colum, _row);
      if (sta)
      {
        if (conf_data.rus_disp) m7219 -> print(fsys.utf8rus("Успех"));
        else m7219 -> print( "Sucs!");
      }
      else
      {
        if (conf_data.rus_disp) m7219 -> print(fsys.utf8rus("Косяк"));
        else m7219 -> print( "Fail!");
      }
      m7219 -> write();
      delay(_delay);
      m7219 -> fillScreen(LOW);
      m7219 -> write();
      break;

    case 5:
      if (sta) dig[3] = 0x62; /* u */
      else     dig[3] = 0x54; /* п */
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      delay(_delay);
      break;

    case 6:
      if (sta) dig[3] = 0x62; /* u */
      else     dig[3] = 0x54; /* п */
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      delay(_delay);
      break;

    case 7:
      digit (0x1, dig[0]);
      digit (0x3, dig[1]);
      digit (0x5, dig[2]);
      digit (0x7, dig[3]);
      break;

    case 8:
      m1632 -> clear();
      m1632 -> setTextColor(0, 3);
      m1632 -> setCursor(_colum, _row);
      if (sta)
      {
        if (conf_data.rus_disp) m1632 -> print(fsys.utf8rus("Успех"));
        else m1632 -> print( "Sucs!");
      }
      else
      {
        if (conf_data.rus_disp) m1632 -> print(fsys.utf8rus("Косяк"));
        else m1632 -> print( "Fail!");
      }
      m1632 -> render();
      delay(_delay);
      m1632 -> clear();
      m1632 -> render();
      break;

    default:
      DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
      break;
  }
}

void alarm_msg(byte num, byte disp)
{
  uint8_t            dig[4] = {0};
  switch (disp) // Отображаем инфу на выбранном дисплее
  {
    case 0:
      DBG_OUT_PORT.print("Alarm #"); DBG_OUT_PORT.print(num); DBG_OUT_PORT.println(" is triggered!!!");
      break;
    case 1:
      lcd -> clear();
      lcd -> setCursor(2, 0);
      if (conf_data.rus_disp)
      {
        lcd -> print(fsys.lcd_rus("Будильник N")); lcd -> print(num);
      }
      else
      {
        lcd -> print("Alarm #"); lcd -> print(num); lcd -> print(" is");
      }
      lcd -> setCursor(2, 1);
      if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("сработал!!!"));
      else lcd -> print("triggered!!!");
      break;
    case 2:
      dig[0] = 0x77; /* A */
      dig[1] = 0x38; /* L */
      dig[2] = 0x40; /* - */
      dig[3] = numbertable[num];

      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);
      ht33 -> writeDisplay();
      break;
    case 3:
      dig[0] = 0x77; /* A */
      dig[1] = 0x38; /* L */
      dig[2] = 0x40; /* - */
      dig[3] = numbertable[num];

      //      if (conf_data.rus_disp) dig[2] = roll_seg (dig[2]);
      tm1637 -> setSegments(dig);
      break;
    case 4:
      m7219 -> fillScreen(LOW);
      m7219 -> setCursor(3, 0);
      m7219 -> print("AL-");
      m7219 -> print(num);
      m7219 -> write(); // Send bitmap to display
      break;
    case 5:
      dig[0] = 0x77; /* A */
      dig[1] = 0x38; /* L */
      dig[2] = 0x40; /* - */
      dig[3] = numbertable[num];
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;
    case 6:
      dig[0] = 0x77; /* A */
      dig[1] = 0x38; /* L */
      dig[2] = 0x40; /* - */
      dig[3] = numbertable[num];
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;
    case 7:
      digit (0x1, dig[0]);
      digit (0x3, dig[1]);
      digit (0x5, dig[2]);
      digit (0x7, dig[3]);
      break;
    case 8:
      m1632 -> clear();
      m1632 -> setTextColor(1, 3);
      m1632 -> setCursor(3, 0);
      m1632 -> print("AL-");
      m1632 -> print(num);
      m1632 -> render(); // Send bitmap to display
      break;
    default:
      DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
      break;
  }
}

String pr_str(uint8_t num)
{
  String grad = "\xB0";
  if (ram_data.type_disp == 1) grad = "\xEF";

  String str = "Your IP is " + WiFi.localIP().toString() + " ";

  if (!conf_data.rus_disp)
  {
    switch (num)
    {
      case 1:
        str = "Today is " + String(dayStr(weekday())) + ' ' + String(day()) + ' ' + monthStr(month()) + ' ' + String(year()) + ' ';
        break;
      case 2:
        str = (snr_data.t1 < 99 ?  "Inside " + String(snr_data.t1) + grad + "C " : ' ')
              + (snr_data.h1 >  0 ?              String(snr_data.h1) + "% " : ' ')
              + (snr_data.t2 < 99 ? "Outside " + String(snr_data.t2) + grad + "C " : ' ')
              + (snr_data.h2 >  0 ?              String(snr_data.h2) + "% " : ' ')
              + (snr_data.p  >  0 ?   "press " + String(snr_data.p)  + "mm " : ' ');
        break;
      case 3:
        str = "The end of messages ";
        break;
    }
  }
  else
  {
    switch (num)
    {
      case 1:
        str = "Сегодня" + f_dsp.dow_name_r(weekday()) + String(day()) + f_dsp.month_name_r(month()) + String(year()) + "г. ";
        break;
      case 2:
        str = (snr_data.t1 < 99 || snr_data.h1 ? "Bнутри "  + (snr_data.t1 < 99 ? String(snr_data.t1) + grad + "C " : ' ') + (snr_data.h1 >  0 ? String(snr_data.h1) + "% " : ' ')
               : " Нет данных от внутренних датчиков - проверьте настройки ")
              +  (snr_data.t2 < 99 || snr_data.h2 > 0 ?
                  "Снаружи "
                  + (conf_data.use_pp == 2 ? wf_data_cur.descript + " ветер" + f_dsp.wind_dir_r(wf_data_cur.wind_dir) + String(wf_data_cur.wind_min) + "м/с " : "")
                  + (snr_data.t2 < 99 ? String(snr_data.t2) + grad + "C " : ' ') + (snr_data.h2 > 0 ? String(snr_data.h2) + "% " : ' ')
                  : " Нет данных от внешних датчиков - проверьте настройки")
              + (snr_data.p > 700 ? "давление " + String(snr_data.p) + "мм " : "Нет данных от датчика давления - проверьте настройки ");
        break;
      case 3:
        switch (conf_data.use_pp)
        {
          case 0:
            str = "";
            break;
          case 1:
            str = (wf_data.temp_min > -99 ? "Прогноз погоды от GM на" + f_dsp.td_name_r(wf_data.tod) + String(wf_data.day) + f_dsp.month_name_r(wf_data.month) + " : "
                   + " температура " + String(wf_data.temp_min) + " " + String(wf_data.temp_max) + grad + "C "
                   + "ветер" + f_dsp.wind_dir_r(wf_data.wind_dir) + String(wf_data.wind_max) + "-" + String(wf_data.wind_min) + "м/с " + f_dsp.prc_type_r(wf_data.prec, wf_data.spower, wf_data.rpower)
                   + "oтн. влажность " + String(wf_data.hum_max) + "% "
                   + "давление " + String(wf_data.press_max) + "мм " : "Нет данных по прогнозу погоды - проверьте настройки ");
            break;
          case 2:
            str = (wf_data.temp_min > -99 ? "Прогноз погоды от OWM на " + String(wf_data.day) + f_dsp.month_name_r(wf_data.month) + " : "
                   + wf_data.descript + " температура " + String(wf_data.temp_min) + " " + String(wf_data.temp_max) + grad + "C "
                   + "ветер" + f_dsp.wind_dir_r(wf_data.wind_dir) + String(wf_data.wind_min) + "м/с "
                   + "oтн. влажность " + String(wf_data.hum_min) + "% "
                   + "давление " + String(wf_data.press_min) + "мм " : "Нет данных по прогнозу погоды - проверьте настройки ");
            break;
        }
        break;
      case 4:
        str = "Будильник " + (rtc_data.a_hour < 24 && rtc_data.a_min < 59 ? "зазвонит в " + String(rtc_data.a_hour) + ':' + String(rtc_data.a_min) : "не установлен ")
              + " Текущая яркость: " + String(cur_br)
              + " Ваш IP: " +  WiFi.localIP().toString() + " ";
        break;
    }
  }
  return str;
}

//--------------------------------------------------------------------

void wifi_conn( byte par, byte sta, byte disp)
{
  switch (disp) // Отображаем инфу на выбранном дисплее
  {
    case 0:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          DBG_OUT_PORT.println();
          DBG_OUT_PORT.printf( "True connect to %s", " ", conf_data.sta_ssid);
          break;
        case 2:
          DBG_OUT_PORT.print( " True create a SoftAP " );
          break;
        case 3:
          DBG_OUT_PORT.print(sta);
          break;
        case 4:
          DBG_OUT_PORT.print( " IP : " );
          DBG_OUT_PORT.println(IP_Addr);
          break;
        case 5:
          DBG_OUT_PORT.print( " IP : " );
          DBG_OUT_PORT.println(IP_Addr);
          break;
        case 6:
          DBG_OUT_PORT.println(" Failed");
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      break;
    case 1:
      if (par < 3)
      {
        lcd -> clear();
        lcd -> setCursor(0, 0);
      }
      if ( par == 1)
      { if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus( "Подключаемся к" ));
        else lcd -> print( "True connect to" );
      }
      if ( par == 2)
      {
        if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus( "Создаем" ));
        else lcd -> print( " True create a" );
      };
      if (par < 3) lcd -> setCursor(0, 1);

      if ( par == 1) lcd -> print( conf_data.sta_ssid );

      if ( par == 2)
      {
        if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus( "точку доступа" ));
        else lcd -> print( "SoftAP" );
      }
      if ( par == 3)
      {
        lcd -> setCursor(15, 1);
        lcd -> print(sta);
      }
      if (par == 4 || par == 5)
      {
        lcd -> clear();
        lcd -> setCursor(0, 0);
        lcd -> print( "IP: ");
        lcd -> print(IP_Addr[3]);
        sta_msg(disp, 0, 1, true, 2000);
      }

      if (par == 6)
      {
        lcd -> clear();
        sta_msg(disp, 0, 1, false, 2000);
      }
      break;
    case 2:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          dig[0] = 0x39; // C /
          dig[1] = 0x38; // L /
          procherk(2, 2);
          break;
        case 2:
          dig[0] = 0x77; // A /
          dig[1] = 0x73; // P /
          procherk(2, 2);
          break;
        case 3:
          dig[3] = numbertable[sta];
          break;
        case 4:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 5:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 6:
          sta_msg(disp, 0, 0, false, 2000);
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }

      ht33 -> writeDigitRaw(0, dig[0]);
      ht33 -> writeDigitRaw(1, dig[1]);
      ht33 -> writeDigitRaw(3, dig[2]);
      ht33 -> writeDigitRaw(4, dig[3]);
      ht33 -> writeDisplay();
      break;
    case 3:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          dig[0] = 0x39; // C /
          dig[1] = 0x38; // L /
          procherk(2, 2);
          break;
        case 2:
          dig[0] = 0x77; // A /
          dig[1] = 0x73; // P /
          procherk(2, 2);
          break;
        case 3:
          dig[3] = numbertable[sta];
          break;
        case 4:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 5:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 6:
          sta_msg(disp, 0, 0, false, 2000);
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }

      //if (conf_data.rus_disp) dig[2] = roll_seg (dig[2]);
      tm1637 -> setSegments(dig);
      break;
    case 4:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          m7219 -> fillScreen(LOW);
          m7219 -> setCursor(1, 0);
          m7219 -> print("CL--");
          break;
        case 2:
          m7219 -> fillScreen(LOW);
          m7219 -> setCursor(1, 0);
          m7219 -> print("AP--");
          break;
        case 3:
          m7219 -> drawChar(25, -1, (sta == 3 ? 'v' : 'x'), HIGH, LOW, 1);
          break;
        case 4:
          m7219 -> fillScreen(LOW);
          m7219 -> setCursor(1, 0);
          m7219 -> print("CL");
          m7219 -> print(IP_Addr[3]);
          m7219 -> write(); // Send bitmap to display
          delay(2000);
          break;
        case 5:
          m7219 -> fillScreen(LOW);
          m7219 -> setCursor(1, 0);
          m7219 -> print("AP");
          m7219 -> print(IP_Addr[3]);
          m7219 -> write(); // Send bitmap to display
          delay(2000);
          break;
        case 6:
          sta_msg(disp, 1, 0, false, 2000);
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      m7219 -> write(); // Send bitmap to display
      break;
    case 5:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          dig[0] = 0x39; // C /
          dig[1] = 0x38; // L /
          procherk(2, 2);
          break;
        case 2:
          dig[0] = 0x77; // A /
          dig[1] = 0x73; // P /
          procherk(2, 2);
          break;
        case 3:
          dig[3] = numbertable[sta];
          break;
        case 4:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 5:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 6:
          sta_msg(disp, 0, 0, false, 2000);
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;
    case 6:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          dig[0] = 0x39; // C /
          dig[1] = 0x38; // L /
          procherk(2, 2);
          break;
        case 2:
          dig[0] = 0x77; // A /
          dig[1] = 0x73; // P /
          procherk(2, 2);
          break;
        case 3:
          dig[3] = numbertable[sta];
          break;
        case 4:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 5:
          dig[0] = ( numbertable[IP_Addr[3] / 100]);
          dig[1] = ( numbertable[(IP_Addr[3] / 10) % 10]);
          dig[2] = ( numbertable[IP_Addr[3] % 10]);
          sta_msg(disp, 0, 0, true, 2000);
          break;
        case 6:
          sta_msg(disp, 0, 0, false, 2000);
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }

      max7 -> setRow(0, 0, fsys.mir_seg(dig[0]));
      max7 -> setRow(0, 1, fsys.mir_seg(dig[1]));
      max7 -> setRow(0, 2, fsys.mir_seg(dig[2]));
      max7 -> setRow(0, 3, fsys.mir_seg(dig[3]));
      break;

    case 7:
      break;
    case 8:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          m1632 -> clear();
          m1632 -> setTextColor(0, 3);
          m1632 -> setCursor(1, 0);
          m1632 -> print("CL--");
          break;
        case 2:
          m1632 -> clear();
          m1632 -> setTextColor(0, 3);
          m1632 -> setCursor(1, 0);
          m1632 -> print("AP--");
          break;
        case 3:
          m1632 -> drawChar(25, -1, (sta == 3 ? 'v' : 'x'), 0, 3, 1);
          break;
        case 4:
          m1632 -> clear();
          m1632 -> setTextColor(0, 3);
          m1632 -> setCursor(1, 0);
          m1632 -> print("CL");
          m1632 -> print(IP_Addr[3]);
          m1632 -> render(); // Send bitmap to display
          delay(2000);
          break;
        case 5:
          m1632 -> clear();
          m1632 -> setTextColor(0, 3);
          m1632 -> setCursor(1, 0);
          m1632 -> print("AP");
          m1632 -> print(IP_Addr[3]);
          m1632 -> render(); // Send bitmap to display
          delay(2000);
          break;
        case 6:
          sta_msg(disp, 1, 0, false, 2000);
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      m1632 -> render(); // Send bitmap to display
      break;
    default:
      DBG_OUT_PORT.printf("Invalid display type %u/n", disp);
      break;
  }
}

