
#ifndef Fdsp_h
#define Fdsp_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class FD
{
  public:

   void
    roll_seg (byte&),// Переворот сегмента
    mir_seg (byte&), // Адаптация для мах7219
    prn7(String&),   // Печать на семисегментном дисплее
    utf8rus(char*),  // Конвертация из UTF8 в СР1251 для матриц
    utf8rus(String&),       // Конвертация из UTF8 в СР1251 для матриц
    lcd_rus(String&);       // Конвертация из UTF8 в СР1251 для LCD
 	private:
  protected:
};
extern FD fdisp;

class MSG // Сообщения на индикатор
{
  public:
  String 
	callback(uint8_t, uint8_t, uint8_t, bool), 
	alarm_msg(uint8_t, uint8_t, bool); // Номер активного будильника
	private:
  protected:
};
extern MSG mdisp;
#endif
