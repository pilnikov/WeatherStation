//------------------------------------------------------  Получаем данные с датчиков
snr_data_t GetSnr(ram_data_t rd, conf_data_t cf)
{
  snr_data_t ts_data;
  snr_data_t es_data1;
  snr_data_t es_data2;
  snr_data_t _snr_data;

  _snr_data.t1 = 99;
  _snr_data.t2 = 99;
  _snr_data.t3 = 99;
  _snr_data.h1 = 0;
  _snr_data.h2 = 0;
  _snr_data.h3 = 0;
  _snr_data.p = 700;

  rd.temp_rtc = 99;

  if ((rd.type_snr1 == 5 || rd.type_snr2 == 5 || rd.type_snr3 == 5) && rd.type_rtc == 1)
  {
    RtcTemperature t1 = DS3231.GetTemperature();
    rd.temp_rtc = round(t1.AsFloatDegC());
  }

# if defined(__xtensa__)
  if (web_cli)
  {
    if (rd.type_snr1 == 1 || rd.type_snr2 == 1 || rd.type_snr3 == 1 || rd.type_snrp == 1)
    {
      dmsg.callback(cf.type_disp, 2, 0, cf.rus_lng); // сообщение на индикатор о начале обмена с TS
      String ts_str = ts_rcv(cf.ts_ch_id, cf.AKey_r);  // Получаем строчку данных от TS
      ts_data = e_srv.get_ts(ts_str); // Парсим строчку от TS
      dmsg.callback(cf.type_disp, 2, 1, cf.rus_lng); // сообщение на индикатор о результатах обмена с TS
    }
    if (rd.type_snr1 == 2 || rd.type_snr2 == 2 || rd.type_snr3 == 2 || rd.type_snrp == 2) es_data1 = e_srv.get_es(es_rcv(cf.esrv1_addr)); // Получаем данные от внешнего сервера1
    if (rd.type_snr1 == 3 || rd.type_snr2 == 3 || rd.type_snr3 == 3 || rd.type_snrp == 3) es_data2 = e_srv.get_es(es_rcv(cf.esrv2_addr)); // Получаем данные от внешнего сервера2

    if (cf.use_pp == 2) {
      wf_data_cur = getOWM_current(cf.pp_city_id, cf.owm_key);// Получаем данные от OWM
    }
  }
#endif
  if (rd.type_snr1 > 0 || rd.type_snr2 > 0 || rd.type_snr3 > 0)
  {
    _snr_data = sens.read_snr(rd.type_snr1, rd.type_snr2, rd.type_snr3, rd.type_snrp, rd.temp_rtc, ts_data, es_data1, es_data2, wf_data_cur); // Заполняем матрицу данных с датчиков
  }

# if defined(__xtensa__)
  if (web_cli)
  {
    if (cf.use_tst1 || cf.use_tst2 || cf.use_tst3 || cf.use_tsh1 || cf.use_tsh2 || cf.use_tsh3 || cf.use_tsp)
    {
      dmsg.callback(cf.type_disp, 1, 0, cf.rus_lng); // сообщение на индикатор о начале обмена с TS
      ts_snd(e_srv.put_ts(cf.AKey_w, cf.use_tst1, cf.use_tst2, cf.use_tst3, cf.use_tsh1, cf.use_tsh2, cf.use_tsh3, cf.use_tsp, _snr_data)); // Отправляем инфу на TS
      dmsg.callback(cf.type_disp, 1, 1, cf.rus_lng); // сообщение на индикатор о результатах обмена с TS
    }
  }
#endif
  return _snr_data;
}

#if defined(__AVR_ATmega2560__)
void(* resetFunc) (void) = 0; //Programm reset
#endif

# if defined(__xtensa__)
//------------------------------------------------------  Делаем запрос данных с Gismeteo
String gs_rcv(unsigned long city_id)
{
  if (debug_level == 10) DBG_OUT_PORT.println(F("True get data from GisMeteo"));
  String out = "No connect to network";
  if (web_cli)
  {
    String addr = "http://informer.gismeteo.ru/xml/";
    addr += String(city_id);
    addr += ".xml";
    out = nsys.http_client(addr);
  }
  if (debug_level == 10) DBG_OUT_PORT.println(out);
  return out;
}

/*
  =======================================================================
  Берем текущую погоду с сайта openweathermap.org
  =======================================================================
*/
const char* owmHost = "api.openweathermap.org";


wf_data_t getOWM_current(unsigned long cityID, char weatherKey[32])
{
  wf_data_t prog;

  String out = "No connect to network";
  DBG_OUT_PORT.print(F("\n Current weather from ")); DBG_OUT_PORT.println(owmHost);
  String addr = "http://";
  addr += owmHost;
  addr += "/data/2.5/forecast?id=";
  addr += cityID;
  addr += "&units=metric&appid=";
  addr += weatherKey;
  addr += "&lang=ru&cnt=1";
  DBG_OUT_PORT.println(addr);

  out = nsys.http_client(addr);
  //DBG_OUT_PORT.println(F(out);

  String line = remove_sb(out);

  DBG_OUT_PORT.println("\n Now " + line);

  DynamicJsonDocument jsonBuf(2048);
  DeserializationError error = deserializeJson(jsonBuf, line);

  if (error)
  {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
    return prog;
  }
  JsonObject root = jsonBuf.as<JsonObject>();

  prog.descript = root["list"]["weather"]["description"].as<String>();
  prog.descript.toLowerCase();
  prog.temp_min = root["list"]["main"]["temp"];
  prog.hum_min = root["list"]["main"]["humidity"];
  prog.press_min = root["list"]["main"]["grnd_level"]; //давление "на поверхности земли в текущей местности (с учетом высоты)"
  prog.press_max = root["list"]["main"]["sea_level"];  //давление "на уровне моря"

  prog.press_max -= prog.press_min; // разница давления "на уровне моря" и "у земли"
  prog.press_min /= 1.3332239; // перевод в мм.рт.ст

  prog.wind_min = root["list"]["wind"]["speed"];
  uint16_t dir = root["list"]["wind"]["deg"];
  prog.cloud = root["list"]["clouds"]["all"];
  prog.wind_dir = rumb_conv(dir);

  DBG_OUT_PORT.print(F("descript.."));
  DBG_OUT_PORT.println(prog.descript);
  DBG_OUT_PORT.print(F("temp min.."));
  DBG_OUT_PORT.println(prog.temp_min);
  DBG_OUT_PORT.print(F("humidity min.."));
  DBG_OUT_PORT.println(prog.hum_min);
  DBG_OUT_PORT.print(F("pressure min.."));
  DBG_OUT_PORT.println(prog.press_min);
  DBG_OUT_PORT.print(F("pressure max.."));
  DBG_OUT_PORT.println(prog.press_max);
  DBG_OUT_PORT.print(F("wind min.."));
  DBG_OUT_PORT.println(prog.wind_min);
  DBG_OUT_PORT.print(F("cloud.."));
  DBG_OUT_PORT.println(prog.cloud);
  DBG_OUT_PORT.print(F("win dir.."));
  DBG_OUT_PORT.println(prog.wind_dir);

  return prog;
}

/*
  =======================================================================
  Берем ПРОГНОЗ!!! погоды с сайта openweathermap.org
  =======================================================================
*/
wf_data_t getOWM_forecast(unsigned long cityID, char weatherKey[32])
{
  wf_data_t prog;

  String out = "No connect to network";
  DBG_OUT_PORT.print(F("\n Weather forecast for tomorrow from ")); DBG_OUT_PORT.println(owmHost);
  String addr = "http://";
  addr += owmHost;
  addr += "/data/2.5/forecast/daily?id=";
  addr += cityID;
  addr += "&units=metric&appid=";
  addr += weatherKey;
  addr += "&lang=ru&cnt=2";
  //if (debug_level == 10)
  DBG_OUT_PORT.println(addr);

  out = nsys.http_client(addr);
  if (debug_level == 10) DBG_OUT_PORT.println(out);

  String tempz = tvoday(out);
  DBG_OUT_PORT.println("\n" + tempz);

  DynamicJsonDocument jsonBuf(2048);
  DeserializationError error = deserializeJson(jsonBuf, tempz);
  if (error)
  {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
    return prog;
  }
  JsonObject root = jsonBuf.as<JsonObject>();

  prog.cloud = root["clouds"];
  prog.wind_min = root["speed"];
  uint16_t dir = root["deg"];
  prog.temp_min = root["temp"]["min"];
  prog.temp_max = root["temp"]["max"];
  prog.descript = root["weather"]["description"].as<String>();
  prog.descript.toLowerCase();
  prog.hum_min = root["humidity"];
  prog.press_max = root["pressure"];

  prog.press_min = (prog.press_max - 43) / 1.3332239; // перевод в мм.рт.ст

  time_t dt = root["dt"];

  prog.day = day(dt);
  prog.month = month(dt);

  prog.wind_dir = rumb_conv(dir);

  DBG_OUT_PORT.print(F("cloud.."));
  DBG_OUT_PORT.println(prog.cloud);
  DBG_OUT_PORT.print(F("wind min.."));
  DBG_OUT_PORT.println(prog.wind_min);
  DBG_OUT_PORT.print(F("win dir.."));
  DBG_OUT_PORT.println(prog.wind_dir);
  DBG_OUT_PORT.print(F("temp min.."));
  DBG_OUT_PORT.println(prog.temp_min);
  DBG_OUT_PORT.print(F("temp max.."));
  DBG_OUT_PORT.println(prog.temp_max);
  DBG_OUT_PORT.print(F("descript.."));
  DBG_OUT_PORT.println(prog.descript);
  DBG_OUT_PORT.print(F("humidity min.."));
  DBG_OUT_PORT.println(prog.hum_min);
  DBG_OUT_PORT.print(F("pressure max.."));
  DBG_OUT_PORT.println(prog.press_max);
  DBG_OUT_PORT.print(F("pressure min.."));
  DBG_OUT_PORT.println(prog.press_min);
  DBG_OUT_PORT.print(F("day.."));
  DBG_OUT_PORT.println(prog.day);
  DBG_OUT_PORT.print(F("month.."));
  DBG_OUT_PORT.println(prog.month);

  return prog;
}

// ------------------------------------------------------Удаляем квадратные скобки
String remove_sb(String in_str) {
  String out_str = String();
  uint16_t i = 0;
  while (i < in_str.length())
  {
    char c = in_str[i];
    if (c == '[' || c == ']') c = ' ';
    out_str += c;
    i++;
  }
  return out_str;
}

// ------------------------------------------------------Конвертиуем градусы в румбы (стороны света)
inline uint8_t rumb_conv(uint16_t dir)
{
  uint8_t w_dir = 0;
  if (dir > 23 && dir < 338) w_dir = ((dir - 23) / 45) + 1;
  return w_dir;
}

// ------------------------------------------------------Вырезаем данные по прогнозу на один день (из 2х)
String tvoday(String line) {
  String s = String();
  int start_sym = line.indexOf(']'); // позиция первого искомого символа ('{' после '[')

  start_sym = line.indexOf('{', start_sym + 1);

  int stop_sym = line.lastIndexOf(']');
  s = line.substring(start_sym, stop_sym);

  line = String();
  line = remove_sb(s);

  return line;
}

//------------------------------------------------------  Делаем запрос данных с внешнего сервера
String es_rcv(char es_addr[17])
{
  if (debug_level == 10) DBG_OUT_PORT.println(F("True get data from ext server"));
  String out = "No ext srv";
  if (web_cli)
  {
    String addr = "http://";
    addr += String(es_addr);
    addr += "/jsnr";
    if (debug_level == 10) DBG_OUT_PORT.println(addr);
    out = nsys.http_client(addr);
  }
  if (debug_level == 10) DBG_OUT_PORT.println(out);
  return out;
}

//------------------------------------------------------  Делаем запрос данных с ThingSpeak
String ts_rcv(unsigned long id, char api[17])
{
  if (debug_level == 10) DBG_OUT_PORT.println(F("True get data from TS"));
  String out = "No connect to network";
  if (web_cli)
  {
    String addr = "http://api.thingspeak.com/channels/";
    addr += String(id);
    addr += "/feed/last?key=";
    addr += String(api);
    out = nsys.http_client(addr);
  }
  //if (debug_level == 10)
  DBG_OUT_PORT.print(F("TS<-R response from ts "));
  DBG_OUT_PORT.println(out);
  return out;
}

//------------------------------------------------------  Отправляем данные на ThingSpeak
String ts_snd(String inStr)
{
  if (debug_level == 10) DBG_OUT_PORT.println(F("True put data to TS"));
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
  DBG_OUT_PORT.print(F("TS->W response from ts "));
  DBG_OUT_PORT.println(out);
  return out;
}

//------------------------------------------------------  Управляем радиоприемничком по телнету
String radio_snd(String cmd)
{
  if (debug_level == 10) DBG_OUT_PORT.println(F("True put data to Radio"));
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
  DBG_OUT_PORT.print(F("Response from Radio: "));
  DBG_OUT_PORT.println(out);
  return out;
}


//-------------------------------------------------------- Получаем точное время с НТП сервера
void GetNtp()
{
  bool result = false;
  RtcDateTime cur_time  = RtcDateTime(__DATE__, __TIME__);

  DBG_OUT_PORT.println(F("True sync time with NTP"));
  if (web_cli)
  {
    dmsg.callback(conf_data.type_disp, 0, 0, conf_data.rus_lng);; //сообщение на индикатор

    IPAddress addr(89, 109, 251, 21);
    cur_time = NTP_t.getTime(addr, conf_data.time_zone);
    if (cur_time.Year() < 2021)
    {
      IPAddress addr(10, 98, 34, 10);
      cur_time = NTP_t.getTime(addr, conf_data.time_zone);
    }
    if (cur_time.Year() < 2021)
    {
      IPAddress addr(88, 212, 196, 95);
      cur_time = NTP_t.getTime(addr, conf_data.time_zone);
    }
    if (cur_time.Year() > 2020)
    {
      setTime(cur_time);

      RtcDateTime cur_time1 = RtcDateTime(cur_time.Year() - 30, cur_time.Month(), cur_time.Day(), cur_time.Hour(), cur_time.Minute(), cur_time.Second()); //Потому что макуна считает с 2000го, а тайм с 1970го

      //----------------------------------------set time to chip dsXXXX
      switch (ram_data.type_rtc)
      {
        case 1:
          DS3231.SetDateTime(cur_time1);
          break;
        case 2:
          DS1302.SetDateTime(cur_time1);
          break;
        case 3:
          DS1307.SetDateTime(cur_time1);
          break;
      }
      result = true;
    }
    if (result) dmsg.callback(conf_data.type_disp, 0, 1, conf_data.rus_lng); //сообщение на индикатор
    else dmsg.callback(conf_data.type_disp, 0, 2, conf_data.rus_lng); //сообщение на индикатор

    set_alarm();  //актуализируем будильники
  }
  if (result)   DBG_OUT_PORT.println(F("Sucsess !!!"));
  else   DBG_OUT_PORT.println(F("Failed !!!"));
}
#endif

//------------------------------------------------------  Обрабатываем клавиатуру
void keyb_read()
{
  bool but0_pressed = !digitalRead(conf_data.gpio_btn); // false - кнопка нажата
  static bool but0_press;
  static unsigned long setting_ms;

  if (but0_pressed & !but0_press) setting_ms = millis(); // Нажимаем кнопку - запускаем таймер, начинаем отсчет времени удержания

  if (!but0_pressed & but0_press & (millis() - setting_ms > 150) & (millis() - setting_ms < 400)) // держим от 0,15 до 0,4 сек
  {
    disp_mode++; // меняем содержимое экрана на 7ми сегментных индикаторах
    if (disp_mode > 12) disp_mode = 0;

    max_st = 4;
    end_run_st = !end_run_st_buf; //Запуск бегущей строки;

    but0_press = but0_pressed;
  }

#if defined(__xtensa__)
  bool serv_act = (web_cli || web_ap);

  if (but0_pressed && !serv_act && millis() - setting_ms > 2000 && conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  // Включаем светодиод
  if (but0_pressed &&  serv_act && millis() - setting_ms > 2000 && conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH); // Выключаем светодиод
  if (but0_pressed && millis() - setting_ms > 9000 && conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(conf_data.gpio_led, blinkColon); // Мигаем светодиодом

  if (!but0_pressed && but0_press && !serv_act && millis() - setting_ms > 2000 && millis() - setting_ms < 9000)
  {
    serv_ms = millis();
    start_serv();  // держим от 2 до 9 сек - Запускаем web морду
    but0_press = but0_pressed;
  }

  if (!but0_pressed && but0_press && serv_act && millis() - setting_ms > 2000 && millis() - setting_ms < 9000)
  {
    serv_ms = millis() + 60000L;
    stop_serv();  // держим от 2 до 9 сек - Останавливаем web морду
    but0_press = but0_pressed;
  }
  if (!but0_pressed && but0_press && millis() - setting_ms > 9000 && millis() - setting_ms < 15000)
  {
    DBG_OUT_PORT.println(F("Reboot esp..."));
    ESP.restart();// держим от 9 до 15 сек - Перезапускаемся
  }
#endif

  if (but0_pressed && millis() - setting_ms > 15000)                // держим больше 15 сек - сбрасываем усе на дефолт
  {
    if (debug_level == 10) DBG_OUT_PORT.println(F("Set default value and reboot..."));
    conf_data = defaultConfig();
    saveConfig(conf_f, conf_data);

#if defined(__AVR_ATmega2560__)
    DBG_OUT_PORT.println(F("Reboot mega..."));
    delay(500);
    resetFunc();
#endif

#if defined(__xtensa__)
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    delay(100);
    //ESP.reset();
    DBG_OUT_PORT.println(F("Reboot esp..."));
    delay(500);
    ESP.restart();
#endif
  }
  but0_press = but0_pressed;
}

//------------------------------------------------------  Отправляем данные по USART
String uart_st(snr_data_t sn, wf_data_t wf, conf_data_t cf, rtc_data_t rt, uint8_t c_br)
{
  DynamicJsonDocument jsonBuffer(500);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["T"] = rt.ct;
  json["U"] = c_br;
  json["V"] = sn.t1;
  json["W"] = sn.t2;
  json["X"] = sn.h1;
  json["Y"] = sn.h2;
  json["Z"] = sn.p;
  json["M"] = rt.a_hour;
  json["N"] = rt.a_min;

  if (cf.use_pp > 0)
  {
    json["A"] = wf.tod;
    json["B"] = wf.cloud;
    json["C"] = wf.prec;
    json["D"] = wf.rpower;
    json["E"] = wf.spower;
    json["F"] = wf.press_min;
    json["G"] = wf.temp_max;
    json["H"] = wf.temp_min;
    json["I"] = wf.wind_max;
    json["J"] = wf.wind_min;
    json["K"] = wf.wind_dir;
    json["L"] = (int)(wf.hum_max + wf.hum_min) / 2;
    json["O"] = wf.day;
    json["P"] = wf.month;
  }

  json["Q"] = "End";

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  return st;
}

void send_uart()
{
  DBG_OUT_PORT.println(uart_st(snr_data, wf_data, conf_data, rtc_data, cur_br));
}

//------------------------------------------------------  Термостат
void Thermo(snr_data_t sn, conf_data_t cf)
{
  bool act = 0;
  if (cf.type_thermo > 0)
  {
    if (cf.src_thermo == 0)
    {
      act = (sn.t1 > cf.lb_thermo) && (sn.t1 < cf.hb_thermo);
    }
    else
    {
      act = (sn.t2 > cf.lb_thermo) && (sn.t2 < cf.hb_thermo);
    }
    if (act)
    {
      if (cf.type_thermo == 1)
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS ON!!!");
        digitalWrite(cf.gpio_trm, HIGH);
      }
      else
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS OFF!!!");
        digitalWrite(cf.gpio_trm, LOW);
      }
    }
    else
    {
      if (cf.type_thermo == 1)
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS OFF!!!");
        digitalWrite(cf.gpio_trm, LOW);
      }
      else
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS ON!!!");
        digitalWrite(cf.gpio_trm, HIGH);
      }
    }
  }
}

//------------------------------------------------------  Внутренняя флэшка LittleFS
# if defined(__xtensa__)

void printFile(const char* filename) {
  // Open file for reading
#if defined(ESP8266)
  File file = LittleFS.open(filename, "r");
#elif defined(ARDUINO_ARCH_ESP32)
  File file = LITTLEFS.open(filename, "r");
#endif

  if (!file)
  {
    DBG_OUT_PORT.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) DBG_OUT_PORT.print((char)file.read());
  DBG_OUT_PORT.println();

  // Close the file
  file.close();
}


void fs_setup()
{
#if defined(ESP8266)
  if (!LittleFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LittleFS.format();
  }
  else
  {
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUT_PORT.printf(" FS File: %s, size: %s\n", fileName.c_str(), fsys.formatBytes(fileSize).c_str());
    }
  }
#elif defined(ARDUINO_ARCH_ESP32)

  if (!LITTLEFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LITTLEFS.format();
  }
  else
  {
    File root = LITTLEFS.open("/");

    String output = "[";
    if (root.isDirectory()) {
      File file = root.openNextFile();
      while (file) {
        if (output != "[") output += ',';
        output += "{\"type\":\"";
        output += (file.isDirectory()) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(file.name());
        output += "\"}";
        file = root.openNextFile();
      }
      DBG_OUT_PORT.println(output);
    }
  }
# endif
}
#endif
