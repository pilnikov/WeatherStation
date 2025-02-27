
#include "Snr.h"

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

#define ARDUINOJSON_USE_LONG_LONG 1

#include <ArduinoJson.h>


snr_cfg_t SNRJS::from_json(String message)
{
  snr_cfg_t _data;

    // Allocate the document on the stack.
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    JsonDocument doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F(". Using default configuration!!!"));
      _data = def_conf();
      return _data;
    }
	else
	{
      DBG_OUT_PORT.println(F("Read msg sucsses!!!"));

      //---Sensor type---------------------------------------
      _data.type_snr1         = doc["s1_t"];
      _data.type_snr2         = doc["s2_t"];
      _data.type_snr3         = doc["s3_t"];
      _data.type_snrp         = doc["sp_t"];
    }
	return _data;
}

String SNRJS::to_json(snr_cfg_t _data)
{
  if (debug_level == 3)
  {
    DBG_OUT_PORT.print(F("Start forming snr_cfg_data to json string"));
    DBG_OUT_PORT.print(F("Snr 1 type is "));
    DBG_OUT_PORT.println(_data.type_snr1);
    DBG_OUT_PORT.print(F("Snr 2 type is "));
    DBG_OUT_PORT.println(_data.type_snr2);
    DBG_OUT_PORT.print(F("Snr 3 type is "));
    DBG_OUT_PORT.println(_data.type_snr2);
    DBG_OUT_PORT.print(F("Snr p type is "));
    DBG_OUT_PORT.println(_data.type_snrp);

  }

  if (_data.type_snr1  >  13) _data.type_snr1  = 0;
  if (_data.type_snr2  >  13) _data.type_snr2  = 0;
  if (_data.type_snr3  >  13) _data.type_snr3  = 0;
  if (_data.type_snrp  >  12) _data.type_snrp  = 0;

  JsonDocument doc;
  JsonObject json = doc.to<JsonObject>();

  //---Sensor.html---------------------------------------
  //---Options for sensor--------------------------------
  //---Sensor type---------------------------------------
  json["s1_t"]              = _data.type_snr1;
  json["s2_t"]              = _data.type_snr2;
  json["s3_t"]              = _data.type_snr3;
  json["sp_t"]              = _data.type_snrp;

  String msg = String();
  if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  else
  if (debug_level == 3)
  {
      DBG_OUT_PORT.println(msg);  
  }
  
  return msg;
}

snr_cfg_t SNRJS::def_conf()
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

#include <EEPROM.h>

snr_cfg_t SNRJS::from_json(String message)
{
  snr_cfg_t _data;
  EEPROM.get(0, _data);           // прочитали из адреса 0
  return _data;
}

snr_cfg_t SNRJS::def_conf()
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

String SNRJS::to_json(snr_cfg_t _data)
{
  EEPROM.put(0, _data);           // записали по адресу 0
  String msg = "OK";
  return msg;
}
#endif
