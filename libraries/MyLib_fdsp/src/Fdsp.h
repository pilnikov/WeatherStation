
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
    String 
	td_name_r(uint8_t),    // Время суток по русски
    wind_dir_r(uint16_t),  // Направление ветра по русски
    prc_type_r(uint8_t, bool, bool),   // Тип осадков по русски
    dow_name_r(uint8_t),   // Название Дня недели по русски
    month_name_r(uint8_t), // Название Месяца по русски

    render_number(int),    // Форматирование в одно знакоместо
    render_00number(int),  // Форматирование в  два знакоместа
    form_time_string(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, bool, bool),    // Формирование строки с текущим временем для LCD
    IP_to_str (byte, byte, byte, byte); // Формирование строки IP адреса
  private:
  protected:
};
extern FD fdisp;
#endif