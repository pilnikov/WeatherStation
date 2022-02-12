/******************************************************************
  A library for controling a set of 8x8 LEDs with a MAX7219 or
  MAX7221 displays.
  This is a plugin for Adafruit's core graphics library, providing
  basic graphics primitives (points, lines, circles, etc.).
  You need to download and install Adafruit_GFX to use this library.

  Adafruit invests time and resources providing this open
  source code, please support Adafruit and open-source hardware
  by purchasing products from Adafruit!

  Written by Mark Ruys.
  BSD license, check license.txt for more information.
  All text above must be included in any redistribution.
 ******************************************************************/

#include "Max72.h"
#include <SPI.h>

// The opcodes for the MAX7221 and MAX7219
#define OP_NOOP         0
#define OP_DIGIT0       1
#define OP_DIGIT1       2
#define OP_DIGIT2       3
#define OP_DIGIT3       4
#define OP_DIGIT4       5
#define OP_DIGIT5       6
#define OP_DIGIT6       7
#define OP_DIGIT7       8
#define OP_DECODEMODE   9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

Max72::Max72(byte csPin, byte hDisplays, byte vDisplays)
{

  Max72::SPI_CS = csPin;
  byte displays = hDisplays * vDisplays;
  Max72::hDisplays = hDisplays;
  Max72::bitmapSize = displays << 3;

  Max72::bitmap = (byte*)malloc(bitmapSize);

}
void Max72::begin(void) {

  pinMode(SPI_CS, OUTPUT);

  // Clear the screen
  fillScreen(0);

  // Make sure we are not in test mode
  spiTransfer(OP_DISPLAYTEST, 0);

  // We need the multiplexer to scan all segments
  spiTransfer(OP_SCANLIMIT, 7);

  // We don't want the multiplexer to decode segments for us
  spiTransfer(OP_DECODEMODE, 0);

  // Enable display
  shutdown(false);

  // Set the brightness to a medium value
  setIntensity(7);
}

void Max72::shutdown(boolean b)
{
  spiTransfer(OP_SHUTDOWN, b ? 0 : 1);
}

void Max72::setIntensity(byte intensity)
{
  spiTransfer(OP_INTENSITY, intensity);
}

void Max72::fillScreen(uint16_t color)
{
  memset(bitmap, color ? 0xff : 0, bitmapSize);
}


void Max72::write()
{
  // Send the bitmap buffer to the displays.
  for (byte row = OP_DIGIT7; row >= OP_DIGIT0; row--)
  {
    spiTransfer(row);
  }
}

void Max72::spiTransfer(byte opcode, byte data)
{
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  // If opcode > OP_DIGIT7, send the opcode and data to all displays.
  // If opcode <= OP_DIGIT7, display the column with data in our buffer for all displays.
  // We do not support (nor need) to use the OP_NOOP opcode.

  // Enable the line
  digitalWrite(SPI_CS, LOW);

  // Now shift out the data, two bytes per display. The first byte is the opcode,
  // the second byte the data.
  byte end = opcode - OP_DIGIT0;
  byte start = bitmapSize + end;
  do
  {
    start -= 8;
    SPI.transfer(opcode);
    SPI.transfer(opcode <= OP_DIGIT7 ? bitmap[start] : data);
  }
  while (start > end);

  // Latch the data onto the display(s)
  digitalWrite(SPI_CS, HIGH);

  SPI.endTransaction();
}

void Max72::setRam(byte *data, uint8_t size)
{
  // Set the bitmap buffer from the data.
  byte dataSize = size;
  if (dataSize > bitmapSize) dataSize = bitmapSize;
  memmove(bitmap, data, dataSize);
}
