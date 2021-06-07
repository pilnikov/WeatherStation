
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

static bool play_snd = false, fpp = false;
uint8_t song_num = 15;

Synt Buzz;

void setup() {
  fpp = true;
  // put your setup code here, to run once:
  DBG_OUT_PORT.begin(115200);
  //gimn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() % 15000 == 0)
  {
    play_snd = true;
    song_num++;
    if (song_num > 15) song_num = 0;
  }
  //char b1[440];
  //printFromPGM(&songs[song_num], b1);

  Buzz.play(song_num, BUZ_PIN, play_snd, snd_pola, fpp);  //inital sound card
  play_snd = false;
  fpp = false;
}

#if defined(__AVR_ATmega2560__)

// функция для печати из PROGMEM
void printFromPGM(int charMap, char * _buf)
{
  uint16_t _ptr = pgm_read_word(charMap); // получаем адрес из таблицы ссылок
  uint8_t i = 0;        // переменная - индекс массива буфера
  do
  {
    _buf[i] = (char)(pgm_read_byte(_ptr++)); // прочитать символ из PGM в ячейку буфера, подвинуть указатель
  } while (_buf[i++] != NULL);              // повторять пока прочитанный символ не нулевой, подвинуть индекс буфера
}

#elif defined(__xtensa__)

// функция для печати из PROGMEM
void printFromPGM(const void* charMap, char * _buf)
{
  const void* _ptr = pgm_read_ptr(charMap); // получаем адрес из таблицы ссылок
  uint8_t i = 0;        // переменная - индекс массива буфера
  do
  {
    _buf[i] = (char)(pgm_read_byte(_ptr++)); // прочитать символ из PGM в ячейку буфера, подвинуть указатель
  } while (_buf[i++] != NULL);              // повторять пока прочитанный символ не нулевой, подвинуть индекс буфера
}
#endif
