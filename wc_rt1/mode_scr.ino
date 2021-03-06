uint8_t seg7_mode(uint8_t&, uint8_t, byte*, uint8_t);


uint8_t seg7_mode(uint8_t &mod,  uint8_t _width, byte *in, uint8_t _offset)
{
  uint8_t h = conf_data.use_pm && hour() > 12 ? h = hour() - 12 : hour();
  size_tstr = snprintf(tstr, 8, "--------");
  uint8_t s_tstr = _width * 2;
  bool out = false;

  switch (_width)
  {
    case 4:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (snr_data.t1 <= -99 || snr_data.t1 >= 99) mod ++;
          else
          {
            if (snr_data.t1 > -10)  size_tstr = snprintf(tstr, s_tstr, "t\1%2d", snr_data.t1);
            else size_tstr = snprintf(tstr, s_tstr, "t\2%2d", abs(snr_data.t1));
            out = true;
          }
          break;

        case 2: //Влажность канал 1
          if (snr_data.h1 <= 0 || snr_data.h1 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H\1%2d", snr_data.h1);
            out = true;
          }
          break;

        case 3: //Температура канал 2
          if (snr_data.t2 <= -99 || snr_data.t2 >= 99) mod ++;
          else
          {
            if (snr_data.t2 > -10) size_tstr = snprintf(tstr, s_tstr, "t\3%2d", snr_data.t2);
            else size_tstr = snprintf(tstr, s_tstr, "t\4%2d", abs(snr_data.t2));
            out = true;
          }
          break;

        case 4: //Влажность канал 2
          if (snr_data.h2 <= 0 || snr_data.h2 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H\3%2d", snr_data.h2);
            out = true;
          }
          break;

        case 5: //Температура канал 3
          if (snr_data.t3 <= -99 || snr_data.t3 >= 99) mod ++;
          else
          {
            if (snr_data.t3 > - 10)size_tstr = snprintf(tstr, s_tstr, "t\5%2d", snr_data.t3);
            else size_tstr = snprintf(tstr, s_tstr, "t\6%2d", abs(snr_data.t3));
            out = true;
          }
          break;

        case 6: //Влажность канал 3
          if (snr_data.h3 <= 0 || snr_data.h3 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H\5%d", snr_data.h3);
            out = true;
          }
          break;

        case 7: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "P%3d", snr_data.p);
            out = true;
          }
          break;

        case 8: //День недели, дата
          size_tstr = snprintf(tstr, s_tstr, "%2s%2d", name_week7[weekday()], day());
          out = true;
          break;

        case 9: //Месяц, год
          size_tstr = snprintf(tstr, s_tstr, "%2d%2d", month(), year() % 100);
          out = true;
          break;

        case 10: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "%2d%02d", rtc_data.a_hour, rtc_data.a_min);
            out = true;
          }
          break;

        case 11: //Секунды
          size_tstr = snprintf(tstr, s_tstr, "%02d5 ", second());
          out = true;
          break;

        case 12: //Текущая яркость
          size_tstr = snprintf(tstr, s_tstr, "L%3d", cur_br);
          out = true;
          break;

        default:
          size_tstr = snprintf(tstr, s_tstr, "%2d%02d", h, minute());
          out = true;
          break;
      }
      break;
    case 6:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (snr_data.t1 <= -99 || snr_data.t1 >= 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "t1%4d", snr_data.t1);
            out = true;
          }
          break;

        case 2: //Влажность канал 1
          if (snr_data.h1 <= 0 || snr_data.h1 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H1%4d", snr_data.h1);
            out = true;
          }
          break;

        case 3: //Температура канал 2
          if (snr_data.t2 <= -99 || snr_data.t2 >= 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "t2%4d", snr_data.t2);
            out = true;
          }
          break;

        case 4: //Влажность канал 2
          if (snr_data.h2 <= 0 || snr_data.h2 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H2%4d", snr_data.h2);
            out = true;
          }
          break;

        case 5: //Температура канал 3
          if (snr_data.t3 <= -99 || snr_data.t3 >= 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "t3%4d", snr_data.t3);
            out = true;
          }
          break;

        case 6: //Влажность канал 3
          if (snr_data.h3 <= 0 || snr_data.h3 > 99) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "H3%4d", snr_data.h3);
            out = true;
          }
          break;

        case 7: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "P%5d", snr_data.p);
            out = true;
          }
          break;

        case 8: //День недели, дата месяц
          size_tstr = snprintf(tstr, s_tstr, "%2s.%02d.%02d", name_week7[weekday()], day(), month());
          out = true;
          break;

        case 9: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "AL%2d.%02d", rtc_data.a_hour, rtc_data.a_min);
            out = true;
          }
          break;

        case 10: //Текущая яркость
          size_tstr = snprintf(tstr, s_tstr, " L %2u  ", cur_br);
          out = true;
          break;

        default:
          size_tstr = snprintf(tstr, s_tstr, "%2d.%02d.%02d", h, minute(), second());
          out = true;
          break;
      }
      break;
    case 8:
      switch (mod)
      {
        case 1: //Температура, Влажность канал 1
          if ((snr_data.h1 <= 0 || snr_data.h1 > 99) && (snr_data.t1 <= -99 || snr_data.t1 >= 99)) mod ++;
          else
          {
            if ((snr_data.h1 > 0 || snr_data.h1 <= 99) && (snr_data.t1 > -99 || snr_data.t1 < 99)) size_tstr = snprintf(tstr, s_tstr, "1.t%3d.H%2d", snr_data.t1, snr_data.h1);
            if (snr_data.t1 <= -99 || snr_data.t1 >= 99) size_tstr = snprintf(tstr, s_tstr, "1.t---.H%2d", snr_data.t1, snr_data.h1);
            if (snr_data.h1 <= 0   || snr_data.h1 >  99) size_tstr = snprintf(tstr, s_tstr, "1.t%3d.H--", snr_data.t1, snr_data.h1);
            out = true;
          }
          break;

        case 2: //Температура, Влажность канал 2
          if ((snr_data.h2 <= 0 || snr_data.h2 > 99) && (snr_data.t2 <= -99 || snr_data.t2 >= 99)) mod ++;
          else
          {
            if ((snr_data.h2 > 0 || snr_data.h2 <= 99) && (snr_data.t2 > -99 || snr_data.t2 < 99)) size_tstr = snprintf(tstr, s_tstr, "2.t%3d.H%2d", snr_data.t2, snr_data.h2);
            if (snr_data.t2 <= -99 || snr_data.t2 >= 99) size_tstr = snprintf(tstr, s_tstr, "2.t---.H%2d", snr_data.t2, snr_data.h2);
            if (snr_data.h2 <= 0   || snr_data.h2 >  99) size_tstr = snprintf(tstr, s_tstr, "2.t%3d.H--", snr_data.t2, snr_data.h2);
            out = true;
          }
          break;

        case 3: //Температура, Влажность канал 3
          if ((snr_data.h3 <= 0 || snr_data.h3 > 99) && (snr_data.t3 <= -99 || snr_data.t3 >= 99)) mod ++;
          else
          {
            if ((snr_data.h3 > 0 || snr_data.h3 <= 99) && (snr_data.t3 > -99 || snr_data.t3 < 99)) size_tstr = snprintf(tstr, s_tstr, "3.t%3d.H%2d", snr_data.t3, snr_data.h3);
            if (snr_data.t3 <= -99 || snr_data.t3 >= 99) size_tstr = snprintf(tstr, s_tstr, "3.t---.H%2d", snr_data.t3, snr_data.h3);
            if (snr_data.h3 <= 0   || snr_data.h3 >  99) size_tstr = snprintf(tstr, s_tstr, "3.t%3d.H--", snr_data.t3, snr_data.h3);
            out = true;
          }
          break;

        case 4: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800) mod ++;
          else
          {
            size_tstr = snprintf(tstr, s_tstr, " P%4d  ", snr_data.p);
            out = true;
          }
          break;

        case 5: //День недели, дата, месяц, год
          size_tstr = snprintf(tstr, s_tstr, "%2s.%02d.%02d.02d", name_week7[weekday()], day(), month(), year() % 100);
          out = true;
          break;

        case 6: //Актуальный будильник, текущая яркость
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) size_tstr = sprintf(tstr, "A----L%2d", cur_br);
          else
          {
            size_tstr = snprintf(tstr, s_tstr, "A%2u.%02dL%2d", rtc_data.a_hour, rtc_data.a_min, cur_br);
            out = true;
          }
          break;

        default:
          size_tstr = snprintf(tstr, s_tstr, "%2d-%02d-%02d", h, minute(), second());
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
    print_(tstr, strlen(tstr), in, _offset, font14s, 2, 0);
    return strlen(tstr);
  }
  memset (in + _offset, 0, strlen(tstr));
  return -1;
}
