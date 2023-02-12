/////////////////////////////////////////////////////////////////////////////////////////
#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

  //------------------------------------------------------  Инициализируем встроенную файловую систему LittleFS
  lfs.begin();
  DBG_OUT_PORT.println(F("LittleFS started"));

  //------------------------------------------------------  Загружаем основную конфигурацию
  conf_f = "/conf_main.json";
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading")
  {
    DBG_OUT_PORT.println(F("Failed to open conf_main.json for reading. Using default configuration!!!"));
    conf_data = main_cfg.def_conf();
    from_client = main_cfg.to_json(conf_data);
    lfs.writeFile(conf_f, from_client.c_str());
  }
  else
  {
    conf_data = main_cfg.from_json(from_client);
  }
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  conf_data.ch1_in ? pinMode(conf_data.ch1_gpio, INPUT) : pinMode(conf_data.ch1_gpio, OUTPUT);
  conf_data.ch2_in ? pinMode(conf_data.ch2_gpio, INPUT) : pinMode(conf_data.ch2_gpio, OUTPUT);

  //------------------------------------------------------  Загружаем конфигурацию WiFi
  conf_f = "/conf_wifi.json";
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading")
  {
    DBG_OUT_PORT.println(F("Failed to open conf_wifi.json for reading. Using default configuration!!!"));
    wifi_data = wifi_cfg.def_conf();
    from_client = wifi_cfg.to_json(wifi_data);
    lfs.writeFile(conf_f, from_client.c_str());
  }
  else
  {
    wifi_data = wifi_cfg.from_json(from_client);
  }
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);

  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
    //------------------------------------------------------  Запускаем сервер
    web_setup();
    start_serv();
  }
#if CONFIG_IDF_TARGET_ESP32
  Wire.setPins(21, 22);
#elif CONFIG_IDF_TARGET_ESP32S2
  Wire.setPins(1, 2);
#elif CONFIG_IDF_TARGET_ESP32C3
  Wire.setPins(5, 6);
#elif defined(ESP8266)
  Wire.begin(4, 5);
# endif

  lightMeter.begin();
}

void loop()
{
  //------------------------------------------------------ Распределяем системные ресурсы

  time_tck = millis();
  server.handleClient();
  //  ArduinoOTA.handle();

  uint16_t lm = lightMeter.readLightLevel();
  //DBG_OUT_PORT.print(F("Lux.."));
  //DBG_OUT_PORT.println(lm);

  if  (lm < 512) ft = lm / 2;

  if ((ft > conf_data.lim_h) & ch1_auto &  ch1_set) ch1_set = false;
  if ((ft < conf_data.lim_l) & ch1_auto & !ch1_set) ch1_set = true;
  if ((ft > conf_data.lim_h) & ch2_auto &  ch2_set) ch2_set = false;
  if ((ft < conf_data.lim_l) & ch2_auto & !ch2_set) ch2_set = true;

  if (conf_data.ch1_dig && !conf_data.ch1_in)
  {
    digitalWrite(conf_data.ch1_gpio, ch1_set);
  }
  else
  {
    analogWrite(conf_data.ch1_gpio, ch1_val);
  }

  if (conf_data.ch2_dig && !conf_data.ch2_in)
  {
    digitalWrite(conf_data.ch2_gpio, ch2_set);
  }
  else
  {
    analogWrite(conf_data.ch2_gpio, ch2_val);
  }

  if (bumpless & (millis() > (setting_ms + 200))) bumpless = false;
  delay (100);
}
