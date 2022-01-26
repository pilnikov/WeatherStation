/////////////////////////////////////////////////////////////////////////////////////////
#include "conf.h"


void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

  //------------------------------------------------------  Инициализируем встроенную файловую систему LittleFS
  fs_setup();
  DBG_OUT_PORT.println(F("file system started"));

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки из EEPROM

  conf_data = loadConfig(conf_f);

  //  conf_data = defaultConfig();
  //  saveConfig(conf_f, conf_data);
  DBG_OUT_PORT.println(F("config loaded"));

  pinMode(conf_data.pin1, OUTPUT);
  pinMode(conf_data.pin2, OUTPUT);

  //--------------------------------------------------------  Запускаем основные сетевые сервисы

  //--------------------------------------------------------  Запускаем WiFi
  myIP = start_wifi(conf_data.sta_ssid, conf_data.sta_pass, conf_data.ap_ssid, conf_data.ap_pass);

  if (web_cli || web_ap)
  {
    //------------------------------------------------------  Переопределяем консоль

    //------------------------------------------------------  Запускаем сервер, ОТА, MDNS
    ArduinoOTA.setHostname(conf_data.ap_ssid);

    // Authentication
    ArduinoOTA.setPassword(conf_data.ap_pass);

    ArduinoOTA.begin();
    DBG_OUT_PORT.println("OTA Ready");
    DBG_OUT_PORT.print("IP address: ");
    DBG_OUT_PORT.println(WiFi.localIP());

    MDNS.begin(conf_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(conf_data.ap_ssid);
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
