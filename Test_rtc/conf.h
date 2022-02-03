#ifndef conf_h
#define conf_h

#define FW_Ver 1.0 //16.05.17 added udp debug console

// ------------------------------------------------------------- Include


#if defined(ESP8266)
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <hw.h>

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <ESP32SSDP.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#endif


#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <pgmspace.h>
#include <Wire.h>
#include <my_wifi.h>
#include <Netwf.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>
#endif

#define ARDUINOJSON_USE_LONG_LONG 1

#include <ArduinoJson.h>

#include <Sysf2.h>
#include <Snd.h>
#include "Songs.h"

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

#include <myrtc.h>

// ----------------------------------- Typedef
rtc_hw_data_t  rtc_hw;
rtc_cfg_data_t rtc_cfg;
rtc_time_data_t rtc_time;
rtc_alm_data_t rtc_alm;

wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;

// ----------------------------------- Internal header files
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include "web.h"
#endif

// ----------------------------------- Force define func name
void printFile(const char*);
void fs_setup();

// ----------------------------------- Web server
#if defined(ESP8266)
static ESP8266WebServer server(80);
static ESP8266HTTPUpdateServer httpUpdater;
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
static WebServer server(80);
HTTPUpdateServer httpUpdater;
#endif

// ---------------------------------------------------- HW info
#if defined(ESP8266)
ESP8266HWInfo hwi;
#endif

// ---------------------------------------------------- Common
const char *conf_f = "/config.json";  // config file name
char tstr[25];

unsigned long   serv_ms = 60000;

bool               play_snd  = false;

uint8_t gpio_sda = 255;
uint8_t gpio_scl = 255;
uint8_t gpio_snd = 255;


#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
IPAddress myIP;
NF nsys;
WF wifi;
#endif

CT myrtc;
CfgCT myrtccfg;
SF fsys;
Synt Buzz;               //Конструктор пищалки

#endif /*conf_h*/
