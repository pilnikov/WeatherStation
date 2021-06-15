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
#define cp(a) (pgm_read_byte(a))


#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif


class Synt
{
  public:
#if defined(ESP8266)
    bool play(const void*, uint8_t, bool, bool);
#elif defined (__AVR__) || defined (ARDUINO_ARCH_ESP32)
    bool play(uint16_t, uint8_t, bool, bool);
#endif
    void beep(uint8_t, bool);
  private:
   void soundNote(uint8_t note, uint16_t dur, uint8_t out, bool pola);

#if defined(ESP8266)
	const void* p;
#elif defined (__AVR__) || defined (ARDUINO_ARCH_ESP32)
	uint16_t p;
#endif
	unsigned long dela = 0;
	long wn;
	byte ddu, doc;
	bool is_played = false;
  protected:
};

extern Synt muzic;
#endif  // Snd_h