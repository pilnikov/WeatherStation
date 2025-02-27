#include "Scr_frm.h"
#include "fonts.h"

CT  rtc7;   //For RTC Common
FFF scr7;  //For frame former funcions

uint8_t SSCF::seg_add_data(uint8_t &mod,  uint8_t _width, byte *in, uint8_t _offset, snr_data_t snr, rtc_time_data_t rtd,  rtc_alm_data_t rta, bool pm, uint16_t c_br) // Формирует подстроку с доп данными (не текущее время)
{
  const char* name_week_0 = PSTR("  ");
  const char* name_week_1 = PSTR("8c");
  const char* name_week_2 = PSTR("\357H");
  const char* name_week_3 = PSTR("8t");
  const char* name_week_4 = PSTR("cP");
  const char* name_week_5 = PSTR("4t");
  const char* name_week_6 = PSTR("\357t");
  const char* name_week_7 = PSTR("c6");

  const char* const name_week7[] = {name_week_0, name_week_1, name_week_2, name_week_3, name_week_4, name_week_5, name_week_6, name_week_7};
  char tStr[25];

  rtc_hms_t alt = rtc7.unix_to_hms(rta.time);

  uint8_t h = pm && rtd.hour != 12 ? rtd.hour % 12 : rtd.hour;
  h = h % 100;

  bool _repeat = true;
  memset (tStr, 0, 25);

  do
  {
    switch (_width)
    {
      case 4:
        switch (mod)
        {
          case 1: //Температура канал 1
            if (snr.t1 <= -99 || snr.t1 >= 99) mod ++;
            else
            {
              if (snr.t1 > -10)  sprintf_P(tStr, PSTR("t\1%2d"), snr.t1);
              else sprintf_P(tStr, PSTR("t\2%2d"), (int)abs(snr.t1));
              _repeat = false;
            }
            break;

          case 2: //Влажность канал 1
            if (snr.h1 <= 0 || snr.h1 > 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("H\1%2d"), snr.h1);
              _repeat = false;
            }
            break;

          case 3: //Температура канал 2
            if (snr.t2 <= -99 || snr.t2 >= 99) mod ++;
            else
            {
              if (snr.t2 > -10) sprintf_P(tStr, PSTR("t\3%2d"), snr.t2);
              else sprintf_P(tStr, PSTR("t\4%2d"), (int)abs(snr.t2));
              _repeat = false;
            }
            break;

          case 4: //Влажность канал 2
            if (snr.h2 <= 0 || snr.h2 > 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("H\3%2d"), snr.h2);
              _repeat = false;
            }
            break;

          case 5: //Температура канал 3
            if (snr.t3 <= -99 || snr.t3 >= 99) mod ++;
            else
            {
              if (snr.t3 > - 10)sprintf_P(tStr, PSTR("t\5%2d"), snr.t3);
              else sprintf_P(tStr, PSTR("t\6%2d"), (int)abs(snr.t3));
              _repeat = false;
            }
            break;

          case 6: //Влажность канал 3
            if (snr.h3 <= 0 || snr.h3 > 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("H\5%d"), snr.h3);
              _repeat = false;
            }
            break;

          case 7: //Давление
            if (snr.p <= 700 || snr.p >= 800) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("P%3d"), snr.p);
              _repeat = false;
            }
            break;

          case 8: //День недели, дата

            sprintf_P(tStr, PSTR("%2s%2d"), name_week7[rtd.wday], rtd.day);
            _repeat = false;
            break;

          case 9: //Месяц, год
            sprintf_P(tStr, PSTR("%2d%2d"), rtd.month, rtd.year % 100);
            _repeat = false;
            break;

          case 10: //Актуальный будильник
            if (rta.num == 7) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("%2d%02d"), alt.h, alt.m);
              _repeat = false;
            }
            break;

          case 11: //Секунды
            sprintf_P(tStr, PSTR("%02d5 "), rtd.sec);
            _repeat = false;
            break;

          case 12: //Текущая яркость
            sprintf_P(tStr, PSTR("L%3d"), c_br);
            _repeat = false;
            break;

          default:
            sprintf_P(tStr, PSTR("%2d%02d"), h, rtd.min);
            _repeat = false;
            break;
        }
        break;
      case 6:
        switch (mod)
        {
          case 1: //Температура канал 1
            if (snr.t1 <= -99 || snr.t1 >= 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("t1%4d"), snr.t1);
              _repeat = false;
            }
            break;

          case 2: //Влажность канал 1
            if (snr.h1 <= 0 || snr.h1 > 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("H1%4d"), snr.h1);
              _repeat = false;
            }
            break;

          case 3: //Температура канал 2
            if (snr.t2 <= -99 || snr.t2 >= 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("t2%4d"), snr.t2);
              _repeat = false;
            }
            break;

          case 4: //Влажность канал 2
            if (snr.h2 <= 0 || snr.h2 > 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("H2%4d"), snr.h2);
              _repeat = false;
            }
            break;

          case 5: //Температура канал 3
            if (snr.t3 <= -99 || snr.t3 >= 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("t3%4d"), snr.t3);
              _repeat = false;
            }
            break;

          case 6: //Влажность канал 3
            if (snr.h3 <= 0 || snr.h3 > 99) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("H3%4d"), snr.h3);
              _repeat = false;
            }
            break;

          case 7: //Давление
            if (snr.p <= 700 || snr.p >= 800)mod ++;
            else
            {
              sprintf_P(tStr, PSTR("P%5d"), snr.p);
              _repeat = false;
            }
            break;

          case 8: //День недели, дата месяц
            sprintf_P(tStr, PSTR("%2s.%02d.%02d"), name_week7[rtd.wday], rtd.day, rtd.month);
            _repeat = false;
            break;

          case 9: //Актуальный будильник
            if (rta.num == 7) mod ++;
            else
            {
              sprintf_P(tStr, PSTR("AL%2d.%02d"), alt.h, alt.m);
              _repeat = false;
            }
            break;

          case 10: //Текущая яркость
            sprintf_P(tStr, PSTR(" L %2u  "), c_br);
            _repeat = false;
            break;

          default:
            sprintf_P(tStr, PSTR("%2d.%02d.%02d"), h, rtd.min, rtd.sec);
            _repeat = false;
            break;
        }
        break;
      case 8:
        switch (mod)
        {
          case 1: //Температура, Влажность канал 1
            if ((snr.h1 <= 0 || snr.h1 > 99) && (snr.t1 <= -99 || snr.t1 >= 99)) mod ++;
            else
            {
              if ((snr.h1 > 0 || snr.h1 <= 99) && (snr.t1 > -99 || snr.t1 < 99)) sprintf_P(tStr, PSTR("1.t%3d.H%2d"), snr.t1, snr.h1);
              if (snr.t1 <= -99 || snr.t1 >= 99) sprintf_P(tStr, PSTR("1.t---.H%2d"), snr.h1);
              if (snr.h1 <= 0   || snr.h1 >  99) sprintf_P(tStr, PSTR("1.t%3d.H--"), snr.t1);
              _repeat = false;
            }
            break;

          case 2: //Температура, Влажность канал 2
            if ((snr.h2 <= 0 || snr.h2 > 99) && (snr.t2 <= -99 || snr.t2 >= 99)) mod ++;
            else
            {
              if ((snr.h2 > 0 || snr.h2 <= 99) && (snr.t2 > -99 || snr.t2 < 99)) sprintf_P(tStr, PSTR("2.t%3d.H%2d"), snr.t2, snr.h2);
              if (snr.t2 <= -99 || snr.t2 >= 99) sprintf_P(tStr, PSTR("2.t---.H%2d"), snr.h2);
              if (snr.h2 <= 0   || snr.h2 >  99) sprintf_P(tStr, PSTR("2.t%3d.H--"), snr.t2);
              _repeat = false;
            }
            break;

          case 3: //Температура, Влажность канал 3
            if ((snr.h3 <= 0 || snr.h3 > 99) && (snr.t3 <= -99 || snr.t3 >= 99)) mod ++;
            else
            {
              if ((snr.h3 > 0 || snr.h3 <= 99) && (snr.t3 > -99 || snr.t3 < 99)) sprintf_P(tStr, PSTR("3.t%3d.H%2d"), snr.t3, snr.h3);
              if (snr.t3 <= -99 || snr.t3 >= 99) sprintf_P(tStr, PSTR("3.t---.H%2d"), snr.h3);
              if (snr.h3 <= 0   || snr.h3 >  99) sprintf_P(tStr, PSTR("3.t%3d.H--"), snr.t3);
              _repeat = false;
            }
            break;

          case 4: //Давление
            if (snr.p <= 700 || snr.p >= 800) mod ++;
            else
            {
              sprintf_P(tStr, PSTR(" P%4d  "), snr.p);
              _repeat = false;
            }
            break;

          case 5: //День недели, дата, месяц, год
            sprintf_P(tStr, PSTR("%2s.%02d.%02d.%02d"), name_week7[rtd.wday], rtd.day, rtd.month, rtd.year % 100);
            _repeat = false;
            break;

          case 6: //Актуальный будильник, текущая яркость
            if (rta.num == 7) sprintf_P(tStr, PSTR("A----L%2d"), c_br % 100);
            else sprintf_P(tStr, PSTR("A%2u.%02dL%2d"), alt.h, alt.m, c_br % 100);
            _repeat = false;
            break;

          default:
            sprintf_P(tStr, PSTR("%2d-%02d-%02d"), h % 100, rtd.min % 100, rtd.sec % 100);
            _repeat = false;
            break;
        }
        break;

      default:
        _repeat = false;
        break;
    }
  } while (_repeat);

  scr7.utf8rus(tStr);
  scr7.print_(tStr, strlen(tStr), in, _offset, font14s, 2, 0);
  return strlen(tStr);
}

