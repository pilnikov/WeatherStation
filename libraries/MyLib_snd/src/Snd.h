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

const uint16_t notes[] =
{ 0,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};


class Synt 
{
  public:
    void play(const char *, uint8_t,  bool);
    void beep(uint8_t);
  private:
    void soundNote(uint8_t note, uint16_t dur, uint8_t out);  
  protected:
};

extern Synt muzic;
#endif  // Snd_h


