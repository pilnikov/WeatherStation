
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

static bool play_snd = true, is_played, is_played_buf;
uint8_t s_num = 15;
Synt Buzz;

void setup() {
  // put your setup code here, to run once:
  DBG_OUT_PORT.begin(115200);
  //gimn();
 }

void loop() {
  if (!is_played & is_played_buf)
  {
    is_played_buf = is_played;
    play_snd = true;
    s_num++;
    if (s_num > 15) s_num = 0;
  }

#if defined(__xtensa__)
  //inital sound card
  is_played = Buzz.play(pgm_read_ptr(&songs[s_num]), BUZ_PIN, play_snd, snd_pola);
#elif defined (__AVR__)
  is_played = Buzz.play(pgm_read_word(&songs[s_num]), BUZ_PIN, play_snd, snd_pola);   //inital sound card
#endif
  play_snd = false;
}
