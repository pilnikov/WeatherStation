#include "Netwf.h"

//--------------------------------------------------------- HTTP Client

String NF::http_client( String incoming )
{
 	
    WiFiClient client;

    HTTPClient http;

	String payload = incoming;
    	
	//DBG_OUT_PORT.print("[HTTP] begin...\n");
    if (http.begin(client, payload)) 
	{  // HTTP


      //DBG_OUT_PORT.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      DBG_OUT_PORT.printf("[HTTP] GET... code: %d\n", httpCode);
	  if (httpCode > 0) 
	  {
        // HTTP header has been send and Server response header has been handled
        //DBG_OUT_PORT.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
		{
          payload = http.getString();
          //DBG_OUT_PORT.println(payload);
        }
      }
	   else {
		  payload = "[HTTP] GET failed";
		  DBG_OUT_PORT.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
      
	  }
      http.end();
    } 
	else {
		payload = "[HTTP} Unable to connect\n";
		DBG_OUT_PORT.print(payload);
	}
 
  return payload;
 }



//--------------------------------------------------------- SSDP init
void NF::ssdp_init()
{
    DBG_OUT_PORT.printf("Starting SSDP...\n");

    SSDP.setSchemaURL("description.xml");
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
  
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    DBG_OUT_PORT.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    DBG_OUT_PORT.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DBG_OUT_PORT.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DBG_OUT_PORT.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      DBG_OUT_PORT.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      DBG_OUT_PORT.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      DBG_OUT_PORT.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      DBG_OUT_PORT.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      DBG_OUT_PORT.println("End Failed");
    }
  });

  ArduinoOTA.begin();
  DBG_OUT_PORT.println("OTA Ready");
//  DBG_OUT_PORT.print("IP address: ");
//  DBG_OUT_PORT.println(WiFi.localIP());
}