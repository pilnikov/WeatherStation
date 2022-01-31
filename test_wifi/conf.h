#ifndef conf_h
#define conf_h

#define FW_Ver 1.0 //16.05.17 added udp debug console

// ------------------------------------------------------------- Include

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include <include/WiFiState.h> // WiFiState structure details

WiFiState state;
#ifndef RTC_USER_DATA_SLOT_WIFI_STATE
#define RTC_USER_DATA_SLOT_WIFI_STATE 33u
#endif

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <ESP32SSDP.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

#endif


#define ARDUINOJSON_USE_LONG_LONG 1

#include <pgmspace.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>

#include <Netwf.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>

#define DBG_OUT_PORT Serial

#include "web.h"

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
  st_ip2 = false,
  cli = false,
  ap = false;
} conf_data_t;

// ----------------------------------- Force define func name
void printFile(const char*);
void fs_setup();
conf_data_t loadConfig(const char*);
void saveConfig(const char*, conf_data_t);
conf_data_t defaultConfig();


bool sta_init();
bool sta_check();
bool start_client();
void stop_client();

bool ap_init();

IPAddress start_wifi(const char*, const char*, const char*, const char*);
void stop_wifi();

void wifi_conn(byte, byte, byte);

// ----------------------------------- Web server
#if defined(ESP8266)
static ESP8266WebServer server(80);
static ESP8266HTTPUpdateServer httpUpdater;
#endif

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
static WebServer server(80);
HTTPUpdateServer httpUpdater;
#endif

// ---------------------------------------------------- Common

const char *conf_f = "/config.json";  // config file name

unsigned long   serv_ms = 60000;

uint8_t         debug_level  = 0; // 0 - отключен

IPAddress myIP;

File fsUploadFile;

NF nsys;

conf_data_t conf_data;

#endif /*conf_h*/
