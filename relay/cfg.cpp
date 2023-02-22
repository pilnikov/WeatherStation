#include "cfg.h"

conf_data_t CFG::from_json(String message) {
  conf_data_t _data;

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  DynamicJsonDocument doc(500);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
    DBG_OUT_PORT.println(F(". Using default configuration!!!"));
    _data = def_conf();
    return _data;
  } else {
    DBG_OUT_PORT.println(F("Read msg sucsses!!!"));
    uint8_t __pin = doc["ch1_gpio"];
    _data.ch1_gpio = selector(__pin);
    _data.ch1_in = doc["ch1_type"];
    _data.ch1_dig = doc["ch1_dig"];
    _data.ch1_on_code = doc["ch1_on_code"];
    _data.ch1_off_code = doc["ch1_off_code"];

    __pin = doc["ch2_gpio"];
    _data.ch2_gpio = selector(__pin);
    _data.ch2_in = doc["ch2_type"];
    _data.ch2_dig = doc["ch2_dig"];
    _data.ch2_on_code = doc["ch2_on_code"];
    _data.ch2_off_code = doc["ch2_off_code"];

    _data.lim_l = doc["lim_l"];
    _data.lim_h = doc["lim_h"];
  }
  return _data;
}

uint8_t CFG::selector(uint8_t _pin) {
  bool valid = false;
#if defined(ESP8266)
  const uint8_t gpio[6] = { 0, 2, 12, 13, 14, 15 };
  for (uint8_t i = 0; i < 6; i++) {
    if (_pin == gpio[i]) valid = true;
  }
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  const uint8_t gpio[18] = { 0, 2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
  for (uint8_t i = 0; i < 18; i++) {
    if (_pin == gpio[i]) valid = true;
  }
#endif

  if (!valid) _pin = 255;
  return _pin;
}

String CFG::to_json(conf_data_t _data) {
  if (debug_level == 3) {
    DBG_OUT_PORT.print(F("Start forming conf_data to json string"));
  }

  DynamicJsonDocument doc(500);
  JsonObject json = doc.to<JsonObject>();

  json["ch1_gpio"] = _data.ch1_gpio;
  json["ch1_type"] = _data.ch1_in;
  json["ch1_dig"] = _data.ch1_dig;

  json["ch1_on_code"] = _data.ch1_on_code;
  json["ch1_off_code"] = _data.ch1_off_code;

  json["ch2_gpio"] = _data.ch2_gpio;
  json["ch2_type"] = _data.ch2_in;
  json["ch2_dig"] = _data.ch2_dig;

  json["ch2_on_code"] = _data.ch2_on_code;
  json["ch2_off_code"] = _data.ch2_off_code;

  json["lim_l"] = _data.lim_l;
  json["lim_h"] = _data.lim_h;

  String msg = String();
  if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  return msg;
}

conf_data_t CFG::def_conf() {
  conf_data_t _data;

  // ---------------------------------------------------- Default

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data"));

  _data.ch1_on_code = 10;
  _data.ch1_off_code = 20;
  _data.ch2_on_code = 30;
  _data.ch2_off_code = 40;
  _data.ch1_gpio = 255;
  _data.ch2_gpio = 255;
  _data.lim_l = 128;
  _data.lim_h = 128;
  _data.ch1_in = false;
  _data.ch2_in = false;
  _data.ch1_dig = true;
  _data.ch2_dig = true;

  return _data;
}
