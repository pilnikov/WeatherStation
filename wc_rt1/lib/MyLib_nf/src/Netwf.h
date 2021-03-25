// Sysf.h

#ifndef Netwf_h
#define Netwf_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM) 
#include <OTA.h>
#include <HttpClient.h>
#include <WiFiClient.h>
#endif

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266SSDP.h>
#include <ArduinoOTA.h>
#endif


#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32SSDP.h>
#include <ArduinoOTA.h>
#endif



#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

class NF 
{
   public:
    String    http_client(String);
    void      ssdp_init();
    void      OTA_init(char*, char*);
    IPAddress str_to_ip(String);
 
   private:
   protected:
};

extern NF Nfunc;

#endif  // Netwf_h


