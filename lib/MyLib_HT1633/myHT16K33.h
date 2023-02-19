#ifndef myHT16K33_h
  #define myHT16K33_h
  
  // include appropriate version of Arduino code
  #if (ARDUINO >= 100)
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  
  // include Wire for I2C comms  
  #include <Wire.h>
  
  // different commands
  #define HT16K33_CMD_RAM     0x00
  #define HT16K33_CMD_KEYS    0x40
  #define HT16K33_CMD_SETUP   0x80
  #define HT16K33_CMD_ROWINT  0xA0
  #define HT16K33_CMD_DIMMING 0xE0
  
  // other options
  #define HT16K33_DISPLAY_OFF 0x00
  #define HT16K33_DISPLAY_ON  0x01
  #define HT16K33_BLINK_OFF   0x00

  
  // actual class
  class HT16K33
  {
    public:
      void init(uint8_t addr);
      
      // brightness control
      void setBrightness(uint8_t brightness);
      void setBlink(uint8_t blink);
      
      
      // orientation
     
      // buffer stuff
      void clear(void);
      void setRow(uint8_t row, uint16_t value);
      uint16_t getRow(uint8_t row);
      
      // read/write
      void write(void);
      
    private:
      uint16_t *_buffer;
      uint8_t  _i2c_addr;
      bool     _reversed;
      
      void writeRow(uint8_t row);
      
  };
  
#endif // #HT16K33
    