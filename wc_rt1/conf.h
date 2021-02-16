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
  bool      rus_lng;
  bool      use_tst1;
  bool      use_tst2;
  bool      use_tst3;
  bool      use_tsh1;
  bool      use_tsh2;
  bool      use_tsh3;
  bool      use_tsp;
  bool      use_es;
  uint8_t   use_pp;
  uint8_t   man_br;
  uint8_t   nm_start;
  uint8_t   nm_stop;
  uint8_t   alarms[7][5];
  uint8_t   type_font;
  uint8_t   type_vdrv;
  uint8_t   type_disp;
  char      ch1_name[8];
  char      ch2_name[8];
  char      ch3_name[8];
  uint8_t   type_snr1;
  uint8_t   type_snr2;
  uint8_t   type_snr3;
  uint8_t   type_snrp;
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
  char      esrv1_addr[17];
  char      esrv2_addr[17];
  char      radio_addr[17];
  char      owm_key[35];
  char      test[3];
  };
********************************************************** Ram config
  struct ram_data_t
  {
  uint8_t   type_vdrv;      // Тип микросхемы драйвера дисплея 0 - Нет, 1 - TM1637, 2 - MAX7219, 3 - 74HC595, 4 - HT1621, 5 - HT1632, 6 - ILI9341, 11 - HT16K33, 12 - PCF8574
  uint8_t   type_disp;      // Тип дисплея 0 - Внешний, 1 - 7SEGx4D, 2 - 7SEGx6D, 3 - 7SEGx8D, 10 - 14SEGx4D, 11 - 14SEGx8D, 12 - 16SEGx4D, 13 - 16SEGx8D, 19 - 2Linex16D, 20 - M32x8Mono, 21 - M32x16Mono, 22 - M32x16BiColor, 23 - M32x16Color, 24 - M64x32Color, 25 - M64x64Color, 29 - 320x240Color, 30 - Custom_1, 31 - Custom_2
  uint8_t   type_snr1;      // Тип датчика канал 1: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snr2;      // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snr3;      // Тип датчика канал 2: 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 2 - Взять с ES2, 4 - DHT21, 5 - DS3231, 6 - SI7021, 7 - AM2320, 8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
  uint8_t   type_snrp;      // Тип датчика давления 0 - Нет, 1 - Взять с TC, 2 - Взять с ES1, 3 - Взять с ES2,                                                                8 - BMP180, 9 - BMP280, 10 - BME280, 11 - Взять из прогноза
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
//#include <WiFiManager.h>
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
#include <BH1750.h>

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
#if defined(ESP8266)
static const int     setting_pin  PROGMEM =  0;  // (D3)
static const int         DHT_PIN  PROGMEM =  0;  // (D3) Pin which is connected to the DHT sensor.
static const int       WRCLK_PIN  PROGMEM =  2;  // (D5)
static const int         SDA_PIN  PROGMEM =  4;  // (D2)
static const int         SCL_PIN  PROGMEM =  5;  // (D1)
static const int         SQW_PIN  PROGMEM = 12;  // (D6) Пин RtcSquareWave
static const int         DIO_PIN  PROGMEM = 13;  // (D7)
static const int         CLK_PIN  PROGMEM = 14;  // (D5) Clk
static const int         BUZ_PIN  PROGMEM = 15;  // (D8) Пин пищалки
static const int          CS_PIN  PROGMEM = 16;  // (D0) CS . DIN 13 (D7) CLK 14 (D5)
static const int        uart_pin  PROGMEM = 16;  // (D0)
static const int         ANA_SNR  PROGMEM = A0;  // (A0) Пин фоторезистора
static const int       TERMO_OUT  PROGMEM =  2;  // (D5) Выход термостата
#endif

// ------------------------------------------------------ GPIO
#if defined(ESP32)
static const int     setting_pin  PROGMEM =   0;  // (D3)
static const int       WRCLK_PIN  PROGMEM =   2;  // (D5)
static const int         BUZ_PIN  PROGMEM =   3;  // (D8) Канал PWM
static const int         SQW_PIN  PROGMEM =  12;  // (D6) Пин RtcSquareWave
static const int         DIO_PIN  PROGMEM =  13;  // (D7)
//static const int         CLK_PIN  PROGMEM =  14;  // (D5) Clk
static const int          CS_PIN  PROGMEM =  15;  // (D0) CS . DIN 13 (D7) CLK 14 (D5)
static const int        uart_pin  PROGMEM =  16;  // (D0)
static const int         SDA_PIN  PROGMEM =  21;  // (D2)
static const int         SCL_PIN  PROGMEM =  22;  // (D1)
static const int        BUZ2_PIN  PROGMEM =  32;  // (D8) Пин пищалки
static const int         CLK_PIN  PROGMEM =  33;  // (D4)
static const int         DHT_PIN  PROGMEM =  36;  // (D3) Pin which is connected to the DHT sensor.
static const int     LED_BUILTIN  PROGMEM =   5;  // (D3)
static const int         ANA_SNR  PROGMEM =  39;  // (D3) Пин фоторезистора
static const int       TERMO_OUT  PROGMEM =   2;  // (D5) Выход термостата
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

#define        uart_pin  0
#define         DHT_PIN  2
#define     setting_pin  3
#define         SQW_PIN  5
#define          CS_PIN  8
#define         CLK_PIN  9
#define         DIO_PIN  10
#define         BUZ_PIN  11
#define         SDA_PIN  12
#define         SCL_PIN  13
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

void printFile(const char);
void fs_setup();

conf_data_t loadConfig(const char);
void saveConfig(const char, conf_data_t);
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
inline uint8_t rumb_conv(uint16_t);
String remove_sb(String);
String tvoday(String);
void nm_veri();
void Thermo();

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

// ---------------------------------------------------- LM

BH1750 lightMeter;

// ---------------------------------------------------- 


bool web_ap        = false;
bool web_cli       = false;
unsigned long serv_ms = 60000;

char tstr[64];
static const char* name_week[]  = {"", "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"};
static const char* name_week7[] = {"", "8c", "пH", "8t", "CP", "4t", "пt", "c6"};
// ---------------------------------------------------- WiFi Default
static const char  ap_ssid_def[] PROGMEM = "WiFi_Clock";
static const char  ap_pass_def[] PROGMEM = "";
static const char sta_ssid_def[] PROGMEM = "My_WiFi";
static const char sta_pass_def[] PROGMEM = "";


// ---------------------------------------------------- Common
const char *conf_f = "/config.json";  // config file name

const char ntp_server[] = "ru.pool.ntp.org";

bool                disp_on  = true;
bool               nm_is_on  = false;
bool             but0_press  = false;
bool              uart_send  = false;
unsigned long   irq_end[10];
unsigned long    setting_ms  = 0;

uint8_t            hour_cnt  = 0;
uint8_t           disp_mode  = 0;
uint8_t           disp_mode2 = 0;
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

// ---------------------------------------------------- Songs

const char *songs[] = {":d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6,x", //The Simpsons
                       ":d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6,x",//Indiana
                       ":d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6,x", //Entertainer
                       ":d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f,x",//Looney
                       ":d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6,x",//Bond
                       ":d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b,x",//MASH
                       ":d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#,x6",//StarWars
                       ":d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#,x",//GoodBad
                       ":d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#,x",//TopGun
                       ":d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#,x",//A-Team
                       ":d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6,x",//Flinstones
                       ":d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#,x",//Jeopardy
                       ":d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.",//MahnaMahna
                       ":d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d,x",//MissionImp
                       ":d=4,o=5,b=125:16e,16p,16f,16e,16e,16p,16e,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16e,16p,16f,16e,16e,16p,16f,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d,x",//KnightRider
                       ":d=16,o=6,b=95:d#,8d,x" //For inital pointer
                      };


#endif /* _wc_h */
