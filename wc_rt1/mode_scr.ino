void seg7_mode(uint8_t,  uint8_t);


void seg7_mode(uint8_t mod,  uint8_t _width)
{
  uint8_t h = conf_data.use_pm && hour() > 12 ? h = hour() - 12 : hour();
  size_tstr = snprintf(tstr, 9, "--------");

  switch (_width)
  {
    case 4:
      switch (mod)
      {
        case 1: //Температура канал 1
          if (snr_data.t1 <= -99 || snr_data.t1 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else
          {
            if (snr_data.t1 > -10)  size_tstr = snprintf(tstr, 5, "t_%d", snr_data.t1);
            else size_tstr = snprintf(tstr, 10, "t=%d", abs(snr_data.t1));
          }
          break;

        case 2: //Влажность канал 1
          if (snr_data.h1 <= 0 || snr_data.h1 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 10, "H_%d", snr_data.h1);
          break;

        case 3: //Температура канал 2
          if (snr_data.t2 <= -99 || snr_data.t2 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else
          {
            if (snr_data.t2 > -10) size_tstr = snprintf(tstr, 10, "tT%d", snr_data.t2);
            else size_tstr = snprintf(tstr, 10, "t\323%d", abs(snr_data.t2));
          }
          break;

        case 4: //Влажность канал 2
          if (snr_data.h2 <= 0 || snr_data.h2 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 10, "HT%d", snr_data.h2);
          break;

        case 5: //Температура канал 3
          if (snr_data.t3 <= -99 || snr_data.t3 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else
          {
            if (snr_data.t3 > - 10)size_tstr = snprintf(tstr, 10, "tZ%d", snr_data.t3);
            else size_tstr = snprintf(tstr, 10, "t{%d", abs(snr_data.t3));
          }
          break;

        case 6: //Влажность канал 3
          if (snr_data.h3 <= 0 || snr_data.h3 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 10, "HZ%d", snr_data.h3);
          break;

        case 7: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 10, "P%3u", snr_data.p);
          break;

        case 8: //День недели, дата
          size_tstr = snprintf(tstr, 10, "%2s%2u", name_week7[weekday()], day());
          break;

        case 9: //Месяц, год
          size_tstr = snprintf(tstr, 10, "%02u%d", month(), year() % 100);
          break;

        case 10: //Секунды
          size_tstr = snprintf(tstr, 10, "%02d5", second());
          break;

        case 11: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 10, "%2u%02u", rtc_data.a_hour, rtc_data.a_min);
          break;

        case 12: //Текущая яркость
          size_tstr = snprintf(tstr, 10, "L %2d", cur_br);
          break;

        default:
          size_tstr = snprintf(tstr, 10, "%d%02d", h, minute());
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
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 7, "t1 %d", snr_data.t1);
          break;

        case 2: //Влажность канал 1
          if (snr_data.h1 <= 0 || snr_data.h1 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, "H1. %d", snr_data.h1);
          break;

        case 3: //Температура канал 2
          if (snr_data.t2 <= -99 || snr_data.t2 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, "t2 %d", snr_data.t2);
          break;

        case 4: //Влажность канал 2
          if (snr_data.h2 <= 0 || snr_data.h2 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, "H2  %d", snr_data.h2);
          break;

        case 5: //Температура канал 3
          if (snr_data.t3 <= -99 || snr_data.t3 >= 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, "t3 %d", snr_data.t3);
          break;

        case 6: //Влажность канал 3
          if (snr_data.h3 <= 0 || snr_data.h3 > 99)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, "H3  %d", snr_data.h3);
          break;

        case 7: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, " P. %d", snr_data.p);
          break;

        case 8: //День недели, дата месяц
          size_tstr = snprintf(tstr, 14, "%2s.%02.u%02u", name_week7[weekday()], day(), month());
          break;

        case 9: //Актуальный будильник
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 14, "AL%2u.%02u", rtc_data.a_hour, rtc_data.a_min);
          break;

        case 10: //Текущая яркость
          size_tstr = snprintf(tstr, 14, " L %2u  ", cur_br);
          break;

        default:
          size_tstr = snprintf(tstr, 14, "%2u.%02u.%02u", h, minute(), second());
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
            seg7_mode(disp_mode, _width);
          }
          else
          {
            if ((snr_data.h1 > 0 || snr_data.h1 <= 99) && (snr_data.t1 > -99 || snr_data.t1 < 99)) size_tstr = snprintf(tstr, 18, "1t%3dH%d", snr_data.t1, snr_data.h1);
            if (snr_data.t1 <= -99 || snr_data.t1 >= 99) size_tstr = snprintf(tstr, 18, "1t---H%d", snr_data.t1, snr_data.h1);
            if (snr_data.h1 <= 0   || snr_data.h1 >  99) size_tstr = snprintf(tstr, 18, "1t%3dH--", snr_data.t1, snr_data.h1);
          }
          break;

        case 2: //Температура, Влажность канал 2
          if ((snr_data.h2 <= 0 || snr_data.h2 > 99) && (snr_data.t2 <= -99 || snr_data.t2 >= 99))
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else
          {
            if ((snr_data.h2 > 0 || snr_data.h2 <= 99) && (snr_data.t2 > -99 || snr_data.t2 < 99)) size_tstr = snprintf(tstr, 18, "2t%3dH%d", snr_data.t2, snr_data.h2);
            if (snr_data.t2 <= -99 || snr_data.t2 >= 99) size_tstr = snprintf(tstr, 18, "2t---H%d", snr_data.t2, snr_data.h2);
            if (snr_data.h2 <= 0   || snr_data.h2 >  99) size_tstr = snprintf(tstr, 18, "2t%3dH--", snr_data.t2, snr_data.h2);
          }
          break;

        case 3: //Температура, Влажность канал 3
          if ((snr_data.h3 <= 0 || snr_data.h3 > 99) && (snr_data.t3 <= -99 || snr_data.t3 >= 99))
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else
          {
            if ((snr_data.h3 > 0 || snr_data.h3 <= 99) && (snr_data.t3 > -99 || snr_data.t3 < 99)) size_tstr = snprintf(tstr, 18, "3t%3dH%d", snr_data.t3, snr_data.h3);
            if (snr_data.t3 <= -99 || snr_data.t3 >= 99) size_tstr = snprintf(tstr, 18, "3t---H%d", snr_data.t3, snr_data.h3);
            if (snr_data.h3 <= 0   || snr_data.h3 >  99) size_tstr = snprintf(tstr, 18, "3t%3dH--", snr_data.t3, snr_data.h3);
          }
          break;

        case 4: //Давление
          if (snr_data.p <= 700 || snr_data.p >= 800)
          {
            disp_mode ++;
            seg7_mode(disp_mode, _width);
          }
          else size_tstr = snprintf(tstr, 18, "  P%03u  ", snr_data.p);
          break;

        case 5: //День недели, дата, месяц, год
          size_tstr = snprintf(tstr, 18, "%2s%02u%02u02u", name_week7[weekday()], day(), month(), year() % 100);
          break;

        case 6: //Актуальный будильник, текущая яркость
          if (rtc_data.a_hour == 62 && rtc_data.a_min == 62) size_tstr = sprintf(tstr, "A----L%2u", cur_br);
          else size_tstr = snprintf(tstr, 18, "A%2u.%02uL%2u", rtc_data.a_hour, rtc_data.a_min, cur_br);
          break;

        default:
          size_tstr = snprintf(tstr, 18, "%2u-%02u-%02u", h, minute(), second());
          break;
      }
      break;

    default:
      break;
  }
  print_(tstr, strlen(tstr), screen, font14s, 2, 0);
}
