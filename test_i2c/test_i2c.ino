#include <Wire.h>

#if defined(__xtensa__)
static const int         SDA_PIN  PROGMEM =  5;  // (D2)
static const int         SCL_PIN  PROGMEM =  4;  // (D1)
#endif


byte error, address;
int nDevices;  //Кол-во найденных устройств

void setup()
{
#if defined(__xtensa__)
  Wire.begin(SDA_PIN, SCL_PIN);
#elif defined (__AVR__)
  Wire.begin();
#endif



  Serial.begin(9600);
  Serial.println(F("\nI2C Scanner"));
}

void loop() {
  int nDevices;
  byte err, addr;

  Serial.println("Start scan I2C bus...");
  nDevices = 0;
  Serial.print(F("   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"));
  for (addr = 0; addr <= 127; addr++ )
  {
    if ((addr % 0x10) == 0)
    {
      Serial.println();
      if (addr < 16)
        Serial.print('0');
      Serial.print(addr, 16);
      Serial.print(" ");
      if (addr == 0)
      {
        addr++;
        Serial.print("-- ");
      }
    }
    Wire.beginTransmission(addr);
    err = Wire.endTransmission();
    if (err == 0)
    {
      if (addr < 16)
        Serial.print("0");
      Serial.print(addr, HEX);
      nDevices++;
    }
    else
    {
      Serial.print("--");
    }
    Serial.print(" ");
    delay(1);
  }
  Serial.println();
  if (nDevices == 0)
    Serial.print(F("No I2C devices found\n"));
  else
  {
    Serial.print(nDevices);
    Serial.print(F(" I2C devices found\n"));
    Serial.println("done\n");
  }
  /*
      byte ack = 5;

      Wire.beginTransmission(0x23);
      Wire.write(0x11);
      ack = Wire.endTransmission();

      // Wait a few moments to wake up
      delay(10);
      Serial.print("ack - "); Serial.println(ack);

      Wire.beginTransmission(0x1a);
      Wire.write(0x123); // reset register pointer
      Wire.endTransmission();
      uint8_t rv = Wire.requestFrom(0x1a, 0x41);
      Serial.print("41 =");
      Serial.println(rv);
      rv = Wire.requestFrom(0x1a, 0x14);
      Serial.print("14 =");
      Serial.println(rv);
      rv = Wire.requestFrom(0x1a, 0x15);
      Serial.print("15 =");
      Serial.println(rv);
      rv = Wire.requestFrom(0x1a, 0x16);
      Serial.print("16 =");
      Serial.println(rv);

      Wire.beginTransmission(0x1a);
      Wire.write(0x16); // reset register pointer
      Wire.write(0x8); // reset register pointer
      Wire.endTransmission();

      rv = Wire.requestFrom(0x1a, 0x16);
      Serial.print("16_2 =");
      Serial.println(rv);

  */
  delay(5000);  // Ждем 5 сек. до следующего сканирования
}
