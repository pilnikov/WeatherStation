// Snr.h

#ifndef Snr_h
#define Snr_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Adafruit_Sensor.h>
#include <Adafruit_Si7021.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_BMP280.h>
#include <AM2320.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#if defined(__xtensa__) ||  defined(__AVR__) || CONFIG_IDF_TARGET_ESP32C3
#include <DHT_U.h>
#endif

#include "OneWireNg_CurrentPlatform.h"
#include "drivers/DSTherm.h"
#include "utils/Placeholder.h"

#define PARASITE_POWER  false


#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#define SEALEVELPRESSURE_HPA (1013.25)

typedef struct 
{
  int8_t
	t1 =  99, //Температура. Канал 1
    t2 =  99, //Температура. Канал 2
    t3 =  99; //Температура. Канал 3
  uint8_t
	h1 =   0, //Влажность. Канал 1
    h2 =   0, //Влажность. Канал 2
    h3 =   0; //Влажность. Канал 3
  uint16_t  
     p =   0, //Давление
     f =   0; //Освещенность  
} snr_data_t;

typedef struct 
{
  uint8_t
  type_snr1 = 0, // Тип датчика канал 1: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  type_snr2 = 0, // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  type_snr3 = 0, // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  type_snrp = 0; // Тип датчика давления 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 3 - Взять с ES2,                                                8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза 
} snr_cfg_t;


class SNR
{
public:

	void dht_preset(uint8_t, uint8_t);
	snr_cfg_t init(snr_cfg_t);
	snr_data_t read_snr(snr_cfg_t, uint8_t, snr_data_t, snr_data_t, snr_data_t, wf_data_t);

private:
	//----------------------------------------------------------dht
#if defined(__xtensa__) ||  defined(__AVR__) || CONFIG_IDF_TARGET_ESP32C3
	DHT_Unified* dht;
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
	DHT* dht;
#endif
	uint32_t delayMS;

	//-------------------------------------------------------am2320
	AM2320 am23;

	//----------------------------------------------------------htu
	Adafruit_Si7021 si = Adafruit_Si7021();

	//----------------------------------------------------------bmp
	Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
	//----------------------------------------------------------bmp2
	Adafruit_BMP280 bmp2; // I2C

	//----------------------------------------------------------bme

	Adafruit_BME280 bme; // I2C

	float humi_read(uint8_t);
	float temp_read(uint8_t);
	float pres_read(uint8_t);
    void  ds_init(uint8_t);
    int   ds_read();

	Placeholder<OneWireNg_CurrentPlatform> _ow;

	bool printId(const OneWireNg::Id& id);

	int printScratchpad(const DSTherm::Scratchpad& scrpd);

protected:
};
#endif //Snr_h