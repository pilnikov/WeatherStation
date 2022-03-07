
#ifndef Fdisp_h
#define Fdisp_h

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

//----------------------------------------------------------------------------------------------------------------------------------define
#define new_max
//#define font4pt
//#define font5pt


class MyDsp
{
  public:
  void
	pr_str(uint8_t&, uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_time_data_t, rtc_alm_data_t, String, uint8_t, char*, bool, String),
	disp_init(byte type_vdrv, byte type_disp, byte gpio_uart, byte gpio_dio, byte gpio_clk, byte gpio_dcs, byte gpio_dwr, byte ht_addr, byte lcd_addr, byte *screen, uint8_t &text_size, bool rus_lng),
	pcf8574_init(byte, uint8_t, uint8_t, bool),
	lcd_time(byte*, bool),
	m7219_init(byte, byte, byte*),
	m7219_ramFormer(byte*),
	m7219_ramFormer2(byte*, uint8_t, uint8_t),
	ht1621_init(byte, byte, byte, byte*),
	ht1632_init(byte, byte),
	ht1632_ramFormer(byte*, const uint8_t, const uint8_t),
	tm1637_init(byte, byte),
	a595_init(byte, byte&, uint8_t&),
	m3216_ramFormer(byte*, uint8_t, uint8_t),
	m7adopt(byte*, uint8_t, uint8_t),
	ili9341_init(),
	ili_time(bool, bool),
	ht1633_init(byte),
	ht1633_ramFormer(byte*, uint8_t, uint8_t),
	ht1633_ramFormer2(byte*, uint8_t, uint8_t),
	drawWifiQuality(),
	write_dsp(bool, uint8_t, uint8_t, uint16_t, bool, byte*, uint8_t, uint8_t, uint8_t),
	scroll_disp(uint8_t, byte*),
	scroll_start(bool, bool, uint8_t, uint8_t, bool, bool&, char*, byte*),

	display_off(byte),
	display_on(byte),

	runing_string_start(uint8_t&, uint8_t, conf_data_t, snr_data_t, wf_data_t, wf_data_t, rtc_time_data_t, rtc_alm_data_t, String, uint16_t, bool, String, uint8_t&, bool&, char*, byte*);
  bool 
	time_view(bool, bool, bool, rtc_time_data_t, rtc_alm_data_t, byte*, conf_data_t, snr_data_t, uint16_t);

  uint8_t 
    seg7_mode(uint8_t&, uint8_t, byte*, uint8_t, conf_data_t, snr_data_t, rtc_time_data_t,  rtc_alm_data_t, bool, uint16_t);
  int8_t 
    getWifiQuality();

  private:
//----------------------------------------------------------------------------------------------------------------------------------Global Var
	uint8_t       q_dig = 6;                         // количество цифр на дисплее
	bool          old[6];
	const uint8_t dposx[6] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
	unsigned char change[6];                         // убегающая цифра
	uint16_t      buffud[64];
  
  protected:
};
#endif /*disp_h*/