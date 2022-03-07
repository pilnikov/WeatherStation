#ifndef global_construct_h
#define global_construct_h

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <myrtc.h>
#include <Sysfn.h>
#include <Snd.h>
#include "Songs.h"
#include <Snr.h>
#include <Fdisp.h>
#include <Fdsp.h>
#include <BH1750.h>

#if defined(ESP8266)
#include <hw.h>
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM)
#include <Netwf_rt.h>
#endif

#if defined(ESP8266)
#include <ESP8266WebServer.h>
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <WebServer.h>
#endif

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <My_LFS.h>
#include <my_wifi.h>
#include <Netwf.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ntp.h>
#include <Exts.h>
#include <udp_cons.h>
#endif

// ---------------------------------------------------- Constructors
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
ES e_srv;
NF nsys;
WF wifi;
WFJS wifi_cfg;
LFS lfs;

udp_cons print_console_udp;
#endif

// ----------------------------------- Web server
#if defined(ESP8266)
ESP8266WebServer server(80);
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
WebServer server(80);
#endif

// ---------------------------------------------------- HW info
#if defined(ESP8266)
ESP8266HWInfo hwi;
#endif

// ---------------------------------------------------- LM
BH1750 lightMeter;

// ---------------------------------------------------- Other
FD f_dsp2; //For Display
MyDsp mydsp;

Synt Buzz;               //Конструктор пищалки

CT myrtc; //For RTC Common
RTCJS myrtccfg; //For RTC Config
SF hw_chk; //For HW Check
SNR sens_f; //For Sensor Common
SNRJS mysnrcfg; //For Sensor Config

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
// ---------------------------------------------------- News Client
NewsApiClient *newsClient;
#endif

#endif
