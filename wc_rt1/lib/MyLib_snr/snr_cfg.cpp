
#include "Snr.h"

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

#define ARDUINOJSON_USE_LONG_LONG 1
#include <pgmspace.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

snr_cfg_t CfgSNR::loadCfgSnr(const char *filename)
{
  snr_cfg_t _data;

  File file = LittleFS.open(filename, "r");

  if (!file)
  {
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(filename);
    DBG_OUT_PORT.println(F(". Using default configuration!!!"));
    _data = defaultCfgSnr();
    saveCfgSnr(filename, _data);
  }
  else
  {
    // Allocate the document on the stack.
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    DynamicJsonDocument doc(3100);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
      DBG_OUT_PORT.print(F("deserializeJson() for "));
      DBG_OUT_PORT.print(filename);
      DBG_OUT_PORT.print(F(" failed: "));
      DBG_OUT_PORT.print(error.c_str());
      DBG_OUT_PORT.println(F(". Using default configuration!!!"));
      _data = defaultCfgSnr();
      saveCfgSnr(filename, _data);
      return _data;
    }

    file.close();

    if (!error)
    {
      DBG_OUT_PORT.print(F("Read "));
      DBG_OUT_PORT.print(filename);
      DBG_OUT_PORT.println(F(" sucsses!!!"));
      //---Sensor type---------------------------------------
      _data.type_snr1         = doc["snr1_t"];
      _data.type_snr2         = doc["snr2_t"];
      _data.type_snr3         = doc["snr3_t"];
      _data.type_snrp         = doc["snrp_t"];
    }
    else
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.print(F("Failed to read "));
      DBG_OUT_PORT.print(filename);
      DBG_OUT_PORT.println(F(" - Using default configuration!!!"));
      _data = defaultCfgSnr();
      saveCfgSnr(filename, _data);
    }
  }
  return _data;
}
void CfgSNR::saveCfgSnr(const char *filename, snr_cfg_t _data)
{
  if (debug_level == 3)
  {
    DBG_OUT_PORT.print(F("Start saving snr_cfg_data to "));
    DBG_OUT_PORT.println(filename);
  }

  if ( _data.type_snr1    < 0  || _data.type_snr1  >  13) _data.type_snr1  = 0;
  if ( _data.type_snr2    < 0  || _data.type_snr2  >  13) _data.type_snr2  = 0;
  if ( _data.type_snr3    < 0  || _data.type_snr3  >  13) _data.type_snr3  = 0;
  if ( _data.type_snrp    < 0  || _data.type_snrp  >  12) _data.type_snrp  = 0;

  DynamicJsonDocument doc(3000);
  JsonObject json = doc.to<JsonObject>();

  //---Sensor.html---------------------------------------
  //---Options for sensor--------------------------------
  //---Sensor type---------------------------------------
  json["snr1_t"]              = _data.type_snr1;
  json["snr2_t"]              = _data.type_snr2;
  json["snr3_t"]              = _data.type_snr3;
  json["snrp_t"]              = _data.type_snrp;

  // Delete existing file, otherwise the configuration is appended to the file
  LittleFS.remove(filename);
  File configFile = LittleFS.open(filename, "w"); //Open config file for writing
  if (!configFile)
  {
    DBG_OUT_PORT.print(F("Failed open "));
    DBG_OUT_PORT.print(filename);
    DBG_OUT_PORT.println(F(" for writing"));
    return;
  }
  if (serializeJson(doc, configFile) == 0)
  {
    DBG_OUT_PORT.print(F("Failed write to "));
    DBG_OUT_PORT.println(filename);
    return;
  }
  DBG_OUT_PORT.print(F("End write buffer to "));
  DBG_OUT_PORT.println(filename);
  configFile.close();
}

snr_cfg_t CfgSNR::defaultCfgSnr()
{
  snr_cfg_t _data;

  // ---------------------------------------------------- WiFi Default

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital snr_cfg_data"));

  _data.type_snr1        = 0;
  _data.type_snr2        = 0;
  _data.type_snr3        = 0;
  _data.type_snrp        = 0;

  return _data;
}

#elif defined(__AVR_ATmega2560__)

#include <Snr.h>

#include <EEPROM.h>

snr_cfg_t CfgSNR::loadCfgSnr(const char *filename)
{
  snr_cfg_t _data;
  EEPROM.get(0, _data);           // прочитали из адреса 0
  return _data;
}

snr_cfg_t CfgSNR::defaultCfgSnr()
{
  snr_cfg_t _data;
  if (debug_level == 3)
  {
    DBG_OUT_PORT.println(F("Start inital snr_cfg_data"));
  }


#if defined _dacha
  _data.type_snr1        = 4;
  _data.type_snr2        = 6;
  _data.type_snr3        = 10;
  _data.type_snrp        = 10;

#elif defined _work
  _data.type_snr1        = 10;
  _data.type_snr2        = 4;
  _data.type_snr3        = 0;
  _data.type_snrp        = 10;
#endif

  return _data;
}

void CfgSNR::saveCfgSnr(const char *filename, snr_cfg_t _data)
{
  EEPROM.put(0, _data);           // записали по адресу 0
}
#endif
