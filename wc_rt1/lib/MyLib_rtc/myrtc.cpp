
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


	static RTCJS conf;

	static uint32_t prev_ms;

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
      break;

    case 3:
      ds1307 = new RtcDS1307<TwoWire> (Wire);

      ds1307 -> Begin();
      ds1307 -> SetSquareWavePin(DS1307SquareWaveOut_Low);
      break;
  }

  if (debug_level == 13) DBG_OUT_PORT.print(F("compiled: "));
  if (debug_level == 13) DBG_OUT_PORT.print(__DATE__);
  if (debug_level == 13) DBG_OUT_PORT.println(__TIME__);

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

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
      if (debug_level == 13)
      {
	      char _buf[25];
		  memset(_buf, 0, 25);

		  rtc_time_data_t rt;
		  rt.hour = _now.Hour();
		  rt.min = _now.Minute();
		  rt.sec = _now.Second();
		  rt.wday = _now.DayOfWeek() + 1;
		  rt.month = _now.Month();
		  rt.day = _now.Day();
		  rt.year = _now.Year(); //костыль
		  
		  cur_time_str(rt, false, _buf);
		  DBG_OUT_PORT.println(_buf);
	  }
	  
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

rtc_alm_data_t CT::set_alarm(rtc_hw_data_t hw_data, rtc_cfg_data_t cfg_data, rtc_time_data_t time_data) //Устанавливаем будильник
{
  rtc_alm_data_t  alm_data;

  alm_data.num  = 7;
  alm_data.hour = 25;
  alm_data.min  = 61;
  alm_data.muz  = 0;

  uint16_t 
  minut_sut = 24 * 60, // минут в сутках
  minut_alm = (uint16_t) alm_data.hour * 60 +  alm_data.min, // актуальный будильник (в минутах)  
  minut_cur = (uint16_t)time_data.hour * 60 + time_data.min; // текущее время (в минутах)
  uint8_t  nmin = 7; // Номер актуального будильника (вычисляемый тут)

  bool new_alm_is_set = false; //Параметры для нового будильника найдены и установлены

  if (debug_level == 13)
  {
    for (uint8_t i = 0; i <= 6; i++)
    {
      DBG_OUT_PORT.print(i); DBG_OUT_PORT.print(F(" alarm is...."));
      for (int n = 0; n <= 4; n++)
      {
        DBG_OUT_PORT.print(cfg_data.alarms[i][n]); DBG_OUT_PORT.print(F(","));
      }
      DBG_OUT_PORT.println();
    }
  }

  for (uint8_t j = 0; j <= 6; j++) //Ищем ближайший актуальный будильник
  {
	uint16_t minut_cur_alm = (uint16_t)cfg_data.alarms[j][1] * 60 + cfg_data.alarms[j][2]; //минут в проверяемом будильнике

    //----------------------------------------------------------------------поиск ближайшего сегоднешнего будильника
    bool snday = (time_data.wday > 6 || time_data.wday < 2); //Сегодня выходной
	if (cfg_data.alarms[j][0] > 0 //будильник активирован
        && ((cfg_data.alarms[j][0] == 1 || (cfg_data.alarms[j][0] == 2) & !snday) || (cfg_data.alarms[j][0] == 3 && snday) || cfg_data.alarms[j][0] == 4) //проверка на валидность
        &&  minut_cur_alm >  minut_cur //минут проверяемого будильника >  текущих минут (чтоб отсеялись "вчерашние" и "завтрашние")
        &&  minut_cur_alm <= minut_alm //минут проверяемого будильника <=         минут ближайшего, n-1 будильника (текущий - ближе)
       )
    {
      alm_data.num = j; //запоминание номера ближайшего будильника
      alm_data.hour = cfg_data.alarms[j][1];
      alm_data.min = cfg_data.alarms[j][2];
      alm_data.muz = cfg_data.alarms[j][3];
	  minut_alm = (uint16_t) alm_data.hour * 60 +  alm_data.min; //запоминание минут ближайшего будильника
	  new_alm_is_set = true; //сегодня еще есть актуальные будильники
    }
    //----------------------------------------------------------------------поиск самого раннего завтрашнего будильника
    snday = ((time_data.wday + 1) > 6); //А не выходной ли завтра?
    if (cfg_data.alarms[j][0] > 0 //будильник активирован
        && ((cfg_data.alarms[j][0] == 1 || (cfg_data.alarms[j][0] == 2) & !snday) || ((cfg_data.alarms[j][0] == 3) & snday) || cfg_data.alarms[j][0] == 4) //проверка на валидность
        && minut_cur_alm < minut_sut //минут проверяемого будильника <  минут в сутках (чтоб отсеялись "послезавтрашние")
       )
    {
      minut_sut = minut_cur_alm; //запоминаем кол-во минут в самом раннем будильнике
      nmin = j; //запоминаем номер самого раннего будильника
    }
  }

  if (!new_alm_is_set && nmin < 7 && minut_sut < minut_cur) //Будильники завтра все же есть 
  {
    alm_data.num  = nmin;
    alm_data.hour = cfg_data.alarms[nmin][1];
    alm_data.min  = cfg_data.alarms[nmin][2];
    alm_data.muz  = cfg_data.alarms[nmin][3];
    new_alm_is_set = true;
  }

  //if ((debug_level == 13) & rtc_cfg.alarm) DBG_OUT_PORT.printf("alarm is... %02u : %02u  melody # %02u\n", rtc_cfg.a_hour, rtc_cfg.a_min, rtc_cfg.a_muz);
  if (new_alm_is_set) DBG_OUT_PORT.printf("alarm is... %02u : %02u  melody # %02u\n", alm_data.hour, alm_data.min, alm_data.muz);

  if ((hw_data.a_type == 1) & new_alm_is_set)
  {
    // Alarm 1 set to trigger every day when
    // the hours, minutes, and seconds match
    DS3231AlarmOne alarm1(0, alm_data.hour, alm_data.min, 0, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    //DS3231AlarmOne alarm1(0, 0, 0, 0, DS3231AlarmOneControl_OncePerSecond);
    ds3231 -> SetAlarmOne(alarm1);
    // Alarm 2 set to trigger at the top of the every hour
    // throw away any old alarm state before we ran
    ds3231 -> LatchAlarmsTriggeredFlags(); // reset the flag
  }
  return alm_data;
}

bool CT::Alarmed(bool irq, rtc_hw_data_t hw_data, rtc_cfg_data_t* cfg_data, rtc_time_data_t time_data, rtc_alm_data_t* alm_data)
{
  alm_data->al1_on = false;
  alm_data->al1_on = false;
  
  if (hw_data.a_type == 1)
  {
    if (irq)  // check our flag that gets sets in the interupt
    {
      // this gives us which alarms triggered and
      // then allows for others to trigger again
      DS3231AlarmFlag flag = ds3231 -> LatchAlarmsTriggeredFlags(); // reset the flag

      if (flag & DS3231AlarmFlag_Alarm1) alm_data->al1_on = true; //Сработал будильник №1
      if (flag & DS3231AlarmFlag_Alarm2) alm_data->al2_on = true; //Сработал будильник №2
      DBG_OUT_PORT.println(F("Interrupt done"));
     }
  }
  else
  {
    alm_data->al1_on = ((time_data.hour == alm_data->hour) & (time_data.min == alm_data->min) & (time_data.sec == 0)); //Сработал будильник №1
    alm_data->al2_on = ((time_data.min == 0) & (time_data.sec == 0));                                                  //Сработал будильник №2
  }

  if (alm_data->al1_on) //Сработал будильник №1
  {
    //if (debug_level == 13) DBG_OUT_PORT.println(F("alarm one is run!"));
    DBG_OUT_PORT.println(F("alarm one is run!"));

    if (cfg_data->alarms[alm_data->num][0] == 4)
    {
      cfg_data->alarms[alm_data->num][0] = 0; //Сбрасываем одноразовый будильник если это был он
      const char *conf_f = "/conf_rtc.json";
      //conf.saveConfig(conf_f, *cfg_data);
    }
  }

  if (alm_data->al2_on) //Сработал будильник №2
  {
    //    if (debug_level == 13) DBG_OUT_PORT.println(F("alarm two is run!"));
    DBG_OUT_PORT.println(F("alarm two is run!"));
  }
  return (alm_data->al1_on || alm_data->al2_on);
}

long CT::man_set_time(rtc_hw_data_t hw_data, const RtcDateTime &dt)
{
  long _ct = dt;
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
  return _ct;
}

void CT::GetTime(rtc_hw_data_t hw_data, rtc_time_data_t* time_data)
{
  if (millis() >= prev_ms)
  {
    time_data->ct++;
    prev_ms += 1000;
  }
  
  RtcDateTime dt = RtcDateTime(time_data->ct);

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
  }

  time_data->hour = dt.Hour();
  time_data->min = dt.Minute();
  time_data->sec = dt.Second();
  time_data->wday = dt.DayOfWeek() + 1;
  time_data->month = dt.Month();
  time_data->day = dt.Day();
  time_data->year = dt.Year(); //костыль
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

  if (lng) sprintf_P(in, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdnr[time_data.wday - 1], time_data.day, time_data.month, time_data.year, time_data.hour, time_data.min, time_data.sec);
  else sprintf_P(in, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdne[time_data.wday - 1], time_data.day, time_data.month, time_data.year, time_data.hour, time_data.min, time_data.sec);
}

//-------------------------------------------------------- Получаем точное время с НТП сервера
RtcDateTime CT::GetNtp(rtc_cfg_data_t cfg_data, rtc_time_data_t time_data)
{
  RtcDateTime c_time(2021, time_data.month, time_data.day,
					time_data.hour, time_data.min, time_data.sec),
			out_time(time_data.year, time_data.month, time_data.day,
					time_data.hour, time_data.min, time_data.sec); 
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

  DBG_OUT_PORT.println(F("True sync time with NTP"));

  IPAddress myip;
  myip.fromString(cfg_data.ntp_srv[0]);
  c_time = NTP_t.getTime(myip, cfg_data.time_zone);
  if (c_time.Year() < 2022)
  {
	myip.fromString(cfg_data.ntp_srv[1]);
    c_time = NTP_t.getTime(myip, cfg_data.time_zone);
  }
  if (c_time.Year() < 2022)
  {
    myip.fromString(cfg_data.ntp_srv[2]);
	c_time = NTP_t.getTime(myip, cfg_data.time_zone);
  }
  if (c_time.Year() < 2022)
  {
	  DBG_OUT_PORT.println(F("Failed !!!"));
	  return out_time;
  }
  DBG_OUT_PORT.println(F("Sucsess !!!"));
  out_time = RtcDateTime(c_time.Year() - 30, c_time.Month(), c_time.Day(), c_time.Hour(), c_time.Minute(), c_time.Second()); //Потому что макуна считает с 2000го, а тайм с 1970го
#endif
  return out_time;
}

int CT::get_temperature()
{
  RtcTemperature t1 = ds3231 -> GetTemperature();
  int temp = round(t1.AsFloatDegC());
  return temp;   
}