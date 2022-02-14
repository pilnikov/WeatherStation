// Sysf.h

#ifndef Sysfn_h
#define Sysfn_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

#include <Wire.h> // must be included here so that Arduino library object file references work

typedef struct
{
	bool
		am2320_present = false,
		si7021_present = false,
		bmp180_present = false,
		bmp280_present = false,
		bme280_present = false,
		ds3231_present = false,
		ds1307_present = false,
		lcd_present    = false,
		ht1633_present = false,
		bh1750_present = false;
	byte 
		lcd_addr,
		ht_addr,
		bm_addr;
} hw_data_t;

class SF 
{
  public:
    void i2c_scan_do();
    hw_data_t hw_present();
	
  private:
    uint8_t nDevices;  //Кол-во найденных устройств
    uint8_t addr[128]; //Адреса найденных устройств
  protected:
};
#endif  // Sysf_h


