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
#include <Sysf.h>
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
void render_number(int);
void render_number_in_place(int, int, int);
void render_00number(int);
void render_00number_in_place(int, int, int);
void mx_mov_str(String, uint8_t, unsigned long);
void parser(String);

void matrix_init();
void _matrix(uint8_t);


#define DBG_OUT_PORT Serial

//#define _debug
#define new_max
//#define font4pt
//#define font5pt

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
