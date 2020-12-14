// Udt.h

#ifndef Udt_h
#define Udt_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

typedef struct 
   {
    uint8_t  h1 =   0; //Внутренняя влажность
    int8_t   t1 =  99; //Внутренняя температура
    uint8_t  h2 =   0; //Внешнняя влажность
    int8_t   t2 =  99; //Внешнняя температура
    uint16_t  p =   0; //Давление
    uint16_t ft =   0; //Освещенность  
   } snr_data_t;
	
	
typedef struct 
   {
    uint8_t  day       =   0; // день  , для которого составлен прогноз
    uint8_t  month     =   0; // месяц , для которого составлен прогноз
    uint16_t year      =2015; // год,    для которого составлен прогноз
    uint8_t  tod       =   0; // время суток, для которого составлен прогноз: 0 - ночь 1 - утро, 2 - день, 3 - вечер
    uint8_t  cloud     =   0; // облачность по градациям: 0 - ясно, 1- малооблачно, 2 - облачно, 3 - пасмурно
    uint8_t  prec      =  10; // тип осадков: 4 - дождь, 5 - ливень, 6,7 – снег, 8 - гроза, 9 - нет данных, 10 - без осадков
    bool     rpower  = false; // интенсивность осадков, если они есть. 0 - возможен дождь/снег, 1 - дождь/снег
    bool     spower  = false; // вероятность грозы, если прогнозируется: 0 - возможна гроза, 1 - гроза
    uint16_t press_max = 700; // атмосферное давление, в мм.рт.ст.
    uint16_t press_min =   0; //
    int8_t   temp_max  =  99; // температура воздуха, в градусах Цельсия
    int8_t   temp_min  = -99; //
    uint8_t  wind_max  =  99; // приземный ветер min, max минимальное и максимальное значения средней скорости ветра, без порывов
    uint8_t  wind_min  =   0; //
    uint8_t  wind_dir  =   0; // направление ветра в румбах, 0 - северный, 1 - северо-восточный, и т.д.
    uint8_t  hum_max   =  99; // относительная влажность воздуха, в %
    uint8_t  hum_min   =   0; //
    int8_t   heat_max  =  99; // комфорт - температура воздуха по ощущению одетого по сезону человека, выходящего на улицу
    int8_t   heat_min  = -99; //
    String   descript  =  ""; // дополнительное описание состояния погоды 
 } wf_data_t;

typedef struct 
{
  char      sta_ssid[33];
  char      sta_pass[33];
  char      ap_ssid[17];
  char      ap_pass[17];
  uint16_t  br_level[4];
  int8_t    time_zone;
  bool      auto_br;
  bool      auto_corr;
  bool      use_pm;
  bool      every_hour_beep;
  bool      rus_disp;
  bool      use_ts_i;
  bool      use_ts_e;
  bool      use_ts_p;
  bool      use_es;
  uint8_t   use_pp;
  uint8_t   man_br;
  uint8_t   night_mode_start;
  uint8_t   night_mode_stop;
  uint8_t   alarms[7][5];
  uint8_t   type_font;
  uint8_t   type_disp;
  uint8_t   type_int_snr;
  uint8_t   type_ext_snr;
  uint8_t   type_prs_snr;
  uint8_t   type_rtc;
  uint8_t   type_thermo;
  uint8_t   src_thermo;
  int       lb_thermo;
  int       hb_thermo;
  unsigned long period;
  unsigned long pp_city_id;
  unsigned long ts_ch_id;
  char      AKey_r[17];
  char      AKey_w[17];
  char      esrv_addr[17];
  char      radio_addr[17];
  char      owm_key[35];
  char      test[3];
} conf_data_t;

typedef struct
{
  uint8_t   type_disp;      // Тип дисплея 0 - Нет.Внешний, 1 - LCD1602, 2 - HT1633, 3 - TM1637, 4 - MAX7219, 5 - MAX7219 7S, 6 - MAX7219 7S8D, 7 - HT1621, 8 - HT1632
  uint8_t   type_int_snr;   // Тип внутреннего сенсора 0 - Нет, 1 - Взять с TC, 2 - Взять с ES, 3 - DHT21, 4 - DS3231, 5 - SI7021, 6 - AM2320, 7 - BMP180, 8 - BMP280, 9 - BME280, 10 - Взять из прогноза
  uint8_t   type_ext_snr;   // Тип внешнего сенсора    0 - Нет, 1 - Взять с TC, 2 - Взять с ES, 3 - DHT21, 4 - DS3231, 5 - SI7021, 6 - AM2320, 7 - BMP180, 8 - BMP280, 9 - BME280, 10 - Взять из прогноза
  uint8_t   type_prs_snr;   // Тип датчика давления    0 - Нет, 1 - Взять с TC, 2 - Взять с ES, 											   7 - BMP180, 8 - BMP280, 9 - BME280, 10 - Взять из прогноза 
  uint8_t   type_rtc;       // Тип RTC 0 - Нет, 1 - DS3231, 2 - DS1302, 3 - DS1307
  uint8_t   temp_rtc;       // Температура чипа DS3231,
  uint8_t   lcd_addr;       // Адрес LCD дисплея
  uint8_t   ht_addr;        // Адрес HT1633
  uint16_t  lb;             // Текущая яркость
  bool      bh1750_present; // Наличие датчика освещенности BH1750
} ram_data_t;

typedef struct
{
  long          ct        = 1530687234; // Текущее время (UNIX format)
  uint8_t       a_num     = 0;          // Номер будильника
  uint8_t       n_cur_alm = 6;          // Номер активного будильника
  uint8_t       a_hour    = 62;         // Час срабатывания будильника
  uint8_t       a_min     = 62;         // Минута срабатывания будильника
  uint8_t       a_muz     = 0;          // Номер мелодии будильника
  bool          alarm     = false;      // Будильник ????
  bool          alarm_on  = false;      // Будильник активен
  bool          wasAlarm  = false;      // Было срабатывание будильника
} rtc_data_t;

#endif //Udt_h