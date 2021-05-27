
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
#include "ESP32RGBmatrixPanel.h"
#include "fonts.h"
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
uint16_t auto_br(uint16_t);

void bat (uint8_t, byte*);
void digit (uint8_t, uint8_t, byte*);
void mon_day (uint8_t, uint8_t);
void ala (uint8_t);
bool time_m32_8(byte*, uint8_t, byte*, char*, uint8_t*, bool*);

//-----------------------------------------------------------------------------new

//----------------------------------------------------------------------------Common

//----------------------------------------------------------------------------7 SEG
#if defined(__AVR__)

const char name_week7[8] [2] PROGMEM = {"", "8c", "\357H", "8t", "cP", "4t", "\357t", "c6"};
const char stdr[4] [5] PROGMEM = {"ночь", "yтро", "день", "вечер"};
const char swnr[8] [15] PROGMEM = {"северный", "северо-восточный", "восточный", "юго-восточный", "южный", "юго-западный", "западный", "северо-западный"};
const char sprcr[6] [16] PROGMEM = {"дождь", "ливень", "снегопад", "сильный снегопад", "гроза", "без осадков"};
const char sdnr[7] [11] PROGMEM = {"воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота"};
const char sdne[7] [10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char smnr[12] [7] PROGMEM = {"января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
const char smne[12] [10] PROGMEM = {"January", "February ", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

#elif defined(__xtensa__)
static const char* name_week7[8] = {"", "8c", "\357H", "8t", "cP", "4t", "\357t", "c6"};
static const char* stdr[4] = {"ночь", "yтро", "день", "вечер"};
static const char* swnr[8] = {"северный", "северо-восточный", "восточный", "юго-восточный", "южный", "юго-западный", "западный", "северо-западный"};
static const char* sprcr[6] = {"дождь", "ливень", "снегопад", "сильный снегопад", "гроза", "без осадков"};
static const char* sdnr[7] = {"воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота"};
static const char* sdne[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
static const char* smnr[12] = {"января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
static const char* smne[12] = {"January", "February ", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

#endif
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
static char oldDigit[q_dig];                       // убегающая цифра

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
static ESP32RGBmatrixPanel * m3264;
#endif

//---------------------------------------------------------------------------ILI9341
#include "Adafruit_ILI9341.h"

static Adafruit_ILI9341 * tft;
