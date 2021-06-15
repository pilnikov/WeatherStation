//------------------------------------------------------  Получаем данные с датчиков
void GetSnr()
{
  snr_data_t ts_data;
  snr_data_t es_data;

  snr_data.t1 = 99;
  snr_data.t2 = 99;
  snr_data.t3 = 99;
  snr_data.h1 = 0;
  snr_data.h2 = 0;
  snr_data.h3 = 0;
  snr_data.p = 700;

  ram_data.temp_rtc = 99;

  if ((ram_data.type_snr1 == 5 || ram_data.type_snr2 == 5 || ram_data.type_snr3 == 5) && ram_data.type_rtc == 1)
  {
    RtcTemperature t1 = DS3231.GetTemperature();
    ram_data.temp_rtc = round(t1.AsFloatDegC());
  }

# if defined(__xtensa__)
  if (web_cli)
  {
    if (ram_data.type_snr1 == 1 || ram_data.type_snr2 == 1 || ram_data.type_snr3 == 1 || ram_data.type_snrp == 1)
    {
      dmsg.callback(conf_data.type_disp, 2, 0, conf_data.rus_lng); // сообщение на индикатор о начале обмена с TS
      String ts_str = ts_rcv(conf_data.ts_ch_id, conf_data.AKey_r);  // Получаем строчку данных от TS
      ts_data = e_srv.get_ts(ts_str); // Парсим строчку от TS
      dmsg.callback(conf_data.type_disp, 2, 1, conf_data.rus_lng); // сообщение на индикатор о результатах обмена с TS
    }
    if (ram_data.type_snr1 == 2 || ram_data.type_snr2 == 2 || ram_data.type_snr3 == 2 || ram_data.type_snrp == 2) es_data = e_srv.get_es(es_rcv(conf_data.esrv1_addr)); // Получаем данные от внешнего сервера1
    if (ram_data.type_snr1 == 3 || ram_data.type_snr2 == 3 || ram_data.type_snr3 == 3 || ram_data.type_snrp == 3) es_data = e_srv.get_es(es_rcv(conf_data.esrv2_addr)); // Получаем данные от внешнего сервера2

    if (conf_data.use_pp == 2) {
      wf_data_cur = getOWM_current(conf_data.pp_city_id, conf_data.owm_key);// Получаем данные от OWM
    }
  }
#endif
  if (ram_data.type_snr1 > 0 || ram_data.type_snr2 > 0 || ram_data.type_snr3 > 0)
  {
    snr_data = sens.read_snr(ram_data.type_snr1, ram_data.type_snr2, ram_data.type_snr3, ram_data.type_snrp, ram_data.temp_rtc, ts_data, es_data, wf_data_cur); // Заполняем матрицу данных с датчиков
  }

# if defined(__xtensa__)
  if (web_cli)
  {
    if (conf_data.use_tst1 || conf_data.use_tst2 || conf_data.use_tst3 || conf_data.use_tsh1 || conf_data.use_tsh2 || conf_data.use_tsh3 || conf_data.use_tsp)
    {
      dmsg.callback(conf_data.type_disp, 1, 0, conf_data.rus_lng); // сообщение на индикатор о начале обмена с TS
      ts_snd(e_srv.put_ts(conf_data.AKey_w, conf_data.use_tst1, conf_data.use_tst2, conf_data.use_tst3, conf_data.use_tsh1, conf_data.use_tsh2, conf_data.use_tsh3, conf_data.use_tsp, snr_data)); // Отправляем инфу на TS
      dmsg.callback(conf_data.type_disp, 1, 1, conf_data.rus_lng); // сообщение на индикатор о результатах обмена с TS
    }
  }
#endif
}

#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)

// функция копирования из PROGMEM
void copyFromPGM(const char* const* charMap, char * _buf)
{
  uint16_t _ptr = pgm_read_word(charMap); // получаем адрес из таблицы ссылок
  uint8_t i = 0;        // переменная - индекс массива буфера
  do
  {
    _buf[i] = (char)(pgm_read_byte(_ptr++)); // прочитать символ из PGM в ячейку буфера, подвинуть указатель
  } while (_buf[i++] > 0);              // повторять пока прочитанный символ не нулевой, подвинуть индекс буфера
}
#endif


#if defined(__AVR_ATmega2560__)
void(* resetFunc) (void) = 0; //Programm reset
#endif


#if defined(ESP8266)
// функция копирования из PROGMEM
void copyFromPGM(const void* charMap, char * _buf)
{
  const void* _ptr = pgm_read_ptr(charMap); // получаем адрес из таблицы ссылок
  uint8_t i = 0;        // переменная - индекс массива буфера
  do
  {
    _buf[i] = (char)(pgm_read_byte(_ptr++)); // прочитать символ из PGM в ячейку буфера, подвинуть указатель
  } while (_buf[i++] > 0);              // повторять пока прочитанный символ не нулевой, подвинуть индекс буфера
}
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

  if (but0_pressed && !but0_press) setting_ms = millis(); // Нажимаем кнопку - запускаем таймер, начинаем отсчет времени удержания

  if (!but0_pressed && but0_press && millis() - setting_ms > 150 && millis() - setting_ms < 400) // держим от 0,15 до 0,4 сек
  {
    disp_mode++; // меняем содержимое экрана на 7ми сегментных индикаторах
    if (disp_mode > 12) disp_mode = 0;
    irq_end[0] = millis();

    num_st++;    // перебираем строки на матрицах и LCD
    if (num_st > 4) num_st = 1;
    st1 = pr_str(num_st);
    f_dsp.utf8rus(st1);
    cur_sym_pos[0] = 0;
    cur_sym_pos[1] = 0;
    end_run_st = false; //Запуск бегущей строки;

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
String uart_st()
{
  DynamicJsonDocument jsonBuffer(500);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["T"] = now();
  json["U"] = cur_br;
  json["V"] = snr_data.t1;
  json["W"] = snr_data.t2;
  json["X"] = snr_data.h1;
  json["Y"] = snr_data.h2;
  json["Z"] = snr_data.p;
  json["M"] = rtc_data.a_hour;
  json["N"] = rtc_data.a_min;

  if (conf_data.use_pp > 0)
  {
    json["A"] = wf_data.tod;
    json["B"] = wf_data.cloud;
    json["C"] = wf_data.prec;
    json["D"] = wf_data.rpower;
    json["E"] = wf_data.spower;
    json["F"] = wf_data.press_min;
    json["G"] = wf_data.temp_max;
    json["H"] = wf_data.temp_min;
    json["I"] = wf_data.wind_max;
    json["J"] = wf_data.wind_min;
    json["K"] = wf_data.wind_dir;
    json["L"] = (int)(wf_data.hum_max + wf_data.hum_min) / 2;
    json["O"] = wf_data.day;
    json["P"] = wf_data.month;
  }

  json["Q"] = "End";

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  return st;
}

void send_uart()
{
  DBG_OUT_PORT.println(uart_st());
}

//------------------------------------------------------  Термостат
void Thermo()
{
  bool act = 0;
  if (conf_data.type_thermo > 0)
  {
    if (conf_data.src_thermo == 0)
    {
      act = (snr_data.t1 > conf_data.lb_thermo) && (snr_data.t1 < conf_data.hb_thermo);
    }
    else
    {
      act = (snr_data.t2 > conf_data.lb_thermo) && (snr_data.t2 < conf_data.hb_thermo);
    }
    if (act)
    {
      if (conf_data.type_thermo == 1)
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS ON!!!");
        digitalWrite(conf_data.gpio_trm, HIGH);
      }
      else
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS OFF!!!");
        digitalWrite(conf_data.gpio_trm, LOW);
      }
    }
    else
    {
      if (conf_data.type_thermo == 1)
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS OFF!!!");
        digitalWrite(conf_data.gpio_trm, LOW);
      }
      else
      {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS ON!!!");
        digitalWrite(conf_data.gpio_trm, HIGH);
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
