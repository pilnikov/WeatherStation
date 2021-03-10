// Snd.cpp

#include "Snd.h"

//*********************************************************************************************************
void Synt::beep(uint8_t out)
{
	soundNote(NOTE_C8, 200, out);
	digitalWrite(out, false);
}

//*********************************************************************************************************
void Synt::soundNote(uint8_t note, uint16_t dur, uint8_t out)
{
	const static uint16_t Freqs[] =
	{
	  FREQ_C11,  FREQ_CS11, FREQ_D11,  FREQ_DS11, FREQ_E11,  FREQ_F11,
	  FREQ_FS11, FREQ_G11,  FREQ_GS11, FREQ_A11,  FREQ_AS11, FREQ_B11
	};
	uint16_t freq;
	uint8_t octave;

	if (millis() > dela[0])
	{
		octave = note >> 4;
		note = note & 0xf;
		freq = pgm_read_word_near(&Freqs[note]);
		freq >>= (11 - octave);

#if !defined(ESP32)
		tone(out, freq, dur);
		dela[0] = millis() + dur;
		noTone(out);
#else
		ledcWriteTone(out, freq);
		dela[0] = millis() + dur;

		ledcWriteTone(out, 0);
#endif
	}
}

void Synt::play(const char* in, uint8_t out, bool rst)
{

	byte default_dur = 4;
	byte default_oct = 6;
	int bpm = 63;
	int num;
	long wholenote;
	long duration;
	byte note;
	byte scale;

	static byte ddu;
	static long wn;
	static const char* p;

	if (rst)
	{
		if (millis() > dela[2])
		{
			if (millis() > dela[1])
			{
				p = in;
				// format: d=N,o=N,b=NNN:
				// find the start (skip name, etc)

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
				DBG_OUT_PORT.print("ddur: ");
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
				DBG_OUT_PORT.print("doct: ");
				DBG_OUT_PORT.println(default_oct, 10);
#endif

				// get BPM
				if (*p == 'b')
				{
					p++; p++;              // skip "b="
					num = 0;
					while (isdigit(*p))
					{
						num = (num * 10) + (*p++ - '0');
					}
					bpm = num;
					p++;                   // skip colon
				}

				// BPM usually expresses the number of quarter notes per minute
				wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

				wn = wholenote;
				ddu = default_dur;
			}

			// now begin note loop

			if (*p != 'x')
			{
				//DBG_OUT_PORT.println(*p);

				// first, get note duration, if available
				num = 0;
				while (isdigit(*p)) num = (num * 10) + (*p++ - '0');

				duration = wn / ddu;  // we will need to check if we are a dotted note after
				if (num) duration = wn / num;

				// now get the note
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

				if (*p == ',')
					p++;       // skip comma for next note (or we may be at the end)

				  // now play the note

				if (note)
				{
					//soundNote(NOTE_C8, duration);
#if !defined(ESP32)
					tone(out, notes[(scale - 4) * 12 + note]);
					dela[1] = millis() + duration;
					noTone(out);
#else
					ledcWriteTone(out, notes[(scale - 4) * 12 + note]);
					dela[1] = millis() + duration;
					ledcWriteTone(out, 0);
#endif
				}
				else
				{
					dela[2] = millis() + duration;
				}
			}
		}
	}
}
