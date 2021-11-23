// Sysf.h

#ifndef Sysf_h
#define Sysf_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>
#include <Wire.h>

#ifdef __AVR__
  #include <avr/pgmspace.h>
#elif defined(__xtensa__)
  #include <pgmspace.h>
#else
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

class SF 
{
  public:
    void i2c_scan_do();
	void i2c_parse();
    String formatBytes(unsigned long);
    ram_data_t i2c_scan(conf_data_t);
	
  private:
	bool am2320_present = false;
	bool si7021_present = false;
	bool bmp180_present = false;
	bool bmp280_present = false;
	bool bme280_present = false;
	bool ds3231_present = false;
	bool ds1307_present = false;
	bool lcd_present    = false;
	bool ht1633_present = false;
    bool bh1750_present = false;
    byte lcd_addr;
	byte ht_addr;
	byte bm_addr;
    uint8_t nDevices;  //Кол-во найденных устройств
    uint8_t addr[128]; //Адреса найденных устройств
  protected:
};

extern SF Sfunc;

#endif  // Sysf_h


