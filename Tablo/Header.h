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
String pr_str(uint8_t&, uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_data_t, String, uint8_t);

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

// ------------------------------------------------------ GPIO
#if defined(ARDUINO_ARCH_ESP32)
static const int           A_PIN  PROGMEM =   2;  // Пин A
static const int           B_PIN  PROGMEM =   5;  // Пин B
static const int           C_PIN  PROGMEM =  18;  // Пин C
static const int           D_PIN  PROGMEM =  23;  // Пин D

static const int         CLK_PIN  PROGMEM =  14;  // Пин CLK MUST be on PORTB! (Use pin 11 on Mega)
static const int         LAT_PIN  PROGMEM =  27;  // Пин LAT
static const int          OE_PIN  PROGMEM =  32;  // Пин OE
#endif


#if defined(__AVR_ATmega2560__)
static const int           A_PIN  PROGMEM =  54; //A0 Пин A
static const int           B_PIN  PROGMEM =  55; //A1 Пин B
static const int           C_PIN  PROGMEM =  56; //A2 Пин C
static const int           D_PIN  PROGMEM =  57; //A3 Пин D

static const int         CLK_PIN  PROGMEM =  11;  // Пин CLK MUST be on PORTB! (Use pin 11 on Mega)
static const int         LAT_PIN  PROGMEM =  10;  // Пин LAT
static const int          OE_PIN  PROGMEM =   9;  // Пин OE
#endif

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

const uint8_t irq_q = 5;
static uint8_t _st = 0;
static unsigned long buff_ms, _sum = 120002L;
unsigned long t3 = conf_data.period * 4000L;
const unsigned long timers[irq_q] = {240000L, t3, 24, 3, 1}, base_t = 15L, _offset = trunc(base_t / irq_q + 1); // значения * base_t -> время в мс

static uint8_t cur_sym_pos[3] = {0, 0, 0};
static uint16_t buffud[64];
static bool d_notequal[q_dig];
const uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                       // убегающая цифра
static uint8_t  num_st = 1;
uint8_t max_st = 4; //номер и макс кол-во прокручиваемых строк

String st1 = "Starting....";

byte screen[64]; // display buffer

bool play_snd = false, nm_is_on = false, disp_on = true, end_run_st = false;

uint8_t cur_br = 255, debug_level = 0;
