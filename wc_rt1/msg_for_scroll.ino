
void pr_str(uint8_t &num, uint8_t _max, conf_data_t cf, snr_data_t sn, wf_data_t wf, wf_data_t wfc, rtc_data_t rt, String local_ip, uint8_t c_br, char out[])
{
  const char* stdr_0 = PSTR("ночь");
  const char* stdr_1 = PSTR("yтро");
  const char* stdr_2 = PSTR("день");
  const char* stdr_3 = PSTR("вечер");

  const char* const stdr[] = {stdr_0, stdr_1, stdr_2, stdr_3};

  const char* swnr_0 = PSTR("северный");
  const char* swnr_1 = PSTR("северо-восточный");
  const char* swnr_2 = PSTR("восточный");
  const char* swnr_3 = PSTR("юго-восточный");
  const char* swnr_4 = PSTR("южный");
  const char* swnr_5 = PSTR("юго-западный");
  const char* swnr_6 = PSTR("западный");
  const char* swnr_7 = PSTR("северо-западный");

  const char* const swnr[] = {swnr_0, swnr_1, swnr_2, swnr_3, swnr_4, swnr_5, swnr_6, swnr_7};

  const char* sprcr_0 = PSTR("дождь");
  const char* sprcr_1 = PSTR("ливень");
  const char* sprcr_2 = PSTR("снегопад");
  const char* sprcr_3 = PSTR("сильный снегопад");
  const char* sprcr_4 = PSTR("гроза");
  const char* sprcr_5 = PSTR("без осадков");

  const char* const sprcr[] = {sprcr_0, sprcr_1, sprcr_2, sprcr_3, sprcr_4, sprcr_5};

  const char* sdnr_1 = PSTR("воскресенье");
  const char* sdnr_2 = PSTR("понедельник");
  const char* sdnr_3 = PSTR("вторник");
  const char* sdnr_4 = PSTR("среда");
  const char* sdnr_5 = PSTR("четверг");
  const char* sdnr_6 = PSTR("пятница");
  const char* sdnr_7 = PSTR("суббота");

  const char* const sdnr[] = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

  const char* sdne_1 = PSTR("Sunday");
  const char* sdne_2 = PSTR("Monday");
  const char* sdne_3 = PSTR("Tuesday");
  const char* sdne_4 = PSTR("Wednesday");
  const char* sdne_5 = PSTR("Thursday");
  const char* sdne_6 = PSTR("Friday");
  const char* sdne_7 = PSTR("Saturday");

  const char* const sdne[] = {sdne_1, sdne_2, sdne_3, sdne_4, sdne_5, sdne_6, sdne_7};

  const char* smnr_01 = PSTR("января");
  const char* smnr_02 = PSTR("февраля");
  const char* smnr_03 = PSTR("марта");
  const char* smnr_04 = PSTR("апреля");
  const char* smnr_05 = PSTR("мая");
  const char* smnr_06 = PSTR("июня");
  const char* smnr_07 = PSTR("июля");
  const char* smnr_08 = PSTR("августа");
  const char* smnr_09 = PSTR("сентября");
  const char* smnr_10 = PSTR("октября");
  const char* smnr_11 = PSTR("ноября");
  const char* smnr_12 = PSTR("декабря");

  const char* const smnr[] = {smnr_01, smnr_02, smnr_03, smnr_04, smnr_05, smnr_06, smnr_07, smnr_08, smnr_09, smnr_10, smnr_11, smnr_12};

  const char* smne_01 = PSTR("January");
  const char* smne_02 = PSTR("February");
  const char* smne_03 = PSTR("March");
  const char* smne_04 = PSTR("April");
  const char* smne_05 = PSTR("May");
  const char* smne_06 = PSTR("June");
  const char* smne_07 = PSTR("July");
  const char* smne_08 = PSTR("August");
  const char* smne_09 = PSTR("September");
  const char* smne_10 = PSTR("October");
  const char* smne_11 = PSTR("November");
  const char* smne_12 = PSTR("December");

  const char* const smne[] = {smne_01, smne_02, smne_03, smne_04, smne_05, smne_06, smne_07, smne_08, smne_09, smne_10, smne_11, smne_12};

  char grad = '\260';
  if (cf.type_disp == 19) grad = '\357';

  char buf[254];

  uint16_t ala_t = (int) rt.a_hour * 60 + rt.a_min;
  uint16_t cur_t = (int) rt.hour * 60 + rt.min;
  uint8_t ala_h = trunc((ala_t - cur_t) / 60);
  ala_h = ala_h % 100;
  uint8_t ala_m = (ala_t - cur_t - (ala_h * 60)) % 100;
  bool _repeat = true;
  static uint8_t newsIndex;

  do
  {
    memset(buf, 0, 254);

    if (!cf.rus_lng)
    {
      switch (num)
      {
        case 1:
          sprintf_P(out, PSTR(" Today is %S %d %S %d"), sdne[rt.wday - 1], rt.day, smne[rt.month - 1], rt.year);
          _repeat = false;
          break;
        case 2:
          if (sn.t1 > -99 && sn.t1 < 99)
          {
            sprintf_P(out, PSTR(" %s %d%cC %d%%"), cf.ch1_name, sn.t1, grad, sn.h1);
            _repeat = false;
          }
          if (sn.t2 > -99 && sn.t2 < 99)
          {
            sprintf_P(buf, PSTR(" %s "), cf.ch2_name);
            strcat(out, buf);
            if (cf.use_pp == 2)
            {
              wfc.descript.toCharArray(buf, wfc.descript.length() + 1);
              strcat(out, buf);
              sprintf_P(buf, PSTR(" wind %S %dm/s"), swnr[wfc.wind_dir], wfc.wind_min);
              strcat(out, buf);
            }
            sprintf_P(buf, PSTR(" %d%cC %d%%"), sn.t2, grad, sn.h2);
            strcat(out, buf);
            _repeat = false;
          }
          if (sn.t3 > -99 && sn.t3 < 99)
          {
            sprintf_P(buf, PSTR(" %s %d%cC %d%%"), cf.ch3_name, sn.t3, grad, sn.h3);
            strcat(out, buf);
            _repeat = false;
          }
          if (sn.p > 700)
          {
            sprintf_P(buf, PSTR(" preesure %dmm.m."), sn.p);
            strcat(out, buf);
            _repeat = false;
          }
          break;
        case 3:
          switch (cf.use_pp)
          {
            case 1:
              if (wf.temp_min > -99)
              {
                sprintf_P(out, PSTR(" Weather forecast from GM on %S %d %S: temp from %d to %d%cC wind %S %d - %dm/s %S humid. %d%% press %dmm.m."),
                          stdr[wf.tod], wf.day, smnr[wf.month - 1],
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_max, wf.wind_min, sprcr[wf.prec],
                          wf.hum_max, wf.press_max);
                _repeat = false;
              }
              break;
            case 2:
              wf.descript.toCharArray(buf, wf.descript.length() + 1);

              if (wf.temp_min > -99)
              {
                sprintf_P(out, PSTR(" Weather forecast from OWM on %d %S: %s temp from %d to %d%cC wind %S %dm/s humid %d%% press %dmm.m."),
                          wf.day, smnr[wf.month - 1], buf,
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_min, wf.hum_min, wf.press_min);
                _repeat = false;
              }
              break;
            default:
              break;
          }
          break;
        case 4:
          if ((ala_t > cur_t) & (ala_h < 24))
          {
            sprintf_P(out, PSTR(" Alarm after %2dh. %2dmin. on %2d:%02d"), ala_h, ala_m, rt.a_hour, rt.a_min);
            _repeat = false;
          }
          break;
        case 5:
          if (cf.news_en)
          {
            String news_s = "News not support this platform";
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            news_s = "";
            news_s = (String)cf.news_source + ": " + newsClient.getTitle(newsIndex);
# endif
            strcpy(out, news_s.c_str());
            newsIndex ++;
            if (newsIndex > 9) newsIndex = 0;
            _repeat = false;
          }
          break;
        case 6:
          local_ip.toCharArray(buf, local_ip.length() + 1);
          sprintf_P(out, PSTR(" Curr brigthness:%2d Your IP:%s"), c_br, buf);
          _repeat = false;
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
          sprintf_P(out, PSTR(" Сегодня %S %d %S %dг."), sdnr[rt.wday - 1], rt.day, smnr[rt.month - 1], rt.year);
          _repeat = false;
          break;
        case 2:
          if ((sn.t1 > -99) & (sn.t1 < 99))
          {
            sprintf_P(out, PSTR(" %s %d%cC %d%%"), cf.ch1_name, sn.t1, grad, sn.h1);
            _repeat = false;
          }

          if (sn.t2 > -99 && sn.t2 < 99)
          {
            sprintf_P(buf, PSTR(" %s "), cf.ch2_name);
            strcat(out, buf);
            if (cf.use_pp == 2)
            {
              wfc.descript.toCharArray(buf, wfc.descript.length() + 1);
              strcat(out, buf);
              sprintf_P(buf, PSTR(" ветер %S %dм/с"), swnr[wfc.wind_dir], wfc.wind_min);
              strcat(out, buf);
            }
            sprintf_P(buf, PSTR(" %d%cC %d%%"), sn.t2, grad, sn.h2);
            strcat(out, buf);
            _repeat = false;
          }

          if (sn.t3 > -99 && sn.t3 < 99)
          {
            sprintf_P(buf, PSTR(" %s %d%cC %d%%"), cf.ch3_name, sn.t3, grad, sn.h3);
            strcat(out, buf);
            _repeat = false;
          }
          if (sn.p > 700)
          {
            sprintf_P(buf, PSTR(" давление %dмм рт.ст."), sn.p);
            strcat(out, buf);
            _repeat = false;
          }
          break;
        case 3:
          switch (cf.use_pp)
          {
            case 1:
              if (wf.temp_min > -99)
              {
                sprintf_P(out, PSTR(" Прогноз погоды от GM на %S %d %S: температура от %d до %d%cC ветер %S %d - %dм/с %S, oтн.влажность %d%%, давление %dмм.рт.ст."),
                          stdr[wf.tod], wf.day, smnr[wf.month - 1],
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_max, wf.wind_min, sprcr[wf.prec],
                          wf.hum_max, wf.press_max);
                _repeat = false;
              }
              break;
            case 2:
              wf.descript.toCharArray(buf, wf.descript.length() + 1);

              if (wf.temp_min > -99)
              {
                sprintf_P(out, PSTR(" Прогноз погоды от OWM на %d %S: %s температура от %d до %d%cC ветер %S %dм/с, oтн.влажность %d%%, давление %dмм.рт.ст."),
                          wf.day, smnr[wf.month - 1], buf,
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_min, wf.hum_min, wf.press_min);
                _repeat = false;
              }
              break;
            default:
              break;
          }
          break;
        case 4:
          if ((ala_t > cur_t) & (ala_h < 24))
          {
            sprintf_P(out, PSTR(" Будильник зазвонит через %2dч. %2dмин. в %2d:%02d"), ala_h, ala_m, rt.a_hour, rt.a_min);
            _repeat = false;
          }
          break;
        case 5:
          if (cf.news_en)
          {
            String news_s = "Новости недоступны для этой платформы";
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            news_s = "";
            news_s = (String)cf.news_source + ": " + newsClient.getTitle(newsIndex);
# endif
            strcpy(out, news_s.c_str());
            newsIndex ++;
            if (newsIndex > 9) newsIndex = 0;
            _repeat = false;

          }
          break;
        case 6:
          local_ip.toCharArray(buf, local_ip.length() + 1);
          sprintf_P(out, PSTR(" Текущая яркость: %2d Ваш IP: %s"), c_br, buf);
          _repeat = false;
          break;
        default:
          break;
      }
    }
    num++;
    if (num > _max)
    {
      num = 1;
      _repeat = true;
    }
  } while (_repeat);
}
