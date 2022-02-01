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

  wifi_data = wifi.loadConfig(conf_f);

  //wifi_data = defaultConfig();
  //wifi.saveConfig(conf_f, wifi_data);
  DBG_OUT_PORT.println(F("config loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);
  myIP = wifi_data_cur.addr;

  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
    //------------------------------------------------------  Запускаем сервер, ОТА, MDNS
    nsys.OTA_init(wifi_data.ap_ssid, wifi_data.ap_pass);

    MDNS.begin(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

    web_setup();
    start_serv();

    //------------------------------------------------------ Запускаем SSDP
    nsys.ssdp_init();

  }
  DBG_OUT_PORT.println(F("End of setup"));
}

void loop()
{
  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
    server.handleClient();
    ArduinoOTA.handle();
  }
}
