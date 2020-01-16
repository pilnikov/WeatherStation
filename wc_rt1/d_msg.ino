String pr_str(uint8_t num)
{
  String grad = "\xB0";
  if (ram_data.type_disp == 1) grad = "\xEF";

  String bstr = "Your IP is " + WiFi.localIP().toString() + " ";

  if (!conf_data.rus_disp)
  {
    switch (num)
    {
      case 1:
        bstr = "Today is " + String(dayStr(weekday())) + ' ' + String(day()) + ' ' + monthStr(month()) + ' ' + String(year()) + ' ';
        break;
      case 2:
        bstr = (snr_data.t1 < 99 ?  "Inside " + String(snr_data.t1) + grad + "C " : ' ')
               + (snr_data.h1 >  0 ?              String(snr_data.h1) + "% " : ' ')
               + (snr_data.t2 < 99 ? "Outside " + String(snr_data.t2) + grad + "C " : ' ')
               + (snr_data.h2 >  0 ?              String(snr_data.h2) + "% " : ' ')
               + (snr_data.p  >  0 ?   "press " + String(snr_data.p)  + "mm " : ' ');
        break;
      case 3:
        bstr = "The end of messages ";
        break;
    }
  }
  else
  {
    switch (num)
    {
      case 1:
        bstr = "Сегодня" + f_dsp.dow_name_r(weekday()) + String(day()) + f_dsp.month_name_r(month()) + String(year()) + "г. ";
        break;
      case 2:
        bstr = (snr_data.t1 < 99 || snr_data.h1 ? "Bнутри "  + (snr_data.t1 < 99 ? String(snr_data.t1) + grad + "C " : ' ') + (snr_data.h1 >  0 ? String(snr_data.h1) + "% " : ' ')
                : " Нет данных от внутренних датчиков - проверьте настройки ")
               +  (snr_data.t2 < 99 || snr_data.h2 > 0 ?
                   "Снаружи "
                   + (conf_data.use_pp == 2 ? wf_data_cur.descript + " ветер" + f_dsp.wind_dir_r(wf_data_cur.wind_dir) + String(wf_data_cur.wind_min) + "м/с " : "")
                   + (snr_data.t2 < 99 ? String(snr_data.t2) + grad + "C " : ' ') + (snr_data.h2 > 0 ? String(snr_data.h2) + "% " : ' ')
                   : " Нет данных от внешних датчиков - проверьте настройки ")
               + (snr_data.p > 700 ? "давление " + String(snr_data.p) + "мм.рт.ст " : "Нет данных от датчика давления - проверьте настройки ");
        break;
      case 3:
        switch (conf_data.use_pp)
        {
          case 0:
            bstr = "";
            break;
          case 1:
            bstr = (wf_data.temp_min > -99 ? "Прогноз погоды от GM на" + f_dsp.td_name_r(wf_data.tod) + String(wf_data.day) + f_dsp.month_name_r(wf_data.month) + " : "
                    + " температура от " + String(wf_data.temp_min) + " до " + String(wf_data.temp_max) + grad + "C "
                    + "ветер" + f_dsp.wind_dir_r(wf_data.wind_dir) + String(wf_data.wind_max) + "-" + String(wf_data.wind_min) + "м/с " + f_dsp.prc_type_r(wf_data.prec, wf_data.spower, wf_data.rpower)
                    + "oтн. влажность " + String(wf_data.hum_max) + "% "
                    + "давление " + String(wf_data.press_max) + "мм.рт.ст " : "Нет данных по прогнозу погоды - проверьте настройки ");
            break;
          case 2:
            bstr = (wf_data.temp_min > -99 ? "Прогноз погоды от OWM на " + String(wf_data.day) + f_dsp.month_name_r(wf_data.month) + " : "
                    + wf_data.descript + " температура от " + String(wf_data.temp_min) + " до " + String(wf_data.temp_max) + grad + "C "
                    + "ветер" + f_dsp.wind_dir_r(wf_data.wind_dir) + String(wf_data.wind_min) + "м/с "
                    + "oтн. влажность " + String(wf_data.hum_min) + "% "
                    + "давление " + String(wf_data.press_min) + "мм.рт.ст " : "Нет данных по прогнозу погоды - проверьте настройки ");
            break;
        }
        break;
      case 4:
        bstr = "Будильник " + (rtc_data.a_hour < 24 && rtc_data.a_min < 59 ? "зазвонит в " + String(rtc_data.a_hour) + ':' + String(rtc_data.a_min) : "не установлен ")
               + " Текущая яркость: " + String(cur_br)
               + " Ваш IP: " +  WiFi.localIP().toString() + " ";
        break;
    }
  }
  return bstr;
}
