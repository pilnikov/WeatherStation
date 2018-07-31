
String callback(uint8_t disp, uint8_t type, uint8_t stage)
{
  String bstr = String();
  switch (stage)
  {
    case 0:
      switch (disp)
      {
        case 0:
          switch (type)
          {
            case 0:
              DBG_OUT_PORT.println("True sync with NTP server. Please wait!");
              break;
            case 1:
              DBG_OUT_PORT.println("True Put data to TS. Please wait!");
              break;
            case 2:
              DBG_OUT_PORT.println("True Get data from TS. Please wait!");
              break;
            case 3:
              DBG_OUT_PORT.println("True connect to WiFi. Please wait!");
              break;
            case 4:
              DBG_OUT_PORT.println("True create a AP. Please wait!");
              break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              bstr = "Sync with NTP ";
              if (conf_data.rus_disp) bstr = "Сверяемся c НТП ";
              break;
            case 1:
              st1 = "Put data to TS ";
              if (conf_data.rus_disp) st1 = "Шлем инфу на TS ";
              break;
            case 2:
              st1 = "Get data from TS ";
              if (conf_data.rus_disp) st1 = "Берем инфу c TS ";
              break;
            case 3:
              st1 = "True con to WiFi ";
              if (conf_data.rus_disp) st1 = "Цепляемся к WiFi ";
              break;
            case 4:
              st1 = "True create a AP ";
              if (conf_data.rus_disp) st1 = "Создаем Точку ";
              break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
              break;
          }
          break;
        case 5:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
              break;
          }
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
          switch (type)
          {
            case 0:
              DBG_OUT_PORT.println("Sync with NTP server. Sucsess!");
              break;
            case 1:
              DBG_OUT_PORT.println("Put data to TS. Sucsess!");
              break;
            case 2:
              DBG_OUT_PORT.println("Get data from TS. Sucsess!");
              break;
            case 3:
              DBG_OUT_PORT.println("Connect to WiFi. Sucsess!");
              break;
            case 4:
              DBG_OUT_PORT.println("Create a AP. Sucsess!");
              break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              bstr = "Time from NTP OK ";
              if (conf_data.rus_disp) bstr = "Сверка c НТП OK ";
              break;
            case 1:
              st1 = "Data to TS OK! ";
              if (conf_data.rus_disp) st1 = "Инфа на TS OK! ";
              break;
            case 2:
              st1 = "Data from TS OK! ";
              if (conf_data.rus_disp) st1 = "Инфа c TS OK! ";
              break;
            case 3:
              st1 = "WiFi connected! ";
              if (conf_data.rus_disp) st1 = "WiFi подключено! ";
              break;
            case 4:
              st1 = "AP created! ";
              if (conf_data.rus_disp) st1 = "Точка создана! ";
              break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
          }
          break;
        case 4:
          switch (type)
          {
              case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
          }
          break;
        case 5:
          switch (type)
          {
             case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
          }
          break;
        case 6:
          switch (type)
          {
             case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
          }
          break;
        case 7:
          switch (type)
          {
             case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
              break;
          }
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
          switch (type)
          {
            case 0:
              DBG_OUT_PORT.println("True sync with NTP server. Please wait!");
              break;
            case 1:
              DBG_OUT_PORT.println("True Put data to TS. Please wait!");
              break;
            case 2:
              DBG_OUT_PORT.println("True Get data from TS. Please wait!");
              break;
            case 3:
              DBG_OUT_PORT.println("True connect to WiFi. Please wait!");
              break;
            case 4:
              DBG_OUT_PORT.println("True create a AP. Please wait!");
              break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              bstr = "Sync with NTP";
              if (conf_data.rus_disp) bstr = "Сверяемся c НТП ";
              break;
            case 1:
              st1 = "Put data to TS";
              if (conf_data.rus_disp) st1 = "Шлем инфу на TS ";
              break;
            case 2:
              st1 = "Get data from TS";
              if (conf_data.rus_disp) st1 = "Берем инфу c TS ";
              break;
            case 3:
              st1 = "True con to WiFi";
              if (conf_data.rus_disp) st1 = "Цепляемся к WiFi ";
              break;
            case 4:
              st1 = "True create a AP";
              if (conf_data.rus_disp) st1 = "Создаем Точку ";
              break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
              break;
          }
          break;
        case 5:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
              break;
          }
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
