/**********************************************************************
 *
 * This is a quick library for the Holtek HT16K33 LED driver + keyscan
 * IC. It’s not functionally exhaustive by any stretch, but it should
 * at least work reasonably well.
 *
 * @TODO: build functionality to read key data from the chip.
 *
 **********************************************************************/
#include <Wire.h>
#include "myHT16K33.h"

/**
 * Utility function to flip a 16-bit integer. There may be better ways of doing this—let me know!
 */

// Constructor
void HT16K33::init(uint8_t addr)
{
 
  // set the I2C address
  _i2c_addr = addr;
  
  // assign + zero some buffer data
  _buffer = (uint16_t*)calloc(8, sizeof(uint16_t));
  
  // start everything
  //Wire.begin();
  Wire.beginTransmission(_i2c_addr);
  Wire.write(0x21); // turn it on
  Wire.endTransmission();
  
  // set blink off + brightness all the way up
  setBlink(HT16K33_BLINK_OFF);
  setBrightness(15);
  
  // write the matrix, just in case
  write();
}

/**
 * Sets the brightness of the display.
 */
void HT16K33::setBrightness(uint8_t brightness)
{
  // constrain the brightness to a 4-bit number (0–15)
  brightness = brightness & 0x0F;
  
  // send the command
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_DIMMING | brightness);
  Wire.endTransmission();
}

/**
 * Set the blink rate.
 */
void HT16K33::setBlink(uint8_t blink)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_SETUP | HT16K33_DISPLAY_ON | blink);
  Wire.endTransmission();
}

/**
 * Clears the display buffer. Note that this doesn’t clear the display—you’ll need to call write() to do this.
 */
void HT16K33::clear(void)
{  
  for (uint8_t i = 0; i < 8; i++)
  {
    _buffer[i] = 0;
  }  
}

/**
 * Sets the value of an entire row.
 */
void HT16K33::setRow(uint8_t row, uint16_t value)
{
  // bound check the row
  row = row & 0x07;
  
  // write it
  _buffer[row] = value;
}

/**
 * Gets the value of an entire row.
 */
uint16_t HT16K33::getRow(uint8_t row)
{
	return _buffer[row];
}


/**
 * Write the RAM buffer to the matrix.
 */
void HT16K33::write(void)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_RAM);
  
  for (uint8_t row = 0; row < 8; row++)
  {
    Wire.write(_buffer[row] & 0xFF); // first byte
    Wire.write(_buffer[row] >> 8); // second byte
  }
  Wire.endTransmission();
}

