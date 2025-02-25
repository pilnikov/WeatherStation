
void rtc_setup ()
{

  Rtc.Begin();


  if (!Rtc.IsDateTimeValid())
  {
    // Common Cuases:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");

    // following line sets the RTC to the date & time this sketch was compiled
    // it will also reset the valid flag internally unless the Rtc device is
    // having an issue

    if (!Rtc.GetIsRunning())
    {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  }
  set_time();
  printDateTime();
}

void set_time()
{
  const RtcDateTime& dt =  Rtc.GetDateTime();
  setTime(dt.Hour(), dt.Minute(), dt.Second(), dt.Day(), dt.Month(), dt.Year());
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime()
{
 const RtcDateTime& dt = Rtc.GetDateTime();
 char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u:%02u:%02u %02u/%02u/%04u"),
             dt.Hour(),
             dt.Minute(),
             dt.Second(),
             dt.Day(),
             dt.Month(),
             dt.Year());
  Serial.println(datestring);
  //setTime(dt.Hour(), dt.Minute(), dt.Second(), dt.Day(), dt.Month(), dt.Year());
  Serial.print(weekday()); Serial.print(day()); Serial.print(month()); Serial.println(year());
}
