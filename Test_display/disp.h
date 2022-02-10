
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


#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Adafruit_Protomatter.h>
#endif


#if defined(__AVR_ATmega2560__)
#include <RGBmatrixPanel.h> // Hardware-specific library
#endif


void pr_str(uint8_t&, uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_time_data_t, rtc_alm_data_t, String, uint8_t, char, bool);

void pcf8574_init();
void lcd_time(char*, bool);
void m7219_init();
void m7219_ramFormer(byte*);
void ht1632_init();
void ht1632_ramFormer(byte*, const uint8_t, const uint8_t);

void a595_init();
void m3216_ramFormer(byte*, uint8_t, uint8_t);

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
uint8_t  max_st = 6;      // макс кол-во прокручиваемых строк
uint8_t  text_size = 1;   // размер текста

static bool d_notequal[q_dig];
const uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                   // убегающая цифра
static uint16_t buffud[64];


static bool end_run_st = false, m32_8time_act = false, blinkColon = false;

static byte screen[64]; // display buffer
static char st1[254];
static char st2[20];

//---------------------------------------------------------------------------HT1621
#include <HT1621.h>

// refere to Macduino website for pin connections and their meaning
static HT1621 * ht21;

//---------------------------------------------------------------------------HT1632

static HT1632C * m1632;

//---------------------------------------------------------------------------Matrix
#if defined(__AVR_ATmega2560__)
static RGBmatrixPanel * m3216;
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
static Adafruit_Protomatter * m3216;
#endif

//---------------------------------------------------------------------------ILI9341
#include "Adafruit_ILI9341.h"

static Adafruit_ILI9341 * tft;
