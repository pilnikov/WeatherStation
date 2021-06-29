
String pr_str(uint8_t num, conf_data_t cf, snr_data_t sn, wf_data_t wf, wf_data_t wfc, rtc_data_t rt, uint8_t c_br)
{
  const char stdr_0[] = "ночь";
  const char stdr_1[] = "yтро";
  const char stdr_2[] = "день";
  const char stdr_3[] = "вечер";

  const char* const stdr[] = {stdr_0, stdr_1, stdr_2, stdr_3};

  const char swnr_0[] = "северный";
  const char swnr_1[] = "северо-восточный";
  const char swnr_2[] = "восточный";
  const char swnr_3[] = "юго-восточный";
  const char swnr_4[] = "южный";
  const char swnr_5[] = "юго-западный";
  const char swnr_6[] = "западный";
  const char swnr_7[] = "северо-западный";

  const char* const swnr[] = {swnr_0, swnr_1, swnr_2, swnr_3, swnr_4, swnr_5, swnr_6, swnr_7};

  const char sprcr_0[] = "дождь";
  const char sprcr_1[] = "ливень";
  const char sprcr_2[] = "снегопад";
  const char sprcr_3[] = "сильный снегопад";
  const char sprcr_4[] = "гроза";
  const char sprcr_5[] = "без осадков";

  const char* const sprcr[] = {sprcr_0, sprcr_1, sprcr_2, sprcr_3, sprcr_4, sprcr_5};

  const char sdnr_1[] = "воскресенье";
  const char sdnr_2[] = "понедельник";
  const char sdnr_3[] = "вторник";
  const char sdnr_4[] = "среда";
  const char sdnr_5[] = "четверг";
  const char sdnr_6[] = "пятница";
  const char sdnr_7[] = "суббота";

  const char* const sdnr[] = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

  const char sdne_1[] = "Sunday";
  const char sdne_2[] = "Monday";
  const char sdne_3[] = "Tuesday";
  const char sdne_4[] = "Wednesday";
  const char sdne_5[] = "Thursday";
  const char sdne_6[] = "Friday";
  const char sdne_7[] = "Saturday";

  const char* const sdne[] = {sdne_1, sdne_2, sdne_3, sdne_4, sdne_5, sdne_6, sdne_7};

  const char smnr_01[] = "января";
  const char smnr_02[] = "февраля";
  const char smnr_03[] = "марта";
  const char smnr_04[] = "апреля";
  const char smnr_05[] = "мая";
  const char smnr_06[] = "июня";
  const char smnr_07[] = "июля";
  const char smnr_08[] = "августа";
  const char smnr_09[] = "сентября";
  const char smnr_10[] = "октября";
  const char smnr_11[] = "ноября";
  const char smnr_12[] = "декабря";

  const char* const smnr[] = {smnr_01, smnr_02, smnr_03, smnr_04, smnr_05, smnr_06, smnr_07, smnr_08, smnr_09, smnr_10, smnr_11, smnr_12};

  const char smne_01[] = "January";
  const char smne_02[] = "February";
  const char smne_03[] = "March";
  const char smne_04[] = "April";
  const char smne_05[] = "May";
  const char smne_06[] = "June";
  const char smne_07[] = "July";
  const char smne_08[] = "August";
  const char smne_09[] = "September";
  const char smne_10[] = "October";
  const char smne_11[] = "November";
  const char smne_12[] = "December";

  const char* const smne[] = {smne_01, smne_02, smne_03, smne_04, smne_05, smne_06, smne_07, smne_08, smne_09, smne_10, smne_11, smne_12};

  char grad = '\260';
  if (cf.type_disp == 19) grad = '\357';

  char buf[255], buf1[255];

  uint16_t size_buf  = 1, size_buf1  = 1;

  uint16_t ala_t = (int) rt.a_hour * 60 + rt.a_min;
  uint16_t cur_t = (int) rt.hour * 60 + rt.min;
  uint8_t ala_h = 0, ala_m = 0;

  if (!cf.rus_lng)
  {
    switch (num)
    {
      case 1:
        size_buf = snprintf(buf, 255, " Today is %s %d %s %d", sdne[rt.wday - 1], rt.day, smne[rt.month - 1], rt.year);
        break;
      case 2:
        if (sn.t1 > -99 && sn.t1 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", cf.ch1_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", sn.t1, grad, sn.h1);
          strncat(buf, buf1, size_buf1);
        }
        if (sn.t2 > -99 && sn.t2 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", cf.ch2_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", sn.t2, grad, sn.h2);
          strncat(buf, buf1, size_buf1);
        }
        if (sn.t3 > -99 && sn.t3 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", cf.ch3_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", sn.t3, grad, sn.h3);
          strncat(buf, buf1, size_buf1);
        }
        if (sn.p > 700)
        {
          size_buf1 = snprintf(buf1, 255, " pressure ");
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%dmm rt. st.", sn.p);
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
        size_buf = snprintf(buf, 255, " Сегодня %s %d %s %dг.", sdnr[rt.wday - 1], rt.day, smnr[rt.month - 1], rt.year);
        break;
      case 2:
        if (sn.t1 > -99 && sn.t1 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", cf.ch1_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", sn.t1, grad, sn.h1);
          strncat(buf, buf1, size_buf1);
        }
        if (sn.t2 > -99 && sn.t2 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", cf.ch2_name);
          strncat(buf, buf1, size_buf1);

          if (cf.use_pp == 2)
          {

            size_buf1 = wf.descript.length() + 1;
            strncpy(buf1, wf.descript.c_str(), size_buf1);
            strncat(buf, buf1, size_buf1);
            size_buf1 = snprintf(buf1, 255, " ветер %s %dм/с ", swnr[wfc.wind_dir], wfc.wind_min);
            strncat(buf, buf1, size_buf1);
          }

          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", sn.t2, grad, sn.h2);
          strncat(buf, buf1, size_buf1);
        }
        if (sn.t3 > -99 && sn.t3 < 99)
        {
          size_buf1 = snprintf(buf1, 255, " %s ", cf.ch3_name);
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%d%cC %d%%", sn.t3, grad, sn.h3);
          strncat(buf, buf1, size_buf1);
        }
        if (sn.p > 700)
        {
          size_buf1 = snprintf(buf1, 255, " давление ");
          strncat(buf, buf1, size_buf1);
          size_buf1 = snprintf(buf1, 255, "%3dмм рт. ст.", sn.p);
          strncat(buf, buf1, size_buf1);
        }
        break;
      case 3:
        switch (cf.use_pp)
        {
          case 1:
            wf.temp_min > -99 ? size_buf = snprintf(buf, 255, " Прогноз погоды от GM на %s %d %s:температура от %d до %d%cC ветер %s %d - %dм/с %s oтн. влажность %d%% давление %dмм.рт.ст",
                                                    stdr[wf.tod], wf.day, smnr[wf.month - 1],
                                                    wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_max, wf.wind_min, sprcr[wf.prec],
                                                    wf.hum_max, wf.press_max) : size_buf = snprintf(buf, 255, " Данные по прогнозу погоды не получены - проверьте настройки ");
            break;
          case 2:
            size_buf1 = wf.descript.length() + 1;
            strncpy(buf1, wf.descript.c_str(), size_buf1);

            wf.temp_min > -99 ? size_buf = snprintf(buf, 255, " Прогноз погоды от OWM на %d %s:%s температура от %d до %d%cC ветер %s %dм/с oтн.влажность %d%% давление %dмм.рт.ст",
                                                    wf.day, smnr[wf.month - 1], buf1,
                                                    wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_min, wf.hum_min, wf.press_min)
                                           : size_buf = snprintf(buf, 255, " Данные по прогнозу погоды не получены - проверьте настройки");
            break;
          default:
            break;
        }
        break;
      case 4:
        if (ala_t > cur_t)
        {
          ala_h = trunc((ala_t - cur_t) / 60);
          ala_h = ala_h % 100;
          ala_m = (ala_t - cur_t - (ala_h * 60)) % 100;
        }
        (ala_t > cur_t) ? size_buf = snprintf(buf, 115, " Будильник зазвонит через %2d ч. %2d мин. в %2d:%02d. Текущая яркость: %2d", ala_h, ala_m, rtc_data.a_hour, rtc_data.a_min, cur_br)
                                     : size_buf = snprintf(buf, 100, " Будильник не установлен. Текущая яркость:%2d", cur_br);
        break;
      default:
        break;
    }
  }
  return String(buf);
}
