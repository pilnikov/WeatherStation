#include "Netwf.h"

//--------------------------------------------------------- HTTP Client

String NF::http_client( String incoming )
{
  String payload = String();
  HTTPClient http;

  http.begin( incoming ); //HTTP

  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) 
    {
      if (httpCode == HTTP_CODE_OK) payload = http.getString();
      http.end();
    }
  return payload;
}

//--------------------------------------------------------- SSDP init
void NF::ssdp_init()
{
    DBG_OUT_PORT.printf("Starting SSDP...\n");

    SSDP.setDeviceType("upnp::rootdevice");
    SSDP.setHTTPPort(80);
    SSDP.setName("MyESP");
    SSDP.setSerialNumber("000000001");
    SSDP.setURL("index.html");
    SSDP.setModelName("ESP");
    SSDP.setModelNumber("8266");
    SSDP.setModelURL("http://www.yandex.ru");
    SSDP.setManufacturer("Espressif");
    SSDP.setManufacturerURL("http://www.espressif.com");

    SSDP.begin();
    DBG_OUT_PORT.printf("SSDP Ready!\n");
}

//--------------------------------------------------------- OTA init
void NF::OTA_init(char* ssid, char* pass )
{
  // Hostname 
  ArduinoOTA.setHostname(ssid);

  // Authentication
  ArduinoOTA.setPassword(pass);

  ArduinoOTA.begin();
  DBG_OUT_PORT.println("OTA Ready");
  DBG_OUT_PORT.print("IP address: ");
  DBG_OUT_PORT.println(WiFi.localIP());
}

IPAddress NF::str_to_ip(String inStr)
{
  String vStr = "";
  char inChr; //Обрабатываемый символ
  IPAddress addr;
  uint8_t j = 0;
  for ( uint8_t n = 0; n < inStr.length(); n++)
  {
    if (inChr == '.' || n == inStr.length())
    {
      addr[j] = vStr.toInt();
      j++; //переход к следующей триаде
      vStr = String();
    }

    if (isDigit(inChr)) vStr += (char)inChr; //Формируем значение
  }
  return addr;
}

