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
  DBG_OUT_PORT.println("True read data from COM port......");
#endif

  String inString = Serial_Read();

#ifdef _debug
  DBG_OUT_PORT.println("\n inString.." + inString);
#endif

  parser(inString);

  if (wf_data.day == 0) conf_data.use_pp = 0;

  if (abs(rtc_data.ct - now()) > 2 && rtc_data.ct > 1476797813) setTime(rtc_data.ct);

  if (conf_data.type_snr1 > 0 || conf_data.type_snr2 > 0 || conf_data.type_snrp > 0)
  {
    snr_data = sens.read_snr(ram_data.type_snr1, ram_data.type_snr2, ram_data.type_snr3, ram_data.type_snrp, ram_data.temp_rtc, ts_data, es_data, wf_data_cur); // Заполняем матрицу данных с датчиков
  }

  cur_br = ((ram_data.lb + 1) * 16) - 1;
  DBG_OUT_PORT.print("Cur_br ...");  DBG_OUT_PORT.println(cur_br);
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
    DBG_OUT_PORT.print("inbox char is ...");  DBG_OUT_PORT.println(c);
#endif
    if (c == '{') S_str = String();
    S_str += c; //Формируем строку
    i++;
  }  while (c != '}' && i < 250); //пока не найдем "}"
  digitalWrite(uart_pin, HIGH);
  Serial3.end();

  return S_str;
}


void matrix_time()
{
  for (uint8_t i = 0; i < num; i++) digoldig[i] = d[i]; // перезапись предыдущих значений в буфер

  //----------------------------------------------------------------- заполнение массива
  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (conf_data.use_pm && hour() > 12) h = hour() - 12;
  if (h > 9)  d[0] = h / 10 + '0';
  else d[0] = 0;
  d[1] = h % 10 + '0';
  d[2] = minute() / 10 + '0';
  d[3] = minute() % 10 + '0';
  d[4] = second() / 10 + '\x80';
  d[5] = second() % 10 + '\x80';
  //---------------------------------------------------------------------

  for (uint8_t i = 0; i < num; i++)
  {
    digtrans[i] = (d[i] == digoldig[i]) ? 7 : 0; //при изменении буфера рисует набегающий символ сверху
    /*  DBG_OUT_PORT.print("pos ");
        DBG_OUT_PORT.print(i);
        DBG_OUT_PORT.print(" - ");
        DBG_OUT_PORT.println(digtrans[i]);
    */
  }
}

//-------------------------------------------------------------- Отображение бегущей строки на матричном дисплее
void mx_mov_str(String tape, uint8_t dline, unsigned long dutty)
{
  uint8_t f_wdt = 5 + spacer; // The font width is 5 pixels
  if (cur_sym_pos[dline] <= f_wdt * tape.length() + matrix -> width() - spacer)
  {
    if (millis() - lcd_scroll_time[dline] > dutty)
    {
      lcd_scroll_time[dline] = millis();
      cur_sym_pos[dline]++;
    }

    //    matrix -> fillScreen(0);
    uint8_t letter = cur_sym_pos[dline] / f_wdt; // номер символа выводимого на матрицу
    int8_t x = (matrix -> width() - 1) - cur_sym_pos[dline] % f_wdt;
    uint8_t y = dline * 8; // position of sting verticaly
    while ( x + f_wdt - spacer >= 0 && letter >= 0 )
    {
      if ( letter < tape.length() )
      {
        matrix -> drawChar(x, y, tape[letter], matrix -> ColorHSV(grad, 255, cur_br, true), 0, tsize);
      }
      letter--;
      x -= f_wdt;
    }
  }
  else // End of scrolling
  {
    cur_sym_pos[dline] = 0;
    if (conf_data.rus_lng && conf_data.use_pp) max_st = 3;
    else max_st = 2;
    num_st++; //Перебор строк.
    if (num_st > max_st)
    {
      num_st = 1;
      synchro();
    }
    st1 = pr_str(num_st);

    f_dsp.utf8rus(st1);
  }
}


void parser(String inStr)
{
# ifdef _debug
  DBG_OUT_PORT.print("inStr is ...");  DBG_OUT_PORT.println(inStr);
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
  DBG_OUT_PORT.println("Data from ext server");
  DBG_OUT_PORT.print("Current time is: "); DBG_OUT_PORT.println(rtc_data.ct);
  DBG_OUT_PORT.print("Brightness ...");  DBG_OUT_PORT.println(ram_data.lb);
  DBG_OUT_PORT.print("Temperature inside: ");  DBG_OUT_PORT.print(es_data.t1); DBG_OUT_PORT.print(" degrees Celcius Humidity inside: "); DBG_OUT_PORT.print(es_data.h1); DBG_OUT_PORT.println(" % ");
  DBG_OUT_PORT.print("Temperature outside: "); DBG_OUT_PORT.print(es_data.t2); DBG_OUT_PORT.print(" degrees Celcius Humidity outside: "); DBG_OUT_PORT.print(es_data.h2); DBG_OUT_PORT.println(" % ");
  DBG_OUT_PORT.print("Pressure: "); DBG_OUT_PORT.print(es_data.p); DBG_OUT_PORT.println("mm rt. st.");
  DBG_OUT_PORT.print("Prognoz: "); DBG_OUT_PORT.print(wf_data.day);
  DBG_OUT_PORT.print("Current alarm: "); DBG_OUT_PORT.print(rtc_data.a_hour); DBG_OUT_PORT.print(':'); DBG_OUT_PORT.println(rtc_data.a_min);
  //#endif
}
