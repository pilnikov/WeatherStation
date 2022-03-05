#ifndef global_var_h
#define global_var_h

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>

#include <Sysfn.h>
#include <myrtc.h>
#include <Snr.h>

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <my_wifi.h>
#endif

// ---------------------------------------------------- Common
const char      *conf_f = "/config.json";  // config file name

String          from_client = String();

int             boot_mode = 1;

unsigned long   serv_ms = 60000;

static bool     wasAlarm = false;
static bool     play_snd  = false;

static uint16_t cur_br  = 0;

//---------------------------------------------------------------------------------------------------
static byte     screen[64];       // display buffer
static uint8_t  text_size = 1;    // размер текста

//---------------------------------------------------------------------------------------------------
snr_data_t snr_data;
snr_cfg_t snr_cfg_data;
conf_data_t conf_data;
hw_data_t hw_data;
wf_data_t wf_data_cur;
wf_data_t wf_data;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;
#endif
rtc_hw_data_t  rtc_hw;
rtc_cfg_data_t rtc_cfg;
rtc_time_data_t rtc_time;
rtc_alm_data_t rtc_alm;

#endif /*global_var_h*/
