/*
********************************************************** Sensor
  struct snr_data_t
  {
	int8_t   t1 =  99; //Температура. Канал 1
	uint8_t  h1 =   0; //Влажность. Канал 1
	int8_t   t2 =  99; //Температура. Канал 2
	uint8_t  h2 =   0; //Влажность. Канал 2
	int8_t   t3 =  99; //Температура. Канал 3
	uint8_t  h3 =   0; //Влажность. Канал 3
	uint16_t  p =   0; //Давление
	uint16_t ft =   0; //Освещенность
  };

********************************************************** Prognoz
  struct wf_data_t
  {
   uint8_t  tod       =   0; // время суток, для которого составлен прогноз: 0 - ночь 1 - утро, 2 - день, 3 - вечер
   uint8_t  cloud     =   0; // облачность по градациям: 0 - ясно, 1- малооблачно, 2 - облачно, 3 - пасмурно
   uint8_t  prec      =  10; // тип осадков: 4 - дождь, 5 - ливень, 6,7 – снег, 8 - гроза, 9 - нет данных, 10 - без осадков
   bool     rpower    =   0; // интенсивность осадков, если они есть. 0 - возможен дождь/снег, 1 - дождь/снег
   bool     spower    =   0; // вероятность грозы, если прогнозируется: 0 - возможна гроза, 1 - гроза
   uint16_t press_max = 900; // атмосферное давление, в мм.рт.ст.
   uint16_t press_min =   0; //
   int8_t   temp_max  =  99; // температура воздуха, в градусах Цельсия
   int8_t   temp_min  = -99; //
   uint8_t  wind_max  =  99; // приземный ветер min, max минимальное и максимальное значения средней скорости ветра, без порывов
   uint8_t  wind_min  =   0; //
   uint16_t wind_dir  =   0; // направление ветра в румбах, 0 - северный, 1 - северо-восточный, и т.д.
   uint8_t  hum_max   =  99; // относительная влажность воздуха, в %
   uint8_t  hum_min   =   0; //
   int8_t   heat_max  =  99; // комфорт - температура воздуха по ощущению одетого по сезону человека, выходящего на улицу
   int8_t   heat_min  = -99; //
   String   descript  = "additional info"; // дополнительное описание состояния погоды
  };

********************************************************** Config
  struct main_cfg_t
  {
  uint8_t   boot_mode = 0; // 0 - minimal; 1 - test; 2 - normal

  bool
  auto_br = false,
  auto_corr = false,
  use_pm = false,
  every_hour_beep = false,
  snd_pola = false,
  led_pola = false,
  rus_lng = false,
  wifi_off = false,
  udp_mon = false,
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
  alarms[7][5],
  font_t = 0,
  vdrv_t = 0,
  dsp_t = 0,
  type_snr1 = 0,
  type_snr2 = 0,
  type_snr3 = 0,
  type_snrp = 0,
  type_rtc = 0,
  thermo_t = 0,
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

  int8_t
  time_zone;

  int
  lb_thermo,
  hb_thermo;

  char
  ch1_name[17],
  ch2_name[17],
  ch3_name[17],
  sta_ssid[20],
  sta_pass[20],
  ap_ssid[20],
  ap_pass[20],
  AKey_r[17],
  AKey_w[17],
  esrv1_addr[17],
  esrv2_addr[17],
  radio_addr[17],
  srudp_addr[17],
  owm_key[33],
  news_api_key[33], // Get your News API Key from https://newsapi.org
  news_source[17];  // https://newsapi.org/sources to get full list of news sources available
  } main_cfg_t;

********************************************************** Actual config
  struct hwmcf_t
  {
  uint8_t   vdrv_t;      // Тип микросхемы драйвера дисплея 0 - Нет, 1 - TM1637, 2 - MAX7219, 3 - 74HC595, 4 - HT1621, 5 - HT1632, 6 - ILI9341, 11 - HT16K33, 12 - PCF8574
  uint8_t   dsp_t;      // Тип дисплея 0 - Внешний, 1 - 7SEGx4D, 2 - 7SEGx6D, 3 - 7SEGx8D, 10 - 14SEGx4D, 11 - 14SEGx8D, 12 - 16SEGx4D, 13 - 16SEGx8D, 19 - 2Linex16D, 20 - M32x8Mono, 21 - M32x16Mono, 22 - M32x16BiColor, 23 - M32x16Color, 24 - M64x32Color, 25 - M64x64Color, 29 - 320x240Color, 30 - Custom_1, 31 - Custom_2
  uint8_t   type_snr1;      // Тип датчика канал 1: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT22, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snr2;      // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT22, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snr3;      // Тип датчика канал 3: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT22, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snrp;      // Тип датчика давления 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 3 - Взять с ES2,                                                8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_rtc;       // Тип RTC 0 - Нет, 1 - DS3231, 2 - DS1302, 3 - DS1307
  uint8_t   temp_rtc;       // Температура чипа DS3231,
  uint8_t   lcd_addr;       // Адрес LCD дисплея
  uint8_t   ht_addr;        // Адрес HT1633
  uint8_t   bm_addr;        // Адрес BM E/P 1/2 80
  uint16_t  lb;             // Текущая яркость
  bool      bh1750_present; // Наличие датчика освещенности BH1750
  };

********************************************************* Rtc
  struct rtcmcf_t
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
  };
*/

#ifndef conf_h
#define conf_h

#define FW_Ver 5.0  //25.03.25 code corrected

#ifndef debug_level
#define debug_level 0
#endif

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>
#include <myrtc.h>
#include <Sysfn.h>
#include <Snd.h>
#include <Snr.h>
//#include <Dsp.h>
#include <Scr.h>
#include <BH1750.h>

#if defined(ESP8266)
#include <hw.h>
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
#include <Netwf_rt.h>
#endif

#if defined(ESP8266)
#include <ESP8266WebServer.h>
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <WebServer.h>
#endif

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <My_LFS.h>
#include <my_wifi.h>
#include <Netwf.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ntp.h>
#include <Exts.h>
#include <udp_cons.h>
#endif

#if defined(ESP8266)
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <ESPmDNS.h>
#include <ESP32SSDP.h>
#endif

#if defined(ESP8266)
#include <ESP8266HTTPUpdateServer.h>
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <HTTPUpdateServer.h>
#endif

// ----------------------------------- Force define func name
void sensor_init(snr_cfg_t*);
snr_data_t GetSnr(snr_data_t, snr_cfg_t, main_cfg_t, uint8_t, bool, wf_data_t);
String uart_st(snr_data_t, wf_data_t, main_cfg_t, rtc_time_data_t, rtc_alm_data_t, uint8_t);
void send_uart(snr_data_t, wf_data_t, main_cfg_t, rtc_time_data_t, rtc_alm_data_t, uint8_t);

inline uint8_t rumb_conv(uint16_t);
String remove_sb(String);
String lastday(String);
String firstday(String);
void Thermo(snr_data_t, main_cfg_t, gpio_cfg_t);
void wasAlm_reset();
void alarm1_action(bool, uint8_t, uint8_t&, uint8_t, rtc_cfg_data_t*, uint8_t, bool&, byte,
                   bool&, bool&, byte*, char*);
String radio_snd(String, bool, char*);


#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
String gs_rcv(unsigned long, bool);
String es_rcv(char*, bool);
String ts_rcv(unsigned long, char*, bool);
String ts_snd(String, bool);
void put_to_es(char*, uint8_t, snr_data_t, bool);

String getOWM_forecast(unsigned long, char*);
wf_data_t getOWM_current(unsigned long, char*);
wf_data_t forecast_decode(String, uint8_t);

#endif

void hw_accept(hw_data_t, snr_cfg_t*, uint8_t*, uint8_t*);

#if defined(ESP8266)
void IRAM_ATTR isr1();
void IRAM_ATTR isr2();
#elif defined(__AVR__)
void ISR_ATTR isr1();
void ISR_ATTR isr2();
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void ARDUINO_ISR_ATTR isr1();
#endif

#if defined(ESP8266)
void IRAM_ATTR isr0();
#elif defined(__AVR__)
void ISR_ATTR isr0();
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void ISR_ATTR isr0();
#endif

//-------------------------------------------------------------------------------------

class MAINJS {
public:
  String
    to_json(main_cfg_t);

  main_cfg_t
    from_json(String),
    def_conf();
private:
protected:
};

class GPIOJS {
public:
  String
    to_json(gpio_cfg_t);

  gpio_cfg_t
    from_json(String),
    def_conf();
private:
protected:
};

class KBT {
public:
  void _read(bool, bool, uint8_t, uint8_t, bool, uint8_t&, uint8_t&, bool, uint32_t&, bool&, main_cfg_t*);

private:
protected:
};

//---------------------------------------------------------------------------------------
void web_setup();

void start_serv();
void stop_serv();

void handler_SET_Time_from_NTP();
void handler_SET_Time();
void handler_GET_Time_Cfg();
void handler_GET_WiFi_Cfg_Start();
void handler_GET_IP_1();
void handler_GET_IP_2();
void handler_GET_WiFi_Cfg_End();
void handleExit();
#ifdef MATRIX
  void handler_SET_Font();
#endif  //MATRIX
void handler_GET_Disp_Cfg();
void handler_UPD_Snr_Data();
void handler_UPD_WFC_Data();
void handler_GET_Snr_Cfg_1();
void handler_GET_Snr_Cfg_2();
void handler_GET_Snr_Cfg_3();
void handler_GET_GPIO_Cfg();
void handler_GET_News_Cfg();
void handler_UPD_News();
void handler_GET_Tmst_Cfg();

void handler_SEND_Act_Time();
void handler_SEND_Act_Alarm();
void handler_SEND_BR_Act();

void handler_SEND_Time_Cfg();
void handler_SEND_WiFi_Cfg();
void handler_SEND_Disp_Cfg();
void handler_SEND_Snr_Cfg();
void handler_SEND_TSnr_Cfg();
void handler_SEND_Snr_Data();
void handler_GET_Snr_Data();
void handler_SEND_UART();
void handler_SEND_GPIO_Cfg();
void handler_SEND_Tmst_Cfg();
void handler_SEND_News_Cfg();
void handler_SEND_News_Full();

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

// ---------------------------------------------------- Variant of config
#define _dacha
//#define _work

#endif /*conf_h*/
