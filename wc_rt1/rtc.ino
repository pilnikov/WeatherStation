
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
      pinMode(conf_data.gpio_sqw, INPUT);

      //--------RTC SETUP ------------
      DS3231.Begin();
      DS3231.Enable32kHzPin(false);
      DS3231.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
      DS3231.LatchAlarmsTriggeredFlags();
      // setup external interupt
      attachInterrupt(RtcSquareWaveInterrupt, InteruptServiceRoutine, FALLING);
      break;
    case 3:
      DS1307.Begin();
      DS1307.SetSquareWavePin(DS1307SquareWaveOut_Low);
      break;
  }
  rtc_check();
}

void rtc_check()
{
  if (debug_level == 13) DBG_OUT_PORT.print(F("compiled: "));
  if (debug_level == 13) DBG_OUT_PORT.print(__DATE__);
  if (debug_level == 13) DBG_OUT_PORT.println(__TIME__);

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (debug_level == 13) printDateTime(compiled);
  if (debug_level == 13) DBG_OUT_PORT.println();

  RtcDateTime _now = compiled;


  switch (ram_data.type_rtc)
  {
    case 1:

      //--------RTC SETUP ------------
      // if you are using ESP-01 then uncomment the line below to reset the pins to
      // the available pins for SDA, SCL
      // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL

      DS3231.Begin();

      if (!DS3231.IsDateTimeValid())
      {
        if (DS3231.LastError() != 0)
        {
          // we have a communications error
          // see https://www.arduino.cc/en/Reference/WireEndTransmission for
          // what the number means
          if (debug_level == 13) DBG_OUT_PORT.print(F("RTC communications error = "));
          if (debug_level == 13) DBG_OUT_PORT.println(DS3231.LastError());
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

          DS3231.SetDateTime(compiled);
        }
      }

      if (!DS3231.GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC was not actively running, starting now"));
        DS3231.SetIsRunning(true);
      }

      _now = DS3231.GetDateTime();
      if (_now < compiled)
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC is older than compile time!  (Updating DateTime)"));
        DS3231.SetDateTime(compiled);
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
      setDateTime(_now);
      break;

    case 2:
      if (debug_level == 13) DBG_OUT_PORT.println(F("Starting RTC check"));
      if (!DS1302.GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC was not actively running, starting now"));
        DS1302.SetIsRunning(true);
      }

      if (!DS1302.IsDateTimeValid())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC Time is not valid!"));
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC lost confidence in the DateTime!"));

        // following line sets the RTC to the date & time this sketch was compiled
        // it will also reset the valid flag internally unless the Rtc device is
        // having an issue

        DS1302.SetDateTime(compiled);
      }

      _now = DS1302.GetDateTime();
      if (_now < compiled)
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC is older than compile time!  (Updating DateTime)"));
        DS1302.SetDateTime(compiled);
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
      setDateTime(_now);
      break;

    case 3:
      if (debug_level == 13) DBG_OUT_PORT.println(F("Starting RTC check"));
      if (!DS1307.GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC was not actively running, starting now"));
        DS1307.SetIsRunning(true);
      }

      if (!DS1307.IsDateTimeValid())
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC Time is not valid!"));
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC lost confidence in the DateTime!"));

        // following line sets the RTC to the date & time this sketch was compiled
        // it will also reset the valid flag internally unless the Rtc device is
        // having an issue

        DS1307.SetDateTime(compiled);
      }

      _now = DS1307.GetDateTime();
      if (_now < compiled)
      {
        if (debug_level == 13) DBG_OUT_PORT.println(F("RTC is older than compile time!  (Updating DateTime)"));
        DS1307.SetDateTime(compiled);
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
      setDateTime(_now);
      break;
  }
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
    bool snday = (weekday() > 6 || weekday() < 2); //Сегодня выходной
    if (conf_data.alarms[j][0] > 0 //будильник актививен
        && ((conf_data.alarms[j][0] == 1 || (conf_data.alarms[j][0] == 2) & !snday) || (conf_data.alarms[j][0] == 3 && snday) || conf_data.alarms[j][0] == 4) //проверка на соответствие типу
        && (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] > (uint16_t)hour() * 60 + minute()                 //минут будильника >  текущих минут (чтоб отсеялись "вчерашние" и "завтрашние")
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
    snday = ((weekday() + 1) > 6); //А не выходной ли завтра?
    if (conf_data.alarms[j][0] > 0 //будильник актививен
        && ((conf_data.alarms[j][0] == 1 || (conf_data.alarms[j][0] == 2) & !snday) || ((conf_data.alarms[j][0] == 3) & snday) || conf_data.alarms[j][0] == 4) //проверка на соответствие типу
        && (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] < amin
       )
    {
      amin = (uint16_t)conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2];
      nmin = j;
    }
  }
  if (!rtc_data.alarm && nmin < 7 && (int)conf_data.alarms[nmin][1] * 60 + conf_data.alarms[nmin][2] < (int)hour() * 60 + minute())
  {
    rtc_data.n_cur_alm = nmin;
    rtc_data.a_hour = conf_data.alarms[rtc_data.n_cur_alm][1];
    rtc_data.a_min = conf_data.alarms[rtc_data.n_cur_alm][2];
    rtc_data.a_muz = conf_data.alarms[rtc_data.n_cur_alm][3];
    rtc_data.alarm = true;
  }
  if (debug_level == 13) DBG_OUT_PORT.printf("alarm is... %02u : %02u  melody # %02u\n", rtc_data.a_hour, rtc_data.a_min, rtc_data.a_muz);

  if (ram_data.type_rtc == 1) ds3231_write();
  //------------------------------------------------------------------------------------ Верифицируем ночной режим
}

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u\n"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  DBG_OUT_PORT.print(datestring);
}

void setDateTime(const RtcDateTime& dt)
{
  setTime(dt.Hour(), dt.Minute(), dt.Second(), dt.Day(), dt.Month(), dt.Year());
}


void ds3231_write()
{
  if (rtc_data.alarm)
  {
    // Alarm 1 set to trigger every day when
    // the hours, minutes, and seconds match
    DS3231AlarmOne alarm1(0, rtc_data.a_hour, rtc_data.a_min, 0, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    DS3231.SetAlarmOne(alarm1);
  }
  // Alarm 2 set to trigger at the top of the hour
  DS3231AlarmTwo alarm2(0, 0, 0, DS3231AlarmTwoControl_MinutesMatch);
  DS3231.SetAlarmTwo(alarm2);

  // throw away any old alarm state before we ran
  DS3231.LatchAlarmsTriggeredFlags();
}

bool Alarmed()
{
  bool wasAlarmed = false, wasAlarmed_int = false, wasAlarmed_oth = false, al1_int = false, al2_int = false, al1_oth = false, al2_oth = false;

  if (ram_data.type_rtc == 1)
  {
    if (interuptFlag_int)  // check our flag that gets sets in the interupt
    {
      interuptFlag_int = false; // reset the flag

      // this gives us which alarms triggered and
      // then allows for others to trigger again
      DS3231AlarmFlag flag = DS3231.LatchAlarmsTriggeredFlags();

      if (flag & DS3231AlarmFlag_Alarm1) al1_int = true; //Сработал будильник №1
      if (flag & DS3231AlarmFlag_Alarm2) al2_int = true; //Сработал будильник №2
      wasAlarmed_int = (al1_int || al2_int);
    }
  }
  else
  {
    if (!interuptFlag_oth)  // check our flag that gets sets in the interupt
    {
      al1_oth = ((hour() == rtc_data.a_hour) & (minute() == rtc_data.a_min)); //Сработал будильник №1
      al2_oth = ((minute() == 0) & (second() == 0));                          //Сработал будильник №2
      wasAlarmed_oth = (al1_oth || al2_oth);
      interuptFlag_oth = wasAlarmed_oth; // set the flag
    }
  }

  interuptFlag_oth = ((hour() == rtc_data.a_hour) || (minute() == rtc_data.a_min) || (minute() == 0) || (second() == 0));

  if (al1_int || al1_oth) //Сработал будильник №1
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("alarm one is run!"));

    dmsg.alarm_msg(rtc_data.n_cur_alm, conf_data.type_disp, conf_data.rus_lng);  // Сообщение на индикатор

    switch (conf_data.alarms[rtc_data.n_cur_alm][4])     // Выполняем экшн
    {
      case 0:
        rtc_data.a_muz = conf_data.alarms[rtc_data.n_cur_alm][3];
        copyFromPGM(&songs[rtc_data.a_muz], songBuff);
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
        if (ram_data.type_vdrv == 12)
        {
          lcd->backlight();
          lcd->display();
        }
        if (ram_data.type_vdrv == 2)
        {
          m7219->shutdown(false);
          m7219->write();
        }
        break;
      case 4:
        disp_on = false;
        cur_br = 0;
        snr_data.f = 0;
        switch (ram_data.type_vdrv)
        {
          case 12:
            lcd->noBacklight();
            lcd->noDisplay();
            break;
          case 11:
            ht1633->clear();
            ht1633->setBrightness(0);
            ht1633->write();
            break;
          case 1:
            tm1637->set_br(0);
            tm1637->clear();
            break;
          case 2:
            m7219->shutdown(true);
            m7219->write();
            break;
        }
        break;
      case 5:
#if defined(__xtensa__)
        radio_snd("cli.start");
#endif
        break;
      case 6:
#if defined(__xtensa__)
        radio_snd("cli.stop");
#endif
        break;
    }

    if (conf_data.alarms[rtc_data.n_cur_alm][0] == 4)
    {
      conf_data.alarms[rtc_data.n_cur_alm][0] = 0; //Сбрасываем одноразовый будильник если это был он

      strcpy(conf_data.test, "ok"); //обновляем инфу в епроме
      saveConfig(conf_f, conf_data);
    }
    if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH); // Выключаем светодиод
  }

  if (al2_int || al2_oth) //Сработал будильник №2
  {
    if (debug_level == 13) DBG_OUT_PORT.println(F("alarm two is run!"));
    if (conf_data.every_hour_beep & !nm_is_on)
    {
      rtc_data.a_muz = 15;
      copyFromPGM(&songs[rtc_data.a_muz], songBuff);
      play_snd = true;
    }
  }

  wasAlarmed = (wasAlarmed_int || wasAlarmed_oth); // Сработал один из будильников
  return wasAlarmed;
}

void man_set_time(const RtcDateTime & dt)
{
  setTime(dt.Hour(), dt.Minute(), dt.Second(), dt.Day(), dt.Month(), dt.Year());

  switch (ram_data.type_rtc)
  {
    case 1:
      DS3231.SetDateTime(dt);
      break;
    case 2:
      DS1302.SetDateTime(dt);    // Set the time and date on the chip.
      break;
    case 3:
      DS1307.SetDateTime(dt);
      break;
  }
  set_alarm();
}

void ISR_ATTR InteruptServiceRoutine()
{
  // since this interupted any other running code,
  // don't do anything that takes long and especially avoid
  // any communications calls within this routine
  interuptCount++;
  interuptFlag_int = true;
}
