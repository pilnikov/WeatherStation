// Sound.h

#ifndef Snd_h
#define Snd_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(__xtensa__) 
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
    void play(const char *, uint8_t, bool&, bool);
    void beep(uint8_t, bool);
  private:
    void soundNote(uint8_t note, uint16_t dur, uint8_t out, bool pola);  
    unsigned long dela[2] = {millis(), millis()};
  protected:
};

extern Synt muzic;
#endif  // Snd_h