String pr_str(uint8_t num)
{
  char grad = '\260';
  if (conf_data.type_disp == 19) grad = '\357';

  char buf[120], buf1[120], buf2[120],  buf3[120], buf4[120];
  uint16_t size_buf  = snprintf(buf,  2, " ");
  uint16_t size_buf1 = snprintf(buf1, 2, " ");


  if (!conf_data.rus_lng)
  {
    switch (num)
    {
      case 1:
          copyFromPGM(&sdne[_weekday - 1], buf1, sizeof(buf1));
          copyFromPGM(&smne[_month - 1], buf2, sizeof(buf2));

          size_buf = snprintf(buf, 100, " Today is %s %d %s %d", buf1, _day, buf2, _year);
        break;
      case 2:
        if (snr_data.t1 > -99 && snr_data.t1 < 99)
        {
          size_buf1 = snprintf(buf1, 100, " %s ", conf_data.ch1_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%d%cC %d%%", snr_data.t1, grad, snr_data.h1);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t2 > -99 && snr_data.t2 < 99)
        {
          size_buf1 = snprintf(buf1, 100, " %s ", conf_data.ch2_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%d%cC %d%%", snr_data.t2, grad, snr_data.h2);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t3 > -99 && snr_data.t3 < 99)
        {
          size_buf1 = snprintf(buf1, 100, " %s ", conf_data.ch3_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%d%cC %d%%", snr_data.t3, grad, snr_data.h3);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.p > 700)
        {
          size_buf1 = snprintf(buf1, 100, " pressure ");
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%dmm rt. st.", snr_data.p);
          strncat(buf, buf1, size_buf1);
        }
        break;
      case 3:
        size_buf = snprintf(buf, 100, " The end of messages ");
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
        copyFromPGM(&sdnr[_weekday - 1], buf1, sizeof(buf1));
        copyFromPGM(&smnr[_month - 1], buf2, sizeof(buf2));
        size_buf = snprintf(buf, 100, " Сегодня %s %d %s %dг.", buf1, _day, buf2, _year);
        break;
      case 2:
        if (snr_data.t1 > -99 && snr_data.t1 < 99)
        {
          size_buf1 = snprintf(buf1, 100, " %s ", conf_data.ch1_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%d%cC %d%%", snr_data.t1, grad, snr_data.h1);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t2 > -99 && snr_data.t2 < 99)
        {
          size_buf1 = snprintf(buf1, 100, " %s ", conf_data.ch2_name);
          strncat(buf, buf1, size_buf1);

          if (conf_data.use_pp == 2)
          {
            copyFromPGM(&swnr[wf_data_cur.wind_dir], buf2, sizeof(buf2));

            size_buf1 = wf_data.descript.length() + 1;
            strncpy(buf1, wf_data.descript.c_str(), size_buf1);
            strncat(buf, buf1, size_buf1);
            size_buf1 = snprintf(buf1, 100, " ветер %s %dм/с ", buf2, wf_data_cur.wind_min);
            strncat(buf, buf1, size_buf1);
          }

          size_buf1 = snprintf(buf1, 100, "%d%cC %d%%", snr_data.t2, grad, snr_data.h2);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.t3 > -99 && snr_data.t3 < 99)
        {
          size_buf1 = snprintf(buf1, 100, " %s ", conf_data.ch3_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%d%cC %d%%", snr_data.t3, grad, snr_data.h3);
          strncat(buf, buf1, size_buf1);
        }
        if (snr_data.p > 700)
        {
          size_buf1 = snprintf(buf1, 100, " давление ");
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 100, "%3dмм рт. ст.", snr_data.p);
          strncat(buf, buf1, size_buf1);
        }
        break;
      case 3:
        switch (conf_data.use_pp)
        {
          case 0:
            size_buf = snprintf(buf, 100, " Прогноз погоды отключен");
            break;
          case 1:
            copyFromPGM(&stdr[wf_data.tod], buf1, sizeof(buf1));
            copyFromPGM(&smnr[wf_data.month - 1], buf2, sizeof(buf2));
            copyFromPGM(&swnr[wf_data.wind_dir], buf3, sizeof(buf3));
            copyFromPGM(&sprcr[wf_data.prec], buf4, sizeof(buf4));

            wf_data.temp_min > -99 ? size_buf = snprintf(buf, 100, " Прогноз погоды от GM на %s %d %s:температура от %d до %d%cC ветер %s %d - %dм/с %s oтн. влажность %d%% давление %dмм.рт.ст",
                                                buf1, wf_data.day, buf2,
                                                wf_data.temp_min, wf_data.temp_max, grad, buf3, wf_data.wind_max, wf_data.wind_min, buf4,
                                                wf_data.hum_max, wf_data.press_max) : size_buf = snprintf(buf, 255, " Данные по прогнозу погоды не получены - проверьте настройки ");
            break;
          case 2:
            copyFromPGM(&smnr[wf_data.month - 1], buf2, sizeof(buf2));
            copyFromPGM(&swnr[wf_data.wind_dir], buf3, sizeof(buf3));

            size_buf1 = wf_data.descript.length() + 1;
            strncpy(buf1, wf_data.descript.c_str(), size_buf1);

            wf_data.temp_min > -99 ? size_buf = snprintf(buf, 100, " Прогноз погоды от OWM на %d %s:%s температура от %d до %d%cC ветер %s %dм/с oтн.влажность %d%% давление %dмм.рт.ст",
                                                wf_data.day, buf2, buf1,
                                                wf_data.temp_min, wf_data.temp_max, grad, buf3, wf_data.wind_min, wf_data.hum_min, wf_data.press_min)
                                                : size_buf = snprintf(buf, 100, " Данные по прогнозу погоды не получены - проверьте настройки");
            break;
          default:
            break;
        }
        break;

      case 4:
        uint16_t alat = (int)rtc_data.a_hour * 60 + rtc_data.a_min;
        uint16_t curt = (int) hour() * 60 + minute();
        uint8_t alah = 0, alam = 0;
        if (alat > curt)
        {
          alah = trunc((alat - curt) / 60);
          alam = (alat - curt - (alah * 60));
        }
        (alat > curt) ? size_buf = snprintf(buf, 115, " Будильник зазвонит через %2d ч. %2d мин. в %2d:%02d. Текущая яркость: %2d", alah, alam, rtc_data.a_hour, rtc_data.a_min, cur_br)
            : size_buf = snprintf(buf, 100, " Будильник не установлен. Текущая яркость:%2d", cur_br);
        break;
      default:
        break;
    }
  }
  return String(buf);
}
