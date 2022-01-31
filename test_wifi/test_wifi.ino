/////////////////////////////////////////////////////////////////////////////////////////

#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

  fs_setup();
  DBG_OUT_PORT.println(F("file system started"));

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки из EEPROM

  conf_data = loadConfig(conf_f);

  //conf_data = defaultConfig();
  //saveConfig(conf_f, conf_data);
  DBG_OUT_PORT.println(F("config loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  myIP = start_wifi(conf_data.sta_ssid1, conf_data.sta_pass1, conf_data.ap_ssid, conf_data.ap_pass);

  if (conf_data.cli || conf_data.ap)
  {
    //------------------------------------------------------  Запускаем сервер, ОТА, MDNS
    nsys.OTA_init(conf_data.ap_ssid, conf_data.ap_pass);

    MDNS.begin(conf_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(conf_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

    web_setup();
    start_serv();

    //------------------------------------------------------ Запускаем SSDP
    nsys.ssdp_init();

  }
  DBG_OUT_PORT.println(F("Safe mode!!! End of setup"));
}

void loop()
{
  if (conf_data.cli || conf_data.ap)
  {
    server.handleClient();
    ArduinoOTA.handle();
  }
}
