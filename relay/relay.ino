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

  pinMode(conf_data.pin1, OUTPUT);
  pinMode(conf_data.pin2, OUTPUT);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);


  pinMode(36, INPUT);
  pinMode(39, INPUT);
  pinMode(33, INPUT);
  pinMode(32, INPUT);
  pinMode(13, INPUT);
  pinMode(27, INPUT);

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

  //////////////////////////////////////////////////////////////// for rc
  myButt1.setType(LOW_PULL);
  myButt2.setType(LOW_PULL);
  myButt3.setType(LOW_PULL);
  myButt4.setType(LOW_PULL);

  myButt1.setStepTimeout(50);
  myButt2.setStepTimeout(50);
  myButt3.setStepTimeout(50);
  myButt4.setStepTimeout(50);
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

  //delay (100);

  //------------------------------------------------------ Тест RC
#if CONFIG_IDF_TARGET_ESP32
  myButt1.tick(touchRead(33) < 30 && touchRead(33) > 10);
  myButt2.tick(touchRead(32) < 30 && touchRead(32) > 10);
  myButt3.tick(touchRead(13) < 30 && touchRead(13) > 10);
  myButt4.tick(touchRead(27) < 30 && touchRead(27) > 10);
#endif
  strcpy(esrv1_addr, "192.168.1.101");


  //Main Volume -----------------//////////////////////////////////////////////////////////////////////////////////////////////
  if ((myButt2.isClick()) || (myButt2.isStep())) {  // одиночное нажатие или обработчик удержания с шагами
    if (value1 < 255) value1++;                     // инкремент
    if (value2 < 255) value2++;                     // инкремент
    if (value3 < 255) value3++;                     // инкремент
  }

  if ((myButt1.isClick()) || (myButt1.isStep())) {  // одиночное нажатие или обработчик удержания с шагами
    if (value1 > 0) value1--;                       // декремент
    if (value2 > 0) value2--;                       // декремент
    if (value3 > 0) value3--;                       // декремент
  }

  if (value1 != val_buff1)
  {
    DBG_OUT_PORT.println("Val1 Val2 Val3"); // для примера выведем в порт
    DBG_OUT_PORT.print(value1);             // для примера выведем в порт
    DBG_OUT_PORT.print("   ");              // для примера выведем в порт
    DBG_OUT_PORT.print(value2);             // для примера выведем в порт
    DBG_OUT_PORT.print("   ");              // для примера выведем в порт
    DBG_OUT_PORT.println(value3);           // для примера выведем в порт

    val_buff1 = value1;
    val_buff2 = value2;
    val_buff3 = value3;
    put_to_es(esrv1_addr, value2, value3, digitalRead(36), digitalRead(39));
  }

  //Balance -------------/////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((myButt3.isClick()) || (myButt3.isStep())) {           // одиночное нажатие или обработчик удержания с шагами
    if ((value2 > 0) && (value3 == value1)) value2--;        // декремент
    if ((value3 < value1) && (value2 == value1)) value3++;   // инкремент
  }

  if ((myButt4.isClick()) || (myButt4.isStep())) {           // одиночное нажатие или обработчик удержания с шагами
    if ((value3 > 0) && (value2 == value1)) value3--;        // декремент
    if ((value2 < value1) && (value3 == value1)) value2++;   // инкремент
  }

  if ((value2 != val_buff2) || (value3 != val_buff3))
  {
    DBG_OUT_PORT.println("Val1 Val2 Val3");         // для примера выведем в порт
    DBG_OUT_PORT.print(value1);        // для примера выведем в порт
    DBG_OUT_PORT.print("   ");         // для примера выведем в порт
    DBG_OUT_PORT.print(value2);        // для примера выведем в порт
    DBG_OUT_PORT.print("   ");         // для примера выведем в порт
    DBG_OUT_PORT.println(value3);      // для примера выведем в порт

    val_buff2 = value2;
    val_buff3 = value3;
    put_to_es(esrv1_addr, value2, value3, digitalRead(36), digitalRead(39));
  }
  if (!digitalRead(36) || !digitalRead(39)) put_to_es(esrv1_addr, value2, value3, digitalRead(36), digitalRead(39));
}

//------------------------------------------------------  Отправляем показания датчиков на внешний сервер
void put_to_es(char *es_addr, uint8_t data1, uint8_t data2, bool button1, bool button2)
{
  // uint8_t dl = debug_level;
  // debug_level = 10;
  DBG_OUT_PORT.print(F("\nTrue put data to ext server -> "));

  String but1 = "0", but2 = "0";

  if (!button1) but1 = "1";
  if (!button2) but2 = "1";

  String postStr = "http://";
  postStr += String(es_addr);
  postStr += "/rc_cmd?";
  postStr += "data1=";
  postStr += String(data1);
  postStr += "&data2=";
  postStr += String(data2);
  postStr += "&button1=";
  postStr += but1;
  postStr += "&button2=";
  postStr += but2;

  //postStr += "\r\n";

  DBG_OUT_PORT.println(postStr);

  HTTPClient http;
  bool beg;
#if defined(ESP8266)
  WiFiClient client;
  beg = http.begin(client, postStr); //HTTP
#else
  beg = http.begin(postStr); //HTTP
#endif
  if (beg)
  {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      postStr = http.getString();
      DBG_OUT_PORT.println(postStr);
    }
#if defined(ESP8266)
    delay(3000);
#endif
    http.end();
  }
}
