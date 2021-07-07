
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


String pr_str(uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_data_t, String, uint8_t);

void pcf8574_init();
void lcd_time();
void m7219_init();
void m7219_ramFormer(byte*);
void ht1632_init();
void ht1632_ramFormer(byte*, const uint8_t, const uint8_t);

void a595_init();

//----------------------------------------------------------------------------TM1637

static TM1637 * tm1637;

//----------------------------------------------------------------------------HT1633

static HT16K33 * ht1633;


//---------------------------------------------------------------------------LCD1602
static LiquidCrystal_I2C * lcd;

//---------------------------------------------------------------------------MAX7219 4 x 8 x 8 Matrix Display

#define new_max
//#define font4pt
//#define font5pt

static Max72 * m7219;

const uint8_t q_dig = 6;  // количество цифр на дисплее
uint8_t  max_st = 3;      // макс кол-во прокручиваемых строк

static bool d_notequal[q_dig];
const uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                   // убегающая цифра
static uint16_t buffud[64];


static bool end_run_st = true, m32_8time_act = false, blinkColon = false;

String st1 = String();

byte screen[64]; // display buffer

//---------------------------------------------------------------------------HT1621
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
