/**
   \file HT1621.cpp
   \brief Implementation of a class for dealing with the Holtek HT1621 chip.
   \author Enrico Formenti
   \date 31 january 2015
   \version 1.0
   \copyright BSD license, check the License page on the blog for more information. All this text must be
    included in any redistribution.
    <br><br>
    See macduino.blogspot.com for more details.

*/

#include "HT1621.h"

void HT1621::begin()
{
  pinMode(_DATA_pin, OUTPUT);
  pinMode(_RW_pin, OUTPUT);
  pinMode(_CS_pin, OUTPUT);

  digitalWrite(_CS_pin, HIGH);
  digitalWrite(_RW_pin, HIGH);
  digitalWrite(_DATA_pin, HIGH);

  for (register uint8_t i = 0; i < 16; i++)  ram[i] = 0;
}

// OCIO !!!
// nell'esempio dopo ogni write viene dato un delay
// di 20 microsecondi...
void HT1621::writeBits(uint8_t data, uint8_t cnt)
{
  for (register uint8_t i = 0; i < cnt; i++, data <<= 1)
  {
    pinMode(_DATA_pin, OUTPUT);

    digitalWrite(_RW_pin, LOW);
    delayMicroseconds(20);

    switch (cnt)
    {
      case 4:
        digitalWrite(_DATA_pin, data & 0x8 ? HIGH : LOW);
        break;
      case 5:
        digitalWrite(_DATA_pin, data & 0x10 ? HIGH : LOW);
        break;
      case 8:
        digitalWrite(_DATA_pin, data & 0x80 ? HIGH : LOW);
        break;
    }
    delayMicroseconds(20);

    digitalWrite(_RW_pin, HIGH);
    delayMicroseconds(20);
  }
}

#ifdef __HT1621_READ

uint8_t HT1621::readBits(uint8_t cnt)
{
  uint8_t data, i, state;

  pinMode(_DATA_pin, INPUT);

  for (i = 0, data = 0; i < cnt; data <<= 1, i++) {
    digitalWrite(_RW_pin, LOW);
    delayMicroseconds(20);
    data |= (digitalRead(_DATA_pin) == HIGH);
    digitalWrite(_RW_pin, HIGH);
    delayMicroseconds(20);
  }

  return data;
}

#endif

void HT1621::sendCommand(uint8_t cmd)
{
  TAKE_CS();

  HT1621::writeBits(COMMAND_MODE, 4);

  HT1621::writeBits(cmd, 8);

  RELEASE_CS();
}

void HT1621::write(uint8_t address, uint8_t data)
{
  TAKE_CS();

  HT1621::writeBits(WRITE_MODE, 4);
  HT1621::writeBits(address, 5);
  HT1621::writeBits(data, 4);

#ifndef __HT1621_READ
  ram[address] = data;
#endif

  RELEASE_CS();
}

void HT1621::write_c(uint8_t address, uint8_t data, uint8_t cnt)
{
  TAKE_CS();

  HT1621::writeBits(WRITE_MODE, 4);

  HT1621::writeBits(address, 5);

  for (register uint8_t i = 0; i < cnt; i++)
  {
    HT1621::writeBits(data, 4);
#ifndef __HT1621_READ
    ram[address + i] = data;
#endif
  }

  RELEASE_CS();
}

void HT1621::write(uint8_t address, uint8_t *data, uint8_t cnt)
{
  TAKE_CS();

  HT1621::writeBits(WRITE_MODE, 4);

  HT1621::writeBits(address, 5);

  for (register uint8_t i = 0; i < cnt; i++)
  {
    HT1621::writeBits(data[address + i], 4);
#ifndef __HT1621_READ
    ram[address + i] = data[address + i];
#endif
  }

  RELEASE_CS();
}

void HT1621::clear_all()
{
  HT1621::write_c(0, 0, 32);
}

void HT1621::write_all(uint8_t *data)
{
  HT1621::write(0, data, 32);
}

#ifdef __HT1621_READ

uint8_t HT1621::read(uint8_t address)
{
  TAKE_CS();

  writeBits(READ_MODE, 4);
  writeBits(address, 5);
  ram[address] = readBits(4);

  RELEASE_CS();

  return ram[address];
}

void HT1621::read(uint8_t address, uint8_t *data, uint8_t cnt)
{
  TAKE_CS();

  writeBits(READ_MODE, 4);
  writeBits(address, 5);
  for (register uint8_t i = 0; i < cnt; i++) ram[address + i] = readBits(4);

  RELEASE_CS();
}

#endif

