/*
********************************************************** Sensor
struct snr_data_t
 {
   uint8_t  h1 =   0; //Внутренняя влажность
   int8_t   t1 =  99; //Внутренняя температура
   uint8_t  h2 =   0; //Внешнняя влажность
   int8_t   t2 =  99; //Внешнняя температура
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
struct conf_data_t
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
  uint8_t   type_rtc;
  unsigned long period;
  unsigned long pp_city_id;
  unsigned long ts_ch_id;
  char      AKey_r[17];
  char      AKey_w[17];
  char      esrv_addr[17];
  char      radio_addr[17];
  char      owm_key[32];
  char      test[3];
 };
********************************************************** Ram config
struct ram_data_t
 {
  uint8_t   type_disp;      // Тип дисплея 0 - Нет.Внешний, 1 - LCD1602, 2 - HT1633, 3 - TM1637, 4 - MAX7219, 5 - MAX7219 7S, 6 - MAX7219 7S8D, 7 - HT1621, 8 - HT1632
  uint8_t   type_int_snr;   // Тип внутреннего сенсора 0 - Нет, 1 - Взять с TC, 2 - Взять с ES, 3 - DHT21, 4 - DS3231, 5 - SI7021, 6 - AM2320, 7 - BMP180, 8 - BMP280, 9 - BME280, 10 - Взять из прогноза
  uint8_t   type_ext_snr;   // Тип внешнего сенсора    0 - Нет, 1 - Взять с TC, 2 - Взять с ES, 3 - DHT21, 4 - DS3231, 5 - SI7021, 6 - AM2320, 7 - BMP180, 8 - BMP280, 9 - BME280, 10 - Взять из прогноза
  uint8_t   type_prs_snr;   // Тип датчика давления    0 - Нет, 1 - Взять с TC, 2 - Взять с ES,                                                7 - BMP180, 8 - BMP280, 9 - BME280, 10 - Взять из прогноза 
  uint8_t   type_rtc;       // Тип RTC 0 - Нет, 1 - DS3231, 2 - DS1302, 3 - DS1307
  uint8_t   temp_rtc;       // Температура чипа DS3231,
  uint8_t   lcd_addr;       // Адрес LCD дисплея
  uint8_t   ht_addr;        // Адрес HT1633
  uint16_t  lb;             // Текущая яркость
  bool      bh1750_present; // Наличие датчика освещенности BH1750
 };
********************************************************* Rtc
struct rtc_data_t
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
 };
*/


#ifndef _wc_h
#define _wc_h

#define FW_Ver 1.0 //16.05.17 added udp debug console

// ------------------------------------------------------------- Include

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM)
#include <WiFi.h>
#include <OTA.h>
#include <HttpClient.h>
#include <WiFiClient.h>
#include <Netwf_rt.h>
#endif

#if defined(ESP8266)
#include <pgmspace.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>

//#include <IRremoteESP8266.h>

#include <ArduinoOTA.h>
//#include <Ticker.h>
#include <hw.h>
#include <Netwf.h>
#include <FS.h>
#endif

#if defined(ESP32)
#include <pgmspace.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <WiFiManager.h>
//#include <ESPAsyncTCP.h>
#include <WebServer.h>
#include <Update.h>
#include <ESP32SSDP.h>
//#include <IRremote.h>

#include <ArduinoOTA.h>
//#include <Ticker.h>
//#include <hw.h>
#include <Netwf.h>
#include <FS.h>
#endif

#include <ArduinoJson.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SPI.h>

#include <Sysf2.h>
#include <Snd.h>
#include <ntp.h>
#include <Exts.h>
#include <Snr.h>
#include <Fdsp.h>

//#define DEBUG_UDP

// ------------------------------------------------------ ConsoleUDP
#ifdef DEBUG_UDP
#include <udp_cons.h>
udp_cons print_console_udp;
#define DBG_OUT_PORT print_console_udp
#else
#define DBG_OUT_PORT Serial
#endif


// ------------------------------------------------------ GPIO
#if defined(ESP8266) || defined(ESP32) 
static const int         SDA_PIN  PROGMEM =  4;  // (D2)
static const int         SCL_PIN  PROGMEM =  5;  // (D1)
static const int        uart_pin  PROGMEM = 16;  // (D0)
static const int         led_pin  PROGMEM =  2;  // (D4)
static const int        RECV_PIN  PROGMEM =  2;  // (D4) an IR detector/demodulatord is connected to GPIO pin 2
static const int     setting_pin  PROGMEM =  0;  // (D3)
static const int         BUZ_PIN  PROGMEM = 15;  // (D8) Пин пищалки 
static const int         DHT_PIN  PROGMEM =  0;  // (D3) Pin which is connected to the DHT sensor. 
static const int           pinCS  PROGMEM = 16;  // (D0) CS . DIN 13 (D7) CLK 14 (D5)
static const int      TM1637_DIO  PROGMEM = 13;  // (D7)
static const int      TM1637_CLK  PROGMEM = 14;  // (D5)
static const int          kDiPin  PROGMEM = 13;  // (D7) Dio 
static const int          kClPin  PROGMEM = 14;  // (D5) Clk 
static const int          kCePin  PROGMEM = 16;  // (D0) Chip Enable 
static const int RtcSquareWavePin PROGMEM = 12;  // (D6) Пин SQW 
static const int        DATA_PIN  PROGMEM = 13;  // (D7)
static const int         CLK_PIN  PROGMEM =  2;  // (D4)
static const int       WRCLK_PIN  PROGMEM = 14;  // (D5)
static const int          CS_PIN  PROGMEM = 16;  // (D0) Chip Enable 
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM)

/*
********************************************************
    LED1        = PB_4,
    LED2        = PB_5,
    LED3        = PB_6,
    LED4        = PB_7,
    USER_BUTTON = PA_3,
    SERIAL_TX   = PA_7,
    SERIAL_RX   = PA_6,
    USBTX       = PA_7,
    USBRX       = PA_6,
    I2C_SCL     = PC_5,
    I2C_SDA     = PC_4,
    SPI_MOSI    = PC_2,
    SPI_MISO    = PC_3,
    SPI_SCK     = PC_1,
    SPI_CS      = PC_0,
    PWM_OUT     = PD_4,
********************************************************
*/

#define         SDA_PIN  12
#define         SCL_PIN  13
#define        uart_pin  0
#define         led_pin  4
#define        RECV_PIN  1
#define     setting_pin  3
#define         BUZ_PIN  11
#define         DHT_PIN  2
#define           pinCS  8
#define      TM1637_DIO  10
#define      TM1637_CLK  9
#define          kDiPin  10
#define          kClPin  9
#define          kCePin  8
#define RtcSquareWavePin 5
#endif

// ----------------------------------- Typedef
#include <Udt.h>

snr_data_t snr_data;
wf_data_t wf_data_cur;
wf_data_t wf_data;
conf_data_t conf_data;
ram_data_t ram_data;
rtc_data_t rtc_data;

// ----------------------------------- Internal header files
#include "disp.h"
#include "rtc.h"
#include "web.h"

// ----------------------------------- Force define func name

void fs_setup();

conf_data_t loadConfig();
void saveConfig();
conf_data_t defaultConfig();

bool sta_init();
bool sta_check();
bool start_client();
void stop_client();

bool ap_init();

void start_wifi();
void stop_wifi();

void GetNtp();
void GetSnr();
String uart_st();
void send_uart();
void keyb_read();
void nm_veri();

void wifi_conn(byte, byte, byte);
String gs_rcv (unsigned long);
String es_rcv (char*);
String ts_rcv (unsigned long, char*);
String ts_snd (String);

// ----------------------------------- NTP
NTPTime NTP_t;

// ----------------------------------- Time
time_t cur_time;

tmElements_t ds_tm_stru;

// ----------------------------------- Web server
#if defined(ESP8266)
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
#endif

#if defined(ESP32)
WebServer server(80);
#endif

// ---------------------------------------------------- HW info
#if defined(ESP8266)
ESP8266HWInfo hwi;
#endif


bool web_ap        = false;
bool web_cli       = false;
unsigned long serv_ms = 60000;

static const char* name_week[] = {"", "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"};

// ---------------------------------------------------- WiFi Default
static const char  ap_ssid_def[] PROGMEM = "WiFi_Clock";
static const char  ap_pass_def[] PROGMEM = "";
static const char sta_ssid_def[] PROGMEM = "My_WiFi";
static const char sta_pass_def[] PROGMEM = "";


// ---------------------------------------------------- Common
const char ntp_server[] = "ru.pool.ntp.org";

bool                disp_on  = true;
bool               nm_is_on  = false;
bool             but0_press  = false;
bool              uart_send  = false;
unsigned long   irq_end[10];
unsigned long    setting_ms  = 0;

uint8_t            hour_cnt  = 0;
uint8_t           disp_mode  = 1;
uint16_t             cur_br  = 0;
uint16_t         cur_br_buf  = 0;

uint8_t         debug_level  = 0; // 0 - отключен

// ---------------------------------------------------- Constructors
IPAddress IP_Addr;

Synt Buzz;               //Конструктор пищалки

# if defined(ESP8266) || defined(ESP32) 
File fsUploadFile;
# endif

ES e_srv;
SF fsys;
NF nsys;
SNR sens;
FD f_dsp;
MSG dmsg;


#endif /* _wc_h */


