#ifndef myrtc_h
#define myrtc_h

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

#include <Wire.h> // must be included here so that Arduino library object file references work

#include <RtcDS3231.h>
#include <RtcDS1307.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <LittleFS.h>
#endif

#include <RtcDateTime.h>

#define ARDUINOJSON_USE_LONG_LONG 1

#include <ArduinoJson.h>

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <pgmspace.h>
#include <ntp.h>
#include <IPAddress.h>
#include <My_LFS.h>
#elif defined(__AVR__)
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#endif

//**********************************************************HW Config
typedef struct
{
  uint8_t
  gpio_sqw = 255,
  gpio_dio = 255,
  gpio_clk = 255,
  gpio_dcs = 255,

  a_type    = 0;          	// Актуальный тип МС RTC (0 - Нет, 1 - DS3231, 1 - DS1307, 2 - DS1302)
} rtc_hw_data_t;

typedef struct
{
  uint8_t
  type = 0;					// Тип (0 - Отключен, 1 - Ежедневно, 2 - По рабочим, 3 - По выходным, 4 - Разово)
  unsigned long
  time = 0;					// Время срабатывания
  uint8_t
  act = 0;					// Экшн (0 -:- 15 - Сыграет мелодию (номер мелодии), 20 - Включит ночной режим, 21 - Выключит ночной режим, 22 - Включит  дисплей, 23 - Выключит дисплей, 24 - Включит  радио, 25 - Выключит радио
} alarm_t;

//**********************************************************SW Config
typedef struct
{
  bool
  auto_corr = false, 		// Автокоррекция через НТП.
  use_pm = false,			// 12ти часовой формат представления времени.
  every_hour_beep = false; 	// Пищщать каждый час.

  long
  nm_start  = 0, 			// Ночной режим. Время старта.
  nm_stop   = 0; 			// Ночной режим. Время окончания.
  alarm_t  
  alarms[7]; 				// Будиьники (7 штук) 
  
  uint8_t
  c_type    = 0;			// Тип МС RTC (0 - Нет, 1 - DS3231, 1 - DS1302, 2 - DS1307) 
  int8_t
  time_zone = 0;			// Тайм зона (-12 / +12)

  char
  ntp_srv[3][17];			// Адрес НТП сервера (3 штуки)
} rtc_cfg_data_t;

typedef struct
{
  unsigned long
  ct        = 1630687234; 	// Текущее время (UNIX format)

  uint8_t
  hour      = 25,         	// Текущее время. Час.
  min       = 62,         	// Текущее время. Минута.
  sec       = 62,         	// Текущее время. Секунда.
  day       = 32,         	// Текущее время. День.
  wday      =  9,         	// Текущее время. День недели.
  month     = 13;         	// Текущее время. Месяц.

  uint16_t
  year      = 2030;       	// Текущее время. Год.

  bool
  nm_is_on  = false;      	// Ночной режим активен
} rtc_time_data_t;

typedef struct
{
  uint8_t
  num     = 7;         		// Номер активного будильника
  long
  time    = 0;				// Время срабатывания активного будильника
  uint8_t
  act     = 0;        	  	// Действие для активного будильника

  bool
  al1_on    = false,		// Будильник 1 активен
  al2_on    = false;		// Будильник 2 активен
} rtc_alm_data_t;

typedef struct
{
  uint8_t
  h 		= 0,       		// Часы
  m			= 0,			// Минуты
  s			= 0;			// Секунды
 } rtc_hms_t;

class RTCJS
{
  public:
    String
    to_json(rtc_cfg_data_t);

    rtc_cfg_data_t
    from_json(String),
    def_conf();
  private:
  protected:
};

class CT
{
  private:
  public:
	void 
	rtc_init(rtc_hw_data_t),
	cur_time_str(rtc_time_data_t, bool, char*),
	man_set_time(rtc_hw_data_t, unsigned long),
	dt_from_unix(rtc_time_data_t*);
	
	unsigned long
	GetTime(rtc_hw_data_t),
	GetNtp(rtc_cfg_data_t, unsigned long);
	
	inline unsigned long trunc_to_one_day(unsigned long in){unsigned long out = in % 86400; return out;}
    inline unsigned long hms_to_unix(rtc_hms_t in){unsigned long out = in.s + in.m * 60 + in.h * 3600; return out;}
	
	bool
	Alarmed(bool, rtc_hw_data_t, rtc_alm_data_t*, unsigned long);

	inline bool nm_act(unsigned long cur_time, unsigned long nm_start, unsigned long nm_stop){
	bool nm_is_on = false;
	if (nm_start <  nm_stop) nm_is_on = (cur_time >= nm_start && cur_time < nm_stop);
    else nm_is_on = (cur_time >= nm_start || cur_time < nm_stop);
	return nm_is_on;}
	
	rtc_alm_data_t
	set_alarm(rtc_cfg_data_t, unsigned long, bool);

	inline rtc_hms_t unix_to_hms(unsigned long in){
	rtc_hms_t out;
	out.h = in / 3600;
	out.m = in % 3600 / 60;
	out.s = in % 3600 % 60 / 60;
	return out;}

	int 
	get_temperature();
  protected:
};

#endif
