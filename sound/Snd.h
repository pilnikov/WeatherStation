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
#elif defined (__AVR__)
#include <avr/pgmspace.h>
#endif


#define OCTAVE_OFFSET 0
#define isdigit(n) (n >= '0' && n <= '9')

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif


class Synt
{
  public:
    void play(char*, uint8_t, bool, bool);
    void beep(uint8_t, bool);
  private:
    void soundNote(uint8_t note, uint16_t dur, uint8_t out, bool pola);
    char *p;
    unsigned long dela = millis();
    uint8_t default_dur = 4, default_oct = 6;
    int bpm = 63, num;
    long wholenote, duration;
    byte note, scale, ddu;
    long wn;
    bool set_up = false, is_played = false;
  protected:
};

extern Synt muzic;
#endif  // Snd_h
