
uint8_t seg7_mode(uint8_t&, uint8_t, byte*, uint8_t, conf_data_t, snr_data_t, rtc_data_t, uint8_t);


uint8_t seg7_mode(uint8_t &mod,  uint8_t _width, byte *in, uint8_t _offset, conf_data_t cf, snr_data_t sn, rtc_data_t rt, uint8_t c_br)
{

  const char name_week_0[] = "";
  const char name_week_1[] = "8c";
  const char name_week_2[] = "\357H";
  const char name_week_3[] = "8t";
  const char name_week_4[] = "cP";
  const char name_week_5[] = "4t";
  const char name_week_6[] = "\357t";
  const char name_week_7[] = "c6";

  const char* const name_week7[] = {name_week_0, name_week_1, name_week_2, name_week_3, name_week_4, name_week_5, name_week_6, name_week_7};

  char tstr[10];
  uint8_t h = cf.use_pm && rt.hour != 12 ? rt.hour % 12 : rt.hour;
   h = h % 100;

  uint8_t s_tstr = _width * 2;
  uint8_t size_tstr = 1;
  bool out = false;

  switch (_width)
  {
    case 4:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (sn.t1 <= -99 || sn.t1 >= 99) mod ++;
          else
          {
            if (sn.t1 > -10)  size_tstr = snprintf(tstr, s_tstr, "t\1%2d", sn.t1);
            else size_tstr = snprintf(tstr, s_tstr, "t\2%2d", abs(sn.t1));
            out = true;
          }
          break;

        case 2: //Влажность канал 1
          if (sn.h1 <= 0 || sn.h1 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H\1%2d", sn.h1);
            out = true;
          }
          break;

        case 3: //Температура канал 2
          if (sn.t2 <= -99 || sn.t2 >= 99) mod ++;
          else
          {
            if (sn.t2 > -10) size_tstr = snprintf(tstr, s_tstr, "t\3%2d", sn.t2);
            else size_tstr = snprintf(tstr, s_tstr, "t\4%2d", abs(sn.t2));
            out = true;
          }
          break;

        case 4: //Влажность канал 2
          if (sn.h2 <= 0 || sn.h2 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H\3%2d", sn.h2);
            out = true;
          }
          break;

        case 5: //Температура канал 3
          if (sn.t3 <= -99 || sn.t3 >= 99) mod ++;
          else
          {
            if (sn.t3 > - 10)size_tstr = snprintf(tstr, s_tstr, "t\5%2d", sn.t3);
            else size_tstr = snprintf(tstr, s_tstr, "t\6%2d", abs(sn.t3));
            out = true;
          }
          break;

        case 6: //Влажность канал 3
          if (sn.h3 <= 0 || sn.h3 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H\5%d", sn.h3);
            out = true;
          }
          break;

        case 7: //Давление
          if (sn.p <= 700 || sn.p >= 800) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "P%3d", sn.p);
            out = true;
          }
          break;

        case 8: //День недели, дата

          size_tstr = snprintf(tstr, s_tstr, "%2s%2d", name_week7[rt.wday], rt.day);
          out = true;
          break;

        case 9: //Месяц, год
          size_tstr = snprintf(tstr, s_tstr, "%2d%2d", rt.month, rt.year % 100);
          out = true;
          break;

        case 10: //Актуальный будильник
          if (rt.a_hour == 62 && rt.a_min == 62) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "%2d%02d", rt.a_hour, rt.a_min);
            out = true;
          }
          break;

        case 11: //Секунды
          size_tstr = snprintf(tstr, s_tstr, "%02d5 ", rt.sec);
          out = true;
          break;

        case 12: //Текущая яркость
          size_tstr = snprintf(tstr, s_tstr, "L%3d", c_br);
          out = true;
          break;

        default:
          size_tstr = snprintf(tstr, s_tstr, "%2d%02d", h, rt.min);
          out = true;
          break;
      }
      break;
    case 6:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (sn.t1 <= -99 || sn.t1 >= 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "t1%4d", sn.t1);
            out = true;
          }
          break;

        case 2: //Влажность канал 1
          if (sn.h1 <= 0 || sn.h1 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H1%4d", sn.h1);
            out = true;
          }
          break;

        case 3: //Температура канал 2
          if (sn.t2 <= -99 || sn.t2 >= 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "t2%4d", sn.t2);
            out = true;
          }
          break;

        case 4: //Влажность канал 2
          if (sn.h2 <= 0 || sn.h2 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H2%4d", sn.h2);
            out = true;
          }
          break;

        case 5: //Температура канал 3
          if (sn.t3 <= -99 || sn.t3 >= 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "t3%4d", sn.t3);
            out = true;
          }
          break;

        case 6: //Влажность канал 3
          if (sn.h3 <= 0 || sn.h3 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H3%4d", sn.h3);
            out = true;
          }
          break;

        case 7: //Давление
          if (sn.p <= 700 || sn.p >= 800)mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "P%5d", sn.p);
            out = true;
          }
          break;

        case 8: //День недели, дата месяц
          size_tstr = snprintf(tstr, s_tstr, "%2s.%02d.%02d", name_week7[rt.wday], rt.day, rt.month);
          out = true;
          break;

        case 9: //Актуальный будильник
          if (rt.a_hour == 62 && rt.a_min == 62) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "AL%2d.%02d", rt.a_hour, rt.a_min);
            out = true;
          }
          break;

        case 10: //Текущая яркость
          size_tstr = snprintf(tstr, s_tstr, " L %2u  ", c_br);
          out = true;
          break;

        default:
          size_tstr = snprintf(tstr, s_tstr, "%2d.%02d.%02d", h, rt.min, rt.sec);
          out = true;
          break;
      }
      break;
    case 8:
      switch (mod)
      {
        case 1: //Температура, Влажность канал 1
          if ((sn.h1 <= 0 || sn.h1 > 99) && (sn.t1 <= -99 || sn.t1 >= 99)) mod ++;
          else
          {
            if ((sn.h1 > 0 || sn.h1 <= 99) && (sn.t1 > -99 || sn.t1 < 99)) size_tstr = snprintf(tstr, s_tstr, "1.t%3d.H%2d", sn.t1, sn.h1);
            if (sn.t1 <= -99 || sn.t1 >= 99) size_tstr = snprintf(tstr, s_tstr, "1.t---.H%2d", sn.h1);
            if (sn.h1 <= 0   || sn.h1 >  99) size_tstr = snprintf(tstr, s_tstr, "1.t%3d.H--", sn.t1);
            out = true;
          }
          break;

        case 2: //Температура, Влажность канал 2
          if ((sn.h2 <= 0 || sn.h2 > 99) && (sn.t2 <= -99 || sn.t2 >= 99)) mod ++;
          else
          {
            if ((sn.h2 > 0 || sn.h2 <= 99) && (sn.t2 > -99 || sn.t2 < 99)) size_tstr = snprintf(tstr, s_tstr, "2.t%3d.H%2d", sn.t2, sn.h2);
            if (sn.t2 <= -99 || sn.t2 >= 99) size_tstr = snprintf(tstr, s_tstr, "2.t---.H%2d", sn.h2);
            if (sn.h2 <= 0   || sn.h2 >  99) size_tstr = snprintf(tstr, s_tstr, "2.t%3d.H--", sn.t2);
            out = true;
          }
          break;

        case 3: //Температура, Влажность канал 3
          if ((sn.h3 <= 0 || sn.h3 > 99) && (sn.t3 <= -99 || sn.t3 >= 99)) mod ++;
          else
          {
            if ((sn.h3 > 0 || sn.h3 <= 99) && (sn.t3 > -99 || sn.t3 < 99)) size_tstr = snprintf(tstr, s_tstr, "3.t%3d.H%2d", sn.t3, sn.h3);
            if (sn.t3 <= -99 || sn.t3 >= 99) size_tstr = snprintf(tstr, s_tstr, "3.t---.H%2d", sn.h3);
            if (sn.h3 <= 0   || sn.h3 >  99) size_tstr = snprintf(tstr, s_tstr, "3.t%3d.H--", sn.t3);
            out = true;
          }
          break;

        case 4: //Давление
          if (sn.p <= 700 || sn.p >= 800) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, " P%4d  ", sn.p);
            out = true;
          }
          break;

        case 5: //День недели, дата, месяц, год
          size_tstr = snprintf(tstr, s_tstr, "%2s.%02d.%02d.%02d", name_week7[rt.wday], rt.day, rt.month, rt.year % 100);
          out = true;
          break;

        case 6: //Актуальный будильник, текущая яркость
          if (rt.a_hour == 62 && rt.a_min == 62) size_tstr = sprintf(tstr, "A----L%2d", c_br % 100);
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "A%2u.%02dL%2d", rt.a_hour % 100, rt.a_min % 100, c_br % 100);
            out = true;
          }
          break;

        default:
          size_tstr = snprintf(tstr, s_tstr, "%2d-%02d-%02d", h % 100, rt.min % 100, rt.sec % 100);
          out = true;
          break;
      }
      break;

    default:
      break;
  }
  if (out)
  {
    f_dsp.utf8rus(tstr);
    f_dsp.print_(tstr, strlen(tstr), in, _offset, font14s, 2, 0);
    return strlen(tstr);
  }
  else
  {
    memset (in + _offset, 0, s_tstr);
    return -1;
  }
}
