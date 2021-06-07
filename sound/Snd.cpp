// Snd.cpp

#include "Snd.h"
#include "TN.h"

// for debugging
//#define _debug

//*********************************************************************************************************
void Synt::beep(uint8_t out, bool pola)
{
  soundNote(NOTE_C8, 200, out, pola);
}

//*********************************************************************************************************
void Synt::soundNote(uint8_t note, uint16_t dur, uint8_t out, bool pola)
{
  static const uint16_t Freqs[] PROGMEM =
  {
    FREQ_C11,  FREQ_CS11, FREQ_D11,  FREQ_DS11, FREQ_E11,  FREQ_F11,
    FREQ_FS11, FREQ_G11,  FREQ_GS11, FREQ_A11,  FREQ_AS11, FREQ_B11
  };
  uint16_t freq;
  uint8_t octave;

  if (millis() > dela)
  {
    octave = note >> 4;
    note = note & 0xf;
    freq = pgm_read_word_near(&Freqs[note]);
    freq >>= (11 - octave);

#if !defined(ESP32)
    tone(out, freq, dur);
    noTone(out);
#else
    ledcWriteTone(out, freq);
    ledcWriteTone(out, 0);
#endif
    dela = millis() + dur;
    digitalWrite(out, pola ? HIGH : LOW);
  }
}

void Synt::play(char *ip, uint8_t out, bool _play, bool pola)
{
  static const uint16_t notes[] PROGMEM =
  { 0,
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
  };


  if (ip == NULL) return;
  if (!setup & !is_played) *p = 'x';
  if (_play) set_up = true;

  //setup sections (run once, before playin song)
  if (set_up)
  {
#ifdef _debug
    DBG_OUT_PORT.print(F("setup section.....\n "));
#endif
    p = ip;
	while (*p != ':') p++;   // ignore name
    p++;                     // skip ':'

    // get default duration
    if (*p == 'd')
    {
      p++; p++;              // skip "d="
      num = 0;
      while (isdigit(*p))
      {
        num = (num * 10) + (*p++ - '0');
      }
      if (num > 0) default_dur = num;
      p++;                   // skip comma
    }
#ifdef _debug
    DBG_OUT_PORT.print(F("ddur: "));
    DBG_OUT_PORT.println(default_dur, 10);
#endif

    // get default octave
    if (*p == 'o')
    {
      p++; p++;              // skip "o="
      num = *p++ - '0';
      if (num >= 3 && num <= 7) default_oct = num;
      p++;                   // skip comma
    }

#ifdef _debug
    DBG_OUT_PORT.print(F("doct: "));
    DBG_OUT_PORT.println(default_oct, 10);
#endif

    // get BPM
    if (*p == 'b')
    {
      p++; p++;              // skip "b="
      num = 0;
      while (isdigit(*p)) num = (num * 10) + (*p++ - '0');
      bpm = num;
      p++;                   // skip colon
    }

    // BPM usually expresses the number of quarter notes per minute
    wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

    wn = wholenote;
    ddu = default_dur;
    set_up = false;
    is_played = true;
    dela = 0;
  }// End setup sections

  if (is_played)
  {
    if (millis() > dela)
    {
#ifdef _debug
      DBG_OUT_PORT.print(F("play section.....\n "));
#endif
      // now begin note loop
      //DBG_OUT_PORT.println(*p);
      // first, get note duration, if available
      num = 0;
      while (isdigit(*p)) num = (num * 10) + (*p++ - '0');

      duration = wn / ddu;            // we will need to check if we are a dotted note after
      if (num) duration = wn / num; // now get the note
      note = 0;

      switch (*p)
      {
        case 'c':
          note = 1;
          break;
        case 'd':
          note = 3;
          break;
        case 'e':
          note = 5;
          break;
        case 'f':
          note = 6;
          break;
        case 'g':
          note = 8;
          break;
        case 'a':
          note = 10;
          break;
        case 'b':
          note = 12;
          break;
        case 'p':
        default:
          note = 0;
      }
      p++;

      // now, get optional '#' sharp
      if (*p == '#')
      {
        note++;
        p++;
      }

      // now, get optional '.' dotted note
      if (*p == '.')
      {
        duration += duration / 2;
        p++;
      }

      // now, get scale
      scale = default_oct;

      if (isdigit(*p))
      {
        scale = *p - '0';
        p++;
      }

      scale += OCTAVE_OFFSET;

      if (*p == ',') p++;       // skip comma for next note (or we may be at the end)
#ifdef _debug
      DBG_OUT_PORT.print(F("pointer: "));
      DBG_OUT_PORT.println(*p);
#endif

      if (note) // now play the note
      {
        uint16_t _tone = pgm_read_word_near(&notes[(scale - 4) * 12 + note]);
#ifdef _debug
        DBG_OUT_PORT.print(F("pitch: "));
        DBG_OUT_PORT.println(_tone);
#endif
#if !defined(ESP32)
        tone(out, _tone);
#else
        ledcWriteTone(out, _tone);
#endif

        delay(duration);
        //sound off
#if !defined(ESP32)
        noTone(out);
#else
        ledcWriteTone(out, 0);
#endif
      }
      else
      {
        dela = millis() + duration;
#ifdef _debug
        DBG_OUT_PORT.print(F("dela: "));
        DBG_OUT_PORT.println(dela - millis());
#endif
      }
    }
    if (*p == 'x')
    {
      is_played = false; //End of playing
      digitalWrite(out, pola ? HIGH : LOW);
      free(p);
    }
  }
}