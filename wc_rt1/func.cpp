#include "conf.h"

MSG dmsg_ff;  //For Messages
FD f_dsp_ff;  //For Display

MyDsp mydsp_ff;

CT myrtc_ff;        //For RTC Common
RTCJS myrtccfg_ff;  //For RTC Config
SNR sens_ff;        //For Sensor Common
MAINJS maincfg_ff;

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
ES e_srv_ff;
NF nsys_ff;
LFS lfs_ff;
#endif


static unsigned long setting_ms = millis();
static bool tmr_started = false, btn_released = false;
volatile bool btn_state_flag = false, _wasAlarmed_int = false;

void sensor_init(snr_cfg_t* cf) {

  if (cf->type_snr1 > 0 || cf->type_snr2 > 0 || cf->type_snr3 > 0) {
    if (cf->type_snr1 == 4 || cf->type_snr2 == 4 || cf->type_snr3 == 4) {
      sens_ff.dht_preset(cf->gpio_dht, 22);  //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)
    }

    sens_ff.init(cf);

    DBG_OUT_PORT.print(F("Snr type on channel 1 = "));
    DBG_OUT_PORT.println(cf->type_snr1);
    DBG_OUT_PORT.print(F("Snr type on channel 2 = "));
    DBG_OUT_PORT.println(cf->type_snr2);
    DBG_OUT_PORT.print(F("Snr type on channel 3 = "));
    DBG_OUT_PORT.println(cf->type_snr3);
    DBG_OUT_PORT.print("Snr type on pressure = ");
    DBG_OUT_PORT.println(cf->type_snrp);
    DBG_OUT_PORT.println(F("sensor inital"));
  }
}
//------------------------------------------------------  Получаем данные с датчиков
snr_data_t GetSnr(snr_data_t sb, snr_cfg_t rd, main_cfg_t cf, uint8_t type_rtc, bool cli, wf_data_t wfc) {
  snr_data_t td;
  snr_data_t ed1;
  snr_data_t ed2;
  snr_data_t sd;
  snr_data_t wd;

  sd.t1 = 99;
  sd.t2 = 99;
  sd.t3 = 99;
  sd.h1 = 0;
  sd.h2 = 0;
  sd.h3 = 0;
  sd.p = 700;

  uint8_t temp_rtc = 99;

  if ((rd.type_snr1 == 5 || rd.type_snr2 == 5 || rd.type_snr3 == 5) && type_rtc == 1) {
    temp_rtc = myrtc_ff.get_temperature();
  }

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  if (cli) {
    if (rd.type_snr1 == 1 || rd.type_snr2 == 1 || rd.type_snr3 == 1 || rd.type_snrp == 1) {
      dmsg_ff.callback(cf.dsp_t, 2, 0, cf.rus_lng);     // сообщение на индикатор о начале обмена с TS
      String ts_str = ts_rcv(cf.ts_ch_id, cf.AKey_r, cli);  // Получаем строчку данных от TS
      td = e_srv_ff.get_ts(ts_str);                         // Парсим строчку от TS
      dmsg_ff.callback(cf.dsp_t, 2, 1, cf.rus_lng);     // сообщение на индикатор о результатах обмена с TS
    }
    if (rd.type_snr1 == 2 || rd.type_snr2 == 2 || rd.type_snr3 == 2 || rd.type_snrp == 2) ed1 = e_srv_ff.get_es(es_rcv(cf.esrv1_addr, cli));  // Получаем данные от внешнего сервера1
    if (rd.type_snr1 == 3 || rd.type_snr2 == 3 || rd.type_snr3 == 3 || rd.type_snrp == 3) ed2 = e_srv_ff.get_es(es_rcv(cf.esrv2_addr, cli));  // Получаем данные от внешнего сервера2

    if (cf.use_pp == 2) {
      wd.h1 = wfc.hum_min;
      wd.t1 = wfc.temp_min;
      wd.p = wfc.press_min;
    }
  }
#endif
  if ((rd.type_snr1 > 0 && rd.type_snr1 < 14) || (rd.type_snr2 > 0 && rd.type_snr2 < 14) || (rd.type_snr3 > 0 && rd.type_snr3 < 14) || (rd.type_snrp > 0 && rd.type_snrp < 14)) {
    sd = sens_ff.read_snr(rd, temp_rtc, td, ed1, ed2, wd);  // Заполняем матрицу данных с датчиков
  }

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  if (cli) {
    if (cf.use_ts > 0) {
      dmsg_ff.callback(cf.dsp_t, 1, 0, cf.rus_lng);        // сообщение на индикатор о начале обмена с TS
      ts_snd(e_srv_ff.put_ts(cf.AKey_w, cf.use_ts, sd), cli);  // Отправляем инфу на TS
      dmsg_ff.callback(cf.dsp_t, 1, 1, cf.rus_lng);        // сообщение на индикатор о результатах обмена с TS
    }

    if (cf.use_es > 0) put_to_es(cf.esrv1_addr, cf.use_es, sd, cli);  //отправляем показания датчиков на внешний сервер 1
  }
#endif

  if (rd.type_snr1 == 12) {
    sd.t1 = sb.t1;
    sd.h1 = sb.h1;
  }
  if (rd.type_snr2 == 12) {
    sd.t2 = sb.t2;
    sd.h2 = sb.h2;
  }
  if (rd.type_snr3 == 12) {
    sd.t3 = sb.t3;
    sd.h3 = sb.h3;
  }
  if (rd.type_snrp == 12) {
    sd.p = sb.p;
  }

  return sd;
}

#if defined(__AVR_ATmega2560__)
void (*resetFunc)(void) = 0;  //Programm reset
#endif

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
//------------------------------------------------------  Делаем запрос данных с Gismeteo
String gs_rcv(unsigned long city_id, bool cli) {
  if (debug_level == 10) DBG_OUT_PORT.println(F("True get data from GisMeteo"));
  String out = "No connect to network";
  if (cli) {
    String addr = "http://informer.gismeteo.ru/xml/";
    addr += String(city_id);
    addr += ".xml";
    out = nsys_ff.http_client(addr);
  }
  if (debug_level == 10) DBG_OUT_PORT.println(out);
  return out;
}

/*
  =======================================================================
  Берем текущую погоду с сайта openweathermap.org
  =======================================================================
*/
wf_data_t getOWM_current(unsigned long cityID, char* weatherKey) {
  wf_data_t prog;
  const char* owmHost = "api.openweathermap.org";

  String out = "No connect to network";
  DBG_OUT_PORT.print(F("\n Current weather from "));
  DBG_OUT_PORT.println(owmHost);
  String addr = "http://";
  addr += owmHost;
  addr += "/data/2.5/forecast?id=";
  addr += cityID;
  addr += "&units=metric&appid=";
  addr += weatherKey;
  addr += "&lang=ru&cnt=1";
  DBG_OUT_PORT.println(addr);

  out = nsys_ff.http_client(addr);
  //DBG_OUT_PORT.println(F(out);

  String line = remove_sb(out);

  DBG_OUT_PORT.println("\n Now " + line);

  JsonDocument jsonBuf;
  DeserializationError error = deserializeJson(jsonBuf, line);

  if (error) {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
    return prog;
  }
  JsonObject root = jsonBuf.as<JsonObject>();

  prog.descript = root["list"]["weather"]["description"].as<String>();
  prog.descript.toLowerCase();
  prog.temp_min = root["list"]["main"]["temp"];
  prog.hum_min = root["list"]["main"]["humidity"];
  prog.press_min = root["list"]["main"]["grnd_level"];  //давление "на поверхности земли в текущей местности (с учетом высоты)"
  prog.press_max = root["list"]["main"]["sea_level"];   //давление "на уровне моря"

  prog.press_max -= prog.press_min;  // разница давления "на уровне моря" и "у земли"
  prog.press_min /= 1.3332239;       // перевод в мм.рт.ст

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
String getOWM_forecast(unsigned long cityID, char* weatherKey) {
  const char* owmHost = "api.openweathermap.org";

  String out = "No connect to network";

  DBG_OUT_PORT.print(F("\n Weather forecast from "));
  DBG_OUT_PORT.println(owmHost);

  String addr = "http://";
  addr += owmHost;
  addr += "/data/2.5/forecast/daily?id=";
  addr += cityID;
  addr += "&units=metric&appid=";
  addr += weatherKey;
  addr += "&lang=ru&cnt=2";

  //if (debug_level == 10)
  DBG_OUT_PORT.println(addr);

  out = nsys_ff.http_client(addr);
  DBG_OUT_PORT.println(out);

  return out;
}

wf_data_t forecast_decode(String inSt, uint8_t cnt) {
  DBG_OUT_PORT.print(F("\n Weather forecast "));

  wf_data_t prog;
  String tempz;

  if (cnt > 1) {
    tempz = lastday(inSt);
    DBG_OUT_PORT.print(F("on tomorrow \n"));
  } else {
    tempz = firstday(inSt);
    DBG_OUT_PORT.print(F("on today \n"));
  }

  DBG_OUT_PORT.println(tempz);

  JsonDocument jsonBuf;
  DeserializationError error = deserializeJson(jsonBuf, tempz);
  if (error) {
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

  prog.press_min = (prog.press_max - 43) / 1.3332239;  // перевод в мм.рт.ст

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
  while (i < in_str.length()) {
    char c = in_str[i];
    if (c == '[' || c == ']') c = ' ';
    out_str += c;
    i++;
  }
  return out_str;
}

// ------------------------------------------------------Конвертиуем градусы в румбы (стороны света)
inline uint8_t rumb_conv(uint16_t dir) {
  uint8_t w_dir = 0;
  if (dir > 23 && dir < 338) w_dir = ((dir - 23) / 45) + 1;
  return w_dir;
}

// ------------------------------------------------------Вырезаем данные по прогнозу на последний день из 3х
String lastday(String line) {
  String s = String();

  int start_sym = line.lastIndexOf("dt") - 2;
  int stop_sym = line.lastIndexOf(']');
  s = line.substring(start_sym, stop_sym);

  line = String();
  line = remove_sb(s);

  return line;
}

String firstday(String line) {
  String s = String();

  int start_sym = line.indexOf("dt") - 2;
  int stop_sym = line.lastIndexOf("dt") - 3;
  s = line.substring(start_sym, stop_sym);

  line = String();
  line = remove_sb(s);

  return line;
}
//------------------------------------------------------  Делаем запрос показаний датчиков с внешнего сервера
String es_rcv(char* es_addr, bool cli) {
  if (debug_level == 10) DBG_OUT_PORT.println(F("True get data from ext server"));
  String out = "No ext srv";
  if (cli) {
    String addr = "http://";
    addr += String(es_addr);
    addr += "/jsnr";
    if (debug_level == 10) DBG_OUT_PORT.println(addr);
    out = nsys_ff.http_client(addr);
  }
  if (debug_level == 10) DBG_OUT_PORT.println(out);
  return out;
}

//------------------------------------------------------  Отправляем показания датчиков на внешний сервер
void put_to_es(char* es_addr, uint8_t use_es, snr_data_t sd, bool cli) {
  // uint8_t dl = debug_level;
  // debug_level = 10;
  DBG_OUT_PORT.print(F("\nTrue put data to ext server -> "));

  if (cli) {
    String postStr = "http://";
    postStr += String(es_addr);
    postStr += "/rcv_snr?";
    if ((use_es & 0b00000001) && sd.t1 < 99) {
      postStr += "&est1=";
      postStr += String(sd.t1);
    }

    if ((use_es & 0b00000010) && sd.t2 < 99) {
      postStr += "&est2=";
      postStr += String(sd.t2);
    }

    if ((use_es & 0b00000100) && sd.t3 < 99) {
      postStr += "&est3=";
      postStr += String(sd.t3);
    }

    if ((use_es & 0b00001000) && sd.h1 > 0 && sd.h1 < 100) {

      postStr += "&esh1=";
      postStr += String(sd.h1);
    }

    if ((use_es & 0b00010000) && sd.h2 > 0 && sd.h2 < 100) {
      postStr += "&esh2=";
      postStr += String(sd.h2);
    }

    if ((use_es & 0b00100000) && sd.h3 > 0 && sd.h3 < 100) {
      postStr += "&esh3=";
      postStr += String(sd.h3);
    }

    if ((use_es & 0b01000000) && sd.p > 700 && sd.p < 800) {
      postStr += "&esp=";
      postStr += String(sd.p);
    }
    //postStr += "\r\n";

    DBG_OUT_PORT.println(postStr);

    HTTPClient http;
    bool beg;
#if defined(ESP8266)
    WiFiClient client;
    beg = http.begin(client, postStr);  //HTTP
#else
    beg = http.begin(postStr);  //HTTP
#endif
    if (beg) {
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        postStr = http.getString();
        DBG_OUT_PORT.println(postStr);
      }
#if defined(ESP8266)
      delay(3000);
#endif
      http.end();
    }
  }
}

//------------------------------------------------------  Делаем запрос данных с ThingSpeak
String ts_rcv(unsigned long id, char* api, bool cli) {
  if (debug_level == 10) DBG_OUT_PORT.println(F("True get data from TS"));
  String out = "No connect to network";
  if (cli) {
    String addr = "http://api.thingspeak.com/channels/";
    addr += String(id);
    addr += "/feed/last?key=";
    addr += String(api);
    out = nsys_ff.http_client(addr);
  }
  //if (debug_level == 10)
  DBG_OUT_PORT.print(F("TS<-R response from ts "));
  DBG_OUT_PORT.println(out);
  return out;
}

//------------------------------------------------------  Отправляем данные на ThingSpeak
String ts_snd(String inStr, bool cli) {
  if (debug_level == 10) DBG_OUT_PORT.println(F("True put data to TS"));
  String out = "No connect to network";
  if (cli) {
    WiFiClient client;

    const int httpsPort = 80;
    const char* host = "api.thingspeak.com";  // Your domain

    if (!client.connect(host, httpsPort)) {
      client.stop();
      out = "connection failed";
    } else {
      client.print(String("GET ") + inStr + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

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
String radio_snd(String cmd, bool cli, char* radio_addr) {
  if (debug_level == 10) DBG_OUT_PORT.println(F("True put data to Radio"));
  String out = "No connect with Radio";
  if (cli) {
    WiFiClient client;
    const int port = 23;

    if (!client.connect(radio_addr, port)) {
      client.stop();
      out = "connection failed";
    } else {
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
#endif

//------------------------------------------------------  Обрабатываем клавиатуру
void keyb_read(bool cli, bool ap, byte gpio_btn, uint8_t& disp_mode, uint8_t& max_st,
               byte thermo_t, byte vdrv_t, byte gpio_led, bool led_pola, bool blinkColon, uint32_t& serv_ms, main_cfg_t* main_cfg, bool& end_run_st) {
  btn_released = btn_state_flag & digitalRead(gpio_btn);
  if (btn_state_flag & !digitalRead(gpio_btn)) tmr_started = true;
  if (btn_released) tmr_started = false;
  btn_state_flag = false;

  if (btn_released & (millis() - setting_ms > 150) & (millis() - setting_ms < 2000))  // держим от 0,15 до 2 сек
  {
    disp_mode++;  // меняем содержимое экрана на 7ми сегментных индикаторах
    if (disp_mode > 12) disp_mode = 0;
    max_st = 7;
    f_dsp_ff.scroll_init();
    end_run_st = true;
  }

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  bool serv_act = (cli || ap);
  if (btn_released & (millis() - setting_ms > 2000 && millis() - setting_ms < 9000))  // держим от 2 до 9 сек
  {
    if (!serv_act) {
      serv_ms = millis();
      start_serv();                                                                              //Запускаем web морду
      if ((thermo_t == 0) & (vdrv_t != 5)) digitalWrite(gpio_led, led_pola ? LOW : HIGH);  // Включаем светодиод
    } else {
      serv_ms = millis() + 60000L;
      //stop_serv();  //Останавливаем web морду
      if ((thermo_t == 0) & (vdrv_t != 5)) digitalWrite(gpio_led, led_pola ? HIGH : LOW);  // Выключаем светодиод
    }
  }

#endif
  if ((millis() - setting_ms > 9000) & (thermo_t == 0) & (vdrv_t != 5)) digitalWrite(gpio_led, blinkColon);  // Мигаем светодиодом

  if (btn_released & (millis() - setting_ms > 9000) & (millis() - setting_ms < 15000))  //держим от 9 до 15 сек
  {
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    stop_serv();
    DBG_OUT_PORT.println(F("Reboot ESPp..."));
    ESP.restart();  //Перезагружаемся
#elif defined(__AVR__)
    DBG_OUT_PORT.println(F("Reboot Mega..."));
    resetFunc();
#endif
  }

  if (btn_released & (millis() - setting_ms > 15000))  //держим больше 15 сек
  {
    if (debug_level == 10) DBG_OUT_PORT.println(F("Set default value and reboot..."));  //Cбрасываем усе на дефолт и перезагружаемся
    *main_cfg = maincfg_ff.def_conf();
    String from_client = maincfg_ff.to_json(*main_cfg);

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    const char* conf_f = "/config.json";
    lfs_ff.writeFile(conf_f, from_client.c_str());

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
void IRAM_ATTR isr1()  //Отпускаем кнопку
{
  btn_state_flag = true;
}

#elif defined(__AVR__)
void ISR_ATTR isr1()  //Отпускаем кнопку
{
  btn_state_flag = true;
}

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void ARDUINO_ISR_ATTR isr1()  //Отпускаем кнопку
{
  btn_state_flag = true;
}
#endif


#if defined(ESP8266)
void IRAM_ATTR isr0() {
  _wasAlarmed_int = true;
}
#elif defined(__AVR__)
void ISR_ATTR isr0() {
  _wasAlarmed_int = true;
}
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void ISR_ATTR isr0() {
  _wasAlarmed_int = true;
}
#endif

//------------------------------------------------------  Отправляем данные по USART
String uart_st(snr_data_t snr, wf_data_t wf, main_cfg_t mcf, rtc_time_data_t rt, rtc_alm_data_t rta, uint8_t c_br) {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["T"] = rt.ct;
  json["U"] = c_br;
  json["V"] = snr.t1;
  json["W"] = snr.t2;
  json["X"] = snr.h1;
  json["Y"] = snr.h2;
  json["Z"] = snr.p;
  json["M"] = rta.time;

  if (mcf.use_pp > 0) {
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
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed forming json string"));
  return st;
}

void send_uart(snr_data_t snr, wf_data_t wf, main_cfg_t mcf, rtc_time_data_t rtc_time, rtc_alm_data_t rtc_alm, uint8_t br) {
  DBG_OUT_PORT.println(uart_st(snr, wf, mcf, rtc_time, rtc_alm, br));
}

//------------------------------------------------------  Термостат
void Thermo(snr_data_t snr, main_cfg_t mcf, gpio_cfg_t gcf) {
  bool act = 0;
  if (mcf.thermo_t > 0) {
    if (mcf.src_thermo == 0) {
      act = (snr.t1 > mcf.lb_thermo) && (snr.t1 < mcf.hb_thermo);
    } else {
      act = (snr.t2 > mcf.lb_thermo) && (snr.t2 < mcf.hb_thermo);
    }
    if (act) {
      if (mcf.thermo_t == 1) {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS ON!!!");
        digitalWrite(gcf.gpio_trm, HIGH);
      } else {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS OFF!!!");
        digitalWrite(gcf.gpio_trm, LOW);
      }
    } else {
      if (mcf.thermo_t == 1) {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS OFF!!!");
        digitalWrite(gcf.gpio_trm, LOW);
      } else {
        //DBG_OUT_PORT.println(F("Thermostate OUT IS ON!!!");
        digitalWrite(gcf.gpio_trm, HIGH);
      }
    }
  }
}
void wasAlm_reset() {
  _wasAlarmed_int = false;
}

void alarm1_action(bool cli, uint8_t a_act, uint8_t& a_act_out, uint8_t a_num, rtc_cfg_data_t* rtc_cfg, uint8_t a_type, bool& nmon, byte vdrv,
                   bool& disp_on, bool& play_snd, byte* screen, char* radio_addr) {
  //  dmsg_ff.alarm_msg(rtc_cfg.n_cur_alm, rtc_cfg.dsp_t, rtc_cfg.rus_lng);  // Сообщение на индикатор

  switch (a_act)  // Выполняем экшн
  {
    case 20:
      nmon = true;  // Включаем ночной режим
      break;
    case 21:
      nmon = false;  // Выключаем ночной режим
      break;
    case 22:
      disp_on = true;
      mydsp_ff.display_on(vdrv);
      break;
    case 23:
      disp_on = false;
      f_dsp_ff.CLS(screen, sizeof screen);
      mydsp_ff.display_off(vdrv);
      break;
    case 24:
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
      radio_snd("cli.start", cli, radio_addr);
#endif
      break;
    case 25:
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
      radio_snd("cli.stop", cli, radio_addr);
#endif
      break;
    default:
      a_act_out = a_act;
      play_snd = true;
      break;
  }
  // Сбрасываем разовый будильник (при необходимости)
  if (a_type == 4) {
    rtc_cfg->alarms[a_num].type = 0;
    String from_client = myrtccfg_ff.to_json(*rtc_cfg);
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    const char* conf_f = "/conf_rtc.json";
    lfs_ff.writeFile(conf_f, from_client.c_str());
#endif
    *rtc_cfg = myrtccfg_ff.from_json(from_client);
  }
}

void hw_accept(hw_data_t hd, snr_cfg_t* snr_cfg, uint8_t* vd_typ, uint8_t* rt_typ) {
  snr_cfg->bm_addr = hd.bm_addr;

  // if selected -> deselect auto
  if (!hd.ds3231_present && snr_cfg->type_snr1 == 5) {
    snr_cfg->type_snr1 = 0;
    DBG_OUT_PORT.println(F("DS3231 as a sensor on CH#1 is not found -> deselected"));
  }
  if (!hd.ds3231_present && snr_cfg->type_snr2 == 5) {
    snr_cfg->type_snr2 = 0;
    DBG_OUT_PORT.println(F("DS3231 as a sensor on CH#2 is not found -> deselected"));
  }
  if (!hd.ds3231_present && snr_cfg->type_snr3 == 5) {
    snr_cfg->type_snr3 = 0;
    DBG_OUT_PORT.println(F("DS3231 as a sensor on CH#3 is not found -> deselected"));
  }
  if (!hd.si7021_present && snr_cfg->type_snr1 == 6) {
    snr_cfg->type_snr1 = 0;
    DBG_OUT_PORT.println(F("SI7021 as a sensor on CH#1 is not found -> deselected"));
  }
  if (!hd.si7021_present && snr_cfg->type_snr2 == 6) {
    snr_cfg->type_snr2 = 0;
    DBG_OUT_PORT.println(F("SI7021 as a sensor on CH#2 is not found -> deselected"));
  }
  if (!hd.si7021_present && snr_cfg->type_snr3 == 6) {
    snr_cfg->type_snr3 = 0;
    DBG_OUT_PORT.println(F("SI7021 as a sensor on CH#2 is not found -> deselected"));
  }
  /*
	  if (!hd.am2320_present && snr_cfg->type_snr1 == 7)
	  {
		snr_cfg->type_snr1 = 0;
		DBG_OUT_PORT.println(F("AM2320 as a sensor on CH#1 is not found -> deselected"));
	  }
	  if (!hd.am2320_present && snr_cfg->type_snr2 == 7)
	  {
		snr_cfg->type_snr2 = 0;
		DBG_OUT_PORT.println(F("AM2320 as a sensor on CH#2 is not found -> deselected"));
	  }
	  if (!hd.am2320_present && snr_cfg->type_snr3 == 7)
	  {
		snr_cfg->type_snr3 = 0;
		DBG_OUT_PORT.println(F("AM2320 as a sensor on CH#3 is not found -> deselected"));
	  }
	*/
  if (!hd.bmp180_present && snr_cfg->type_snr1 == 8) {
    snr_cfg->type_snr1 = 0;
    DBG_OUT_PORT.println(F("BMP180 as a sensor on CH#1 is not found -> deselected"));
  }
  if (!hd.bmp180_present && snr_cfg->type_snr2 == 8) {
    snr_cfg->type_snr2 = 0;
    DBG_OUT_PORT.println(F("BMP180 as a sensor on CH#2 is not found -> deselected"));
  }
  if (!hd.bmp180_present && snr_cfg->type_snr3 == 8) {
    snr_cfg->type_snr3 = 0;
    DBG_OUT_PORT.println(F("BMP180 as a sensor on CH#3 is not found -> deselected"));
  }
  if (!hd.bmp180_present && snr_cfg->type_snrp == 8) {
    snr_cfg->type_snrp = 0;
    DBG_OUT_PORT.println(F("BMP180 as a pressure sensor is not found -> deselected"));
  }
  if (!hd.bmp280_present && snr_cfg->type_snr1 == 9) {
    snr_cfg->type_snr1 = 0;
    DBG_OUT_PORT.println(F("BMP280 as a sensor on CH#1 is not found -> deselected"));
  }
  if (!hd.bmp280_present && snr_cfg->type_snr2 == 9) {
    snr_cfg->type_snr2 = 0;
    DBG_OUT_PORT.println(F("BMP280 as a sensor on CH#2 is not found -> deselected"));
  }
  if (!hd.bmp280_present && snr_cfg->type_snr3 == 9) {
    snr_cfg->type_snr3 = 0;
    DBG_OUT_PORT.println(F("BMP280 as a sensor on CH#3 is not found -> deselected"));
  }
  if (!hd.bmp280_present && snr_cfg->type_snrp == 9) {
    snr_cfg->type_snrp = 0;
    DBG_OUT_PORT.println(F("BMP280 as a pressure sensor is not found -> deselected"));
  }
  if (!hd.bme280_present && snr_cfg->type_snr1 == 10) {
    snr_cfg->type_snr1 = 0;
    DBG_OUT_PORT.println(F("BME280 as a sensor on CH#1 is not found -> deselected"));
  }
  if (!hd.bme280_present && snr_cfg->type_snr2 == 10) {
    snr_cfg->type_snr2 = 0;
    DBG_OUT_PORT.println(F("BME280 as a sensor on CH#2 is not found -> deselected"));
  }
  if (!hd.bme280_present && snr_cfg->type_snr3 == 10) {
    snr_cfg->type_snr3 = 0;
    DBG_OUT_PORT.println(F("BME280 as a sensor on CH#3 is not found -> deselected"));
  }
  if (!hd.bme280_present && snr_cfg->type_snrp == 10) {
    snr_cfg->type_snrp = 0;
    DBG_OUT_PORT.println(F("BME280 as a pressure sensor is not found -> deselected"));
  }
  if (!hd.lcd_present && *vd_typ == 12) {
    *vd_typ = 0;
    DBG_OUT_PORT.println(F("LCD I2C is not found -> deselected"));
  }
  if (!hd.ht1633_present && *vd_typ == 11) {
    *vd_typ = 0;
    DBG_OUT_PORT.println(F("HT1633 is not found -> deselected"));
  }
  if (!hd.ds3231_present && *rt_typ == 1) {
    *rt_typ = 0;  //DS3231 is not present
    DBG_OUT_PORT.println(F("DS3231 is not found -> deselected"));
  }
  if (!hd.ds1307_present && *rt_typ == 3) {
    *rt_typ = 0;  //DS1307 is not present
    DBG_OUT_PORT.println(F("DS1307 is not found -> deselected"));
  }
  if (!hd.bh1750_present) {
    DBG_OUT_PORT.println(F("BH1750 is not found -> get light level from Analog Input"));
  }
}
