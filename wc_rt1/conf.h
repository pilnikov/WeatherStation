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
  char      sta_ssid[17];
  char      sta_pass[17];
  char      ap_ssid[17];
  char      ap_pass[17];
  uint16_t  br_level[4];
  int8_t    time_zone;
  bool      auto_br;
  bool      auto_corr;
  bool      use_pm;
  bool      every_hour_beep;
  bool      snd_pola;
  bool      led_pola;
  bool      rus_lng;
  bool      use_tst1;
  bool      use_tst2;
  bool      use_tst3;
  bool      use_tsh1;
  bool      use_tsh2;
  bool      use_tsh3;
  bool      use_tsp;
  bool      wifi_off;
  bool      udp_mon;
  uint8_t   use_pp;
  uint8_t   man_br;
  uint8_t   nmd_br;
  uint8_t   nm_start;
  uint8_t   nm_stop;
  uint8_t   alarms[7][5];
  uint8_t   type_font;
  uint8_t   type_vdrv;
  uint8_t   type_disp;
  char      ch1_name[17];
  char      ch2_name[17];
  char      ch3_name[17];
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
  char      srudp_addr[17];
  char      owm_key[35];
  char      test[3];

  uint8_t gpio_sda = 255;
  uint8_t gpio_scl = 255;
  uint8_t gpio_dio = 255;
  uint8_t gpio_clk = 255;
  uint8_t gpio_dcs = 255;
  uint8_t gpio_dwr = 255;
  uint8_t gpio_trm = 255;
  uint8_t gpio_sqw = 255;
  uint8_t gpio_snd = 255;
  uint8_t gpio_led = 255;
  uint8_t gpio_btn = 255;
  uint8_t gpio_dht = 255;
  uint8_t gpio_ana = 255;
  uint8_t gpio_uar = 255;
  uint8_t gpio_bz2 = 255;

  };
********************************************************** Ram config
  struct ram_data_t
  {
  uint8_t   type_vdrv;      // Тип микросхемы драйвера дисплея 0 - Нет, 1 - TM1637, 2 - MAX7219, 3 - 74HC595, 4 - HT1621, 5 - HT1632, 6 - ILI9341, 11 - HT16K33, 12 - PCF8574
  uint8_t   type_disp;      // Тип дисплея 0 - Внешний, 1 - 7SEGx4D, 2 - 7SEGx6D, 3 - 7SEGx8D, 10 - 14SEGx4D, 11 - 14SEGx8D, 12 - 16SEGx4D, 13 - 16SEGx8D, 19 - 2Linex16D, 20 - M32x8Mono, 21 - M32x16Mono, 22 - M32x16BiColor, 23 - M32x16Color, 24 - M64x32Color, 25 - M64x64Color, 29 - 320x240Color, 30 - Custom_1, 31 - Custom_2
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
  };
********************************************************* Rtc
  struct rtc_data_t
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


#ifndef _wc_h
#define _wc_h

#define FW_Ver 1.0 //16.05.17 added udp debug console

// ------------------------------------------------------------- Include
//#include "..\lib\MyLib_Udt\Udt.h"

#include "Udt.h"

#if defined(__AVR_ATmega2560__)
#include <EEPROM.h>
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM)
#include <WiFi.h>
#include <OTA.h>
#include <HttpClient.h>
#include <WiFiClient.h>

#include "Netwf_rt.h"

#endif

#if defined(ESP8266)
#include <pgmspace.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
//#include <ESPAsyncTCP.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>

//#include "..\lib\MyLib_hw\src\hw.h"
//#include "..\lib\MyLib_nf\src\Netwf.h"

#include "hw.h"
#endif

#if defined(ARDUINO_ARCH_ESP32)
#include <pgmspace.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Update.h>
#include <ESP32SSDP.h>
#include <LITTLEFS.h>
//#include <IRremote.h>

#include <ArduinoOTA.h>

//#include "..\lib\MyLib_nf\src\Netwf.h"
#endif


#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <Wire.h>
#include <SPI.h>

#if defined(__xtensa__)
#include "Netwf.h"
#include <FS.h>
#include <WiFiUdp.h>
#endif
/*
  #include "..\lib\MyLib_sf2\src\Sysf2.h"
  #include "..\lib\MyLib_snd\src\Snd.h"
  #include "..\lib\MyLib_ntp\src\ntp.h"
  #include "..\lib\MyLib_es\src\Exts.h"
  #include "..\lib\MyLib_snr\src\Snr.h"
  #include "..\lib\MyLib_fdsp\src\Fdsp.h"
  #include "..\lib\BH1750-master\BH1750.h"
*/

#include "Sysf2.h"
#include "Snd.h"
#include "Songs.h"
#include "Snr.h"
#include "Fdsp.h"
#include "BH1750.h"

#if defined(__xtensa__)
#include "ntp.h"
#include "Exts.h"

// ------------------------------------------------------ ConsoleUDP
#include <udp_cons.h>
udp_cons print_console_udp;
#endif

#define DBG_OUT_PORT Serial

// ------------------------------------------------------ GPIO
#if defined(ARDUINO_ARCH_ESP32)
static const int           A_PIN  PROGMEM =   2;  // Пин A
static const int           B_PIN  PROGMEM =  16;  // Пин B
static const int           C_PIN  PROGMEM =  14;  // Пин C
static const int           D_PIN  PROGMEM =  13;  // Пин D

static const int         CLK_PIN  PROGMEM =  14;  // Пин CLK MUST be on PORTB! (Use pin 11 on Mega)
static const int         LAT_PIN  PROGMEM =  27;  // Пин LAT
static const int          OE_PIN  PROGMEM =  32;  // Пин OE
#endif


#if defined(__AVR_ATmega2560__)
static const int           A_PIN  PROGMEM =  A0;  // Пин A
static const int           B_PIN  PROGMEM =  A1;  // Пин B
static const int           C_PIN  PROGMEM =  A2;  // Пин C
static const int           D_PIN  PROGMEM =  A3;  // Пин D

static const int         CLK_PIN  PROGMEM =  11;  // Пин CLK MUST be on PORTB! (Use pin 11 on Mega)
static const int         LAT_PIN  PROGMEM =  10;  // Пин LAT
static const int          OE_PIN  PROGMEM =   9;  // Пин OE

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

#endif

// ----------------------------------- Typedef

snr_data_t snr_data;
wf_data_t wf_data_cur;
wf_data_t wf_data;
conf_data_t conf_data;
ram_data_t ram_data;
rtc_data_t rtc_data;

// ----------------------------------- Internal header files
#include "disp.h"
#include "rtc.h"

#if defined(__xtensa__)
#include "web.h"
#endif
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
snr_data_t GetSnr(ram_data_t, conf_data_t);
String uart_st(snr_data_t, wf_data_t, conf_data_t, rtc_data_t, uint8_t);
void send_uart();
void keyb_read();
inline uint8_t rumb_conv(uint16_t);
String remove_sb(String);
String tvoday(String);
void Thermo(snr_data_t, conf_data_t);

void wifi_conn(byte, byte, byte);
String gs_rcv (unsigned long);
String es_rcv (char*);
String ts_rcv (unsigned long, char*);
String ts_snd (String);

// ----------------------------------- NTP
#if defined(__xtensa__)
static NTPTime NTP_t;
#endif

// ----------------------------------- Web server
#if defined(ESP8266)
static ESP8266WebServer server(80);
static ESP8266HTTPUpdateServer httpUpdater;
#endif

#if defined(ARDUINO_ARCH_ESP32)
static WebServer server(80);
#endif

// ---------------------------------------------------- HW info
#if defined(ESP8266)
ESP8266HWInfo hwi;
#endif

// ---------------------------------------------------- LM

BH1750 lightMeter;


// ---------------------------------------------------- Common
# if defined(__xtensa__)
const char ntp_server[] = "ru.pool.ntp.org";
#endif

const char *conf_f = "/config.json";  // config file name

bool               play_snd  = false;

bool                web_ap   = false;
bool                web_cli  = false;

bool                disp_on  = true;
bool               nm_is_on  = false;

unsigned long   irq_end[10];
unsigned long   serv_ms = 60000;

uint8_t            hour_cnt  = 0;
uint8_t           disp_mode  = 0;
uint16_t             cur_br  = 0;

uint8_t         debug_level  = 0; // 0 - отключен

// ---------------------------------------------------- Constructors

Synt Buzz;               //Конструктор пищалки

# if defined(__xtensa__)
IPAddress IP_Addr;
File fsUploadFile;

ES e_srv;
NF nsys;
# endif

SF fsys;
SNR sens;
FD f_dsp;
HT h_dsp;
MSG dmsg;

#endif /* _wc_h */
