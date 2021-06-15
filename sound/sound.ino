
#include "Snd.h"
#include "Songs.h"

#define _debug

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

bool snd_pola = false;
#if defined(__xtensa__)
static const int         BUZ_PIN  PROGMEM =  15;  // Канал PWM (буззер)
#elif  defined (__AVR__)
static const int         BUZ_PIN  PROGMEM =  A7;  // Канал PWM (буззер)
#endif

static bool play_snd = true;
uint16_t song_num;
uint8_t s_num = 15;
Synt Buzz;

void setup() {
  // put your setup code here, to run once:
  DBG_OUT_PORT.begin(115200);
  //gimn();
  song_num = pgm_read_word(&songs[s_num]);
}

void loop() {
  if (millis() % 15000 == 0)
  {
    play_snd = true;
    s_num++;
    if (s_num > 15) s_num = 0;
    song_num = pgm_read_word(&songs[s_num]);

  }

  Buzz.play(song_num, BUZ_PIN, play_snd, snd_pola);  //inital sound card
  play_snd = false;
}
