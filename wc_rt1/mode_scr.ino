

void seg7_mode (uint8_t mod,  uint8_t from, uint8_t _width)
{
  uint8_t h = conf_data.use_pm && hour() > 12 ? h = hour() - 12 : hour();
  size_tstr = sprintf(tstr, "--------");

  switch (_width)
  {
    case 4:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (snr_data.t1 <= -99 || snr_data.t1 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else
          {
            if (snr_data.t1 > 0)  size_tstr = sprintf(tstr, "t_%-2u", abs(snr_data.t1));
            else size_tstr = sprintf(tstr, "t=%-2u", abs(snr_data.t1));
          }
          break;

        case 2: //Влажность канал 1
          if (snr_data.h1 <= 0 || snr_data.h1 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "H_%-2u", snr_data.h1);
          break;

        case 3: //Температура канал 2
          if (snr_data.t2 <= -99 || snr_data.t2 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else
          {
            if (snr_data.t2 > 0) size_tstr = sprintf(tstr, "tT%-2u", abs(snr_data.t2));
            else size_tstr = sprintf(tstr, "t\323%-2u", abs(snr_data.t2));
          }
          break;

        case 4: //Влажность канал 2
          if (snr_data.h2 <= 0 || snr_data.h2 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "HT%-2u", snr_data.h2);
          break;

        case 5: //Температура канал 3
          if (snr_data.t3 <= -99 || snr_data.t3 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else
          {
            if (snr_data.t3 > 0)size_tstr = sprintf(tstr, "tZ%-2u", abs(snr_data.t3));
            else size_tstr = sprintf(tstr, "t{%-2u", abs(snr_data.t3));
          }
          break;

        case 6: //Влажность канал 3
          if (snr_data.h3 <= 0 || snr_data.h3 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "HZ%-2u", snr_data.h3);
          break;

        case 7: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "P%3u", snr_data.p);
          break;

        case 8: //День недели, дата
          size_tstr = sprintf(tstr, "%2s%2u", name_week7[weekday()], day());
          break;

        case 9: //Месяц, год
          size_tstr = sprintf(tstr, "%02u%02u", month(), year() % 100);
          break;

        case 10: //Секунды
          size_tstr = sprintf(tstr, "%02u5", second());
          break;

        case 11: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "%2u.%02u", rtc_data.a_hour, rtc_data.a_min);
          break;

        case 12: //Текущая яркость
          size_tstr = sprintf(tstr, "L %2u", cur_br);
          break;

        default:
          size_tstr = sprintf(tstr, "%2u%02u", h, minute());
          break;
      }
      break;
    case 6:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (snr_data.t1 <= -99 || snr_data.t1 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else 
          {
            if (snr_data.t1 > 0) size_tstr = sprintf(tstr, "t1.  %-2u", abs(snr_data.t1));
            else size_tstr = sprintf(tstr, "t1. -%-2u", abs(snr_data.t1));
          }
          break;

        case 2: //Влажность канал 1
          if (snr_data.h1 <= 0 || snr_data.h1 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "H1.  %-2u", snr_data.h1);
          break;

        case 3: //Температура канал 2
          if (snr_data.t2 <= -99 || snr_data.t2 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else 
          {
            if (snr_data.t2 > 0) size_tstr = sprintf(tstr, "t2.  %-2u", abs(snr_data.t2));
            else size_tstr = sprintf(tstr, "t2. -%-2u", abs(snr_data.t2));
          }
          break;

        case 4: //Влажность канал 2
          if (snr_data.h2 <= 0 || snr_data.h2 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "H2.  %-2u", snr_data.h2);
          break;

        case 5: //Температура канал 3
          if (snr_data.t3 <= -99 || snr_data.t3 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else 
          {
            if (snr_data.t3 > 0) size_tstr = sprintf(tstr, "t3.  %-2u", abs(snr_data.t3));
            else size_tstr = sprintf(tstr, "t3. -%-2u", abs(snr_data.t3));
          }
          break;

        case 6: //Влажность канал 3
          if (snr_data.h3 <= 0 || snr_data.h3 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "H3.  %-2u", snr_data.h3);
          break;

        case 7: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, " P %3u", snr_data.p);
          break;

        case 8: //День недели, дата месяц
          size_tstr = sprintf(tstr, "%2s.%02u.%02u.", name_week7[weekday()], day(), month());
          break;

        case 9: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "AL.%2u.%02u", rtc_data.a_hour, rtc_data.a_min);
          break;

        case 10: //Текущая яркость
          size_tstr = sprintf(tstr, " L %2u  ", cur_br);
          break;

        default:
          size_tstr = sprintf(tstr, "%2u.%02u.%02u", h, minute(), second());
          break;
      }
      break;
    case 8:
      switch (mod)
      {
        case 1: //Температура, Влажность канал 1
          if ((snr_data.h1 <= 0 || snr_data.h1 > 99) && (snr_data.t1 <= -99 || snr_data.t1 >= 99))
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else
          {
            if ((snr_data.h1 > 0 || snr_data.h1 <= 99) && (snr_data.t1 > -99 || snr_data.t1 < 99)) size_tstr = sprintf(tstr, "1t%3dH%02u", snr_data.t1, snr_data.h1);
            if (snr_data.t1 <= -99 || snr_data.t1 >= 99) size_tstr = sprintf(tstr, "1t---H%2d", snr_data.t1, snr_data.h1);
            if (snr_data.h1 <= 0   || snr_data.h1 >  99) size_tstr = sprintf(tstr, "1t%3dH--", snr_data.t1, snr_data.h1);
          }
          break;

        case 2: //Температура, Влажность канал 2
          if ((snr_data.h2 <= 0 || snr_data.h2 > 99) && (snr_data.t2 <= -99 || snr_data.t2 >= 99))
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else
          {
            if ((snr_data.h2 > 0 || snr_data.h2 <= 99) && (snr_data.t2 > -99 || snr_data.t2 < 99)) size_tstr = sprintf(tstr, "2t%3dH%02u", snr_data.t2, snr_data.h2);
            if (snr_data.t2 <= -99 || snr_data.t2 >= 99) size_tstr = sprintf(tstr, "2t---H%2d", snr_data.t2, snr_data.h2);
            if (snr_data.h2 <= 0   || snr_data.h2 >  99) size_tstr = sprintf(tstr, "2t%3dH--", snr_data.t2, snr_data.h2);
          }
          break;

        case 3: //Температура, Влажность канал 3
          if ((snr_data.h3 <= 0 || snr_data.h3 > 99) && (snr_data.t3 <= -99 || snr_data.t3 >= 99))
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else
          {
            if ((snr_data.h3 > 0 || snr_data.h3 <= 99) && (snr_data.t3 > -99 || snr_data.t3 < 99)) size_tstr = sprintf(tstr, "3t%3dH%02u", snr_data.t3, snr_data.h3);
            if (snr_data.t3 <= -99 || snr_data.t3 >= 99) size_tstr = sprintf(tstr, "3t---H%2d", snr_data.t3, snr_data.h3);
            if (snr_data.h3 <= 0   || snr_data.h3 >  99) size_tstr = sprintf(tstr, "3t%3dH--", snr_data.t3, snr_data.h3);
          }
          break;

        case 4: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)
          {
            disp_mode ++;
            seg7_mode(disp_mode, from, _width);
          }
          else size_tstr = sprintf(tstr, "  P%03u  ", snr_data.p);
          break;

        case 5: //День недели, дата, месяц, год
          size_tstr = sprintf(tstr, "%2s%02u.%02u.02u", name_week7[weekday()], day(), month(), year() % 100);
          break;

        case 6: //Актуальный будильник, текущая яркость
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) size_tstr = sprintf(tstr, "A----L%2u", cur_br);
          else size_tstr = sprintf(tstr, "A%2u.%02uL%2u", rtc_data.a_hour, rtc_data.a_min, cur_br);
          break;

        default:
          size_tstr = sprintf(tstr, "%2u-%02u-%02u", h, minute(), second());
          break;
      }
      break;

    default:
      break;
  }
  printDisplay_S(tstr, from, from + _width - 1);
}
