#ifndef conf_h
#define conf_h

#define FW_Ver 1.0 //16.05.17 added udp debug console

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
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <ESP32SSDP.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#endif

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <My_LFS.h>
#include <my_wifi.h>
#endif

#include <myrtc.h>
#include <Snd.h>

#include "Songs.h"
#include "web.h"

// ----------------------------------- Defenitions
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
HTTPUpdateServer httpUpdater;
#endif

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
WF wifi;
WFJS wifi_cfg;
LFS lfs;
#endif

CT myrtc;
RTCJS myrtccfg;
Synt Buzz;               //Конструктор пищалки

// ---------------------------------------------------- Global Variable

const char *conf_f = "/conf_rtc.json";  // config file name

String from_client = String();

char tstr[25];

unsigned long alarm_time = millis();

bool  play_snd  = false, wasAlarm = false, _wasAlarmed_int = false;;

uint8_t gpio_sda = 255;
uint8_t gpio_scl = 255;
uint8_t gpio_snd = 255;

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;
#endif

rtc_hw_data_t  rtc_hw;
rtc_cfg_data_t rtc_cfg;
rtc_time_data_t rtc_time;
rtc_alm_data_t rtc_alm;

#endif /*conf_h*/
