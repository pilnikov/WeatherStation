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
  conf_data = main_cfg.from_json(from_client);

  //conf_data = main_cfg.def_conf();
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  pinMode(conf_data.pin1, OUTPUT);
  pinMode(conf_data.pin2, OUTPUT);

 //------------------------------------------------------  Загружаем конфигурацию WiFi
  conf_f = "/conf_wifi.json";
  from_client = lfs.readFile(conf_f);
  wifi_data = wifi_cfg.from_json(from_client);

  //wifi_data = wifi_cfg.def_conf();
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);

  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
    //------------------------------------------------------  Запускаем сервер, ОТА, MDNS
    ArduinoOTA.setHostname(wifi_data.ap_ssid);

    // Authentication
    ArduinoOTA.setPassword(wifi_data.ap_pass);

    ArduinoOTA.begin();
    DBG_OUT_PORT.println("OTA Ready");
    DBG_OUT_PORT.print("IP address: ");
    DBG_OUT_PORT.println(WiFi.localIP());

    MDNS.begin(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

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
  server.handleClient();
  ArduinoOTA.handle();
  uint16_t lm = lightMeter.readLightLevel();
  //DBG_OUT_PORT.print(F("Lux.."));
  //DBG_OUT_PORT.println(lm);

  if  (lm < 512) ft = lm / 2;

  if ((ft > conf_data.lim_h) & pin1_a &  pin1_t) pin1_t = false;
  if ((ft < conf_data.lim_l) & pin1_a & !pin1_t) pin1_t = true;
  if ((ft > conf_data.lim_h) & pin2_a &  pin2_t) pin2_t = false;
  if ((ft < conf_data.lim_l) & pin2_a & !pin2_t) pin2_t = true;

  digitalWrite(conf_data.pin1, pin1_t);
  digitalWrite(conf_data.pin2, pin2_t);

  if (bumpless & (millis() > (setting_ms + 200))) bumpless = false;

  delay (100);
}
