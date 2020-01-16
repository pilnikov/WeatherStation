//------------------------------------------------------  Получаем данные с датчиков
void GetSnr()
{
  snr_data_t ts_data;
  snr_data_t es_data;
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  if (web_cli)
  {
    if (ram_data.type_int_snr == 1 || ram_data.type_ext_snr == 1 || ram_data.type_prs_snr == 1)
    {
      dmsg.callback(ram_data.type_disp, 2, 0, conf_data.rus_disp); // сообщение на индикатор о начале обмена с TS
      String ts_str = ts_rcv(conf_data.ts_ch_id, conf_data.AKey_r);  // Получаем строчку данных от TS
      ts_data = e_srv.get_ts(ts_str); // Парсим строчку от TS
      dmsg.callback(ram_data.type_disp, 2, 1, conf_data.rus_disp); // сообщение на индикатор о результатах обмена с TS
    }
      if (ram_data.type_int_snr ==  2 || ram_data.type_ext_snr ==  2 || ram_data.type_prs_snr ==  2) es_data = e_srv.get_es(es_rcv(conf_data.esrv_addr)); // Получаем данные от внешнего сервера
      if (conf_data.use_pp == 2)  wf_data_cur = getOWM_current(conf_data.pp_city_id, conf_data.owm_key);// Получаем данные от OWM
      if ((ram_data.type_int_snr == 10 || ram_data.type_ext_snr == 10 || ram_data.type_prs_snr == 10) && conf_data.use_pp == 1)  wf_data_cur = wf_data;// Получаем данные от GM
  }

  snr_data = sens.read_snr(ram_data.type_int_snr, ram_data.type_ext_snr, ram_data.type_prs_snr, ram_data.temp_rtc, ts_data, es_data, wf_data_cur); // Опрашиваем датчики

  if (web_cli)
  {
    if (conf_data.use_ts_i || conf_data.use_ts_e || conf_data.use_ts_p)
    {
      dmsg.callback(ram_data.type_disp, 1, 0, conf_data.rus_disp); // сообщение на индикатор о начале обмена с TS
      ts_snd(e_srv.put_ts(conf_data.AKey_w, conf_data.use_ts_i, conf_data.use_ts_e, conf_data.use_ts_p, snr_data)); // Отправляем инфу на TS
      dmsg.callback(ram_data.type_disp, 1, 1, conf_data.rus_disp); // сообщение на индикатор о результатах обмена с TS
    }
  }
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
}

//------------------------------------------------------  Делаем запрос данных с Gismeteo
String gs_rcv (unsigned long city_id)
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  if (debug_level == 10) DBG_OUT_PORT.println("True get data from GisMeteo");
  String out = "No connect to network";
  if (web_cli)
  {
    String addr = "http://informer.gismeteo.ru/xml/";
    addr += String(city_id);
    addr += ".xml";
    out = nsys.http_client (addr);
  }
  if (debug_level == 10) DBG_OUT_PORT.println(out);
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return out;
}

// =======================================================================
// Берем текущую погоду с сайта openweathermap.org
// =======================================================================

const char *owmHost = "api.openweathermap.org";


wf_data_t getOWM_current(unsigned long cityID, char weatherKey[32])
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  wf_data_t prog;

  String out = "No connect to network";
  DBG_OUT_PORT.print("\n Current weather from "); DBG_OUT_PORT.println(owmHost);
  String addr = "http://";
  addr += owmHost;
  addr += "/data/2.5/forecast?id=";
  addr += cityID;
  addr += "&units=metric&appid=";
  addr += weatherKey;
  addr += "&lang=ru&cnt=1";
  //DBG_OUT_PORT.println(addr);
  out = nsys.http_client (addr);
  //DBG_OUT_PORT.println(out);

  String line;
  uint16_t i = 0;
  while ( i < out.length())
  {
    char c = out[i];
    if (c == '[' || c == ']') c = ' ';
    line += c;
    i++;
  }

  DBG_OUT_PORT.println("\n Now " + line);

  DynamicJsonDocument jsonBuf(1024);
  DeserializationError error = deserializeJson(jsonBuf, line);
  if (error)
  {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
    return prog;
  }
  JsonObject root = jsonBuf.as<JsonObject>();

  prog.descript  = root["list"]["weather"]["description"].as<String>();
  prog.descript.toLowerCase();
  prog.temp_min  = root["list"]["main"]["temp"];
  prog.hum_min   = root["list"]["main"]["humidity"];
  prog.press_min = root["list"]["main"]["pressure"];
  prog.press_min = prog.press_min / 1.3332239;
  prog.wind_min  = root["list"]["wind"]["speed"];
  uint16_t dir   = root["list"]["wind"]["deg"];
  prog.cloud     = root["list"]["clouds"]["all"];
  prog.wind_dir  = rumb_conv(dir);

#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return prog;
}

// =======================================================================
// Берем ПРОГНОЗ!!! погоды с сайта openweathermap.org
// =======================================================================

wf_data_t getOWM_forecast(unsigned long cityID, char weatherKey[32])
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  wf_data_t prog;

  String out = "No connect to network";
  DBG_OUT_PORT.print("\n Weather forecast for tomorrow from "); DBG_OUT_PORT.println(owmHost);
  String addr = "http://";
  addr += owmHost;
  addr += "/data/2.5/forecast/daily?id=";
  addr += cityID;
  addr += "&units=metric&appid=";
  addr += weatherKey;
  addr += "&lang=ru&cnt=2";
  if (debug_level == 10) DBG_OUT_PORT.println(addr);
  out = nsys.http_client (addr);
  if (debug_level == 10) DBG_OUT_PORT.println(out);

  String line;
  uint16_t i = 0;
  while ( i < out.length())
  {
    char c = out[i];
    if (c == '[' || c == ']') c = ' ';
    line += c;
    i++;
  }

  String tempz = tvoday(line);
  DBG_OUT_PORT.println("\n" + tempz);

  DynamicJsonDocument jsonBuf(512);
  DeserializationError error = deserializeJson(jsonBuf, tempz);
  if (error)
  {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
    return prog;
  }
  JsonObject root = jsonBuf.as<JsonObject>();

  prog.cloud     = root ["clouds"];
  prog.wind_min  = root ["speed"];
  uint16_t dir   = root ["deg"];
  prog.temp_min  = root ["temp"]["min"];
  prog.temp_max  = root ["temp"]["max"];
  prog.descript  = root ["weather"]["description"].as<String>();
  prog.descript.toLowerCase();
  prog.hum_min   = root["humidity"];
  prog.press_min = root["pressure"];
  prog.press_min = prog.press_min / 1.3332239;
  time_t dt      = root ["dt"];
  prog.day       = day(dt);
  prog.month     = month(dt);
  prog.wind_dir = rumb_conv(dir);
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return prog;
}

// ------------------------------------------------------Конвертиуем градусы в румбы (стороны света)
inline uint8_t rumb_conv(uint16_t dir)
{
  uint8_t w_dir = 0;
  if (dir > 23 && dir < 338) w_dir = ((dir - 23) / 45) + 1;
  return w_dir;
}

// =======================================================================
String tvoday(String line) {
  String s;
  int strt = line.indexOf('}');
  for (int i = 1; i <= 4; i++) {
    strt = line.indexOf('}', strt + 1);
  }
  s = line.substring(2 + strt, line.length());
  return s;
}

//------------------------------------------------------  Делаем запрос данных с внешнего сервера
String es_rcv (char es_addr[17])
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  if (debug_level == 10) DBG_OUT_PORT.println("True get data from ext server");
  String out = "No ext srv";
  if (web_cli)
  {
    String addr = "http://";
    addr += String(es_addr);
    addr += "/jsnr";
    if (debug_level == 10) DBG_OUT_PORT.println(addr);
    out = nsys.http_client (addr);
  }
  if (debug_level == 10) DBG_OUT_PORT.println(out);
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return out;
}

//------------------------------------------------------  Делаем запрос данных с ThingSpeak
String ts_rcv (unsigned long id, char api[17])
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  if (debug_level == 10) DBG_OUT_PORT.println("True get data from TS");
  String out = "No connect to network";
  if (web_cli)
  {
    String addr = "http://api.thingspeak.com/channels/";
    addr += String(id);
    addr += "/feed/last?key=";
    addr += String(api);
    out = nsys.http_client (addr);
  }
  //if (debug_level == 10)
  DBG_OUT_PORT.print("TS<-R response from ts ");
  DBG_OUT_PORT.println(out);
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return out;
}

//------------------------------------------------------  Отправляем данные на ThingSpeak
String ts_snd (String inStr)
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  if (debug_level == 10) DBG_OUT_PORT.println("True put data to TS");
  String out = "No connect to network";
  if (web_cli)
  {
    WiFiClient client;

    const int httpsPort = 80;
    const char* host = "api.thingspeak.com"; // Your domain

    if (!client.connect(host, httpsPort))
    {
      client.stop();
      out = "connection failed";
    }
    else
    {
      client.print(String("GET ") + inStr + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");

      out = client.readStringUntil('\r');
      client.stop();
    }
  }
  //if (debug_level == 10)
  DBG_OUT_PORT.print("TS->W response from ts ");
  DBG_OUT_PORT.println(out);
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return out;
}

//------------------------------------------------------  Управляем радиоприемничком по телнету
String radio_snd (String cmd)
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  if (debug_level == 10) DBG_OUT_PORT.println("True put data to Radio");
  String out = "No connect with Radio";
  if (web_cli)
  {
    WiFiClient client;
    const int port = 23;

    if (!client.connect(conf_data.radio_addr, port))
    {
      client.stop();
      out = "connection failed";
    }
    else
    {
      client.print(cmd + "\r\n");
      out = client.readStringUntil('\r');
      client.stop();
    }
  }
  //if (debug_level == 10)
  DBG_OUT_PORT.print("Response from Radio: ");
  DBG_OUT_PORT.println(out);
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
  return out;
}


//-------------------------------------------------------- Получаем точное время с НТП сервера
void GetNtp()
{
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(false);
#endif
  bool result = false;

  DBG_OUT_PORT.println("True sync time with NTP");
  if (web_cli)
  {
    dmsg.callback(ram_data.type_disp, 0, 0, conf_data.rus_disp);; //сообщение на индикатор

    IPAddress addr(89, 109, 251, 21);
    cur_time = NTP_t.getTime(addr, conf_data.time_zone);
    if (cur_time < 1529569070)
    {
      IPAddress addr(10, 98, 34, 10);
      cur_time = NTP_t.getTime(addr, conf_data.time_zone);
    }
    if (cur_time < 1529569070)
    {
      IPAddress addr(88, 212, 196, 95);
      cur_time = NTP_t.getTime(addr, conf_data.time_zone);
    }
    if (cur_time > 1529569070)
    {
      setTime(cur_time);

      //----------------------------------------set time to chip dsXXXX
      if (ram_data.type_rtc == 1) DS3231.SetDateTime(cur_time);
      if (ram_data.type_rtc == 2) DS1302.set(cur_time);       // Set the time and date on the chip.
      if (ram_data.type_rtc == 3) DS1307.SetDateTime(cur_time);

      result = true;
    }
    if (result) dmsg.callback(ram_data.type_disp, 0, 1, conf_data.rus_disp); //сообщение на индикатор
    else dmsg.callback(ram_data.type_disp, 0, 2, conf_data.rus_disp); //сообщение на индикатор

    set_alarm();  //актуализируем будильники
  }
  if (result)   DBG_OUT_PORT.println("Sucsess !!!");
  else   DBG_OUT_PORT.println("Failed !!!");
#if defined(ESP32)
  if (ram_data.type_disp == 9) m3264_upd(true);
#endif
}

//------------------------------------------------------  Обрабатываем клавиатуру
void keyb_read()
{
  bool serv_act  = (web_cli || web_ap);
  bool but0_pressed = !digitalRead(setting_pin); // false - кнопка нажата

  if (but0_pressed && !but0_press) setting_ms = millis(); // Нажимаем кнопку - запускаем таймер, начинаем отсчет времени удержания

  if (!but0_pressed && but0_press && millis() - setting_ms > 150 && millis() - setting_ms < 400) // держим от 0,15 до 0,4 сек
  {
    disp_mode ++; // меняем содержимое экрана на 7ми сегментных индикаторах
    if (disp_mode > 10) disp_mode = 0;
    irq_end[0] = millis();

    num_st ++;    // перебираем строки на матрицах и LCD
    if (num_st    >  4) num_st    = 1;
    st1 = pr_str(num_st);
    cur_sym_pos[0] = 0;
    end_run_st = false; //Запуск бегущей строки;

    but0_press = but0_pressed;
  }

  if ( but0_pressed && !conf_data.use_es && !serv_act && millis() - setting_ms > 2000 ) digitalWrite(LED_BUILTIN, LOW);  // Включаем светодиод
  if ( but0_pressed && !conf_data.use_es &&  serv_act && millis() - setting_ms > 2000 ) digitalWrite(LED_BUILTIN, HIGH); // Выключаем светодиод
  if ( but0_pressed                                   && millis() - setting_ms > 9000 ) digitalWrite(LED_BUILTIN, blinkColon); // Мигаем светодиодом

  if (!but0_pressed && but0_press && !serv_act && millis() - setting_ms > 2000 && millis() - setting_ms < 9000)
  {
    serv_ms = millis();
    start_serv();  // держим от 2 до 9 сек - Запускаем web морду
    but0_press = but0_pressed;
  }

  if (!but0_pressed && but0_press &&  serv_act && millis() - setting_ms > 2000 && millis() - setting_ms < 9000)
  {
    serv_ms = millis() + 60000L;
    stop_serv();  // держим от 2 до 9 сек - Останавливаем web морду
    but0_press = but0_pressed;
  }

  if ( !but0_pressed && but0_press && millis() - setting_ms > 9000 && millis() - setting_ms < 15000)
  {
    DBG_OUT_PORT.println("Reboot esp...");
#if defined(ESP8266) || defined(ESP32)
    ESP.restart();// держим от 9 до 15 сек - Перезапускаемся
#endif
  }

  if ( but0_pressed                            && millis() - setting_ms > 15000                               )                // держим больше 15 сек - сбрасываем усе на дефолт
  {
    if (debug_level == 10) DBG_OUT_PORT.println("Set default value and reboot...");
    conf_data = defaultConfig();
    saveConfig(conf_f, conf_data);
    WiFi.disconnect();
#if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_OFF);
    delay(100);
    //ESP.reset();
    DBG_OUT_PORT.println("Reboot esp...");
    ESP.restart();
#endif
  }
  but0_press = but0_pressed;
}

//------------------------------------------------------  Отправляем данные по USART
String uart_st ()
{
  StaticJsonDocument<400> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json ["T"]  = now();
  json ["U"]  = cur_br;
  json ["V"]  = snr_data.t1;
  json ["W"]  = snr_data.t2;
  json ["X"]  = snr_data.h1;
  json ["Y"]  = snr_data.h2;
  json ["Z"]  = snr_data.p;
  json ["M"]  = rtc_data.a_hour;
  json ["N"]  = rtc_data.a_min;

  if (conf_data.use_pp > 0)
  {
    json ["A"] = wf_data.tod;
    json ["B"] = wf_data.cloud;
    json ["C"] = wf_data.prec;
    json ["D"] = wf_data.rpower;
    json ["E"] = wf_data.spower;
    json ["F"] = (int)(wf_data.press_max + wf_data.press_min) / 2;
    json ["G"] = wf_data.temp_max;
    json ["H"] = wf_data.temp_min;
    json ["I"] = wf_data.wind_max;
    json ["J"] = wf_data.wind_min;
    json ["K"] = wf_data.wind_dir;
    json ["L"] = (int)(wf_data.hum_max + wf_data.hum_min) / 2;
  }

  json ["Q"] = "End";

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  return st;
}

void send_uart()
{
  DBG_OUT_PORT.println(uart_st());
}

//------------------------------------------------------  Верифицируем ночной режим
void nm_veri(void)
{
  if  (conf_data.night_mode_start < conf_data.night_mode_stop) nm_is_on = (hour() > conf_data.night_mode_start && hour() <= conf_data.night_mode_stop);
  else nm_is_on = !(hour() >= conf_data.night_mode_stop && hour() < conf_data.night_mode_start);
}

//------------------------------------------------------  Внутренняя флэшка SPIFFS
# if defined(ESP8266) || defined(ESP32)

void printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    DBG_OUT_PORT.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    DBG_OUT_PORT.print((char)file.read());
  }
  DBG_OUT_PORT.println();

  // Close the file
  file.close();
}


void fs_setup()
{
  if (!SPIFFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    SPIFFS.format();
  }
  else
  {
#if defined(ESP8266)
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUT_PORT.printf(" FS File: %s, size: %s\n", fileName.c_str(), fsys.formatBytes(fileSize).c_str());
    }
#endif

#if defined(ESP32)

    File root = SPIFFS.open("/");

    String output = "[";
    if (root.isDirectory()) {
      File file = root.openNextFile();
      while (file) {
        if (output != "[") {
          output += ',';
        }
        output += "{\"type\":\"";
        output += (file.isDirectory()) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(file.name()).substring(1);
        output += "\"}";
        file = root.openNextFile();
      }
      DBG_OUT_PORT.println (output);
    }
# endif
  }
}
#endif
