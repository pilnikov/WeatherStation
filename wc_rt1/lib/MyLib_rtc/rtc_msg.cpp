#include <myrtc.h>

CT myrtc1; //For RTC Common

void RTCMSG::rtc_str(uint8_t num, bool rus_lng, rtc_time_data_t rt, rtc_alm_data_t rta, char out[])
{
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

  bool alarmed = rta.num < 7, tmw = false;

  rtc_hms_t alt = myrtc1.unix_to_hms(rta.time),
            alp = myrtc1.unix_to_hms(myrtc1.trunc_to_one_day(rta.time) - myrtc1.trunc_to_one_day(rt.ct));

  if (myrtc1.trunc_to_one_day(rt.ct) > myrtc1.trunc_to_one_day(rta.time))
  {
	  alp = myrtc1.unix_to_hms(86400 - myrtc1.trunc_to_one_day(rt.ct) + myrtc1.trunc_to_one_day(rta.time));
	  tmw = true;
  }
 
  if (!rus_lng)
  {
    switch (num)
    {
      case 1:
        sprintf_P(out, PSTR("Today is %S %d %S %d"), sdne[rt.wday - 1], rt.day, smne[rt.month - 1], rt.year);
        break;
      case 4:
		if (alarmed)
		{
			if (tmw)
			{
				sprintf_P(out, PSTR(" Alarm tommorow on %d:%02d"), alt.h, alt.m);
			}
			else
			{
				if (alt.h > 0) sprintf_P(out, PSTR(" Alarm after %2dh. %2dmin. on %d:%02d"), alp.h, alp.m, alt.h, alt.m);
				else sprintf_P(out, PSTR(" Alarm after %dmin. on %d:%02d"), alp.m, alt.h, alt.m);
			}
		}
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
        sprintf_P(out, PSTR("Сегодня %S %d %S %dг."), sdnr[rt.wday - 1], rt.day, smnr[rt.month - 1], rt.year);
        break;
      case 4:
        if (alarmed)
        {
			if (tmw)
			{
				sprintf_P(out, PSTR(" Будильник зазвонит завтра в %2d:%02d"), alt.h, alt.m);
			}
			else			
			{
				if (alt.h > 0) sprintf_P(out, PSTR(" Будильник зазвонит через %2dч. %2dмин. в %2d:%02d"), alp.h, alp.m, alt.h, alt.m);
				else sprintf_P(out, PSTR(" Будильник зазвонит через %dмин. в %2d:%02d"), alp.m, alt.h, alt.m);
			}
		}
        break;
      default:
        break;
    }
  }
}
