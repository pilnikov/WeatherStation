
#ifndef Fdsp_h
#define Fdsp_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

typedef struct
{
  byte dig[8];
} scr_buff_t;

class FD
{
  public:
    byte   roll_seg (byte);// Переворот сегмента
    byte   mir_seg (byte); // Адаптация для мах7219
    char  *utf8rus(char*); // Конвертация из UTF8 в СР1251 для матриц
   String 
    lcd_rus(String),       // Конвертация из UTF8 в СР1251 для LCD
    utf8rus(String),       // Конвертация из UTF8 в СР1251 для матриц
	td_name_r(uint8_t),    // Время суток по русски
    wind_dir_r(uint16_t),  // Направление ветра по русски
    prc_type_r(uint8_t, bool, bool),   // Тип осадков по русски
    dow_name_r(uint8_t),   // Название Дня недели по русски
    month_name_r(uint8_t), // Название Месяца по русски

    render_number(int),    // Форматирование в одно знакоместо
    render_00number(int),  // Форматирование в  два знакоместа
    form_time_string(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, bool, bool), // Формирование строки с текущим временем для LCD
    IP_to_str (byte, byte, byte, byte); // Формирование строки IP адреса
    scr_buff_t prn7(String); // Печать на семисегментном дисплее

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
