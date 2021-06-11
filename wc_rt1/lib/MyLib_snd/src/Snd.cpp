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

bool Synt::play(uint16_t _ptr, uint8_t out, bool set_up, bool pola)
{
  static const uint16_t notes[] PROGMEM =
  { 0,
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
  };


  if (cp(_ptr) != ':') return false;
 
  //setup sections (run once, before playin song)
  if (set_up)
  {
    uint8_t default_dur = 4, default_oct = 6;
    int bpm = 63;
    long wholenote = 0;
	char flag;

    p = _ptr;

    // get default duration -------------------------------
    while (cp(p) != 'd') p++; // find 'd', skip ":"
    flag = cp(p);

    while (!isdigit(cp(p))) p++; // find value of d, skip "=,"

    num = 0;
    while (isdigit(cp(p))) 
	{
		num = (num * 10 + cp(p) - '0');// parce the value
		p++;
	}	
    if ( num & (flag == 'd')) default_dur = num; // accept d dur


    // get default octave --------------------------------
    while (cp(p) != 'o') p++;; // find 'o'
    flag = cp(p);

    while (!isdigit(cp(p))) p++; // find value of o, skip "=,"

    num = 0;
    while (isdigit(cp(p))) num = num * 10 + cp(p++) - '0'; // parce the value
    if ((num) & (flag == 'o')) default_oct = num; // accept d octave

    // get BPM --------------------------------------------
    while (cp(p) != 'b') p++; // find 'b', skip ":"

    flag = cp(p);

    while (!isdigit(cp(p))) p++; // find value of b, skip "=,"

    num = 0;
    while (isdigit(cp(p))) 
	{
		num = (num * 10 + cp(p) - '0'); // parce the value
		p++;
	}
	if (flag == 'b') bpm = num; // accept BPM

    // BPM usually expresses the number of quarter notes per minute
    wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

    wn = wholenote;
    ddu = default_dur;
    doc = default_oct;

    is_played = true;

    dela = 0;
    dela2 = 0;
  }// End setup sections

  if (is_played)                // now begin note loop
  {
    Serial.print("Dela  ");
	Serial.println(dela, 10);
	Serial.print("dela2  ");
	Serial.println(dela2,10);

	
	if (millis() > dela2)
    {
      if (millis() > dela)
      {
        
        // first, get note duration, if available
        num = 0;

        while (isdigit(cp(p))) 
		{
			num = num * 10 + cp(p) - '0';
			p++;
		}
        duration = wn / ddu;          // we will need to check if we are a dotted note after

        if (num) duration = wn / num; // now get the note


        note = 0;

        switch (cp(p))
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
        if (cp(p) == '#')
        {
          note++;
          p++;
        }

        // now, get optional '.' dotted note
        if (cp(p) == '.')
        {
          duration += duration / 2;
          p++;
        }

        // now, get scale

        num = 0;

        while (isdigit(cp(p)))
		{
			num = num * 10 + cp(p) - '0';
			p++;
		}
        scale = doc;

        if (num) scale = num;

        scale += OCTAVE_OFFSET;
		Serial.print("Scale ");
		Serial.println(scale, 10);

        if (cp(p) == ',') p++;       // skip comma for next note (or we may be at the end)

        if (note) // now play the note //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        {
			_tone = pgm_read_word_near(&notes[(scale - 4) * 12 + note]);

			#if !defined(ESP32)
				tone(out, _tone);
			#else
				ledcWriteTone(out, _tone);
			#endif
			tone_act = true;
			dela2 = millis() + duration;

			if (millis() > dela2)
			{
				tone_act = false;
				#if !defined(ESP32)
					noTone(out);
				#else
					ledcWriteTone(out, 0);
				#endif
			}
        }
        else dela = millis() + duration;
      }
    }
    if (tone_act) 
	{
		#if !defined(ESP32)
			tone(out, _tone);
		#else
			ledcWriteTone(out, _tone);
		#endif
	}	

    if (cp(p) == 'x')
    {
		#if !defined(ESP32)
			noTone(out);
		#else
			ledcWriteTone(out, 0);
		#endif
		is_played = false; //End of playing
		digitalWrite(out, pola ? HIGH : LOW);
    }
  }
  return is_played;
}