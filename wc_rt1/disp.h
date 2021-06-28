
#include <Adafruit_GFX.h>
#include <LiquidCrystal_I2C.h>
/*
  #include "..\lib\MyLib_M7219\Max72.h"
  #include "..\lib\MyLib_HT1632\SHT1632_3.h"
  #include "..\lib\MyLib_HT1633\myHT16K33.h"
  #include "..\lib\MyLib_TM1637\myTM1637.h"
*/

#include "Max72.h"
#include "SHT1632_3.h"
#include "myHT16K33.h"
#include "myTM1637.h"


#if defined(ARDUINO_ARCH_ESP32)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <RGBmatrixPanel.h>
#include "fonts.h"
#endif


#if defined(__AVR_ATmega2560__)
#include <RGBmatrixPanel.h> // Hardware-specific library
#endif

void dow_sel(uint8_t);

String pr_str(uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_data_t, String, uint8_t);

void pcf8574_init();
void lcd_time();
void m7219_init();
void m7219_ramFormer(byte*);
void ht1632_init();
void ht1632_ramFormer(byte*, const uint8_t, const uint8_t);

void a595_init();

bool scroll_String(int8_t, int8_t, String, uint8_t&, uint8_t&, byte*, const byte*, uint8_t, uint8_t, uint8_t);
uint8_t auto_br(uint16_t, conf_data_t);

void bat (uint8_t, byte*);
void digit (uint8_t, uint8_t, byte*);
void mon_day (uint8_t, uint8_t);
void ala (uint8_t);
bool time_m32_8(byte*, uint8_t, unsigned char*, const uint8_t*, bool*, uint16_t*, bool, const uint8_t, rtc_data_t);

//-----------------------------------------------------------------------------new

//----------------------------------------------------------------------------Common

//----------------------------------------------------------------------------7 SEG

//----------------------------------------------------------------------------TM1637

static TM1637 * tm1637;

//----------------------------------------------------------------------------HT1633

static HT16K33 * ht1633;


//---------------------------------------------------------------------------LCD1602

#define countof(a) (sizeof(a) / sizeof(a[0]))

const uint8_t lcd_row = 2;
const uint8_t lcd_col = 16;

//LiquidCrystal_I2C lcd(lcd_adr, lcd_col, lcd_row); // set the LCD address to 0x27 for a 16 chars and 2 line display

uint8_t  num_st = 1, max_st = 3; //номер и макс кол-во прокручиваемых строк

static LiquidCrystal_I2C * lcd;

//---------------------------------------------------------------------------MAX7219 4 x 8 x 8 Matrix Display

#define new_max
//#define font4pt
//#define font5pt

static Max72 *m7219;

const uint8_t q_dig = 6;  // количество цифр на дисплее

const uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                       // убегающая цифра

uint8_t cur_sym_pos[2] = {0, 0};
bool end_run_st = true, end_run_st_buf = true, m32_8time_act = false, blinkColon = false;

String st1 = String();
uint16_t buffud[64];
bool d_notequal[q_dig];

byte screen[64]; // display buffer

//---------------------------------------------------------------------------HT1621
//массив батарейка
static const uint8_t batt[6] = {0x02, 0x82, 0x92, 0xD2, 0xF2, 0xF3};

//массив больших цифр
static const uint8_t dig1[10] = {0xF5, 0x60, 0xB6, 0xF2, 0x63, 0xD3, 0xD7, 0x70, 0xF7, 0xF3};

//массив маленьких цифр
static const uint8_t dig2[10] = {0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC};

#include <HT1621.h>

// refere to Macduino website for pin connections and their meaning
static HT1621 * ht21;

//---------------------------------------------------------------------------HT1632

static HT1632C * m1632;

//---------------------------------------------------------------------------Matrix
#if defined(ARDUINO_ARCH_ESP32)
static RGBmatrixPanel * m3216;
#endif

#if defined(__AVR_ATmega2560__)
static RGBmatrixPanel * m3216;
#endif


//---------------------------------------------------------------------------ILI9341
#include "Adafruit_ILI9341.h"

static Adafruit_ILI9341 * tft;
