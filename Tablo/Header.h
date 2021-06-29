// testshapes demo for RGBm3216Panel library.
// Demonstrates the drawing abilities of the RGBm3216Panel library.
// For 32x64 RGB LED m3216.

// NOTE THIS CAN ONLY BE USED ON A MEGA! NOT ENOUGH RAM ON UNO!

#include <ArduinoJson.h>
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <RtcDS3231.h>
#include <EEPROM.h>
#include <Wire.h>
#include <BH1750.h>

#include "Songs.h"
#include "fonts.h"
#include "rtc.h"

#include <Snd.h>
#include <Udt.h>
#include <Sysf2.h>
#include <Snr.h>
#include <Fdsp.h>


void synchro();
String Serial_Read();

void parser(String);

void m3216_init();
String pr_str(uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_data_t, uint8_t); 

#define DBG_OUT_PORT Serial

//#define _debug
static const int     CLK  PROGMEM = 11; // MUST be on PORTB! (Use pin 11 on Mega)
static const int     LAT  PROGMEM = 10;
static const int      OE  PROGMEM =  9;
static const int       A  PROGMEM = A0;
static const int       B  PROGMEM = A1;
static const int       C  PROGMEM = A2;
static const int       D  PROGMEM = A3;

static RGBmatrixPanel *m3216;

//-------------------------------------------------------Sensor

static SNR sens;


//--------------------------------------------------- Common

static SF fsys;
static FD f_dsp;

static BH1750 lightMeter;

static Synt Buzz;               //Конструктор пищалки

snr_data_t snr_data;
wf_data_t wf_data;
wf_data_t wf_data_cur;
snr_data_t es_data1;

conf_data_t conf_data;
ram_data_t ram_data;

rtc_data_t rtc_data;


//-------------------------------------------------------m3216

const uint8_t q_dig = 6;  // количество цифр на дисплее

bool end_run_st = true, end_run_st_buf, m32_8time_act = false, blinkColon = false;

String st1 = "Starting....";

byte screen[64]; // display buffer

long irq_end [5];

bool play_snd = false, nm_is_on = false, disp_on = true;

uint8_t cur_br = 255, debug_level = 0;
