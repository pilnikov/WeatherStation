
#ifndef Dsp_h
#define Dsp_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>
#include <LiquidCrystal_I2C.h>
#include <Max72.h>
//#include <SHT1632_3.h>
#include <myHT16K33.h>
#include <myTM1637.h>
#include <HT1632.h>

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

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif



class MyDspHW
{
  public:
  void
	_init(byte vdrv_t, byte dsp_t, gpio_cfg_t gcf, byte ht_addr, byte lcd_addr, uint8_t &text_size),

	_write(uint8_t vdrv_t, uint8_t dsp_t, uint16_t br, uint8_t text_size, uint8_t color_up, uint8_t color_dwn, byte* screen),

	_off(byte),
	_on(byte);
 
  private:
//----------------------------------------------------------------------------------------------------------------------------------Global Var
  void
	tm1637_init(byte, byte),
	pcf8574_init(byte, uint8_t, uint8_t),
	m7219_init(byte, uint8_t),
	ht1621_init(uint8_t, uint8_t, uint8_t),
	ht1632_init(uint8_t, uint8_t, uint8_t, uint8_t),
	ht1633_init(byte),
	ili9341_init(),

#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
	a595_init(byte, byte&, uint8_t&),
	m3216_ramFormer(byte*, uint8_t, uint8_t),
#endif

	m7219_ramFormer(byte*),
	m7219_ramFormer2(byte*, uint8_t, uint8_t),
	ht1632_ramFormer(byte*, const uint8_t, const uint8_t),
	ht1633_ramFormer(byte*, uint8_t, uint8_t),
	ht1633_ramFormer2(byte*, uint8_t, uint8_t),
	roll_seg(byte&);  
  protected:
};

class MyDspBCF // Функции для управления яркостью (BrightControlFunctions)
{
  public:
   uint8_t 
	auto_br(uint16_t, main_cfg_t);       // Авто регулировка яркости с расчетом пределов
   uint16_t
	ft_read(bool, uint16_t, uint8_t);    // Селектор источника измерения освещенности
  
  private:
  protected:
};

#endif /*disp_h*/