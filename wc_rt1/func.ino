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
    RtcTemperature t1 = ds3231 -> GetTemperature();
    rd.temp_rtc = round(t1.AsFloatDegC());
  }

# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
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

# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
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

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
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

  uint32_t pdt = root["dt"];
  RtcDateTime dt = RtcDateTime(pdt);

  prog.day = dt.Day();
  prog.month = dt.Month();

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
  RtcDateTime c_time  = RtcDateTime(rtc_data.ct);

  DBG_OUT_PORT.println(F("True sync time with NTP"));
  if (web_cli)
  {
    dmsg.callback(conf_data.type_disp, 0, 0, conf_data.rus_lng);; //сообщение на индикатор

    IPAddress addr(89, 109, 251, 21);
    c_time = NTP_t.getTime(addr, conf_data.time_zone);
    if (c_time.Year() < 2021)
    {
      IPAddress addr(10, 98, 34, 10);
      c_time = NTP_t.getTime(addr, conf_data.time_zone);
    }
    if (c_time.Year() < 2021)
    {
      IPAddress addr(88, 212, 196, 95);
      c_time = NTP_t.getTime(addr, conf_data.time_zone);
    }
    if (c_time.Year() > 2020)
    {
      RtcDateTime c_time1 = RtcDateTime(c_time.Year() - 30, c_time.Month(), c_time.Day(), c_time.Hour(), c_time.Minute(), c_time.Second()); //Потому что макуна считает с 2000го, а тайм с 1970го
      result = true;
      man_set_time(c_time1);
    }
    if (result) dmsg.callback(conf_data.type_disp, 0, 1, conf_data.rus_lng); //сообщение на индикатор
    else dmsg.callback(conf_data.type_disp, 0, 2, conf_data.rus_lng); //сообщение на индикатор
  }
  if (result)   DBG_OUT_PORT.println(F("Sucsess !!!"));
  else   DBG_OUT_PORT.println(F("Failed !!!"));
}


//-------------------------------------------------------------- cur_time_str
void cur_time_str(rtc_data_t rt, char in[])
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

  if (conf_data.rus_lng) sprintf_P(in, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdnr[rt.wday - 1], rt.day, rt.month, rt.year, rt.hour, rt.min, rt.sec);
  else sprintf_P(in, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdne[rt.wday - 1], rt.day, rt.month, rt.year, rt.hour, rt.min, rt.sec);
}
#endif

//------------------------------------------------------  Обрабатываем клавиатуру
void keyb_read()
{
  btn_released = btn_state_flag & digitalRead(conf_data.gpio_btn);
  if (btn_state_flag & !digitalRead(conf_data.gpio_btn)) tmr_started = true;
  if (btn_released) tmr_started = false;
  btn_state_flag = false;

  if (btn_released & (millis() - setting_ms > 150) & (millis() - setting_ms < 2000)) // держим от 0,15 до 2 сек
  {
    disp_mode++; // меняем содержимое экрана на 7ми сегментных индикаторах
    if (disp_mode > 12) disp_mode = 0;
    max_st = 7;
    runing_string_start(); //Запуск бегущей строки;
  }

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  bool serv_act = (web_cli || web_ap);
  if (btn_released & (millis() - setting_ms > 2000 && millis() - setting_ms < 9000)) // держим от 2 до 9 сек
  {
    if (!serv_act)
    {
      serv_ms = millis();
      start_serv();  //Запускаем web морду
      if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH); // Включаем светодиод
    }
    else
    {
      serv_ms = millis() + 60000L;
      stop_serv();  //Останавливаем web морду
      if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW); // Выключаем светодиод
    }
  }

#endif
  if ((millis() - setting_ms > 9000) & (conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, blinkColon); // Мигаем светодиодом

  if (btn_released & (millis() - setting_ms > 9000) & (millis() - setting_ms < 15000)) //держим от 9 до 15 сек
  {
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    stop_serv();
    DBG_OUT_PORT.println(F("Reboot ESPp..."));
    ESP.restart();                                                                      //Перезагружаемся
#elif defined(__AVR__)
    DBG_OUT_PORT.println(F("Reboot Mega..."));
    resetFunc();
#endif
  }

  if (btn_released & (millis() - setting_ms > 15000))                  //держим больше 15 сек
  {
    if (debug_level == 10) DBG_OUT_PORT.println(F("Set default value and reboot...")); //Cбрасываем усе на дефолт и перезагружаемся
    conf_data = defaultConfig();
    saveConfig(conf_f, conf_data);

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    stop_serv();
    DBG_OUT_PORT.println(F("Reboot ESP..."));
    delay(200);
    ESP.restart();
#elif defined(__AVR_ATmega2560__)
    DBG_OUT_PORT.println(F("Reboot Mega..."));
    resetFunc();
#endif
  }
  if (millis() - setting_ms > 20000) tmr_started = false;
  if (!tmr_started) setting_ms = millis();
  /*  if (tmr_started)
    {
      DBG_OUT_PORT.print(F("Timer ..."));
      DBG_OUT_PORT.println(millis() - setting_ms);
    }
  */
}

#if defined(ESP8266)
void IRAM_ATTR isr1() //Отпускаем кнопку
{
  btn_state_flag = true;
}

#elif defined(__AVR__)
void ISR_ATTR isr1() //Отпускаем кнопку
{
  btn_state_flag = true;
}

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32C3
void ARDUINO_ISR_ATTR isr1() //Отпускаем кнопку
{
  btn_state_flag = true;
}
#endif

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
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

void printFile(const char* filename) {
  // Open file for reading
  File file = LittleFS.open(filename, "r");

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
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32C3
  if (!LittleFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LittleFS.format();
  }
  else
  {
    File root = LittleFS.open("/");

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
