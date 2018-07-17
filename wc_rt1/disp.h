
#include <LiquidCrystal_I2C.h>
#include <Adafruit_LEDBackpack.h>
#include <LedControl.h>
#include <Max72xxPanel.h>
#include <Adafruit_GFX.h>
#include <TM1637Display.h>

void sevenseg_init();
void inline dow_sel(uint8_t);
uint8_t mod_sel();
void inline ss_prep();
void sevenseg();

void lcd_init();
String pr_str(uint8_t);
void Time_LCD();
void matrix_init();
void matrix_time();
void m1632_init();
void m1632_time();
void mov_str(uint8_t, uint8_t, String, uint8_t, unsigned long);
uint16_t auto_br(uint16_t, uint16_t*);
void procherk (uint8_t, uint8_t);

void bat (uint8_t);
void digit (uint8_t, uint8_t);
void mon_day (uint8_t, uint8_t);
void ala (uint8_t);


//----------------------------------------------------------------------------Common

//----------------------------------------------------------------------------7 SEG
static const uint8_t numbertable[] = {
  0x3F, /* 0 *  0 */
  0x06, /* 1 *  1 */
  0x5B, /* 2 *  2 */
  0x4F, /* 3 *  3 */
  0x66, /* 4 *  4 */
  0x6D, /* 5 *  5 */
  0x7D, /* 6 *  6 */
  0x07, /* 7 *  7 */
  0x7F, /* 8 *  8 */
  0x6F, /* 9 *  9 */
  0x77, /* a * 10 */
  0x7C, /* b * 11 */
  0x39, /* C * 12 */
  0x5E, /* d * 13 */
  0x79, /* E * 14 */
  0x71, /* F * 15 */
  0x76, /* H * 16 */
  0x38, /* L * 17 */
  0x73, /* P * 18 */
  0x78, /* t * 19 */
  0x6E, /* У * 20 */
  0x31, /* Г * 21 */
  0x37, /* П * 22 */
  0x40, /* - * 23 */
  0x0,  /*   * 24 */
};

static byte dig[8];                               // разряды индикаторов
static char d[6] = {0, 0, 0, 0, 0, 0};
bool blinkColon = false;

//----------------------------------------------------------------------------TM1637

TM1637Display *tm1637;

//----------------------------------------------------------------------------HT1633

Adafruit_7segment *ht33;


//---------------------------------------------------------------------------MAX7219 7 seg

LedControl * max7;


//---------------------------------------------------------------------------LCD1602

#define countof(a) (sizeof(a) / sizeof(a[0]))

const uint8_t lcd_row = 2;
const uint8_t lcd_col = 16;

//LiquidCrystal_I2C lcd(lcd_adr, lcd_col, lcd_row); // set the LCD address to 0x27 for a 16 chars and 2 line display

uint8_t  num_st = 1, max_st = 3; //номер и макс кол-во прокручевыемых строк

LiquidCrystal_I2C * lcd;

//---------------------------------------------------------------------------MAX7219 4 x 8 x 8 Matrix Display

#define new_max
//#define font4pt
//#define font5pt

uint8_t numberOfHorizontalDisplays = 4;
uint8_t numberOfVerticalDisplays = 1;

Max72xxPanel * m7219;

uint8_t colon; //номер разделителя
const uint8_t num = 6;  // количество цифр
uint8_t digHt;
#ifdef new_max 
const uint8_t digPos[num] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране
#else 
const uint8_t digPos[num] = {0, 4, 11, 16, 23, 28};
#endif
static char digoldig[num];                          // убегающая цифра
static uint8_t digtrans[num];                       // позиция цифры по оси у

uint8_t spacer = 1; // Промежуток между символами (кол-во точек)
uint8_t point = 0;
int cur_sym_pos[2] = {0, 0};
unsigned long lcd_scroll_time[lcd_row] = {millis(), millis()};
bool str_run;
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

