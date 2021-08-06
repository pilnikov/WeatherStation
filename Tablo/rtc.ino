
// CONNECTIONS:
// DS3231 SDA --> SDA GPIO 4 D2
// DS3231 SCL --> SCL GPIO 5 D1
// DS3231 VCC --> 3.3v or 5v
// DS3231 GND --> GND
// SQW --->  GPIO 13 D7 (( Don't forget to pullup (4.7k to 10k to VCC)

void rtc_init()
{
  switch (ram_data.type_rtc)
  {
    case 1:
      // set the interupt pin to input mode
      pinMode(conf_data.gpio_sqw, INPUT_PULLUP);

      //--------RTC SETUP ------------
      ds3231 = new RtcDS3231<TwoWire> (Wire);

      ds3231 -> Begin();
      ds3231 -> Enable32kHzPin(false);
      ds3231 -> SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
      ds3231 -> LatchAlarmsTriggeredFlags();
      // setup external interupt
#if defined(ARDUINO_ARCH_ESP32)
      attachInterrupt(conf_data.gpio_sqw, isr, FALLING);
#else
      attachInterrupt(RtcSquareWaveInterrupt, InteruptServiceRoutine, FALLING);
#endif
      break;
    case 2:
      //ThreeWire myTWire (conf_data.gpio_dio, conf_data.gpio_clk, conf_data.gpio_dcs); // IO, SCLK, CE
      myTWire = new ThreeWire(conf_data.gpio_dio, conf_data.gpio_clk, conf_data.gpio_dcs); // IO, SCLK, CE
      //  ds1302 = new RtcDS1302<ThreeWire> (myTWire);
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

  switch (ram_data.type_rtc)
  {
    case 1:
      ds3231 -> Begin();

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
    switch (ram_data.type_rtc)
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
  man_set_time(_now);
}

void set_alarm() //Устанавливаем будильник
{
  rtc_data.n_cur_alm = 7;
  rtc_data.a_hour = 62;
  rtc_data.a_min = 62;
  rtc_data.a_muz = 0;
  rtc_data.alarm = false;
  uint16_t amin = 24 * 60;
  uint8_t  nmin = 7;

  if (debug_level == 13)
  {
    for (uint8_t i = 0; i <= 6; i++)
    {
      DBG_OUT_PORT.print(i); DBG_OUT_PORT.print(F(" alarm is...."));
      for (int n = 0; n <= 4; n++)
      {
        DBG_OUT_PORT.print(conf_data.alarms[i][n]); DBG_OUT_PORT.print(F(","));
      }
      DBG_OUT_PORT.println();
    }
  }

  for (uint8_t j = 0; j <= 6; j++) //Ищем ближайший актуальный будильник
  {
    bool snday = (rtc_data.wday > 6 || rtc_data.wday < 2); //Сегодня выходной
    if (conf_data.alarms[j][0] > 0 //будильник актививен
        && ((conf_data.alarms[j][0] == 1 || (conf_data.alarms[j][0] == 2) & !snday) || (conf_data.alarms[j][0] == 3 && snday) || conf_data.alarms[j][0] == 4) //проверка на соответствие типу
        && (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] > (uint16_t)rtc_data.hour * 60 + rtc_data.min                 //минут будильника >  текущих минут (чтоб отсеялись "вчерашние" и "завтрашние")
        && (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] <= (uint16_t)rtc_data.a_hour * 60 + rtc_data.a_min //минут будильника <= текущих минут актуального будильника (выбор ближайшего)
       )
    {
      rtc_data.n_cur_alm = j;
      rtc_data.a_hour = conf_data.alarms[rtc_data.n_cur_alm][1];
      rtc_data.a_min = conf_data.alarms[rtc_data.n_cur_alm][2];
      rtc_data.a_muz = conf_data.alarms[rtc_data.n_cur_alm][3];
      rtc_data.alarm = true;
    }
    //----------------------------------------------------------------------поиск самого раннего будильника
    snday = ((rtc_data.wday + 1) > 6); //А не выходной ли завтра?
    if (conf_data.alarms[j][0] > 0 //будильник актививен
        && ((conf_data.alarms[j][0] == 1 || (conf_data.alarms[j][0] == 2) & !snday) || ((conf_data.alarms[j][0] == 3) & snday) || conf_data.alarms[j][0] == 4) //проверка на соответствие типу
        && (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] < amin
       )
    {
      amin = (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2];
      nmin = j;
    }
  }
  if (!rtc_data.alarm && nmin < 7 && (int)conf_data.alarms[nmin][1] * 60 + conf_data.alarms[nmin][2] < (int)rtc_data.hour * 60 + rtc_data.min)
  {
    rtc_data.n_cur_alm = nmin;
    rtc_data.a_hour = conf_data.alarms[rtc_data.n_cur_alm][1];
    rtc_data.a_min = conf_data.alarms[rtc_data.n_cur_alm][2];
    rtc_data.a_muz = conf_data.alarms[rtc_data.n_cur_alm][3];
    rtc_data.alarm = true;
  }
  if (debug_level == 13) DBG_OUT_PORT.printf("alarm is... %02u : %02u  melody # %02u\n", rtc_data.a_hour, rtc_data.a_min, rtc_data.a_muz);

  if (ram_data.type_rtc == 1)
  {
    if (rtc_data.alarm)
    {
      // Alarm 1 set to trigger every day when
      // the hours, minutes, and seconds match
      DS3231AlarmOne alarm1(0, rtc_data.a_hour, rtc_data.a_min, 0, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
      ds3231 -> SetAlarmOne(alarm1);
    }
    // Alarm 2 set to trigger at the top of the hour
    DS3231AlarmTwo alarm2(0, 0, 0, DS3231AlarmTwoControl_MinutesMatch);
    ds3231 -> SetAlarmTwo(alarm2);

    // throw away any old alarm state before we ran
    ds3231 -> LatchAlarmsTriggeredFlags();
  }
}


bool Alarmed()
{
  bool wasAlarmed = false, al1_int = false, al2_int = false, al1_oth = false, al2_oth = false;

  if (ram_data.type_rtc == 1)
  {
    if (wasAlarmed_int)  // check our flag that gets sets in the interupt
    {
      wasAlarmed_int = false; // reset the flag

      // this gives us which alarms triggered and
      // then allows for others to trigger again
      DS3231AlarmFlag flag = ds3231 -> LatchAlarmsTriggeredFlags();

      if (flag & DS3231AlarmFlag_Alarm1) al1_int = true; //Сработал будильник №1
      if (flag & DS3231AlarmFlag_Alarm2) al2_int = true; //Сработал будильник №2
    }
  }
  else
  {
    al1_oth = ((rtc_data.hour == rtc_data.a_hour) & (rtc_data.min == rtc_data.a_min) & (rtc_data.sec == 0)); //Сработал будильник №1
    al2_oth = ((rtc_data.min == 0) & (rtc_data.sec == 0));                                                   //Сработал будильник №2
  }

  if (al1_int || al1_oth) //Сработал будильник №1
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("alarm one is run!"));

    switch (conf_data.alarms[rtc_data.n_cur_alm][4])     // Выполняем экшн
    {
      case 0:
        rtc_data.a_muz = conf_data.alarms[rtc_data.n_cur_alm][3];
        play_snd = true;
        break;

      case 1:
        nm_is_on = true;                       // Включаем ночной режим
        break;
      case 2:
        nm_is_on = false;                      // Выключаем ночной режим
        break;
      case 3:
        disp_on = true;
        break;
      case 4:
        disp_on = false;
        cur_br = 0;
        snr_data.f = 0;
        f_dsp.CLS(screen, sizeof screen);
        m3216_ramFormer(screen, cur_br, text_size);
        break;
      case 5:
        break;
      case 6:
        break;
    }

    if (conf_data.alarms[rtc_data.n_cur_alm][0] == 4)
    {
      conf_data.alarms[rtc_data.n_cur_alm][0] = 0; //Сбрасываем одноразовый будильник если это был он

      saveConfig(conf_data);
    }
  }

  if (al2_int || al2_oth) //Сработал будильник №2
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("alarm two is run!"));
    if (conf_data.every_hour_beep & !nm_is_on)
    {
      rtc_data.a_muz = 15;
      play_snd = true;
    }
  }

  wasAlarmed = (al1_int || al1_oth || al2_int || al2_oth); // Сработал один из будильников
  return wasAlarmed;
}

void man_set_time(const RtcDateTime &dt)
{
  switch (ram_data.type_rtc)
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
      rtc_data.ct = dt;
      break;
  }
  GetTime();
  set_alarm();
}

void GetTime()
{
  if (millis() - prev_ms >= 1000)
  {
    rtc_data.ct++;
    prev_ms += 1000;
  }
  RtcDateTime dt = RtcDateTime(rtc_data.ct);
  switch (ram_data.type_rtc)
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

  rtc_data.hour = dt.Hour();
  rtc_data.min = dt.Minute();
  rtc_data.sec = dt.Second();
  rtc_data.wday = dt.DayOfWeek() + 1;
  rtc_data.month = dt.Month();
  rtc_data.day = dt.Day();
  rtc_data.year = dt.Year(); //костыль
}

#if defined(ESP8266)
void IRAM_ATTR InteruptServiceRoutine()
{
  wasAlarmed_int = true;
}
#elif defined(__AVR__)
void ISR_ATTR InteruptServiceRoutine()
{
  wasAlarmed_int = true;
}
#elif defined(ARDUINO_ARCH_ESP32)
void ARDUINO_ISR_ATTR isr()
{
  wasAlarmed_int = true;
}
#endif
