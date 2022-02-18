
#ifndef conf_h
#define conf_h

#define FW_Ver 1.0 //11.01.22 added udp debug console

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP8266)
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <ESPmDNS.h>
#include <ESP32SSDP.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#endif

#include <Wire.h>
#include <SPI.h>

#include <FS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <my_wifi.h>
#include <BH1750.h>
#include <My_LFS.h>

#include "cfg.h"
#include "web.h"

// ----------------------------------- Typedef
#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

#define ARDUINOJSON_USE_LONG_LONG 1

// ----------------------------------- Constructors

#if defined(ESP8266)
static ESP8266WebServer server(80);
static ESP8266HTTPUpdateServer httpUpdater;

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
static WebServer server(80);
static HTTPUpdateServer httpUpdater;
#endif

BH1750 lightMeter;
CFG main_cfg;
WF wifi;
WFJS wifi_cfg;
LFS lfs;

// ----------------------------------- Variable

conf_data_t conf_data;
wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;

const char *conf_f = "/conf_main.json";  // config file name
String from_client = String();

bool pin1_t   = false;
bool pin2_t   = false;
bool pin1_a   = true;
bool pin2_a   = true;

bool bumpless = false;

uint8_t ft = 0;

static unsigned long setting_ms;

#endif /* conf_h */
