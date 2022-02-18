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

  a_type    = 0;          // Актуальный тип МС RTC
} rtc_hw_data_t;


//**********************************************************SW Config
typedef struct
{
  bool
  auto_corr = false,
  use_pm = false,
  every_hour_beep = false;

  uint8_t
  nm_start = 0,
  nm_stop  = 0,
  alarms[7][5],
  c_type   = 0,
  time_zone;

  char
  ntp_srv[3][17];
} rtc_cfg_data_t;

typedef struct
{
  long
  ct        = 1530687234; // Текущее время (UNIX format)

  uint8_t
  hour      = 62,         // Текущее время. Час.
  min       = 62,         // Текущее время. Минута.
  sec       = 62,         // Текущее время. Секунда.
  day       = 32,         // Текущее время. День.
  wday      =  9,         // Текущее время. День недели.
  month     = 13;         // Текущее время. Месяц.

  uint16_t
  year      = 2030;       // Текущее время. Год.

  bool
  nm_is_on  = false;      // Ночной режим активен
} rtc_time_data_t;

typedef struct
{
  uint8_t
  num     = 6,          // Номер активного будильника
  hour    = 62,         // Час срабатывания активного будильника
  min     = 62,         // Минута срабатывания активного будильника
  muz     = 0;          // Номер мелодии активного будильника

  bool
  al1_on    = false,      // Будильник 1 активен
  al2_on    = false;      // Будильник 2 активен
} rtc_alm_data_t;

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
  public:
	void 
	rtc_init(rtc_hw_data_t),
	GetTime(rtc_hw_data_t, rtc_time_data_t*),
	cur_time_str(rtc_time_data_t, bool, char*);
	
	bool
	Alarmed(bool, rtc_hw_data_t, rtc_cfg_data_t*, rtc_time_data_t, rtc_alm_data_t*);

	long 
	man_set_time(rtc_hw_data_t, const RtcDateTime&);

	rtc_alm_data_t
	set_alarm(rtc_hw_data_t, rtc_cfg_data_t, rtc_time_data_t);
	
	RtcDateTime 
	GetNtp(rtc_cfg_data_t, rtc_time_data_t);

	int
    get_temperature();
// ----------------------------------- interrupt
  private:
  protected:
};

#endif
