#include "Sysf2.h"

void SF::i2c_scan_do()
{
  byte       error, address;
 
   //  DBG_OUT_PORT.println("Scanning...");

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
    else if (error == 4) DBG_OUT_PORT.println("Unknow error at address 0x");
  }
  if (nDevices == 0) DBG_OUT_PORT.println("I2C devices not found\n");
  else 
  {
	  DBG_OUT_PORT.print(nDevices); DBG_OUT_PORT.println(" devices found\n");
  }
}

void SF::i2c_parse()
{

 i2c_scan_do();
 
 if (nDevices != 0) 
  {
    for (int i = 0; i < nDevices; i++ )
    {
     switch (addr[i])
      {
      case 0x40:
        si7021_present  = true ; //SI7021 is present
        DBG_OUT_PORT.println("SI7021 found!\n");
      break;
      case 0xB8:
        am2320_present = true ; //AM2320 is present
        DBG_OUT_PORT.println("AM2320 found!\n");
      break;
      case  0x77:
        bmp180_present  = true ; //BMP180 is present
        DBG_OUT_PORT.println("BMP180/085 found!\n");
      break;
      case  0x76:
        bmp280_present  = true ; //BMP280 is present
        bme280_present  = true ; //BME280 is present
        DBG_OUT_PORT.println("BM P/E 280 found!\n");
      break;
      case  0x3F:
        lcd_present = true; //LCD is present
        DBG_OUT_PORT.print("I2C LCD with address 0x3F found! \n");
		lcd_addr = 0x3F;
      break;
      case  0x27:
        lcd_present = true; //LCD is present
        DBG_OUT_PORT.print("I2C LCD with address 0x27 found! \n");
		lcd_addr = 0x27;
      break;
      case  0x70:
        ht1633_present  = true ; //HT1633 is present
        DBG_OUT_PORT.println("HT1633 with address 0x70 found! \n");
		ht_addr = 0x70;
      break;
      case  0x68:
        ds3231_present  = true ; //DS3231 is present
        ds1307_present  = true ; //DS1307 is present
        DBG_OUT_PORT.println("DS3231/DS1307 found!\n");
      break;
      case  0x23:
        bh1750_present    = true; //BH1750 is present
        DBG_OUT_PORT.println("BH1750 found!\n");
      break;
      }
    }
  }
}

ram_data_t SF::i2c_scan(conf_data_t c_data)
{
  // temporary actions -----------------------------------------------------------------------------------------------------
  ram_data_t r_data;

  i2c_parse();

  r_data.type_rtc       = c_data.type_rtc;
  r_data.type_disp      = c_data.type_disp;
  r_data.type_ext_snr   = c_data.type_ext_snr;
  r_data.type_int_snr   = c_data.type_int_snr;
  r_data.type_prs_snr   = c_data.type_prs_snr;
  r_data.bh1750_present = bh1750_present;
  r_data.lcd_addr       = lcd_addr; //address of LCD
  r_data.ht_addr        = ht_addr; //HT1633 addr

 // if not selected -> select auto
  /*
    if (si7021_present && r_data.type_int_snr != 5 && r_data.type_ext_snr != 5)
    {
     r_data.type_int_snr = 5; //si7021
    }
    if (am2320_present && r_data.type_int_snr != 6 && r_data.type_ext_snr != 6)
    {
     r_data.type_int_snr = 6; //am2320
    }
    if (bmp180_present && r_data.type_int_snr != 7 && r_data.type_ext_snr != 7)
    {
     r_data.type_int_snr = 7; //bmp180
    }
    if (bmp280_present && r_data.type_int_snr != 8 && r_data.type_ext_snr != 8
       && r_data.type_int_snr != 9 && r_data.type_ext_snr != 9)
    {
     r_data.type_int_snr = 8; //bm p/e280
    }
    if (lcd_present    && r_data.type_disp != 1)
    {
     r_data.type_disp = 1; //LCD I2C
    }
    if (ht1633_present && r_data.type_disp != 2)
    {
     r_data.type_disp = 2; //ht1633
    }
    if (ds3231_present && r_data.type_rtc != 1 && r_data.type_rtc != 3)
    {
     r_data.type_rtc = 1; //ds3231/1307
    }
  */
  // if selected -> deselect auto
  if (!ds3231_present && r_data.type_int_snr == 4)
  {
    r_data.type_int_snr = 0;
    DBG_OUT_PORT.println("DS3231 as a internal sensor is not found -> deselected");
  }
  if (!ds3231_present && r_data.type_ext_snr == 4)
  {
    r_data.type_ext_snr = 0;
    DBG_OUT_PORT.println("DS3231 as a internal sensor is not found -> deselected");
  }
  if (!si7021_present && r_data.type_int_snr == 5)
  {
    r_data.type_int_snr = 0;
    DBG_OUT_PORT.println("SI7021 as a internal sensor is not found -> deselected");
  }
  if (!si7021_present && r_data.type_ext_snr == 5)
  {
    r_data.type_ext_snr = 0;
    DBG_OUT_PORT.println("SI7021 as a external sensor is not found -> deselected");
  }
  /*
    if (!am2320_present && r_data.type_int_snr == 6)
    {
      r_data.type_int_snr = 0;
      DBG_OUT_PORT.println("AM2320 as a internal sensor is not found -> deselected");
    }
    if (!am2320_present && r_data.type_ext_snr == 6)
    {
      r_data.type_ext_snr = 0;
      DBG_OUT_PORT.println("AM2320 as a internal sensor is not found -> deselected");
    }
  */
  if (!bmp180_present && r_data.type_int_snr == 7)
  {
    r_data.type_int_snr = 0;
    DBG_OUT_PORT.println("BMP180 as a internal sensor is not found -> deselected");
  }
  if (!bmp180_present && r_data.type_ext_snr == 7)
  {
    r_data.type_ext_snr = 0;
    DBG_OUT_PORT.println("BMP180 as a external sensor is not found -> deselected");
  }
  if (!bmp180_present && r_data.type_prs_snr == 7)
  {
    r_data.type_prs_snr = 0;
    DBG_OUT_PORT.println("BMP180 as a pressure sensor is not found -> deselected");
  }
  if (!bmp280_present && r_data.type_int_snr == 8)
  {
    r_data.type_int_snr = 0;
    DBG_OUT_PORT.println("BMP280 as a internal sensor is not found -> deselected");
  }
  if (!bmp280_present && r_data.type_ext_snr == 8)
  {
    r_data.type_ext_snr = 0;
    DBG_OUT_PORT.println("BMP280 as a external sensor is not found -> deselected");
  }
  if (!bmp280_present && r_data.type_prs_snr == 8)
  {
    r_data.type_prs_snr = 0;
    DBG_OUT_PORT.println("BMP280 as a pressure sensor is not found -> deselected");
  }
  if (!bme280_present && r_data.type_int_snr == 9)
  {
    r_data.type_int_snr = 0;
    DBG_OUT_PORT.println("BME280 as a internal sensor is not found -> deselected");
  }
  if (!bme280_present && r_data.type_ext_snr == 9)
  {
    r_data.type_ext_snr = 0; //BME280 as a external sensor is not present
    DBG_OUT_PORT.println("BME280 as a external sensor is not found -> deselected");
  }
  if (!bme280_present && r_data.type_prs_snr == 9)
  {
    r_data.type_prs_snr = 0;
    DBG_OUT_PORT.println("BME280 as a pressure sensor is not found -> deselected");
  }
  if (!lcd_present    && r_data.type_disp == 1)
  {
    r_data.type_disp    = 0;
    DBG_OUT_PORT.println("LCD I2C is not found -> deselected");
  }
  if (!ht1633_present && r_data.type_disp == 2)
  {
    r_data.type_disp    = 0;
    DBG_OUT_PORT.println("HT1633 is not found -> deselected");
  }
  if (!ds3231_present && r_data.type_rtc  == 1)
  {
    r_data.type_rtc     = 0; //DS3231 is not present
    DBG_OUT_PORT.println("DS3231 is not found -> deselected");
  }
  if (!ds1307_present && r_data.type_rtc  == 3)
  {
    r_data.type_rtc     = 0; //DS1307 is not present
    DBG_OUT_PORT.println("DS1307 is not found -> deselected");
  }
  if (!bh1750_present)
  {
    r_data.bh1750_present = false;
    DBG_OUT_PORT.println("BH1750 is not found -> get light level from A0");
  }
  return r_data;
}
