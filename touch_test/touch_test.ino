
// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined(ARDUINO_ARCH_ESP32) || CONFIG_IDF_TARGET_ESP32C3
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#endif

#if defined(ESP8266)
#include <ESP8266WebServer.h>
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <WebServer.h>
#endif

#include "GyverButton.h"

// объявляем кнопки
GButton myButt1;
GButton myButt2;
GButton myButt3;
GButton myButt4;

// объявляем сервер
#if defined(ESP8266)
ESP8266WebServer server(80);
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
WebServer server(80);
#endif

uint8_t value1 = 0, value2 = 0, value3 = 0, val_buff1 = 0, val_buff2 = 0, val_buff3 = 0;

void setup() {
  Serial.begin(115200);
  // меняем тип на LOW_PULL, потому что по умолчанию стоит HIGH_PULL
  myButt1.setType(LOW_PULL);
  myButt2.setType(LOW_PULL);
  myButt3.setType(LOW_PULL);
  myButt4.setType(LOW_PULL);

  myButt1.setStepTimeout(50);
  myButt2.setStepTimeout(50);
  myButt3.setStepTimeout(50);
  myButt4.setStepTimeout(50);

  pinMode(22, OUTPUT);
}

void loop() {

#if CONFIG_IDF_TARGET_ESP32
  myButt1.tick(touchRead(33) < 30 && touchRead(33) > 10);
  myButt2.tick(touchRead(32) < 30 && touchRead(32) > 10);
  myButt3.tick(touchRead(13) < 30 && touchRead(13) > 10);
  myButt4.tick(touchRead(27) < 30 && touchRead(27) > 10);
#endif
  /*
    if (myButt1.isHolded()) {
      Serial.println("hold 1");
    }
    if (myButt2.isHolded()) {
      Serial.println("hold 2");
    }
    if (myButt3.isHolded()) {
      Serial.println("hold 3");
    }

    if (myButt4.isHolded()) {
      Serial.println("hold 4");
    }
    delay(10);
  */
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
    Serial.println("Val1 Val2 Val3"); // для примера выведем в порт
    Serial.print(value1);             // для примера выведем в порт
    Serial.print("   ");              // для примера выведем в порт
    Serial.print(value2);             // для примера выведем в порт
    Serial.print("   ");              // для примера выведем в порт
    Serial.println(value3);           // для примера выведем в порт
    val_buff1 = value1;
    val_buff2 = value2;
    val_buff3 = value3;
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
    Serial.println("Val1 Val2 Val3");         // для примера выведем в порт
    Serial.print(value1);        // для примера выведем в порт
    Serial.print("   ");         // для примера выведем в порт
    Serial.print(value2);        // для примера выведем в порт
    Serial.print("   ");         // для примера выведем в порт
    Serial.println(value3);      // для примера выведем в порт
    val_buff2 = value2;
    val_buff3 = value3;
  }
  analogWrite(22, value2);         // для примера выведем в порт
}

//------------------------------------------------------  Отправляем показания датчиков на внешний сервер
void put_to_es(char *es_addr, uint8_t data1, uint8_t data2, bool button1, bool button2)
{
  // uint8_t dl = debug_level;
  // debug_level = 10;
  Serial.print(F("\nTrue put data to ext server -> "));

  String but1 = "0", but2 = "0";

  if (button1) but1 = "1";
  if (button2) but2 = "1";

  String postStr = "http://";
  postStr += String(es_addr);
  postStr += "/send_data?";
  postStr += "&data1=";
  postStr += String(data1);
  postStr += "&data2=";
  postStr += String(data2);
  postStr += "&button1=";
  postStr += but1;
  postStr += "&button2=";
  postStr += but2;

  //postStr += "\r\n";

  Serial.println(postStr);

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
      Serial.println(postStr);
    }
#if defined(ESP8266)
    delay(3000);
#endif
    http.end();
  }
}


//-------------------------------------------------------------- handler for receive data from remote controller
void handleRemoteControlRcv()
{
  /*
      url='/send_data?
       + '&data1='  + data1_t
       + '&data2='  + data2_t
       + '&button1='  + btn1_t
       + '&button2='  + btn2_t
  */
  uint8_t data1, data2; bool button1, button2;
  data1 = 0;
  data2 = 0;
  button1 = false;
  button2 = false;

  if (server.arg("data1") != 0)
  {
    data1 = constrain(server.arg("data1").toInt(), 0, 255);
  }

  if (server.arg("data1") != 0)
  {
    data2 = constrain(server.arg("data2").toInt(), 0, 255);
  }

  button1 = server.arg("button1") == "1";
  button2 = server.arg("button2") == "1";

  server.send(200, "text/html", "OK!");

  analogWrite(22, data1);         // для примера выведем в порт
  analogWrite(22, data2);         // для примера выведем в порт
  digitalWrite(22, button1);         // для примера выведем в порт
  digitalWrite(22, button2);         // для примера выведем в порт

#ifdef RGB_BUILTIN
  //  digitalWrite(RGB_BUILTIN, HIGH);   // Turn the RGB LED white
  digitalWrite(RGB_BUILTIN, LOW);    // Turn the RGB LED off

  neopixelWrite(RGB_BUILTIN, data1, data2, 0); // Red
  //  neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS); // Blue/
  //  neopixelWrite(RGB_BUILTIN,0,0,0); // Off / black
  delay(1000);
#endif
}
