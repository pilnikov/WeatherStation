#include "Netwf.h"

//--------------------------------------------------------- HTTP Client

String NF::http_client( String incoming )
{
 	
    WiFiClient client;

    HTTPClient http;

	String payload = incoming;
    	
	//Serial.print("[HTTP] begin...\n");
    if (http.begin(client, payload)) 
	{  // HTTP


      //Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) 
	  {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
		{
          payload = http.getString();
          //Serial.println(payload);
        }
      }
	  //else Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      
      http.end();
    } 
	//else Serial.printf("[HTTP} Unable to connect\n");
 
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

