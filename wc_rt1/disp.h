
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>

#include <myrtc.h>
#include <Snr.h>
#include <Fdsp.h>

#include <Adafruit_GFX.h>
#include <LiquidCrystal_I2C.h>

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
#include <Adafruit_ILI9341.h>

//----------------------------------------------------------------------------------------------------------------------------------function prototype
void pr_str(uint8_t&, uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_time_data_t, rtc_alm_data_t, String, uint8_t, char*, bool, String);
bool time_view(bool, bool, bool, rtc_time_data_t, rtc_alm_data_t, byte*, conf_data_t, snr_data_t,
               unsigned char*, const uint8_t*, bool*, uint16_t*, const uint8_t,  uint16_t);
uint8_t seg7_mode(uint8_t&, uint8_t, byte*, uint8_t, conf_data_t, snr_data_t, rtc_time_data_t,  rtc_alm_data_t, uint8_t, bool);

void disp_init(byte type_vdrv, byte type_disp, byte gpio_uart, byte gpio_dio, byte gpio_clk, byte gpio_dcs, byte gpio_dwr, byte ht_addr, byte lcd_addr, byte *screen, uint8_t &text_size, bool rus_lng);
void pcf8574_init(byte, uint8_t, uint8_t, bool);
void lcd_time(byte*, bool);
void m7219_init(byte, byte, byte*);
void m7219_ramFormer(byte*);
void m7219_ramFormer2(byte*, uint8_t, uint8_t);
void ht1621_init(byte, byte, byte, byte*);
void ht1632_init(byte, byte);
void ht1632_ramFormer(byte*, const uint8_t, const uint8_t);
void tm1637_init(byte, byte);
void a595_init(byte, byte&, uint8_t&);
void m3216_ramFormer(byte*, uint8_t, uint8_t);
void m7adopt(byte*, uint8_t, uint8_t);
void ili9341_init();
void ili_time(bool, bool);
void ht1633_init(byte);
void ht1633_ramFormer(byte*, uint8_t, uint8_t);
void ht1633_ramFormer2(byte*, uint8_t, uint8_t);
void drawWifiQuality();
int8_t getWifiQuality();
void write_dsp(bool, uint8_t, uint8_t, uint16_t, bool, byte*);
void display_off(byte);
void display_on(byte);

//----------------------------------------------------------------------------------------------------------------------------------define
#define new_max
//#define font4pt
//#define font5pt
