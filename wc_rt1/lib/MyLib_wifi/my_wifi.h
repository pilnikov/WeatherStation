#ifndef my_wifi_h
#define my_wifi_h

// ------------------------------------------------------------- Include

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <WiFi.h>
#endif

#define ARDUINOJSON_USE_LONG_LONG 1

#include <pgmspace.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

typedef struct
{
  char
  ap_ssid[20],
          ap_pass[20],

          sta_ssid1[20],
          sta_pass1[20],

          sta_ssid2[20],
          sta_pass2[20],

          ap_ip[17],
          ap_ma[17],

          sta_ip1[17],
          sta_ma1[17],
          sta_gw1[17],
          sta_dns11[17],
          sta_dns21[17],

          sta_ip2[17],
          sta_ma2[17],
          sta_gw2[17],
          sta_dns12[17],
          sta_dns22[17];
  bool
  wifi_off = false,
  st_ip1 = false,
  st_ip2 = false;
} wifi_cfg_data_t;

typedef struct
{
  char
  ssid[20];

  IPAddress
  addr;

  bool
  cli = false,
   ap = false;
} wifi_cur_data_t;

class WF
{
  public:
    void
    saveConfig(const char *, wifi_cfg_data_t),
    end(wifi_cur_data_t),
	_shutdown();

    wifi_cfg_data_t
    loadConfig(const char*),
    defaultConfig();
	
	wifi_cur_data_t
	begin(wifi_cfg_data_t);
  private:
  protected:
};

#endif /*conf_h*/
