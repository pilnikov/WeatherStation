
#include <Snd.h>

#define _debug

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

bool snd_pola = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
static const int         BUZ_PIN  PROGMEM =  4;  // Канал PWM (буззер)
#elif  defined (__AVR__)
static const int         BUZ_PIN  PROGMEM =  A7;  // Канал PWM (буззер)
#endif

static bool play_snd = true, is_played = true;
uint8_t s_num = 15;
Synt Buzz;

void setup() {
  // put your setup code here, to run once:
  DBG_OUT_PORT.begin(115200);
   pinMode(BUZ_PIN, OUTPUT);
  //gimn();
}

void loop() {
  if (!is_played)
  {
    play_snd = true;
    s_num++;
    if (s_num > 15) s_num = 0;
    DBG_OUT_PORT.print(F("Melody #..."));
    DBG_OUT_PORT.println(s_num);
  }
  //inital sound card
  is_played = Buzz.play(s_num, BUZ_PIN, play_snd, snd_pola);
  play_snd = false;
}
