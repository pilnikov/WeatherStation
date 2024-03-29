
#ifndef _conf_h__
#define _conf_h__

#define FW_Ver 1.0  //11.01.22 added udp debug console

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#elif defined(ARDUINO_ARCH_ESP32) || CONFIG_IDF_TARGET_ESP32C3
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#endif

#include <Wire.h>
#include <SPI.h>

#include <FS.h>

#include <my_wifi.h>
#include <BH1750.h>
#include <My_LFS.h>
#include <Sysfn.h>

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
SF hw_chk;  //For HW Check

// ----------------------------------- Variable

conf_data_t conf_data;
wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;
hw_data_t hw_data;

const char *conf_f = "/conf_main.json";  // config file name
String from_client = String();

static bool
  ch1_set = false,
  ch2_set = false,
  ch1_auto = true,
  ch2_auto = true,

  bumpless = false,
  ch1_press = false,
  ch2_press = false;

int buttonState = HIGH;  // variable for reading the pushbutton status

uint32_t setting_ms = 0;

static uint8_t
  ch1_val = 0,
  ch1_val_buf = 0,
  ch2_val = 0,
  ch2_val_buf = 0,
  ft = 0;

String ssta = String();

#endif /* conf_h */
