#include "Sysfn.h"

void SF::i2c_scan_do()
{
  byte       error, address;
 
   //  DBG_OUT_PORT.println(F("Scanning..."));

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      addr[nDevices] = address;
      //DBG_OUT_PORT.println(addr[nDevices]);
      nDevices++;
    }
    //else if (error == 4) DBG_OUT_PORT.println("Unknow error at address 0x");
  }
  if (nDevices == 0) DBG_OUT_PORT.println(F("I2C devices not found\n"));
  else 
  {
	  DBG_OUT_PORT.print(nDevices); DBG_OUT_PORT.println(F(" devices found\n"));
  }
}

hw_data_t SF::hw_present()
{

 i2c_scan_do();
 
 hw_data_t data;
 
 if (nDevices != 0) 
  {
    for (int i = 0; i < nDevices; i++ )
    {
     switch (addr[i])
      {
      case 0x40:
        data.si7021_present  = true ; //SI7021 is present
        DBG_OUT_PORT.println(F("SI7021 found!\n"));
      break;
      case 0xB8:
        data.am2320_present = true ; //AM2320 is present
        DBG_OUT_PORT.println(F("AM2320 found!\n"));
      break;
      case  0x76:
        data.bmp180_present  = true ; //BMP180 is present
		data.bmp280_present  = true ; //BMP280 is present
        data.bme280_present  = true ; //BME280 is present
		data.bm_addr = 0x76;
        DBG_OUT_PORT.println(F("BM P/E 1/2 80 with address 0x76 found!\n"));
      break;
      case  0x77:
        data.bmp180_present  = true ; //BMP280 is present
        data.bmp280_present  = true ; //BME280 is present
        data.bme280_present  = true ; //BMP180 is present
		data.bm_addr = 0x77;
        DBG_OUT_PORT.println(F("BM P/E 1/2 80 with address 0x77 found!\n"));
      break;
      case  0x3F:
        data.lcd_present = true; //LCD is present
        DBG_OUT_PORT.print(F("I2C LCD with address 0x3F found! \n"));
		data.lcd_addr = 0x3F;
      break;
      case  0x27:
        data.lcd_present = true; //LCD is present
        DBG_OUT_PORT.print(F("I2C LCD with address 0x27 found! \n"));
		data.lcd_addr = 0x27;
      break;
      case  0x70:
        data.ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println(F("HT1633 with address 0x70 found! \n"));
		data.ht_addr = 0x70;
      break;
      case  0x71:
        data.ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println(F("HT1633 with address 0x71 found! \n"));
		data.ht_addr = 0x71;
      break;
      case  0x72:
        data.ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println(F("HT1633 with address 0x72 found! \n"));
		data.ht_addr = 0x72;
      break;
      case  0x73:
        data.ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println(F("HT1633 with address 0x73 found! \n"));
		data.ht_addr = 0x73;
      break;
      case  0x74:
        data.ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println(F("HT1633 with address 0x74 found! \n"));
		data.ht_addr = 0x74;
      break;
      case  0x75:
        data.ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println(F("HT1633 with address 0x75 found! \n"));
		data.ht_addr = 0x75;
      break;
      case  0x68:
        data.ds3231_present  = true ; //DS3231 is present
        data.ds1307_present  = true ; //DS1307 is present
        DBG_OUT_PORT.println(F("DS3231/DS1307 found!\n"));
      break;
      case  0x23:
        data.bh1750_present    = true; //BH1750 is present
        DBG_OUT_PORT.println(F("BH1750 found!\n"));
      break;
      }
    }
  }
  return data;
}

