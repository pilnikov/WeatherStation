// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// NOTE THIS CAN ONLY BE USED ON A MEGA! NOT ENOUGH RAM ON UNO!

#include <ArduinoJson.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <RtcDS3231.h>
#include <TimeLib.h>
#include <Wire.h>
#include <BH1750.h>

#include <Udt.h>
#include <Sysf2.h>
#include <Snr.h>
#include <Fdsp.h>

snr_data_t snr_data;
snr_data_t ts_data;
snr_data_t es_data;
wf_data_t wf_data;
wf_data_t wf_data_cur;
conf_data_t conf_data;
ram_data_t ram_data;
rtc_data_t rtc_data;

void synchro();
String Serial_Read();
void mx_mov_str(String, uint8_t, unsigned long);
void parser(String);

void matrix_init();
void _matrix(uint8_t);
void matrix_time();


#define DBG_OUT_PORT Serial

#define _debug
#define new_max

static const uint8_t   uart_pin =  2;

//-------------------------------------------------------Matrix

uint8_t  num_st = 1, max_st = 4;      //номер и макс кол-во прокручевыемых строк
static const uint8_t      tsize = 1;  // Размер шрифта
static uint16_t grad;

uint8_t colon; //номер разделителя
const uint8_t num = 6;  // количество цифр
uint8_t digHt;
#ifdef new_max
const uint8_t digPos[num] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране
#else
const uint8_t digPos[num] = {0, 4, 11, 16, 23, 28};
#endif
static char d[num] = {0, 0, 0, 0, 0, 0};            // цифры
static char digoldig[num];                          // убегающая цифра
static uint8_t digtrans[num];                       // позиция цифры по оси у

uint8_t spacer = 1; // Промежуток между символами (кол-во точек)
uint8_t point = 0;
int cur_sym_pos[2] = {0, 0};
unsigned long lcd_scroll_time[2] = {millis(), millis()}, refresh_time = millis();
bool str_run;
String st1 = "Running String";

const static char* stdr[4] = {"ночь", "yтро", "день", "вечер"};
const static char* swnr[8] = {"северный", "северо-восточный", "восточный", "юго-восточный", "южный", "юго-западный", "западный", "северо-западный"};
const static char* sprcr[6] = {"дождь", "ливень", "снегопад", "сильный снегопад", "гроза", "без осадков"};
const static char* sdnr[7] = {"воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота"};
const static char* sdne[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const static char* smnr[12] = {"января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
const static char* smne[12] = {"January", "February ", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel *matrix;

//-------------------------------------------------------Sensor

SNR sens;

//--------------------------------------------------- Common

SF fsys;
FD f_dsp;

RtcDS3231<TwoWire> DS3231(Wire);       // Конструктор DS3231

uint8_t       cur_br = 255;
bool          inc_st;
bool          web_ap = false;
