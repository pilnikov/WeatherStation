
#ifndef conf_h
#define conf_h

#define FW_Ver 1.0 //11.01.22 added udp debug console

// ------------------------------------------------------------- Include

#include <WiFiClient.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
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
#include <ESP32SSDP.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

#endif

#define ARDUINOJSON_USE_LONG_LONG 1
#include <pgmspace.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>

#include <FS.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>

#include "web.h"

#define DBG_OUT_PORT Serial

#if defined(ESP8266)
static ESP8266WebServer server(80);
static ESP8266HTTPUpdateServer httpUpdater;

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
static WebServer server(80);
static HTTPUpdateServer httpUpdater;
#endif

#include <Wire.h>
#include <BH1750.h>

// ----------------------------------- Typedef
typedef struct
{
  char
  sta_ssid[17],
           sta_pass[17],
           ap_ssid[17],
           ap_pass[17];
  uint8_t
  str1_on = 10,
  str1_off = 20,
  str2_on = 30,
  str2_off = 40,
  pin1 = 11,
  pin2 = 12,
  lim_l = 128,
  lim_h = 128;
} conf_data_t;

conf_data_t conf_data;

// ----------------------------------- Force define func name
void printFile(const char*);
void fs_setup();
conf_data_t loadConfig(const char*);
void saveConfig(const char*, conf_data_t);
conf_data_t defaultConfig();

uint8_t selector (uint8_t);
bool sta_init();
bool sta_check();
bool start_client();
void stop_client();

bool ap_init();

IPAddress start_wifi(const char*, const char*, const char*, const char*);
void stop_wifi();

void wifi_conn(byte, byte, byte);


const char *conf_f = "/config.json";  // config file name

BH1750 lightMeter;

bool                web_ap   = false;
bool                web_cli  = false;
bool           pin1_t        = false;
bool           pin2_t        = false;
bool           pin1_a        = true;
bool           pin2_a        = true;
unsigned long   serv_ms = 60000;

uint8_t ft = 0;

uint8_t         debug_level  = 0; // 0 - отключен

static unsigned long setting_ms = millis();

IPAddress myIP;

File fsUploadFile;

#endif /* conf_h */
