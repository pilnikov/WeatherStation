
void ts_msg(byte disp, bool dir)
{
  switch (disp)
  {
    case 0:
      if (dir) DBG_OUT_PORT.println("True get data from TS. Please wait!");
      else DBG_OUT_PORT.println("True put data to TS. Please wait!");
      break;
    case 1:
      lcd -> clear();
      lcd -> setCursor(0, 0);
      st1 = "Put data to TS";
      if (conf_data.rus_disp) st1 = "Шлем инфу на TS ";
      if (dir)
      {
        st1 = "Get data from TS";
        if (conf_data.rus_disp) st1 = "Берем инфу c TS ";
      }
      lcd -> print(f_dsp.lcd_rus(st1));

      lcd -> setCursor(2, 1);
      st1 = "Please wait!";
      if (conf_data.rus_disp) st1 = "Минуточку! ";
      lcd -> print(f_dsp.lcd_rus(st1));
      break;
    case 2:
      st1 = "> tS";
      if (dir)st1 = "< tS";
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 3:
      st1 = "> tS";
      if (dir)st1 = "< tS";
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 4:
      m7219 -> fillScreen(LOW);
      m7219 -> setCursor(3, 0);
      st1 = "->TS";
      if (dir)st1 = "<-TS";
      m7219 -> print(st1);
      m7219 -> write(); // Send bitmap to display
      break;
    case 5:
      st1 = "> tS";
      if (dir)st1 = "< tS";
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 6:
      st1 = "> tS";
      if (dir)st1 = "< tS";
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 7:
      st1 = "> tS";
      if (dir)st1 = "< tS";
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 8:
      m1632 -> clear();
      m1632 -> setTextColor(0, 3);
      m1632 -> setCursor(3, 0);
      st1 = "->TS";
      if (dir)st1 = "<-TS";
      m1632 -> print(st1);
      m1632 -> render(); // Send bitmap to display
      break;

    default:
      DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
      break;
  }
}

String ntp_msg(uint8_t disp, uint8_t stage)
{
  String bstr = String();
  switch (stage)
  {
    case 0:
      switch (disp)
      {
        case 0:
          DBG_OUT_PORT.println("True sync with NTP server. Please wait!");
          break;
        case 1:
          bstr = "Sync with NTP";
          if (conf_data.rus_disp) bstr = "Берем инфу c NTP ";
          bstr = "Please wait!";
          if (conf_data.rus_disp) bstr = "Минуточку! ";
          break;
        case 2:
          bstr = "HtП-";
          break;
        case 3:
          bstr = "HtП-";
          break;
        case 4:
          bstr =  "-NTP-";
          break;
        case 5:
          bstr = "HtП-";
          break;
        case 6:
          bstr = "HtП-";
          break;
        case 7:
          bstr = "HtП-";
          break;
        case 8:
          bstr =  "-NTP-";
          if (conf_data.rus_disp) bstr = "-НТП-";
          break;
        default:
          DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
          break;
      }
      break;

    case 1:
      switch (disp)
      {
        case 0:
          DBG_OUT_PORT.println("True sync with NTP server. Please wait!");
          break;
        case 1:
          bstr = "Sync with NTP";
          if (conf_data.rus_disp) bstr = "Берем инфу c NTP ";
          bstr = "Please wait!";
          if (conf_data.rus_disp) bstr = "Минуточку! ";
          break;
        case 2:
          bstr = "HtП-";
          break;
        case 3:
          bstr = "HtП-";
          break;
        case 4:
          bstr =  "-NTP-";
          break;
        case 5:
          bstr = "HtП-";
          break;
        case 6:
          bstr = "HtП-";
          break;
        case 7:
          bstr = "HtП-";
          break;
        case 8:
          bstr =  "-NTP-";
          if (conf_data.rus_disp) bstr = "-НТП-";
          break;
        default:
          DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
          break;
      }
      break;

    case 2:
      switch (disp)
      {
        case 0:
          DBG_OUT_PORT.println("Sync with NTP server failed!");
          break;
        case 1:
          bstr = "Sync with NTP";
          if (conf_data.rus_disp) bstr = "Берем инфу c NTP ";
          bstr = "Failed!";
          if (conf_data.rus_disp) bstr = "Неудача! ";
          break;
        case 2:
          bstr = "HtПn";
          break;
        case 3:
          bstr = "HtПn";
          break;
        case 4:
          bstr =  "NTP-x";
          if (conf_data.rus_disp) bstr = "НТП-x";
          break;
        case 5:
          bstr = "HtПn";
          break;
        case 6:
          bstr = "HtПn";
          break;
        case 7:
          bstr = "HtПn";
          break;
        case 8:
          bstr =  "NTP-x";
          if (conf_data.rus_disp) bstr = "НТП-x";
          break;
        default:
          DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
          break;
      }
      break;
    default:
      DBG_OUT_PORT.printf("Invalid display type %u\n", disp);
      break;
  }
  return bstr;
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
      st1 = "Alarm #" + String(num) + " is";
      if (conf_data.rus_disp) st1 = "Будильник N" + String(num);
      lcd -> print(f_dsp.lcd_rus(st1));

      lcd -> setCursor(2, 1);
      st1 = "triggered!!! ";
      if (conf_data.rus_disp) st1 = "сработал!!! ";
      lcd -> print(f_dsp.lcd_rus(st1));
      break;
    case 2:
      st1 = "AL-" + String(num);
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 3:
      st1 = "AL-" + String(num);
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 4:
      m7219 -> fillScreen(LOW);
      m7219 -> setCursor(3, 0);
      m7219 -> print("AL-");
      m7219 -> print(num);
      m7219 -> write(); // Send bitmap to display
      break;
    case 5:
      st1 = "AL-" + String(num);
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 6:
      st1 = "AL-" + String(num);
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 7:
      st1 = "AL-" + String(num);
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
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
                  : " Нет данных от внешних датчиков - проверьте настройки ")
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
          DBG_OUT_PORT.printf("\n True connect to %s", " ", conf_data.sta_ssid);
          break;
        case 2:
          DBG_OUT_PORT.printf("\n True create a SoftAP %s", " ", conf_data.ap_ssid);
          break;
        case 3:
          DBG_OUT_PORT.print(sta);
          break;
        case 4:
          DBG_OUT_PORT.print( "\n IP : " );
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
      if (par == 1)
      {
        st1 = "True connect to";
        if (conf_data.rus_disp) st1 = "Подключаемся к";
      }
      lcd -> print(f_dsp.lcd_rus(st1));

      if (par == 2)
      {
        st1 = " True create a";
        if (conf_data.rus_disp) st1 = "Создаем";
      }
      lcd -> print(f_dsp.lcd_rus(st1));

      if (par < 3) lcd -> setCursor(0, 1);

      if (par == 1) lcd -> print( conf_data.sta_ssid );

      if (par == 2)
      {
        st1 = "SoftAP";
        if (conf_data.rus_disp) st1 = "точку доступа";
      }
      lcd -> print(f_dsp.lcd_rus(st1));

      if (par == 3)
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
        // sta_msg(disp, true); //////////////////////
      }

      if (par == 6)
      {
        lcd -> clear();
        // sta_msg(disp, false); //////////////////////
      }
      break;
    case 2:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          st1 = "CL--";
          break;
        case 2:
          st1 = "AP--";
          break;
        case 3:
          st1 = String(sta);
          break;
        case 4:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); /////////////////////////////
          break;
        case 5:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); ////////////////////////////
          break;
        case 6:
          //sta_msg(disp, false); ///////////////////////////
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 3:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          st1 = "CL--";
          break;
        case 2:
          st1 = "AP--";
          break;
        case 3:
          st1 = String(sta);
          break;
        case 4:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); ///////////////////////
          break;
        case 5:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); //////////////////////
          break;
        case 6:
          //sta_msg(disp, false); /////////////////////
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
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
          //sta_msg(disp, false); ////////////////////////////////
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
          st1 = "CL--";
          break;
        case 2:
          st1 = "AP--";
          break;
        case 3:
          st1 = String(sta);
          break;
        case 4:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); /////////////////////////////////////////
          break;
        case 5:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); /////////////////////////////////////////
          break;
        case 6:
          //sta_msg(disp, false); /////////////////////////////////////////
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
      break;
    case 6:
      switch (par) // Отображаем инфу выбранного параметра
      {
        case 1:
          st1 = "CL--";
          break;
        case 2:
          st1 = "AP--";
          break;
        case 3:
          st1 = String(sta);
          break;
        case 4:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); //////////////////////////////////
          break;
        case 5:
          st1 = String(IP_Addr[3]);
          //sta_msg(disp, true); //////////////////////////////////
          break;
        case 6:
          //sta_msg(disp, false); //////////////////////////////////
          break;
        default:
          DBG_OUT_PORT.printf("Invalid par type %u/n", par);
          break;
      }
      s7dig = f_dsp.prn7(st1);
      s7_write_all(disp, s7dig);
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
          //sta_msg(disp, false);///////////////////////////////////////////
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

