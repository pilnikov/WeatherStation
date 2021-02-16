
#include <Adafruit_GFX.h>
#include <LiquidCrystal_I2C.h>
//#include <Adafruit_LEDBackpack.h>
//#include <LedControl.h>
#include <Max72xxPanel.h>
#include "HT16K33.h"
#include <TM1637.h>

#if defined(ESP32)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ESP32RGBmatrixPanel.h"
#include "font5x7.h"
#endif

void dow_sel(uint8_t);

String pr_str(uint8_t);

void pcf8574_init();
void lcd_time();
void m7219_init();
void m7219_ramFormer();
void m7219_time();
void ht1632_init();
void m1632_time();

void a595_init();
void m3264_time();

bool mov_str(uint8_t, uint8_t, String, uint8_t, int);
uint16_t auto_br(uint16_t, uint16_t*);

void bat (uint8_t);
void digit (uint8_t, uint8_t);
void mon_day (uint8_t, uint8_t);
void ala (uint8_t);
void matrix32x8_time();
void drawPartChar(int8_t, int8_t, unsigned char, uint8_t, uint8_t, int8_t);
//-----------------------------------------------------------------------------new
byte screen[64]; // display buffer

void seg7d4_time();
void seg7d6_time();
void seg7d8_time();


//----------------------------------------------------------------------------Common

//----------------------------------------------------------------------------7 SEG
bool blinkColon = false;

//----------------------------------------------------------------------------TM1637

TM1637* tm1637;

//----------------------------------------------------------------------------HT1633

HT16K33* ht1633;


//---------------------------------------------------------------------------LCD1602

#define countof(a) (sizeof(a) / sizeof(a[0]))

const uint8_t lcd_row = 2;
const uint8_t lcd_col = 16;

//LiquidCrystal_I2C lcd(lcd_adr, lcd_col, lcd_row); // set the LCD address to 0x27 for a 16 chars and 2 line display

uint8_t  num_st = 1, max_st = 3; //номер и макс кол-во прокручиваемых строк

LiquidCrystal_I2C * lcd;

//---------------------------------------------------------------------------MAX7219 4 x 8 x 8 Matrix Display

#define new_max
//#define font4pt
//#define font5pt

uint8_t numberOfHorizontalDisplays = 4;
uint8_t numberOfVerticalDisplays = 1;

Max72xxPanel *m7219;

uint8_t colon; //номер разделителя
const uint8_t num = 6;  // количество цифр
uint8_t digHt;
uint8_t TextSize = 1;

#ifdef new_max 
const uint8_t digPos_x_[num] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
#else 
const uint8_t digPos_x_[num] = {0, 4, 11, 16, 23, 28};
#endif
static char digoldig[num];                        // убегающая цифра
static uint8_t digPos_y_[num];                    // позиция цифры по оси у

uint8_t spacer = 1; // Промежуток между символами (кол-во точек)
uint8_t point = 0;
int cur_sym_pos[2] = {0, 0};
unsigned long scroll_time[lcd_row] = {millis(), millis()};
bool end_run_st, end_run_st_buf;
String st1 = String();

//---------------------------------------------------------------------------HT1621
//буфер экрана
uint8_t buf[32];

//массив батарейка
static const uint8_t batt[6] = {0x02, 0x82, 0x92, 0xD2, 0xF2, 0xF3};

//массив больших цифр
static const uint8_t dig1[10] = {0xF5, 0x60, 0xB6, 0xF2, 0x63, 0xD3, 0xD7, 0x70, 0xF7, 0xF3};

//массив маленьких цифр
static const uint8_t dig2[10] = {0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC};

#include <HT1621.h>

// refere to Macduino website for pin connections and their meaning
HT1621 * ht21;

//---------------------------------------------------------------------------HT1632
#include <Sure_HT1632.h>

HT1632Class * m1632;

//---------------------------------------------------------------------------ILI9341
#include "Adafruit_ILI9341.h"

Adafruit_ILI9341 * tft;
