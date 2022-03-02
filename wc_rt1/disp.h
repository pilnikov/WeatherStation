
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>
#include <Snr.h>
#include <myrtc.h>
#include <Fdsp.h>

#include <Adafruit_GFX.h>
#include <LiquidCrystal_I2C.h>
/*
  #include "..\lib\MyLib_M7219\Max72.h"
  #include "..\lib\MyLib_HT1632\SHT1632_3.h"
  #include "..\lib\MyLib_HT1633\myHT16K33.h"
  #include "..\lib\MyLib_TM1637\myTM1637.h"
*/

#include <Max72.h>
#include <SHT1632_3.h>
#include <myHT16K33.h>
#include <myTM1637.h>


#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Adafruit_Protomatter.h>
#endif


#if defined(__AVR_ATmega2560__)
#include <RGBmatrixPanel.h> // Hardware-specific library
#endif

//---------------------------------------------------------------------------HT1621
#include <HT1621.h>

//---------------------------------------------------------------------------ILI9341
#include "Adafruit_ILI9341.h"

//----------------------------------------------------------------------------------------------------------------------------------function prototype
void pr_str(uint8_t&, uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_time_data_t, rtc_alm_data_t, String, uint8_t, char*, bool, String);
void time_view(uint8_t, uint8_t);
uint8_t seg7_mode(uint8_t&, uint8_t, byte*, uint8_t, conf_data_t, snr_data_t, rtc_time_data_t,  rtc_alm_data_t, uint8_t, bool);

void pcf8574_init();
void lcd_time(char*, bool);
void m7219_init();
void m7219_ramFormer(byte*);
void m7219_ramFormer2(byte*, uint8_t, uint8_t);
void ht1621_init();
void ht1632_init();
void ht1632_ramFormer(byte*, const uint8_t, const uint8_t);
void tm1637_init();
void a595_init();
void m3216_ramFormer(byte*, uint8_t, uint8_t);
void m7adopt(byte*, uint8_t, uint8_t);
void ili9341_init();
void ili_time(void);
void ht1633_init(void);
void ht1633_ramFormer(byte*, uint8_t, uint8_t);
void ht1633_ramFormer2(byte*, uint8_t, uint8_t);
void drawWifiQuality();
int8_t getWifiQuality();

//----------------------------------------------------------------------------------------------------------------------------------define
#define new_max
//#define font4pt
//#define font5pt
