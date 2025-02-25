
#include "myrtc.h"

// ----------------------------------- Конструктор DS3231
	static RtcDS3231<TwoWire> * ds3231;

	// ----------------------------------- Конструктор DS1307
	static RtcDS1307<TwoWire> * ds1307;

	// ----------------------------------- Конструктор DS1302
	static RtcDS1302<ThreeWire> * ds1302;
	static ThreeWire * myTWire;

	#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
	static NTPTime NTP_t;
	#endif

	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
	uint32_t cur_time;


// CONNECTIONS:
// DS3231 SDA --> SDA GPIO 4 D2
// DS3231 SCL --> SCL GPIO 5 D1
// DS3231 VCC --> 3.3v or 5v
// DS3231 GND --> GND
// SQW --->  GPIO 12 D6 (( Don't forget to pullup (4.7k to 10k to VCC)

void CT::rtc_init(rtc_hw_data_t hw_data)
{
  switch (hw_data.a_type)
  {
    case 1:
      //--------RTC SETUP ------------
      ds3231 = new RtcDS3231<TwoWire> (Wire);

      ds3231 -> Begin();

      ds3231 -> Enable32kHzPin(false);
      ds3231 -> SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
      ds3231 -> LatchAlarmsTriggeredFlags();
      break;
    case 2:
      myTWire = new ThreeWire(hw_data.gpio_dio, hw_data.gpio_clk, hw_data.gpio_dcs); // IO, SCLK, CE
      ds1302 = new RtcDS1302<ThreeWire> (*myTWire);
      ds1302 -> Begin();
	  
      break;

    case 3:
      ds1307 = new RtcDS1307<TwoWire> (Wire);

      ds1307 -> Begin();
      ds1307 -> SetSquareWavePin(DS1307SquareWaveOut_Low);
      break;
  }

  if (debug_level == 13)
  {
	  DBG_OUT_PORT.print(F("compiled: "));
	  DBG_OUT_PORT.print(__DATE__);
	  DBG_OUT_PORT.println(__TIME__);
  }

  RtcDateTime _now = compiled;

  if (debug_level == 13) DBG_OUT_PORT.println(F("Starting RTC check"));

  if	(hw_data.a_type == 1)  
  {
	  DS3231AlarmTwo  alarm2(0, 0, 0, DS3231AlarmTwoControl_MinutesMatch);
	  // set the alarm 2
	  ds3231 -> SetAlarmTwo(alarm2);
	  // throw away any old alarm state before we ran
	  ds3231 -> LatchAlarmsTriggeredFlags(); // reset the flag
  }
 
  switch (hw_data.a_type)
  {
    case 1:
      if (!ds3231 -> IsDateTimeValid())
      {
        if (ds3231 -> LastError() != 0)
        {
          // we have a communications error
          // see https://www.arduino.cc/en/Reference/WireEndTransmission for
          // what the number means
          if (debug_level == 13)
          {
            DBG_OUT_PORT.print(F("RTC communications error = "));
            DBG_OUT_PORT.println(ds3231 -> LastError());
          }
        }
        else
        {
          // Common Causes:
          //    1) first time you ran and the device wasn't running yet
          //    2) the battery on the device is low or even missing

          if (debug_level == 13) DBG_OUT_PORT.println(F("RTC lost confidence in the DateTime!"));

          // following line sets the RTC to the date & time this sketch was compiled
          // it will also reset the valid flag internally unless the Rtc device is
          // having an issue
          ds3231 -> SetDateTime(compiled);
        }
      }

      if (!ds3231 -> GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC was not actively running, starting now"));
        ds3231 -> SetIsRunning(true);
      }

      _now = ds3231 -> GetDateTime();
  	  break;

    case 2:
      if (!ds1302 -> GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC was not actively running, starting now"));
        ds1302 -> SetIsRunning(true);
      }

      if (!ds1302 -> IsDateTimeValid())
      {
        if (debug_level == 13)
        {
          DBG_OUT_PORT.println(F("RTC Time is not valid!"));
          DBG_OUT_PORT.println(F("RTC lost confidence in the DateTime!"));
        }
        ds1302 -> SetDateTime(compiled);
      }
      _now = ds1302 -> GetDateTime();
      break;

    case 3:
      if (debug_level == 13) DBG_OUT_PORT.println(F("Starting RTC check"));
      if (!ds1307 -> GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC was not actively running, starting now"));
        ds1307 -> SetIsRunning(true);
      }

      if (!ds1307 -> IsDateTimeValid())
      {
        if (debug_level == 13)
        {
          DBG_OUT_PORT.println(F("RTC Time is not valid!"));
          DBG_OUT_PORT.println(F("RTC lost confidence in the DateTime!"));
        }
        ds1302 -> SetDateTime(compiled);
      }
      _now = ds1307 -> GetDateTime();
      break;
  }

  if (_now < compiled)
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("RTC is older than compile time!  (Updating DateTime)"));
    switch (hw_data.a_type)
    {
      case 1:
        ds3231 -> SetDateTime(compiled);
        break;
      case 2:
        ds1302 -> SetDateTime(compiled);
        break;
      case 3:
        ds1307 -> SetDateTime(compiled);
        break;
    }
  }
  else if (_now > compiled)
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("RTC is newer than compile time. (this is expected)"));
  }
  else if (_now == compiled)
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("RTC is the same as compile time! (not expected but all is fine)"));
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
}

rtc_alm_data_t CT::set_alarm(rtc_cfg_data_t cfg_data, uint32_t c_time, bool ds3) //Устанавливаем актуальный будильник
{
  uint32_t 
  one_day_in_sec 	  = 86400,// секунд в сутках
  alm_in_sec 	 	  = 86401;// актуальный будильник (в секундах)  
  
  uint8_t  nmin = 7; 		  // Номер актуального будильника (вычисляемый тут)

  rtc_alm_data_t  alm_data;

  alm_data.num  = nmin;
  alm_data.time = alm_in_sec;
  alm_data.act  = 30;

  bool new_alm_is_set = false; //Параметры для нового будильника найдены и установлены

  if (debug_level == 13)
  {
    for (uint8_t i = 0; i <= 6; i++)
    {
      DBG_OUT_PORT.print(cfg_data.alarms[i].type); 
	  DBG_OUT_PORT.print(F(","));
      DBG_OUT_PORT.print(cfg_data.alarms[i].time); 
	  DBG_OUT_PORT.print(F(","));
      DBG_OUT_PORT.print(cfg_data.alarms[i].act); 
	  DBG_OUT_PORT.print(F(","));
      DBG_OUT_PORT.println();
	}
  }

  for (uint8_t j = 0; j <= 6; j++) //Ищем ближайший актуальный будильник
  {
	uint32_t alm_cur_in_sec = cfg_data.alarms[j].time % 86400; //секунд в проверяемом будильнике

    //----------------------------------------------------------------------поиск ближайшего сегодняшнего будильника
    
	uint8_t wday = (c_time / 86400 + 4) % 7;  // Вычисляем день недели (0 - ВС 1 - ПН .. 6 - СБ)
	bool snday = (wday > 5 || wday < 1); // Сегодня выходной
	if (cfg_data.alarms[j].type > 0 			//будильник активирован
        && ((cfg_data.alarms[j].type == 1 || (cfg_data.alarms[j].type == 2) & !snday) || (cfg_data.alarms[j].type == 3 && snday) || cfg_data.alarms[j].type == 4) //проверка на валидность
        &&  alm_cur_in_sec >  c_time % 86400	//секунд проверяемого будильника >  текущих секунд (чтобы отсеялись "вчерашние" и "завтрашние")
        &&  alm_cur_in_sec <= alm_in_sec 		//секунд проверяемого будильника <=         секунд ближайшего, n-1 будильника (текущий - ближе)
       )
    {
      alm_data.num 	= j; 						//запоминание номера ближайшего будильника
      alm_data.time = cfg_data.alarms[j].time;
      alm_data.act  = cfg_data.alarms[j].act;
	  alm_in_sec = alm_data.time % 86400; 		//запоминание секунд ближайшего будильника
	  new_alm_is_set = true; 					//сегодня еще есть актуальные будильники
    }
    //----------------------------------------------------------------------поиск самого раннего завтрашнего будильника
    snday = ((wday + 1) > 5); //А не выходной ли завтра?
    if (cfg_data.alarms[j].type > 0 //будильник активирован
        && ((cfg_data.alarms[j].type == 1 || (cfg_data.alarms[j].type == 2) & !snday) || ((cfg_data.alarms[j].type == 3) & snday) || cfg_data.alarms[j].type == 4) //проверка на валидность
        && alm_cur_in_sec < one_day_in_sec //секунд проверяемого будильника <  секунд в сутках (чтоб отсеялись "послезавтрашние")
       )
    {
      one_day_in_sec = alm_cur_in_sec; //запоминаем кол-во секунд в самом раннем будильнике
      nmin = j; //запоминаем номер самого раннего будильника
    }
  }

  if (!new_alm_is_set && nmin < 7 && one_day_in_sec < c_time % 86400) //Будильники завтра все же есть 
  {
    alm_data.num  = nmin;
    alm_data.time = cfg_data.alarms[nmin].time;
    alm_data.act  = cfg_data.alarms[nmin].act;
    new_alm_is_set = true;
  }
    rtc_hms_t alt = unix_to_hms(alm_data.time);
  //if ((debug_level == 13) & rtc_cfg.alarm) DBG_OUT_PORT.printf("alarm is... %02u : %02u  action # %02u\n", alt.h, alt.m, alm_data.act);
  if (new_alm_is_set) DBG_OUT_PORT.printf("alarm is... %02u : %02u  action # %02u\n", alt.h, alt.m, alm_data.act);

  if (ds3 & new_alm_is_set)
  {
    // Alarm 1 set to trigger every day when
    // the hours, minutes, and seconds match
    DS3231AlarmOne alarm1(0, alt.h, alt.m, alt.s, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    //DS3231AlarmOne alarm1(0, 0, 0, 0, DS3231AlarmOneControl_OncePerSecond);
    ds3231 -> SetAlarmOne(alarm1);
    // Alarm 2 set to trigger at the top of the every hour
    // throw away any old alarm state before we ran
    ds3231 -> LatchAlarmsTriggeredFlags(); // reset the flag
  }
  return alm_data;
}

bool CT::Alarmed(bool irq, rtc_hw_data_t hw_data, rtc_alm_data_t * alm_data, uint32_t c_time)
{
  alm_data -> al1_on = false, alm_data -> al2_on = false;
  rtc_hms_t sct = unix_to_hms(c_time);
 
  if (hw_data.a_type == 1)
  {
    if (irq)  // check our flag that gets sets in the interupt
    {
      // this gives us which alarms triggered and
      // then allows for others to trigger again
      DS3231AlarmFlag flag = ds3231 -> LatchAlarmsTriggeredFlags(); // reset the flag

      if (flag & DS3231AlarmFlag_Alarm1) alm_data -> al1_on = true; //Сработал будильник №1
      if (flag & DS3231AlarmFlag_Alarm2) alm_data -> al2_on = true; //Сработал будильник №2
      DBG_OUT_PORT.println(F("Interrupt done"));
     }
  }
  else
  {
    alm_data -> al1_on = (trunc_to_one_day(c_time) == trunc_to_one_day(alm_data -> time)); 	//Сработал будильник №1
	alm_data -> al2_on = ((sct.m == 0) & (sct.s == 0));			                       		//Сработал будильник №2
  }

  if (alm_data -> al1_on) //Сработал будильник №1
  {
    DBG_OUT_PORT.println(F("alarm one is run!"));
  }

  if (alm_data -> al2_on) //Сработал будильник №2
  {
    DBG_OUT_PORT.println(F("alarm two is run!"));
  }
  return (alm_data -> al1_on || alm_data -> al2_on);
}

void CT::man_set_time(rtc_hw_data_t hw_data, uint32_t ct)
{
  RtcDateTime dt;
  
  dt.InitWithUnix32Time(ct);

  switch (hw_data.a_type)
  {
    case 1:
      ds3231 -> SetDateTime(dt);
      break;
    case 2:
      ds1302 -> SetDateTime(dt);    // Set the time and date on the chip.
      break;
    case 3:
      ds1307 -> SetDateTime(dt);
      break;
    default:
      break;
  }
  cur_time = dt.Unix32Time();
}

uint32_t CT::GetTime(rtc_hw_data_t hw_data)
{
  uint32_t ct = cur_time;
  
  RtcDateTime dt;
  dt.InitWithUnix32Time(ct);

  switch (hw_data.a_type)
  {
    case 1:
      dt = ds3231 -> GetDateTime();
      break;
    case 2:
      dt = ds1302 -> GetDateTime();
      break;
    case 3:
      dt = ds1307 -> GetDateTime();
      break;
    default:
      break;
  }
  ct = dt.Unix32Time();
  return ct;
}

//-------------------------------------------------------------- cur_time_str
void CT::cur_time_str(rtc_time_data_t time_data, bool lng, char* in)
{
  const char* sdnr_1 = PSTR("ВС");
  const char* sdnr_2 = PSTR("ПН");
  const char* sdnr_3 = PSTR("ВТ");
  const char* sdnr_4 = PSTR("СР");
  const char* sdnr_5 = PSTR("ЧТ");
  const char* sdnr_6 = PSTR("ПТ");
  const char* sdnr_7 = PSTR("СБ");

  const char* const sdnr[] = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

  const char* sdne_1 = PSTR("Sun");
  const char* sdne_2 = PSTR("Mon");
  const char* sdne_3 = PSTR("Tue");
  const char* sdne_4 = PSTR("Wed");
  const char* sdne_5 = PSTR("Thu");
  const char* sdne_6 = PSTR("Fri");
  const char* sdne_7 = PSTR("Sat");

  const char* const sdne[] = {sdne_1, sdne_2, sdne_3, sdne_4, sdne_5, sdne_6, sdne_7};

  if (lng) sprintf_P(in, PSTR("%s %02u.%02u.%04u %02u:%02u:%02u"), sdnr[time_data.wday - 1], time_data.day, time_data.month, time_data.year, time_data.hour, time_data.min, time_data.sec);
  else sprintf_P(in, PSTR("%s %02u.%02u.%04u %02u:%02u:%02u"), sdne[time_data.wday - 1], time_data.day, time_data.month, time_data.year, time_data.hour, time_data.min, time_data.sec);
}

//-------------------------------------------------------- Получаем точное время с НТП сервера
uint32_t CT::GetNtp(rtc_cfg_data_t cfg_data, uint32_t ct)
{

 uint32_t out_time = ct;

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

  DBG_OUT_PORT.println(F("True sync time with NTP"));

  uint32_t c_time = ct;
  IPAddress myip;

  myip.fromString(cfg_data.ntp_srv[0]);
  c_time = NTP_t.getTime(myip, cfg_data.time_zone);
  if (c_time <= compiled.Unix32Time())
  {
	myip.fromString(cfg_data.ntp_srv[1]);
    c_time = NTP_t.getTime(myip, cfg_data.time_zone);
  }
  if (c_time <= compiled.Unix32Time())
  {
    myip.fromString(cfg_data.ntp_srv[2]);
	c_time = NTP_t.getTime(myip, cfg_data.time_zone);
  }
  if (c_time <= compiled.Unix32Time())
  {
	  DBG_OUT_PORT.println(F("Failed !!!"));
	  return out_time;
  }
  DBG_OUT_PORT.println(F("Sucsess !!!"));
  out_time = c_time;
#endif
  
  return out_time;
}

int CT::get_temperature() 
{
	RtcTemperature t1 = ds3231 -> GetTemperature();
	int temp = round(t1.AsFloatDegC());
	return temp;
}

// конвертировать unix stamp дату и время
void CT::dt_from_unix(rtc_time_data_t* out) 
{

		uint32_t a, b, c, d, e, f;
//        int h, j, k;

		uint32_t in = out -> ct;
        
		out -> sec = in % 60ul;
        in /= 60;
        out -> min = in % 60ul;
        in /= 60;
        out -> hour = in % 24ul;
        in /= 24;

        a = (uint32_t)((4ul * in + 102032) / 146097 + 15);
        b = (uint32_t)(in + 2442113 + a - (a / 4));
        c = (20 * b - 2442) / 7305;
        d = b - 365 * c - (c / 4);
        e = d * 1000 / 30601;
        f = d - e * 30 - e * 601 / 1000;

        // январь и февраль как 13 и 14 месяцы
        if (e <= 13) {
            c -= 4716;
            e -= 1;
        } else {
            c -= 4715;
            e -= 13;
        }

        out -> year = c;
        out -> month = e;
        out -> day = f;
/*
        if (e <= 2) {
            e += 12;
            c -= 1;
        }
        j = c / 100;
        k = c % 100;
        h = f + (26 * (e + 1) / 10) + k + (k / 4) + (5 * j) + (j / 4);  // Уравнение Зеллера
*/
        out -> wday = (out -> ct / 86400 + 4) % 7 + 1;
}
