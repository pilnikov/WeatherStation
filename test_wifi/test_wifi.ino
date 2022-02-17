/////////////////////////////////////////////////////////////////////////////////////////

#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);
  DBG_OUT_PORT.println(F("Test WiFi"));

  fs_setup();
  DBG_OUT_PORT.println(F("file system started"));

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки из EEPROM
  from_client = readFile(conf_f);
  wifi_data = wifi_cfg.from_json(from_client);
  DBG_OUT_PORT.println(wifi_data.sta_ip2);

  //wifi_data = wifi_cfg.def_conf();
  //wifi.saveConfig(conf_f, wifi_data);
  DBG_OUT_PORT.println(F("config loaded"));
  //wifi_data.st_ip1 = false;
  //wifi_data.st_ip2 = false;

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
  if (save_data_req)
  {
    writeFile(conf_f, from_client.c_str());
    wifi_data = wifi_cfg.from_json(from_client);
    save_data_req = false;
  }
}

void writeFile(const char * path, const char * message) {
  DBG_OUT_PORT.printf("Writing file: %s\n", path);
  LittleFS.remove(path);

  File file = LittleFS.open(path, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    DBG_OUT_PORT.println("File written");
  } else {
    DBG_OUT_PORT.println("Write failed");
  }
  file.close();
}

String readFile(const char* path)
{
  DBG_OUT_PORT.printf("Reading file: %s\n", path);
  String msg = String();
  File file = LittleFS.open(path, "r");
  if (!file)
  {
    DBG_OUT_PORT.println("Failed to open file for reading");
  }
  else
  {
    while (file.available())
    {
      msg += (char)file.read();
    }
    file.close();
  }
  return msg;
}
