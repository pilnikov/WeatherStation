#include "Scr_frm.h"
#include "fonts.h"

RTCMSG myrtcmsg;  //For RTC Messages
FFF fffm;         //FrameFormerFunctions


// Наполнение содержимым инфостроки для символьных дисплеев
void MSCF::roll_string_cf(uint8_t& num, uint8_t _max, main_cfg_t mcf, snr_data_t snr, wf_data_t wf, wf_data_t wfc, rtc_time_data_t rt, rtc_alm_data_t rta, String local_ip, uint8_t c_br, char out[], bool cur_cli, String newsClient) {
  const char* stdr_0 = PSTR("ночь");
  const char* stdr_1 = PSTR("yтро");
  const char* stdr_2 = PSTR("день");
  const char* stdr_3 = PSTR("вечер");

  const char* const stdr[] = { stdr_0, stdr_1, stdr_2, stdr_3 };

  const char* swnr_0 = PSTR("северный");
  const char* swnr_1 = PSTR("северо-восточный");
  const char* swnr_2 = PSTR("восточный");
  const char* swnr_3 = PSTR("юго-восточный");
  const char* swnr_4 = PSTR("южный");
  const char* swnr_5 = PSTR("юго-западный");
  const char* swnr_6 = PSTR("западный");
  const char* swnr_7 = PSTR("северо-западный");

  const char* const swnr[] = { swnr_0, swnr_1, swnr_2, swnr_3, swnr_4, swnr_5, swnr_6, swnr_7 };

  const char* sprcr_0 = PSTR("ясно");
  const char* sprcr_1 = PSTR("малооблачно");
  const char* sprcr_2 = PSTR("облачно");
  const char* sprcr_3 = PSTR("пасмурно");
  const char* sprcr_4 = PSTR("дождь");
  const char* sprcr_5 = PSTR("ливень");
  const char* sprcr_6 = PSTR("снегопад");
  const char* sprcr_7 = PSTR("сильный снегопад");
  const char* sprcr_8 = PSTR("гроза");
  const char* sprcr_9 = PSTR("нет данных");
  const char* sprcr_10 = PSTR("без осадков");
  const char* sprcr_16 = PSTR("возможен снегопад");
  const char* sprcr_17 = PSTR("возможен сильный снегопад");
  const char* sprcr_18 = PSTR("возможна гроза");

  const char* const sprcr[] = { sprcr_0, sprcr_1, sprcr_2, sprcr_3, sprcr_4, sprcr_5, sprcr_6,
                                sprcr_7, sprcr_8, sprcr_9, sprcr_10, sprcr_16, sprcr_17, sprcr_18
                              };

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

  const char* const smnr[] = { smnr_01, smnr_02, smnr_03, smnr_04, smnr_05, smnr_06, smnr_07, smnr_08, smnr_09, smnr_10, smnr_11, smnr_12 };

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

  const char* const smne[] = { smne_01, smne_02, smne_03, smne_04, smne_05, smne_06, smne_07, smne_08, smne_09, smne_10, smne_11, smne_12 };

  char grad = '\260';
  if (mcf.dsp_t == 19) grad = '\357';

  char buf[254];

  bool _repeat = true;

  do {
    memset(buf, 0, 254);

    if (!mcf.rus_lng) {
      switch (num) {
        case 1:
          myrtcmsg.rtc_str(num, mcf.rus_lng, rt, rta, out);
          _repeat = false;
          break;
        case 2:
          if (snr.t1 > -99 && snr.t1 < 99) {
            sprintf_P(buf, PSTR(" %s "), mcf.ch1_name);
            strcat(out, buf);
            if (snr.h1 > 0) {
              sprintf_P(buf, PSTR(" %d%cC %d%%"), snr.t1, grad, snr.h1);
            } else {
              sprintf_P(buf, PSTR(" %d%cC"), snr.t1, grad);
            }
            strcat(out, buf);
            _repeat = false;
          }
          if (snr.t2 > -99 && snr.t2 < 99) {
            sprintf_P(buf, PSTR(" %s "), mcf.ch2_name);
            strcat(out, buf);
            if (mcf.use_pp == 2) {
              wfc.descript.toCharArray(buf, wfc.descript.length() + 1);
              strcat(out, buf);
              sprintf_P(buf, PSTR(" wind %s %dm/s"), swnr[wfc.wind_dir], wfc.wind_min);
              strcat(out, buf);
            }
            if (snr.h2 > 0) {
              sprintf_P(buf, PSTR(" %d%cC %d%%"), snr.t2, grad, snr.h2);
            } else {
              sprintf_P(buf, PSTR(" %d%cC"), snr.t2, grad);
            }
            strcat(out, buf);
            _repeat = false;
          }
          if (snr.t3 > -99 && snr.t3 < 99) {
            sprintf_P(buf, PSTR(" %s "), mcf.ch3_name);
            strcat(out, buf);
            if (snr.h3 > 0) {
              sprintf_P(buf, PSTR(" %d%cC %d%%"), snr.t3, grad, snr.h3);
            } else {
              sprintf_P(buf, PSTR(" %d%cC"), snr.t3, grad);
            }
          }
          if (snr.p > 700) {
            sprintf_P(buf, PSTR(" preesure %dmm.m."), snr.p);
            strcat(out, buf);
            _repeat = false;
          }
          break;
        case 3:
          switch (mcf.use_pp) {
            case 1:
              if (wf.temp_min > -99) {
                if ((wf.prec == 6) & (wf.rpower == 0)) wf.prec = 11;
                if ((wf.prec == 7) & (wf.rpower == 0)) wf.prec = 12;
                if ((wf.prec == 8) & (wf.spower == 0)) wf.prec = 13;
                sprintf_P(out, PSTR(" Weather forecast from GM on %s %d %s: temp from %d to %d%cC wind %s %d - %dm/s %s %s humid. %d%% press %dmm.m."),
                          stdr[wf.tod], wf.day, smne[wf.month - 1],
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_max, wf.wind_min, sprcr[wf.cloud], sprcr[wf.prec],
                          wf.hum_max, wf.press_max);
                _repeat = false;
              }
              break;
            case 2:
              wf.descript.toCharArray(buf, wf.descript.length() + 1);

              if (wf.temp_min > -99) {
                sprintf_P(out, PSTR(" Weather forecast from OWM on %d %s: %s temp from %d to %d%cC wind %s %dm/s humid %d%% press %dmm.m."),
                          wf.day, smne[wf.month - 1], buf,
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_min, wf.hum_min, wf.press_min);
                _repeat = false;
              }
              break;
            default:
              break;
          }
          break;
        case 4:
          if (rta.num < 7) {
            myrtcmsg.rtc_str(num, mcf.rus_lng, rt, rta, out);
            _repeat = false;
          }
          break;
        case 5:
          if (mcf.news_en & cur_cli) {
            String news_s = "News not support this platform";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            news_s = "";
            news_s = (String)mcf.news_source + ": " + newsClient;
#endif
            strcpy(out, news_s.c_str());
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
    } else {
      switch (num) {
        case 1:
          myrtcmsg.rtc_str(num, mcf.rus_lng, rt, rta, out);
          _repeat = false;
          break;
        case 2:
          if (snr.t1 > -99 && snr.t1 < 99) {
            sprintf_P(buf, PSTR(" %s "), mcf.ch1_name);
            strcat(out, buf);
            if (snr.h1 > 0) {
              sprintf_P(buf, PSTR("%d%cC %d%%"), snr.t1, grad, snr.h1);
            } else {
              sprintf_P(buf, PSTR(" %d%cC"), snr.t1, grad);
            }
            strcat(out, buf);
            _repeat = false;
          }

          if (snr.t2 > -99 && snr.t2 < 99) {
            sprintf_P(buf, PSTR(" %s "), mcf.ch2_name);
            strcat(out, buf);
            if (mcf.use_pp == 2) {
              wfc.descript.toCharArray(buf, wfc.descript.length() + 1);
              strcat(out, buf);
              sprintf_P(buf, PSTR(" ветер %s %dм/с"), swnr[wfc.wind_dir], wfc.wind_min);
              strcat(out, buf);
            }
            if (snr.h2 > 0) {
              sprintf_P(buf, PSTR(" %d%cC %d%%"), snr.t2, grad, snr.h2);
            } else {
              sprintf_P(buf, PSTR(" %d%cC"), snr.t2, grad);
            }
            strcat(out, buf);
            _repeat = false;
          }
          if (snr.t3 > -99 && snr.t3 < 99) {
            sprintf_P(buf, PSTR(" %s "), mcf.ch3_name);
            strcat(out, buf);
            if (snr.h3 > 0) {
              sprintf_P(buf, PSTR(" %d%cC %d%%"), snr.t3, grad, snr.h3);
            } else {
              sprintf_P(buf, PSTR(" %d%cC"), snr.t3, grad);
            }
            strcat(out, buf);
            _repeat = false;
          }
          if (snr.p > 700) {
            sprintf_P(buf, PSTR(" давление %dмм рт.ст."), snr.p);
            strcat(out, buf);
            _repeat = false;
          }
          break;
        case 3:
          switch (mcf.use_pp) {
            case 1:
              if (wf.temp_min > -99) {
                if ((wf.prec == 6) & (wf.rpower == 0)) wf.prec = 11;
                if ((wf.prec == 7) & (wf.rpower == 0)) wf.prec = 12;
                if ((wf.prec == 8) & (wf.spower == 0)) wf.prec = 13;
                sprintf_P(out, PSTR(" Прогноз погоды от GM на %s %d %s: температура от %d до %d%cC ветер %s %d - %dм/с %s %s, oтн.влажность %d%%, давление %dмм.рт.ст."),
                          stdr[wf.tod], wf.day, smnr[wf.month - 1],
                          wf.temp_min, wf.temp_max, grad, swnr[wf.wind_dir], wf.wind_max, wf.wind_min, sprcr[wf.cloud], sprcr[wf.prec],
                          wf.hum_max, wf.press_max);
                _repeat = false;
              }
              break;
            case 2:
              wf.descript.toCharArray(buf, wf.descript.length() + 1);

              if (wf.temp_min > -99) {
                sprintf_P(out, PSTR(" Прогноз погоды от OWM на %d %s: %s температура от %d до %d%cC ветер %s %dм/с, oтн.влажность %d%%, давление %dмм.рт.ст."),
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
          if (rta.num < 7) {
            myrtcmsg.rtc_str(num, mcf.rus_lng, rt, rta, out);
            _repeat = false;
          }
          break;
        case 5:
          if (mcf.news_en & cur_cli) {
            String news_s = "Новости недоступны для этой платформы";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            news_s = "";
            news_s = (String)mcf.news_source + ": " + newsClient;
#endif
            strcpy(out, news_s.c_str());
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
    if (num > _max) {
      num = 1;
      _repeat = true;
    }
  } while (_repeat);
}


// Селектор запуска бегущей строки для символьных дисплеев
void MSCF::scroll_string_ran_selector(uint8_t vdrv_t, uint8_t dsp_t, bool slow_mode, bool& _end, char* in_st, byte* out) 
{
  if (slow_mode) {
    if (vdrv_t == 11) {
      if ((dsp_t == 11 || dsp_t == 13) && !_end) _end = fffm.matrix_scroll_String(0, 7, in_st, out, font14s, 2, 0, 2); // For 14seg
      if (dsp_t == 31 && !_end) _end = fffm.matrix_scroll_String(20, 25, in_st, out, font14s, 2, 0, 2); // For type 31
    }

    if (vdrv_t == 12 && dsp_t == 19 && !_end) _end = fffm.lcd_scroll_String(16, in_st, out); // For lcd
  }
  else {
    if (dsp_t > 19 && dsp_t < 29 && !_end) _end = fffm.matrix_scroll_String(0, 31, in_st, out, font5x7, 5, 1, 1);  // for matrix
  }
}

//переключатель фрагментов стоки прокрутки для символьных дисплеев
void MSCF::roll_string_part_sw(uint8_t& num, uint8_t _max, main_cfg_t mcf, snr_data_t snr,
                            wf_data_t wf, wf_data_t wfc, rtc_time_data_t rt, rtc_alm_data_t rta, String local_ip,
                            uint16_t c_br, bool cli, String ns, uint8_t& nidx, bool& _end, char* in_str, byte* out)
{
  if (_end) {
    memset(in_str, 0, 254);

    MSCF::roll_string_cf(num, _max, mcf, snr, wf, wfc, rt, rta, local_ip, c_br, in_str, cli, ns);

    DBG_OUT_PORT.print(F("num_st = "));
    DBG_OUT_PORT.println(num);
    DBG_OUT_PORT.print(F("in str = "));
    DBG_OUT_PORT.println(in_str);
    DBG_OUT_PORT.print(F("nidx = "));
    DBG_OUT_PORT.println(nidx);


    if (mcf.rus_lng & (mcf.vdrv_t == 12)) fffm.lcd_rus(in_str); //for lcd
    if (mcf.rus_lng & (mcf.vdrv_t != 12)) fffm.utf8rus(in_str); //for other

    fffm.h_scroll_init();
    _end = false;

    if (mcf.dsp_t == 20) memset(out, 0, 32);

    if (num == 6) nidx++;

    if (nidx > 9) nidx = 0;
  }
}
