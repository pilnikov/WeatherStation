
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

String pr_str(uint8_t);

void pcf8574_init();
void lcd_time();
void m7219_init();
void m7219_ramFormer(byte*);
void ht1632_init();
void ht1632_ramFormer(byte*, const uint8_t, const uint8_t);

void a595_init();

bool scroll_String(int8_t, int8_t, String, int&, int&, byte*, const byte*, uint8_t, uint8_t, uint8_t);
uint8_t auto_br(uint16_t);

void bat (uint8_t, byte*);
void digit (uint8_t, uint8_t, byte*);
void mon_day (uint8_t, uint8_t);
void ala (uint8_t);
bool time_m32_8(byte*, uint8_t, byte*, unsigned char*, uint8_t*, bool*);

//-----------------------------------------------------------------------------new

//----------------------------------------------------------------------------Common

//----------------------------------------------------------------------------7 SEG
const char name_week_0[] PROGMEM = ""; 
const char name_week_1[] PROGMEM = "8c"; 
const char name_week_2[] PROGMEM = "\357H"; 
const char name_week_3[] PROGMEM = "8t"; 
const char name_week_4[] PROGMEM = "cP"; 
const char name_week_5[] PROGMEM = "4t"; 
const char name_week_6[] PROGMEM = "\357t"; 
const char name_week_7[] PROGMEM = "c6";

const char* const name_week7[] PROGMEM = {name_week_0, name_week_1, name_week_2, name_week_3, name_week_4, name_week_5, name_week_6, name_week_7};

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

const char sdnr_1[] PROGMEM = "воскресенье"; 
const char sdnr_2[] PROGMEM = "понедельник"; 
const char sdnr_3[] PROGMEM = "вторник"; 
const char sdnr_4[] PROGMEM = "среда"; 
const char sdnr_5[] PROGMEM = "четверг"; 
const char sdnr_6[] PROGMEM = "пятница"; 
const char sdnr_7[] PROGMEM = "суббота";

const char* const sdnr[] PROGMEM = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

const char sdne_1[] PROGMEM = "Sunday"; 
const char sdne_2[] PROGMEM = "Monday"; 
const char sdne_3[] PROGMEM = "Tuesday"; 
const char sdne_4[] PROGMEM = "Wednesday"; 
const char sdne_5[] PROGMEM = "Thursday"; 
const char sdne_6[] PROGMEM = "Friday"; 
const char sdne_7[] PROGMEM = "Saturday";

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

uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                       // убегающая цифра

int cur_sym_pos[4] = {0, 0, 0, 0};
bool end_run_st = true, end_run_st_buf, m32_8time_act = false, blinkColon = false;

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
