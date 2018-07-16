// Snr.h

#ifndef Snr_h
#define Snr_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Adafruit_Sensor.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_BMP280.h>
#include <AM2320.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>
#include <DHT.h>
#if defined(ESP8266) || defined(ESP32) ||  defined(__AVR__)
 #include <DHT_U.h>
#endif

#include <Udt.h>

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#define SEALEVELPRESSURE_HPA (1013.25)
   
class SNR
{
  public :

   BH1750 lightMeter;

   void dht_preset(uint8_t, uint8_t);
   ram_data_t init(ram_data_t);
   uint16_t ft_read(bool);
   snr_data_t read_snr(uint8_t, uint8_t, uint8_t,  uint8_t, snr_data_t, snr_data_t, wf_data_t);
 
  private :
//----------------------------------------------------------dht
#if defined(ESP8266) || defined(ESP32) ||  defined(__AVR__)
   DHT_Unified * dht;
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
   DHT * dht;
#endif
   uint32_t delayMS;

   //-------------------------------------------------------am2320
   AM2320 am23;
   
   //----------------------------------------------------------htu
   Adafruit_HTU21DF sht = Adafruit_HTU21DF();

   //----------------------------------------------------------bmp
   Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
   //----------------------------------------------------------bmp2
   Adafruit_BMP280 bmp2; // I2C
   
   //----------------------------------------------------------bme
   
   Adafruit_BME280 bme; // I2C

   float am_read_h();
   float am_read_t();
   float sht_read_h();
   float sht_read_t();
   float bmp180_read_t();
   float bmp180_read_p();
   float bmp280_read_t();
   float bmp280_read_p();
   float bme280_read_h();
   float bme280_read_t();
   float bme280_read_p();
   void  dht_init();
   float dht_read_h();
   float dht_read_t();
   void  dispsnrDetails();

  protected :

};

//-------------------------------------------------------Sensor

/*
  Типы датчиков температуры, влажности, давления
  0 - нет
  1 - ThingSpeak
  2 - Внешний сервер
  3 - dht22
  4 - Встроеный в DS3231 датчик температуры
  5 - si7021
  6 - am2320
  7 - bmp180
  8 - bmp280
  9 - bme280

  bh1750 - датчик освещенности
*/


#endif //Snr_h