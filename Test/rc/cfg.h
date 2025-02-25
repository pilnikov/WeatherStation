#ifndef cfg_h
#define cfg_h

// ------------------------------------------------------------- Include

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined(ARDUINO_ARCH_ESP32) || CONFIG_IDF_TARGET_ESP32C3
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#endif

#include <ArduinoJson.h>

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

typedef struct
{
  uint8_t
  str1_on = 10,
  str1_off = 20,
  str2_on = 30,
  str2_off = 40,
  pin1 = 11,
  pin2 = 12,
  lim_l = 128,
  lim_h = 128;
} conf_data_t;


class CFG
{
  public:
    String
    to_json(conf_data_t);

    conf_data_t
    from_json(String),
    def_conf();
  private:
    uint8_t
    selector (uint8_t);
  protected:
};

#endif /*cfg_h*/
