// testshapes demo for RGBm3216Panel library.
// Demonstrates the drawing abilities of the RGBm3216Panel library.
// For 32x64 RGB LED m3216.

// NOTE THIS CAN ONLY BE USED ON A MEGA! NOT ENOUGH RAM ON UNO!

#include <ArduinoJson.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <RtcDS3231.h>
#include <TimeLib.h>
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
bool scroll_String(int8_t, int8_t, String, int&, int&, byte*, const byte*, uint8_t, uint8_t, uint8_t);
String pr_str(uint8_t);

void parser(String);

void m3216_init();
bool time_m32_8(byte*, uint8_t, byte*, unsigned char*, uint8_t*, bool*);
void copyFromPGM(const char* const*, byte *, uint8_t);



#define DBG_OUT_PORT Serial

//#define _debug

const char stdr_0[] PROGMEM = "ночь";
const char stdr_1[] PROGMEM = "yтро";
const char stdr_2[] PROGMEM = "день";
const char stdr_3[] PROGMEM = "вечер";

const char* const stdr[] PROGMEM = {stdr_0, stdr_1, stdr_2, stdr_3};

const char swnr_0[] PROGMEM = "северный";
const char swnr_1[] PROGMEM = "северо-восточный";
const char swnr_2[] PROGMEM = "восточный";
const char swnr_3[] PROGMEM = "юго-восточный";
const char swnr_4[] PROGMEM = "южный";
const char swnr_5[] PROGMEM = "юго-западный";
const char swnr_6[] PROGMEM = "западный";
const char swnr_7[] PROGMEM = "северо-западный";

const char* const swnr[] PROGMEM = {swnr_0, swnr_1, swnr_2, swnr_3, swnr_4, swnr_5, swnr_6, swnr_7};

const char sprcr_0[] PROGMEM = "дождь";
const char sprcr_1[] PROGMEM = "ливень";
const char sprcr_2[] PROGMEM = "снегопад";
const char sprcr_3[] PROGMEM = "сильный снегопад";
const char sprcr_4[] PROGMEM = "гроза";
const char sprcr_5[] PROGMEM = "без осадков";

const char* const sprcr[] PROGMEM = {sprcr_0, sprcr_1, sprcr_2, sprcr_3, sprcr_4, sprcr_5};

const char sdnr_1[] PROGMEM = "понедельник";
const char sdnr_2[] PROGMEM = "вторник";
const char sdnr_3[] PROGMEM = "среда";
const char sdnr_4[] PROGMEM = "четверг";
const char sdnr_5[] PROGMEM = "пятница";
const char sdnr_6[] PROGMEM = "суббота";
const char sdnr_7[] PROGMEM = "воскресенье";

const char* const sdnr[] PROGMEM = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

const char sdne_1[] PROGMEM = "Monday";
const char sdne_2[] PROGMEM = "Tuesday";
const char sdne_3[] PROGMEM = "Wednesday";
const char sdne_4[] PROGMEM = "Thursday";
const char sdne_5[] PROGMEM = "Friday";
const char sdne_6[] PROGMEM = "Saturday";
const char sdne_7[] PROGMEM = "Sunday";

const char* const sdne[] PROGMEM = {sdne_1, sdne_2, sdne_3, sdne_4, sdne_5, sdne_6, sdne_7};

const char smnr_01[] PROGMEM = "января";
const char smnr_02[] PROGMEM = "февраля";
const char smnr_03[] PROGMEM = "марта";
const char smnr_04[] PROGMEM = "апреля";
const char smnr_05[] PROGMEM = "мая";
const char smnr_06[] PROGMEM = "июня";
const char smnr_07[] PROGMEM = "июля";
const char smnr_08[] PROGMEM = "августа";
const char smnr_09[] PROGMEM = "сентября";
const char smnr_10[] PROGMEM = "октября";
const char smnr_11[] PROGMEM = "ноября";
const char smnr_12[] PROGMEM = "декабря";

const char* const smnr[] PROGMEM = {smnr_01, smnr_02, smnr_03, smnr_04, smnr_05, smnr_06, smnr_07, smnr_08, smnr_09, smnr_10, smnr_11, smnr_12};

const char smne_01[] PROGMEM = "January";
const char smne_02[] PROGMEM = "February";
const char smne_03[] PROGMEM = "March";
const char smne_04[] PROGMEM = "April";
const char smne_05[] PROGMEM = "May";
const char smne_06[] PROGMEM = "June";
const char smne_07[] PROGMEM = "July";
const char smne_08[] PROGMEM = "August";
const char smne_09[] PROGMEM = "September";
const char smne_10[] PROGMEM = "October";
const char smne_11[] PROGMEM = "November";
const char smne_12[] PROGMEM = "December";

const char* const smne[] PROGMEM = {smne_01, smne_02, smne_03, smne_04, smne_05, smne_06, smne_07, smne_08, smne_09, smne_10, smne_11, smne_12};

static const int     CLK  PROGMEM = 11; // MUST be on PORTB! (Use pin 11 on Mega)
static const int     LAT  PROGMEM = 10;
static const int      OE  PROGMEM =  9;
static const int       A  PROGMEM = A0;
static const int       B  PROGMEM = A1;
static const int       C  PROGMEM = A2;
static const int       D  PROGMEM = A3;

static const int ANA_SNR  PROGMEM = A4; // Пин фоторезистора
static const int DHT_PIN  PROGMEM = A9; // Пин DHT22

static const int uart_pin PROGMEM = 2;

RGBmatrixPanel *m3216;

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
snr_data_t es_data;

conf_data_t conf_data;
ram_data_t ram_data;

rtc_data_t rtc_data;


uint16_t _weekday = 1, _month = 7, _day = 20, _year = 1975; //костыль


//-------------------------------------------------------m3216

const uint8_t q_dig = 6;  // количество цифр на дисплее

bool end_run_st = true, end_run_st_buf, m32_8time_act = false, blinkColon = false;

String st1 = "Starting....";

byte screen[64]; // display buffer

long irq_end [5];

char songBuff[240];
bool play_snd = false, nm_is_on = false, disp_on = true;

uint8_t       cur_br = 255, debug_level = 0;
