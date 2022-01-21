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

  //conf_data = defaultConfig();
  DBG_OUT_PORT.println(F("config loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы

  //--------------------------------------------------------  Запускаем WiFi
  myIP = start_wifi(conf_data.sta_ssid, conf_data.sta_pass, conf_data.ap_ssid, conf_data.ap_pass);

  if (web_cli || web_ap)
  {
    //------------------------------------------------------  Переопределяем консоль

    //------------------------------------------------------  Запускаем сервер, ОТА, MDNS
    nsys.OTA_init(conf_data.ap_ssid, conf_data.ap_pass);

    MDNS.begin(conf_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(conf_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

    web_setup();
    start_serv();
  }

  //------------------------------------------------------  Инициализируем кнопку
  pinMode(0, INPUT_PULLUP);
  pinMode(conf_data.pin1, OUTPUT);
  pinMode(conf_data.pin2, OUTPUT);
}

void loop()
{
  //------------------------------------------------------ Распределяем системные ресурсы
  server.handleClient();
  ArduinoOTA.handle();
  ft = analogRead(A0);
  if((ft > conf_data.lim_l) & (ft < conf_data.lim_l) & !(digitalRead(conf_data.pin1))) digitalWrite(conf_data.pin1, HIGH);
  if((ft < conf_data.lim_l) & (ft > conf_data.lim_l) & !(digitalRead(conf_data.pin1))) digitalWrite(conf_data.pin1, LOW);
  if((ft > conf_data.lim_l) & (ft < conf_data.lim_l) & !(digitalRead(conf_data.pin2))) digitalWrite(conf_data.pin2, HIGH);
  if((ft < conf_data.lim_l) & (ft > conf_data.lim_l) & !(digitalRead(conf_data.pin2))) digitalWrite(conf_data.pin2, LOW);
}
