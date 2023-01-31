#include "cfg.h"

conf_data_t CFG::from_json(String message)
{
  conf_data_t _data;

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  DynamicJsonDocument doc(300);

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
    uint8_t __pin = doc["p1"];
    _data.pin1 = selector(__pin);
    __pin = doc["p2"];
    _data.pin2 = selector(__pin);

    _data.str1_on  = doc["on1"];
    _data.str1_off = doc["of1"];

    _data.str2_on  = doc["on2"];
    _data.str2_off = doc["of2"];

    _data.lim_l = doc["ll"];
    _data.lim_h = doc["lh"];
  }
  return _data;
}

uint8_t CFG::selector (uint8_t _pin)
{
  bool valid = false;
#if defined(ESP8266)
  const uint8_t gpio[6] = {0, 2, 12, 13, 14, 15};
  for (uint8_t i = 0; i < 6; i++)
  {
    if (_pin == gpio[i]) valid = true;
  }
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  const uint8_t gpio[18] = {0, 2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
  for (uint8_t i = 0; i < 18; i++)
  {
    if (_pin == gpio[i]) valid = true;
  }
#endif

  if (!valid) _pin = 255;
  return _pin;
}

String CFG::to_json(conf_data_t _data)
{
  if (debug_level == 3)
  {
    DBG_OUT_PORT.print(F("Start forming conf_data to json string"));
  }

  DynamicJsonDocument doc(300);
  JsonObject json = doc.to<JsonObject>();

  json["pin1_name"] = _data.pin1;
  json["pin2_name"] = _data.pin2;

  json["on1_code"] = _data.str1_on;
  json["off1_code"] = _data.str1_off;
  json["on2_code"] = _data.str2_on;
  json["off2_code"] = _data.str2_off;

  json["lim_l"] = _data.lim_l;
  json["lim_h"] = _data.lim_h;

  String msg = String();
  if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  return msg;
}

conf_data_t CFG::def_conf()
{
  conf_data_t _data;

  // ---------------------------------------------------- Default

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data"));

  _data.str1_on = 10;
  _data.str1_off = 20;
  _data.str2_on = 30;
  _data.str2_off = 40;
  _data.pin1 = 13;
  _data.pin2 = 12;
  _data.lim_l = 128;
  _data.lim_h = 128;

  return _data;
}
