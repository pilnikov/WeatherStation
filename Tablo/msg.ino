String pr_str(uint8_t num)
{
  String grad = "\xB0";
  if (ram_data.type_disp == 1) grad = "\xEF";
  byte IP_Addr[4] = {192, 168, 111, 81};

  String str = "Your IP is " + f_dsp.IP_to_str(IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3]) + " ";

  if (!conf_data.rus_disp)
  {
    switch (num)
    {
      case 1:
        "Today is " + String(dayStr(weekday())) + ' ' + String(day()) + ' ' + monthStr(month()) + ' ' + String(year()) + ' ';
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
        str = " Сегодня " + f_dsp.dow_name_r(weekday()) + String(day()) + f_dsp.month_name_r(month()) + String(year()) + "г. ";
        break;
      case 2:
        str = (snr_data.t1 < 99 || snr_data.h1 ? " Bнутри "  + (snr_data.t1 < 99 ? String(snr_data.t1) + grad + "C " : ' ') + (snr_data.h1 >  0 ? String(snr_data.h1) + "% " : ' ')
               : " Нет данных от внутренних датчиков - проверьте настройки ")
              +  (snr_data.t2 < 99 || snr_data.h2 > 0 ?
                  "Снаружи "
                  + (conf_data.use_pp == 2 ? wf_data_cur.descript + " ветер" + f_dsp.wind_dir_r(wf_data_cur.wind_dir) + String(wf_data_cur.wind_min) + "м/с " : "")
                  + (snr_data.t2 < 99 ? String(snr_data.t2) + grad + "C " : ' ') + (snr_data.h2 > 0 ? String(snr_data.h2) + "% " : ' ')
                  : " Нет данных от внешних датчиков - проверьте настройки ")
              + (snr_data.p > 700 ? " давление " + String(snr_data.p) + "мм " : " Нет данных от датчика давления - проверьте настройки ");
        break;
      case 3:
        str = (web_ap ? " Нет связи с внешним миром проверьте WiFi" : "");

        if (conf_data.use_pp == 0) str = " Нет данных по прогнозу погоды - проверьте настройки ";
       
        if (conf_data.use_pp == 1)
        {
          str = (wf_data.temp_min > -99 ? " Прогноз погоды от GM на" + f_dsp.td_name_r(wf_data.tod) + String(wf_data.day) + f_dsp.month_name_r(wf_data.month) + " : "
                 + " температура " + String(wf_data.temp_min) + " " + String(wf_data.temp_max) + grad + "C "
                 + " ветер" + f_dsp.wind_dir_r(wf_data.wind_dir) + String(wf_data.wind_max) + "-" + String(wf_data.wind_min) + "м/с " + f_dsp.prc_type_r(wf_data.prec, wf_data.spower, wf_data.rpower)
                 + " oтн. влажность " + String(wf_data.hum_max) + "% "
                 + " давление " + String(wf_data.press_max) + "мм " : " Нет данных по прогнозу погоды - проверьте настройки ");
        }
        if (conf_data.use_pp == 2)
        {
          str = (wf_data.temp_min > -99 ? " Прогноз погоды от OWM на " + String(wf_data.day) + f_dsp.month_name_r(wf_data.month) + " : "
                 + wf_data.descript + " температура " + String(wf_data.temp_min) + " " + String(wf_data.temp_max) + grad + "C "
                 + " ветер" + f_dsp.wind_dir_r(wf_data.wind_dir) + String(wf_data.wind_min) + "м/с "
                 + " oтн. влажность " + String(wf_data.hum_min) + "% "
                 + " давление " + String(wf_data.press_min) + "мм " : " Нет данных по прогнозу погоды - проверьте настройки ");
        }
        break;
      case 4:
        str = "Будильник " + (rtc_data.a_hour < 24 && rtc_data.a_min < 59 ? "зазвонит в " + String(rtc_data.a_hour) + ':' + String(rtc_data.a_min) : "не установлен ")
              + " Текущая яркость: " + String(cur_br)
              + " Ваш IP: " + f_dsp.IP_to_str(IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3])  + " ";
        break;
    }
  }
  return str;
}
