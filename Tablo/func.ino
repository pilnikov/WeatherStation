void synchro()
{

  //Синхронизация системного времени через сом порт (меседж вида "Т unix time")

  // ---------------------------------------------------------------------------- Начальная инициализация
  rtc_data.ct = 1530687234; //Текущее время
  snr_data.h1  =   0; //Внутренняя влажность
  snr_data.t1  =  99; //Внутренняя температура
  snr_data.h2  =   0; //Внешнняя влажность
  snr_data.t2  =  99; //Внешнняя температура
  snr_data.p   = 700; //Давление
  ram_data.lb  =  15; //Яркость

#ifdef _debug
  DBG_OUT_PORT.println(F("True read data from COM port......"));
#endif

  String inString = Serial_Read();

#ifdef _debug
  DBG_OUT_PORT.println(F("\n inString.." + inString);
#endif

  parser(inString);

  if (wf_data.day == 0) conf_data.use_pp = 0;

  if (abs(rtc_data.ct - now()) > 2 && rtc_data.ct > 1476797813) setTime(rtc_data.ct);


  cur_br = ((ram_data.lb + 1) * 16) - 1;
  DBG_OUT_PORT.print(F("Cur_br ..."));  DBG_OUT_PORT.println(cur_br);
}

String Serial_Read() {
  char c; // переменная для чтения сериал порта
  String S_str = String(); // Формируемая из символов строка
  uint8_t i = 0;
  Serial3.begin(19200);
  digitalWrite(uart_pin, LOW);

  do
  {
    if (Serial3.available() > 0) c = Serial3.read(); // Читаем символ
    delay(1);
#ifdef _debug
    DBG_OUT_PORT.print(F("inbox char is ..."));  DBG_OUT_PORT.println(c);
#endif
    if (c == '{') S_str = String();
    S_str += c; //Формируем строку
    i++;
  }  while (c != '}' && i < 250); //пока не найдем "}"
  digitalWrite(uart_pin, HIGH);
  Serial3.end();

  return S_str;
}


void parser(String inStr)
{
# ifdef _debug
  DBG_OUT_PORT.print(F("inStr is ..."));  DBG_OUT_PORT.println(inStr);
# endif

  DynamicJsonDocument jsonBuffer(500);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(jsonBuffer, inStr);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  JsonObject json = jsonBuffer.as<JsonObject>();

  rtc_data.ct        = json["T"];
  ram_data.lb        = json["U"];
  es_data.t1         = json["V"];
  es_data.t2         = json["W"];
  es_data.h1         = json["X"];
  es_data.h2         = json["Y"];
  es_data.p          = json["Z"];

  wf_data.tod        = json ["A"];
  wf_data.cloud      = json ["B"];
  wf_data.prec       = json ["C"];
  wf_data.rpower     = json ["D"];
  wf_data.spower     = json ["E"];
  wf_data.press_max  = json ["F"];
  wf_data.temp_max   = json ["G"];
  wf_data.temp_min   = json ["H"];
  wf_data.wind_max   = json ["I"];
  wf_data.wind_min   = json ["J"];
  wf_data.wind_dir   = json ["K"];
  wf_data.hum_max    = json ["L"];
  wf_data.day        = json ["O"];
  wf_data.month      = json ["P"];

  //#ifdef _debug
  DBG_OUT_PORT.println(F("Data from ext server"));
  DBG_OUT_PORT.print(F("Current time is: ")); DBG_OUT_PORT.println(rtc_data.ct);
  DBG_OUT_PORT.print(F("Brightness ..."));  DBG_OUT_PORT.println(ram_data.lb);
  DBG_OUT_PORT.print(F("Temperature inside: "));  DBG_OUT_PORT.print(es_data.t1); DBG_OUT_PORT.print(F(" degrees Celcius Humidity inside: ")); DBG_OUT_PORT.print(es_data.h1); DBG_OUT_PORT.println(F(" % "));
  DBG_OUT_PORT.print(F("Temperature outside: ")); DBG_OUT_PORT.print(es_data.t2); DBG_OUT_PORT.print(F(" degrees Celcius Humidity outside: ")); DBG_OUT_PORT.print(es_data.h2); DBG_OUT_PORT.println(F(" % "));
  DBG_OUT_PORT.print(F("Pressure: ")); DBG_OUT_PORT.print(es_data.p); DBG_OUT_PORT.println(F("mm rt. st."));
  DBG_OUT_PORT.print(F("Prognoz: ")); DBG_OUT_PORT.print(wf_data.day);
  DBG_OUT_PORT.print(F("Current alarm: ")); DBG_OUT_PORT.print(rtc_data.a_hour); DBG_OUT_PORT.print(':'); DBG_OUT_PORT.println(rtc_data.a_min);
  //#endif
}
