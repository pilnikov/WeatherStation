
uint8_t seg7_mode(uint8_t&, uint8_t, byte*, uint8_t, conf_data_t, snr_data_t, rtc_data_t, uint8_t);


uint8_t seg7_mode(uint8_t &mod,  uint8_t _width, byte *in, uint8_t _offset, conf_data_t cf, snr_data_t sn, rtc_data_t rt, uint8_t c_br)
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

  char tstr[10];
  uint8_t h = cf.use_pm && rt.hour != 12 ? rt.hour % 12 : rt.hour;
  h = h % 100;

  bool _repeat = true;

  do
  {
    switch (_width)
    {
      case 4:
        switch (mod)
        {
          case 1: //Температура канал 1
            if (sn.t1 <= -99 || sn.t1 >= 99) mod ++;
            else
            {
              if (sn.t1 > -10)  sprintf_P(tstr, PSTR("t\1%2d"), sn.t1);
              else sprintf_P(tstr, PSTR("t\2%2d"), (int)abs(sn.t1));
              _repeat = false;
            }
            break;

          case 2: //Влажность канал 1
            if (sn.h1 <= 0 || sn.h1 > 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("H\1%2d"), sn.h1);
              _repeat = false;
            }
            break;

          case 3: //Температура канал 2
            if (sn.t2 <= -99 || sn.t2 >= 99) mod ++;
            else
            {
              if (sn.t2 > -10) sprintf_P(tstr, PSTR("t\3%2d"), sn.t2);
              else sprintf_P(tstr, PSTR("t\4%2d"), (int)abs(sn.t2));
              _repeat = false;
            }
            break;

          case 4: //Влажность канал 2
            if (sn.h2 <= 0 || sn.h2 > 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("H\3%2d"), sn.h2);
              _repeat = false;
            }
            break;

          case 5: //Температура канал 3
            if (sn.t3 <= -99 || sn.t3 >= 99) mod ++;
            else
            {
              if (sn.t3 > - 10)sprintf_P(tstr, PSTR("t\5%2d"), sn.t3);
              else sprintf_P(tstr, PSTR("t\6%2d"), (int)abs(sn.t3));
              _repeat = false;
            }
            break;

          case 6: //Влажность канал 3
            if (sn.h3 <= 0 || sn.h3 > 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("H\5%d"), sn.h3);
              _repeat = false;
            }
            break;

          case 7: //Давление
            if (sn.p <= 700 || sn.p >= 800) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("P%3d"), sn.p);
              _repeat = false;
            }
            break;

          case 8: //День недели, дата

            sprintf_P(tstr, PSTR("%2S%2d"), name_week7[rt.wday], rt.day);
            _repeat = false;
            break;

          case 9: //Месяц, год
            sprintf_P(tstr, PSTR("%2d%2d"), rt.month, rt.year % 100);
            _repeat = false;
            break;

          case 10: //Актуальный будильник
            if (rt.a_hour == 62 && rt.a_min == 62) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("%2d%02d"), rt.a_hour, rt.a_min);
              _repeat = false;
            }
            break;

          case 11: //Секунды
            sprintf_P(tstr, PSTR("%02d5 "), rt.sec);
            _repeat = false;
            break;

          case 12: //Текущая яркость
            sprintf_P(tstr, PSTR("L%3d"), c_br);
            _repeat = false;
            break;

          default:
            sprintf_P(tstr, PSTR("%2d%02d"), h, rt.min);
            _repeat = false;
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
              sprintf_P(tstr, PSTR("t1%4d"), sn.t1);
              _repeat = false;
            }
            break;

          case 2: //Влажность канал 1
            if (sn.h1 <= 0 || sn.h1 > 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("H1%4d"), sn.h1);
              _repeat = false;
            }
            break;

          case 3: //Температура канал 2
            if (sn.t2 <= -99 || sn.t2 >= 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("t2%4d"), sn.t2);
              _repeat = false;
            }
            break;

          case 4: //Влажность канал 2
            if (sn.h2 <= 0 || sn.h2 > 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("H2%4d"), sn.h2);
              _repeat = false;
            }
            break;

          case 5: //Температура канал 3
            if (sn.t3 <= -99 || sn.t3 >= 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("t3%4d"), sn.t3);
              _repeat = false;
            }
            break;

          case 6: //Влажность канал 3
            if (sn.h3 <= 0 || sn.h3 > 99) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("H3%4d"), sn.h3);
              _repeat = false;
            }
            break;

          case 7: //Давление
            if (sn.p <= 700 || sn.p >= 800)mod ++;
            else
            {
              sprintf_P(tstr, PSTR("P%5d"), sn.p);
              _repeat = false;
            }
            break;

          case 8: //День недели, дата месяц
            sprintf_P(tstr, PSTR("%2S.%02d.%02d"), name_week7[rt.wday], rt.day, rt.month);
            _repeat = false;
            break;

          case 9: //Актуальный будильник
            if (rt.a_hour == 62 && rt.a_min == 62) mod ++;
            else
            {
              sprintf_P(tstr, PSTR("AL%2d.%02d"), rt.a_hour, rt.a_min);
              _repeat = false;
            }
            break;

          case 10: //Текущая яркость
            sprintf_P(tstr, PSTR(" L %2u  "), c_br);
            _repeat = false;
            break;

          default:
            sprintf_P(tstr, PSTR("%2d.%02d.%02d"), h, rt.min, rt.sec);
            _repeat = false;
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
              if ((sn.h1 > 0 || sn.h1 <= 99) && (sn.t1 > -99 || sn.t1 < 99)) sprintf_P(tstr, PSTR("1.t%3d.H%2d"), sn.t1, sn.h1);
              if (sn.t1 <= -99 || sn.t1 >= 99) sprintf_P(tstr, PSTR("1.t---.H%2d"), sn.h1);
              if (sn.h1 <= 0   || sn.h1 >  99) sprintf_P(tstr, PSTR("1.t%3d.H--"), sn.t1);
              _repeat = false;
            }
            break;

          case 2: //Температура, Влажность канал 2
            if ((sn.h2 <= 0 || sn.h2 > 99) && (sn.t2 <= -99 || sn.t2 >= 99)) mod ++;
            else
            {
              if ((sn.h2 > 0 || sn.h2 <= 99) && (sn.t2 > -99 || sn.t2 < 99)) sprintf_P(tstr, PSTR("2.t%3d.H%2d"), sn.t2, sn.h2);
              if (sn.t2 <= -99 || sn.t2 >= 99) sprintf_P(tstr, PSTR("2.t---.H%2d"), sn.h2);
              if (sn.h2 <= 0   || sn.h2 >  99) sprintf_P(tstr, PSTR("2.t%3d.H--"), sn.t2);
              _repeat = false;
            }
            break;

          case 3: //Температура, Влажность канал 3
            if ((sn.h3 <= 0 || sn.h3 > 99) && (sn.t3 <= -99 || sn.t3 >= 99)) mod ++;
            else
            {
              if ((sn.h3 > 0 || sn.h3 <= 99) && (sn.t3 > -99 || sn.t3 < 99)) sprintf_P(tstr, PSTR("3.t%3d.H%2d"), sn.t3, sn.h3);
              if (sn.t3 <= -99 || sn.t3 >= 99) sprintf_P(tstr, PSTR("3.t---.H%2d"), sn.h3);
              if (sn.h3 <= 0   || sn.h3 >  99) sprintf_P(tstr, PSTR("3.t%3d.H--"), sn.t3);
              _repeat = false;
            }
            break;

          case 4: //Давление
            if (sn.p <= 700 || sn.p >= 800) mod ++;
            else
            {
              sprintf_P(tstr, PSTR(" P%4d  "), sn.p);
              _repeat = false;
            }
            break;

          case 5: //День недели, дата, месяц, год
            sprintf_P(tstr, PSTR("%2S.%02d.%02d.%02d"), name_week7[rt.wday], rt.day, rt.month, rt.year % 100);
            _repeat = false;
            break;

          case 6: //Актуальный будильник, текущая яркость
            if (rt.a_hour == 62 && rt.a_min == 62) sprintf_P(tstr, PSTR("A----L%2d"), c_br % 100);
            else sprintf_P(tstr, PSTR("A%2u.%02dL%2d"), rt.a_hour % 100, rt.a_min % 100, c_br % 100);
            _repeat = false;
            break;

          default:
            sprintf_P(tstr, PSTR("%2d-%02d-%02d"), h % 100, rt.min % 100, rt.sec % 100);
            _repeat = false;
            break;
        }
        break;

      default:
        _repeat = false;
        break;
    }
  } while (_repeat);

  //  DBG_OUT_PORT.print(F("test string"));
  //  DBG_OUT_PORT.println(tstr);
  
  f_dsp.utf8rus(tstr);
  f_dsp.print_(tstr, strlen(tstr), in, _offset, font14s, 2, 0);
  return strlen(tstr);
}
//#endif
