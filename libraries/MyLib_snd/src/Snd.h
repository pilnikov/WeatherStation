// Sound.h

#ifndef Snd_h
#define Snd_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP8266) || defined(ESP32) 
#include <pgmspace.h>
#endif
#if defined _AVR_
#include <avr/pgmspace.h>
#endif

#include "TN.h"

#define OCTAVE_OFFSET 0
#define isdigit(n) (n >= '0' && n <= '9')

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

class Synt 
{
  public:
    void play(uint8_t, uint8_t,  uint8_t);
    void beep(uint8_t);
  private:
    void soundNote(uint8_t note, uint16_t dur, uint8_t out);  
  protected:
};

extern Synt muzic;
#endif  // Snd_h


