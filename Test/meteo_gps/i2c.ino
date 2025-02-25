#include <Wire.h>

byte error, address, addr[5], nDevices;

void i2c_setup() {
  Wire.begin();
}

void i2c_scan_do()
{
  byte error, address;

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      addr[nDevices] = address;
      nDevices++;
    }
    else if (error == 4) Serial.println("Unknow error at address 0x");
  }
  if (nDevices == 0) Serial.println("I2C devices not found\n");
  else
  {
    Serial.print(nDevices); Serial.println(" devices found\n");
  }
}

void i2c_loop()
{
  i2c_scan_do();

  if (nDevices != 0)
  {
    for (int i = 0; i < nDevices; i++ )
    {
      switch (addr[i])
      {
        case  0x23:
          bh1750_present = true; //BH1750 is present
          Serial.println("BH1750 found!\n");
          break;
        case 0x40:
          si7021_present = true; //SI7021 is present
          Serial.println("SI7021 found!\n");
          break;
        case  0x68:
          ds3231_present = true; //DS3231 is present
          Serial.println("DS3231 found!\n");
          break;
        case 0xB8:
          am2320_present = true; //AM2320 is present
          Serial.println("AM2320 found!\n");
          break;
        case  0x76:
          bmp280_present = true ; //BMP280 is present
          bme280_present = true ; //BME280 is present
          Serial.println("BM P/E 280 found!\n");
          break;
        case  0x77:
          bmp180_present = true ; //BMP180 is present
          Serial.println("BMP180/085 found!\n");
          break;
      }
    }
  }
}
