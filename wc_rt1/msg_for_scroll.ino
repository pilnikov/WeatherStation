String pr_str(uint8_t num)
{
  char grad = '\260';
  if (conf_data.type_disp == 19) grad = '\357';

  char buf[255], buf1[255];
  int size_buf  = snprintf(buf,  2, "");
  int size_buf1 = snprintf(buf1, 2, "");


  if (!conf_data.rus_lng)
  {
    switch (num)
    {
      case 1:
        size_buf = snprintf(buf, 255, " Today is %s %d %s %d", sdne[weekday() - 1], day(), smne[month() - 1], year());
        break;
      case 2:
        if (snr_data.t1 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", conf_data.ch1_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", snr_data.t1, grad, snr_data.h1);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t2 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", conf_data.ch2_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", snr_data.t2, grad, snr_data.h2);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t3 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", conf_data.ch3_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", snr_data.t3, grad, snr_data.h3);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.p > 700)
        {
          size_buf1 = snprintf(buf1, 255, " pressure ");
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%dmm rt. st.", snr_data.p);
          strncat(buf, buf1, size_buf1);
        }
        break;
      case 3:
        size_buf = snprintf(buf, 255, " The end of messages ");
        break;
      default:
        break;
    }
  }
  else
  {
    switch (num)
    {
      case 1:
        size_buf = snprintf(buf, 255, " Сегодня %s %d %s %dг.", sdnr[weekday() - 1], day(), smnr[month() - 1], year());
        break;
      case 2:
        if (snr_data.t1 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", conf_data.ch1_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", snr_data.t1, grad, snr_data.h1);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t2 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", conf_data.ch2_name);
          strncat(buf, buf1, size_buf1);

          if (conf_data.use_pp == 2)
          {
            size_buf1 = wf_data.descript.length() + 1;
            strncpy(buf1, wf_data.descript.c_str(), size_buf1);
            strncat(buf, buf1, size_buf1);
            size_buf1 = snprintf(buf1, 255, " ветер %s %dм/с ", swnr[wf_data_cur.wind_dir], wf_data_cur.wind_min);
            strncat(buf, buf1, size_buf1);
          }

          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", snr_data.t2, grad, snr_data.h2);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t3 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", conf_data.ch3_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", snr_data.t3, grad, snr_data.h3);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.p > 700)
        {
          size_buf1 = snprintf(buf1, 255, " давление ");
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%3dмм рт. ст.", snr_data.p);
          strncat(buf, buf1, size_buf1);
        }
        break;
      case 3:
        switch (conf_data.use_pp)
        {
          case 1:
            wf_data.temp_min > -99 ? size_buf = snprintf(buf, 255, " Прогноз погоды от GM на %s %d %s:температура от %d до %d%cC ветер %s %d - %dм/с %s oтн. влажность %d%% давление %dмм.рт.ст",
                                                stdr[wf_data.tod], wf_data.day, smnr[wf_data.month - 1],
                                                wf_data.temp_min, wf_data.temp_max, grad, swnr[wf_data.wind_dir], wf_data.wind_max, wf_data.wind_min, sprcr[wf_data.prec],
                                                wf_data.hum_max, wf_data.press_max) : size_buf = snprintf(buf, 255, " Данные по прогнозу погоды не получены - проверьте настройки ");
            break;
          case 2:
            size_buf1 = wf_data.descript.length() + 1;
            strncpy(buf1, wf_data.descript.c_str(), size_buf1);

            wf_data.temp_min > -99 ? size_buf = snprintf(buf, 255, " Прогноз погоды от OWM на %d %s:%s температура от %d до %d%cC ветер %s %dм/с oтн.влажность %d%% давление %dмм.рт.ст",
                                                wf_data.day, smnr[wf_data.month - 1], buf1,
                                                wf_data.temp_min, wf_data.temp_max, grad, swnr[wf_data.wind_dir], wf_data.wind_min, wf_data.hum_min, wf_data.press_min)
                                                : size_buf = snprintf(buf, 255, " Данные по прогнозу погоды не получены - проверьте настройки");
            break;
          default:
            break;
        }
        break;
      case 4:

        size_buf1 = WiFi.localIP().toString().length() + 1;
        strncpy(buf1, WiFi.localIP().toString().c_str(), size_buf1);
        rtc_data.a_hour < 24 && rtc_data.a_min < 59 ? size_buf = snprintf(buf, 250, " Будильник зазвонит в %2d:%02d  Текущая яркость: %2d Ваш IP:%s", rtc_data.a_hour, rtc_data.a_min, cur_br, buf1)
            : size_buf = snprintf(buf, 60, " Будильник не установлен  Текущая яркость:%2d Ваш IP:%s", cur_br, buf1);
        break;
      default:
        break;
    }
  }

  return String(buf);
}
