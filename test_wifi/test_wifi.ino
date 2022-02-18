/////////////////////////////////////////////////////////////////////////////////////////

#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);
  DBG_OUT_PORT.println(F("Test WiFi"));

  //------------------------------------------------------  Запускаем LittleFileSystem
  lfs.begin();
  DBG_OUT_PORT.println(F("file system started"));

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки из конфиг файла
  from_client = lfs.readFile(conf_f);
  wifi_data = wifi_cfg.from_json(from_client);

  //wifi_data = wifi_cfg.def_conf();
  //wifi_data.st_ip1 = false;
  //wifi_data.st_ip2 = false;
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
