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

const char *conf_f = "/config.json";  // config file name
char tstr[25];

String from_client = String();

int                boot_mode = 1;

unsigned long   serv_ms = 60000, alarm_time = millis();

bool  play_snd  = false, wasAlarm = false;

bool                disp_on  = true;

uint8_t            hour_cnt  = 0;
uint8_t           disp_mode  = 0;
uint16_t             cur_br  = 0;


//----------------------------------------------------------------------------------------------Global variables for displays
const uint8_t q_dig = 6;  // количество цифр на дисплее
uint8_t  max_st = 6;      // макс кол-во прокручиваемых строк
uint8_t  text_size = 1;   // размер текста

static bool d_notequal[q_dig];
const uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                   // убегающая цифра
static uint16_t buffud[64];

static bool end_run_st = false, m32_8time_act = false, blinkColon = false;

static byte screen[64]; // display buffer
static char st2[20];

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

#endif
