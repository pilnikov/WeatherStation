#ifndef cfg_h
#define cfg_h

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#include <LittleFS.h>
#include <ArduinoJson.h>

#elif defined(__AVR_ATmega2560__)
#include <EEPROM.h>
#endif

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

conf_data_t loadConfig(const char*);
void saveConfig(const char*, conf_data_t);
conf_data_t defaultConfig();

#endif
