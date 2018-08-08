
// CONNECTIONS:
// DS3231 SDA --> SDA GPIO 4 D2
// DS3231 SCL --> SCL GPIO 5 D1
// DS3231 VCC --> 3.3v or 5v
// DS3231 GND --> GND
// SQW --->  GPIO 13 D7 (( Don't forget to pullup (4.7k to 10k to VCC)

void rtc_init()
{
  if (ram_data.type_rtc == 1)
  { // set the interupt pin to input mode
    pinMode(RtcSquareWavePin, INPUT);

    //--------RTC SETUP ------------
    DS3231.Begin();
    DS3231.Enable32kHzPin(false);
    DS3231.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
    // setup external interupt
    attachInterrupt(RtcSquareWaveInterrupt, InteruptServiceRoutine, FALLING);
  }
  if (ram_data.type_rtc == 3)
  { DS1307.Begin();
    DS1307.SetSquareWavePin(DS1307SquareWaveOut_Low);
  }
  rtc_check();
}

void rtc_check()
{
  if (ram_data.type_rtc == 1)
  {
    if (debug_level == 13) DBG_OUT_PORT.println("Starting RTC check");
    if (!DS3231.GetIsRunning())
    {
      if (debug_level == 13) DBG_OUT_PORT.println("RTC was not actively running, starting now");
      DS3231.SetIsRunning(true);
    }

    if (!DS3231.IsDateTimeValid())
    {
      if (debug_level == 13) DBG_OUT_PORT.println( "RTC Time is not valid start time auto correction");
      GetNtp();
    }
    else
    {
      cur_time = DS3231.GetDateTime();
      if (cur_time > 1529569070) setTime(cur_time);
    }
    RtcTemperature temp = DS3231.GetTemperature();
    ram_data.temp_rtc = round(temp.AsFloatDegC());
  }
  if (ram_data.type_rtc == 2)
  {
    while ( cur_time < 1529569070 )cur_time = DS1302.get();
    delay (1000);
    setTime(cur_time);

    if (DS1302.read(ds_tm_stru))
    {
      if (debug_level == 13)
      {
        DBG_OUT_PORT.println("DS1302 read error!  Please check the circuitry.");
        DBG_OUT_PORT.println("Start time auto correction");
      }
      GetNtp();
    }
  }
  if (ram_data.type_rtc == 3)
  {
    {
      if (debug_level == 13) DBG_OUT_PORT.println("Starting RTC check");
      if (!DS1307.GetIsRunning())
      {
        if (debug_level == 13) DBG_OUT_PORT.println("RTC was not actively running, starting now");
        DS1307.SetIsRunning(true);
      }

      if (!DS1307.IsDateTimeValid())
      {
        if (debug_level == 13) DBG_OUT_PORT.println( "RTC Time is not valid start time auto correction");
        GetNtp();
      }
      cur_time = DS1307.GetDateTime();
      setTime(cur_time);
    }
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

  for (uint8_t j = 0; j <= 6; j++) //Ищем ближайший актуальный будильник
  {
    bool snday = (weekday() > 6 || weekday() < 2); //Сегодня выходной
    if (conf_data.alarms[j][0] > 0 //будильник актививен
        && ((conf_data.alarms[j][0] == 1 || (conf_data.alarms[j][0] == 2) && !snday) || (conf_data.alarms[j][0] == 3 && snday) || conf_data.alarms[j][0] == 4) //проверка на соответствие типу
        && (uint16_t) conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] >  (uint16_t) hour() * 60 + minute()                //минут будильника >  текущих минут (чтоб отсеялись "вчерашние" и "завтрашние")
        && (uint16_t) conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] <= (uint16_t) rtc_data.a_hour * 60 + rtc_data.a_min //минут будильника <= текущих минут актуального будильника (выбор ближайшего)
       )
    {
      rtc_data.n_cur_alm = j;
      rtc_data.a_hour = conf_data.alarms[rtc_data.n_cur_alm][1];
      rtc_data.a_min  = conf_data.alarms[rtc_data.n_cur_alm][2];
      rtc_data.a_muz  = conf_data.alarms[rtc_data.n_cur_alm][3];
      rtc_data.alarm  = true;
    }
    //----------------------------------------------------------------------поиск самого раннего будильника
    snday = ((weekday() + 1) > 6); //А не выходной ли завтра?
    if (conf_data.alarms[j][0] > 0 //будильник актививен
        && ((conf_data.alarms[j][0] == 1 || (conf_data.alarms[j][0] == 2) && !snday) || (conf_data.alarms[j][0] == 3 && snday) || conf_data.alarms[j][0] == 4) //проверка на соответствие типу
        && (uint16_t) conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2] < amin
       )
    {
      amin = (uint16_t) conf_data.alarms[j][1] * 60 + conf_data.alarms[j][2];
      nmin = j;
    }
  }
  if (!rtc_data.alarm && nmin < 7 && (int) conf_data.alarms[nmin][1] * 60 + conf_data.alarms[nmin][2] < (int) hour() * 60 + minute())
  {
    rtc_data.n_cur_alm = nmin;
    rtc_data.a_hour = conf_data.alarms[rtc_data.n_cur_alm][1];
    rtc_data.a_min  = conf_data.alarms[rtc_data.n_cur_alm][2];
    rtc_data.a_muz  = conf_data.alarms[rtc_data.n_cur_alm][3];
    rtc_data.alarm  = true;
  }
  if (debug_level == 13) DBG_OUT_PORT.printf("alarm is... %02u : %02u  melody # %02u\n", rtc_data.a_hour, rtc_data.a_min, rtc_data.a_muz);

  if (ram_data.type_rtc == 1) ds3231_write();
  //------------------------------------------------------------------------------------ Верифицируем ночной режим
  nm_veri();
}

void ds3231_write()
{
  if (rtc_data.alarm)
  {
    // Alarm 1 set to trigger every day when
    // the hours, minutes, and seconds match
    DS3231AlarmOne alarm1(0, rtc_data.a_hour, rtc_data.a_min, 0,  DS3231AlarmOneControl_HoursMinutesSecondsMatch);
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
    if (interuptFlag)  // check our flag that gets sets in the interupt
    {
      wasAlarmed_int = true;
      interuptFlag = false; // reset the flag

      // this gives us which alarms triggered and
      // then allows for others to trigger again
      DS3231AlarmFlag flag = DS3231.LatchAlarmsTriggeredFlags();

      if (flag & DS3231AlarmFlag_Alarm1 & rtc_data.alarm) al1_int = true; //Сработал будильник №1
      if (flag & DS3231AlarmFlag_Alarm2 && (minute() == 0 || minute() == 59)) al2_int = true; //Сработал будильник №2
    }
  }
  else
  {
    al1_oth = (hour() == rtc_data.a_hour && minute() == rtc_data.a_min && !rtc_data.wasAlarm && rtc_data.alarm); //Сработал будильник №1
    al2_oth = (minute() == 0 && second() == 0 && !rtc_data.wasAlarm);                                            //Сработал будильник №2
    wasAlarmed_oth = (al1_oth || al2_oth);
    rtc_data.wasAlarm  = ((hour() == rtc_data.a_hour && minute() == rtc_data.a_min) || (minute() == 0 && second() == 0));
  }

  if (al1_int || al1_oth) //Сработал будильник №1
  {
    if (debug_level == 13) DBG_OUT_PORT.println("alarm one is run!");

    dmsg.alarm_msg(rtc_data.n_cur_alm, ram_data.type_disp, conf_data.rus_disp);  // Сообщение на индикатор
    digitalWrite(LED_BUILTIN, blinkColon); // Мигаем светодиодом

    switch (conf_data.alarms[rtc_data.n_cur_alm][4])     // Выполняем экшн
    {
      case 0:
        Buzz.play(songs[rtc_data.a_muz], BUZ_PIN, true);   // Запускаем динамик на проигрывание выбранной мелодии
        break;
      case 1:
        nm_is_on = true;                       // Включаем ночной режим
        break;
      case 2:
        nm_is_on = false;                      // Выключаем ночной режим
        break;
      case 3:
        disp_on = true;
        if (ram_data.type_disp == 1)
        {
          lcd -> backlight();
          lcd -> display();
        }
        if (ram_data.type_disp == 4)
        {
          m7219 -> shutdown(false);
          m7219 -> write();
        }
        break;
      case 4:
        disp_on = false;
        cur_br = 0;
        snr_data.ft = 0;
        switch (ram_data.type_disp)
        {
          case 1:
            lcd -> noBacklight();
            lcd -> noDisplay();
            break;
          case 2:
            ht33 -> clear();
            ht33 -> setBrightness(0);
            ht33 -> writeDisplay();
            break;
          case 3:
            tm1637 -> set(0);
            tm1637 -> clearDisplay();
            break;
          case 4:
            m7219 -> shutdown(true);
            m7219 -> write();
            break;
          case 5:
            max7 -> setIntensity(0, 0);
            max7 -> clearDisplay(0);
            break;
          case 6:
            max7 -> setIntensity(0, 0);
            max7 -> clearDisplay(0);
            break;
        }
        break;
      case 5:
        radio_snd("cli.start");
        break;
      case 6:
        radio_snd("cli.stop");
        break;
    }

    if (conf_data.alarms[rtc_data.n_cur_alm][0] == 4)
    {
      conf_data.alarms[rtc_data.n_cur_alm][0] = 0; //Сбрасываем одноразовый будильник если это был он

      strcpy(conf_data.test, "ok"); //обновляем инфу в епроме
      saveConfig(conf_data);
    }
    digitalWrite(LED_BUILTIN, HIGH); // Выключаем светодиод
  }

  if (al2_int || al2_oth) //Сработал будильник №2
  {
    if (debug_level == 13) DBG_OUT_PORT.println("alarm two is run!");
    if (conf_data.every_hour_beep && !nm_is_on) Buzz.play(songs[15], BUZ_PIN, true); //пищим каждый час
  }

  wasAlarmed = (wasAlarmed_int || wasAlarmed_oth); // Сработал один из будильников
  set_alarm();
  return wasAlarmed;
}

void man_set_time()
{
  setTime(makeTime(ds_tm_stru));

  if (ram_data.type_rtc == 1)
  {
    DS3231.SetDateTime(makeTime(ds_tm_stru));
  }
  if (ram_data.type_rtc == 2)
  {
    DS1302.set(makeTime(ds_tm_stru));    // Set the time and date on the chip.
  }
  if (ram_data.type_rtc == 3)
  {
    DS1307.SetDateTime(makeTime(ds_tm_stru));
  }
  set_alarm();
}

void InteruptServiceRoutine()
{
  // since this interupted any other running code,
  // don't do anything that takes long and especially avoid
  // any communications calls within this routine
  interuptCount++;
  interuptFlag = true;
}

