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
#elif defined(ESP8266)
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
    String Serial_Read(); 
    byte   roll_seg (byte); //переворот сегмента
    byte   mir_seg (byte); //адаптация для мах7219
    String lcd_rus(String);
    String utf8rus(String);
    String formatBytes(size_t);
    ram_data_t i2c_scan(conf_data_t);

  private:
  protected:
};

extern SF Sfunc;

#endif  // Sysf_h


