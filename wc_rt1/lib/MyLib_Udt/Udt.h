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
    int8_t   t1 =  99; //Температура. Канал 1
    uint8_t  h1 =   0; //Влажность. Канал 1
    int8_t   t2 =  99; //Температура. Канал 2
    uint8_t  h2 =   0; //Влажность. Канал 2
    int8_t   t3 =  99; //Температура. Канал 3
    uint8_t  h3 =   0; //Влажность. Канал 3
    uint16_t  p =   0; //Давление
    uint16_t  f =   0; //Освещенность  
} snr_data_t;
	
	
typedef struct 
{
    uint8_t  day       =   0; // день  , для которого составлен прогноз
    uint8_t  month     =   0; // месяц , для которого составлен прогноз
    uint16_t year      =2021; // год,    для которого составлен прогноз
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

//********************************************************** Config
typedef struct 
{
  bool
  auto_br = false,
  auto_corr = false,
  use_pm = false,
  every_hour_beep = false,
  snd_pola = false,
  led_pola = false,
  rus_lng = false,
  time_up = false,
  wifi_off = false,
  udp_mon = false,
  esm = false,
  news_en = false;

  unsigned long 
  period = 10,
  pp_city_id = 0,
  ts_ch_id = 0;

  uint16_t
  br_level[4] = {0, 0, 0, 0};

  uint8_t
  use_pp = 0,
  use_ts = 0,
  use_es = 0,
  man_br = 0,
  nmd_br = 0,
  nm_start = 0,
  nm_stop = 0,
  color_up = 0,
  color_dwn = 0,
  alarms[7][5],
  type_font = 0,
  type_vdrv = 0,
  type_disp = 0,
  type_snr1 = 0,
  type_snr2 = 0,
  type_snr3 = 0,
  type_snrp = 0,
  type_rtc = 0,
  type_thermo = 0,
  src_thermo = 0,

  gpio_sda = 255,
  gpio_scl = 255,
  gpio_dio = 255,
  gpio_clk = 255,
  gpio_dcs = 255,
  gpio_dwr = 255,
  gpio_trm = 255,
  gpio_sqw = 255,
  gpio_snd = 255,
  gpio_led = 255,
  gpio_btn = 255,
  gpio_dht = 255,
  gpio_ana = 255,
  gpio_uar = 255,
  gpio_bz2 = 255;

  int8_t
  time_zone;
  
  int
  lb_thermo,
  hb_thermo;

  char
  ch1_name[17],
  ch2_name[17],
  ch3_name[17],
  sta_ssid[17],
  sta_pass[17],
  ap_ssid[17],
  ap_pass[17],
  AKey_r[17],
  AKey_w[17],
  esrv1_addr[17],
  esrv2_addr[17],
  radio_addr[17],
  srudp_addr[17],
  owm_key[33],
  news_api_key[33], // Get your News API Key from https://newsapi.org
  news_source[17];  // https://newsapi.org/sources to get full list of news sources available
} conf_data_t;

typedef struct
{
  uint8_t   type_vdrv;      // Тип микросхемы драйвера дисплея 0 - Нет, 1 - TM1637, 2 - MAX7219, 3 - 74HC595, 4 - HT1621, 5 - HT1632, 6 - ILI9341, 11 - HT16K33, 12 - PCF8574 
  uint8_t   type_snr1;      // Тип датчика канал 1: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snr2;      // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snr3;      // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snrp;      // Тип датчика давления 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 3 - Взять с ES2,                                                8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза 
  uint8_t   type_rtc;       // Тип RTC 0 - Нет, 1 - DS3231, 2 - DS1302, 3 - DS1307
  uint8_t   temp_rtc;       // Температура чипа DS3231,
  uint8_t   lcd_addr;       // Адрес LCD дисплея
  uint8_t   ht_addr;        // Адрес HT1633
  uint8_t   bm_addr;        // Адрес BM E/P 1/2 80
  uint16_t  lb;             // Текущая яркость
  bool      bh1750_present; // Наличие датчика освещенности BH1750
} ram_data_t;

typedef struct
{
  long          ct        = 1530687234; // Текущее время (UNIX format)
  uint8_t       hour      = 62;         // Текущее время. Час.
  uint8_t       min       = 62;         // Текущее время. Минута.
  uint8_t       sec       = 62;         // Текущее время. Секунда.
  uint8_t       day       = 32;         // Текущее время. День.
  uint8_t       wday      =  9;         // Текущее время. День недели.
  uint8_t       month     = 13;         // Текущее время. Месяц.
  uint16_t      year      = 2030;       // Текущее время. Год.
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