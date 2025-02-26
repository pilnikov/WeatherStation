
#include "conf.h"
#include <ArduinoJson.h>

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#define ARDUINOJSON_USE_LONG_LONG 1

main_cfg_t MAINJS::from_json(String message) {
  main_cfg_t mcf = MAINJS::def_conf();

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  JsonDocument doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    DBG_OUT_PORT.print(F("read main_cf string failed: "));
    DBG_OUT_PORT.println(error.c_str());
    DBG_OUT_PORT.println(F(". Using default configuration"));
  } else {
    DBG_OUT_PORT.println(F("Read msg sucsses!!!"));

    // Get the root object in the document
    memset(mcf.owm_key, 0, 33);
    memset(mcf.esrv1_addr, 0, 17);
    memset(mcf.esrv2_addr, 0, 17);
    memset(mcf.radio_addr, 0, 17);
    memset(mcf.srudp_addr, 0, 17);
    memset(mcf.ch1_name, 0, 17);
    memset(mcf.ch2_name, 0, 17);
    memset(mcf.ch3_name, 0, 17);
    memset(mcf.AKey_r, 0, 17);
    memset(mcf.AKey_w, 0, 17);
    memset(mcf.news_api_key, 0, 33);
    memset(mcf.news_source, 0, 17);

    //---Display.html--------------------------------------
    //---Options for display-------------------------------
    mcf.vdrv_t = doc["vdrv_t"];
    mcf.dsp_t = doc["dsp_t"];
    mcf.rus_lng = doc["rus_lng"];
    mcf.time_up = doc["t_up"];
    mcf.color_up = doc["colu"];
    mcf.color_dwn = doc["cold"];
    mcf.font_t = doc["font_t"];

    //---Brigthness----------------------------------------
    mcf.man_br = doc["man_br"];
    mcf.nmd_br = doc["nmd_br"];
    mcf.auto_br = doc["auto_br"];

    //---Brigthness levels---------------------------------
    for (uint8_t i = 0; i <= 3; i++) mcf.br_level[i] = doc["br_level"][i];

    //---Sensor.html---------------------------------------
    //---Options for sensor--------------------------------
    mcf.pp_city_id = doc["pp_city_id"];
    strcpy(mcf.owm_key, doc["owm_key"]);
    strcpy(mcf.esrv1_addr, doc["esrv1_a"]);
    strcpy(mcf.esrv2_addr, doc["esrv2_a"]);
    strcpy(mcf.radio_addr, doc["radio_a"]);
    strcpy(mcf.srudp_addr, doc["srudp_a"]);
    mcf.udp_mon = doc["udm"];
    mcf.use_pp = doc["upp"];

    //---Sensor type---------------------------------------
    mcf.period = doc["period"];  // minutes
    mcf.use_es = doc["ues"];
    mcf.esm = doc["esm"];

    //---Sensor actual value-------------------------------
    strcpy(mcf.ch1_name, doc["ch1_name"]);
    strcpy(mcf.ch2_name, doc["ch2_name"]);
    strcpy(mcf.ch3_name, doc["ch3_name"]);

    //---TS Account----------------------------------------
    mcf.ts_ch_id = doc["ts_ch_id"];
    strcpy(mcf.AKey_r, doc["AKey_r"]);
    strcpy(mcf.AKey_w, doc["AKey_w"]);

    //---TS sender-----------------------------------------
    mcf.use_ts = doc["uts"];

    //---Thermo.html---------------------------------------
    //---Options for thermostat----------------------------
    mcf.thermo_t = doc["trs_t"];
    mcf.src_thermo = doc["src_trs"];
    mcf.lb_thermo = doc["lb_trs"];
    mcf.hb_thermo = doc["hb_trs"];

    //---News.html---------------------------------------
    //---Options for news----------------------------
    mcf.news_en = doc["news_en"];
    strcpy(mcf.news_api_key, doc["news_api"]);
    strcpy(mcf.news_source, doc["news_src"]);
  }
  return mcf;
}


gpio_cfg_t GPIOJS::from_json(String message) {
  gpio_cfg_t gcf = GPIOJS::def_conf();

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  JsonDocument doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    DBG_OUT_PORT.print(F("read gpio_cfg string failed: "));
    DBG_OUT_PORT.println(error.c_str());
    DBG_OUT_PORT.println(F(". Using default configuration"));
  } else {
    DBG_OUT_PORT.println(F("Read cfg sucsses!!!"));

    //---gpio.html----------------------------------------
    //---Options for HW---------------------------------
    gcf.snd_pola = doc["snd_pola"];
    gcf.led_pola = doc["led_pola"];

    //---GPIO-----------------------------------------------
    gcf.gpio_sda = doc["sda"];
    gcf.gpio_scl = doc["scl"];
    gcf.gpio_dio = doc["dio"];
    gcf.gpio_clk = doc["clk"];
    gcf.gpio_dcs = doc["dcs"];
    gcf.gpio_dwr = doc["dwr"];
    gcf.gpio_trm = doc["trm"];
    gcf.gpio_sqw = doc["sqw"];
    gcf.gpio_snd = doc["snd"];
    gcf.gpio_led = doc["led"];
    gcf.gpio_btn = doc["btn"];
    gcf.gpio_dht = doc["dht"];
    gcf.gpio_ana = doc["ana"];
    gcf.gpio_uar = doc["uar"];
  }
  return gcf;
}

String MAINJS::to_json(main_cfg_t mcf) {
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start forming main_cfg to json"));

  if (mcf.vdrv_t > 20) mcf.vdrv_t = 0;
  if (mcf.dsp_t > 50) mcf.dsp_t = 0;

  JsonDocument doc;
  JsonObject json = doc.to<JsonObject>();

  //---Display.html--------------------------------------
  //---Options for display-------------------------------
  json["vdrv_t"] = mcf.vdrv_t;
  json["dsp_t"] = mcf.dsp_t;
  json["rus_lng"] = mcf.rus_lng;
  json["t_up"] = mcf.time_up;
  json["colu"] = mcf.color_up;
  json["cold"] = mcf.color_dwn;
  json["font_t"] = mcf.font_t;

  //---Brigthness----------------------------------------
  json["man_br"] = mcf.man_br;
  json["nmd_br"] = mcf.nmd_br;
  json["auto_br"] = mcf.auto_br;

  //---Brigthness levels---------------------------------
  JsonArray br_level = doc["br_level"].to<JsonArray>();
  for (uint8_t i = 0; i <= 3; i++) br_level.add(mcf.br_level[i]);

  //---Sensor.html---------------------------------------
  //---Options for sensor--------------------------------
  json["pp_city_id"] = mcf.pp_city_id;
  json["owm_key"] = mcf.owm_key;
  json["esrv1_a"] = mcf.esrv1_addr;
  json["esrv2_a"] = mcf.esrv2_addr;
  json["radio_a"] = mcf.radio_addr;
  json["srudp_a"] = mcf.srudp_addr;
  json["udm"] = mcf.udp_mon;
  json["upp"] = mcf.use_pp;

  //---Sensor type---------------------------------------
  json["period"] = mcf.period;  // minutes
  json["ues"] = mcf.use_es;
  json["esm"] = mcf.esm;

  //---Sensor channel name -------------------------------
  json["ch1_name"] = mcf.ch1_name;
  json["ch2_name"] = mcf.ch2_name;
  json["ch3_name"] = mcf.ch3_name;

  //---TS Account----------------------------------------
  json["ts_ch_id"] = mcf.ts_ch_id;
  json["AKey_r"] = mcf.AKey_r;
  json["AKey_w"] = mcf.AKey_w;

  //---TS sender-----------------------------------------
  json["uts"] = mcf.use_ts;

  //---Thermo.html---------------------------------------
  //---Options for thermostat----------------------------
  json["trs_t"] = mcf.thermo_t;
  json["src_trs"] = mcf.src_thermo;
  json["lb_trs"] = mcf.lb_thermo;
  json["hb_trs"] = mcf.hb_thermo;


  //---News.html---------------------------------------
  //---Options for news----------------------------
  json["news_en"] = mcf.news_en;
  json["news_api"] = mcf.news_api_key;
  json["news_src"] = mcf.news_source;

  String msg = String();
  if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed forming json string"));
  return msg;
}

String GPIOJS::to_json(gpio_cfg_t gcf) {
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start forming main_cfg to json"));

  JsonDocument doc;
  JsonObject json = doc.to<JsonObject>();

  //---gpio.html----------------------------------------
  //---Options for HW---------------------------------
  json["snd_pola"] = gcf.snd_pola;
  json["led_pola"] = gcf.led_pola;

  //---GPIO-----------------------------------------------
  json["sda"] = gcf.gpio_sda;
  json["scl"] = gcf.gpio_scl;
  json["dio"] = gcf.gpio_dio;
  json["clk"] = gcf.gpio_clk;
  json["dcs"] = gcf.gpio_dcs;
  json["dwr"] = gcf.gpio_dwr;
  json["trm"] = gcf.gpio_trm;
  json["sqw"] = gcf.gpio_sqw;
  json["snd"] = gcf.gpio_snd;
  json["led"] = gcf.gpio_led;
  json["btn"] = gcf.gpio_btn;
  json["dht"] = gcf.gpio_dht;
  json["ana"] = gcf.gpio_ana;
  json["uar"] = gcf.gpio_uar;

  String msg = String();
  if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed forming json string"));
  return msg;
}


// ---------------------------------------------------- MCF Default
main_cfg_t MAINJS::def_conf() {
  main_cfg_t mcf;

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital main_cfg"));

  memset(mcf.AKey_r, 0, 17);
  memset(mcf.AKey_w, 0, 17);
  strcpy(mcf.esrv1_addr, "192.168.1.100");
  strcpy(mcf.esrv2_addr, "192.168.1.200");
  strcpy(mcf.radio_addr, "192.168.1.33");
  strcpy(mcf.srudp_addr, "192.168.1.30");
  memset(mcf.owm_key, 0, 33);
  strcpy(mcf.ch1_name, "Внутри");
  strcpy(mcf.ch2_name, "Снаружи");
  strcpy(mcf.ch3_name, "В бане");
  memset(mcf.news_api_key, 0, 33);
  strcpy(mcf.news_source, "lenta");


  mcf.rus_lng = false;
  mcf.time_up = false;
  mcf.udp_mon = false;
  mcf.esm = false;
  mcf.auto_br = false;
  mcf.color_up = 0;
  mcf.color_dwn = 0;
  mcf.use_pp = 0;
  mcf.vdrv_t = 0;
  mcf.dsp_t = 0;
  mcf.use_es = 0;
  mcf.use_ts = 0;
  mcf.thermo_t = 0;
  mcf.src_thermo = 0;
  mcf.lb_thermo = 0;
  mcf.hb_thermo = 0;
  mcf.ts_ch_id = 0;
  mcf.pp_city_id = 28438;
  mcf.period = 10;
  mcf.man_br = 7;
  mcf.nmd_br = 2;

  mcf.br_level[0] = 1;
  mcf.br_level[1] = 200;
  mcf.br_level[2] = 1;
  mcf.br_level[3] = 15;

  mcf.font_t = 0;

  mcf.news_en = false;

  return mcf;
}

// ---------------------------------------------------- GCF Default
gpio_cfg_t GPIOJS::def_conf() {
  gpio_cfg_t gcf;

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital gpio_cfg"));

  gcf.snd_pola = false;
  gcf.led_pola = false;

#if defined(ESP8266)
  gcf.gpio_sda = 4;
  gcf.gpio_scl = 5;
  gcf.gpio_dio = 13;
  gcf.gpio_clk = 14;
  gcf.gpio_dcs = 16;
  gcf.gpio_dwr = 2;
  gcf.gpio_trm = 2;
  gcf.gpio_sqw = 12;
  gcf.gpio_snd = 15;
  gcf.gpio_led = 2;
  gcf.gpio_btn = 0;
  gcf.gpio_dht = 0;
  gcf.gpio_ana = 17;
  gcf.gpio_uar = 16;

#elif CONFIG_IDF_TARGET_ESP32
  gcf.gpio_sda = 23;
  gcf.gpio_scl = 22;
  gcf.gpio_dio = 19;
  gcf.gpio_clk = 18;
  gcf.gpio_dcs = 5;
  gcf.gpio_dwr = 26;
  gcf.gpio_trm = 36;
  gcf.gpio_sqw = 34;
  gcf.gpio_snd = 2;
  gcf.gpio_led = 16;
  gcf.gpio_btn = 0;
  gcf.gpio_dht = 39;
  gcf.gpio_ana = 35;
  gcf.gpio_uar = 36;

#elif CONFIG_IDF_TARGET_ESP32C3
  gcf.gpio_sda = 5;
  gcf.gpio_scl = 6;
  gcf.gpio_dio = 7;
  gcf.gpio_clk = 8;
  gcf.gpio_dcs = 5;
  gcf.gpio_dwr = 0;
  gcf.gpio_trm = 10;
  gcf.gpio_sqw = 1;
  gcf.gpio_snd = 2;
  gcf.gpio_led = 3;
  gcf.gpio_btn = 9;
  gcf.gpio_dht = 10;
  gcf.gpio_ana = 10;
  gcf.gpio_uar = 10;
#endif
  return gcf;
}

#elif defined(__AVR_ATmega2560__)

#include <EEPROM.h>

main_cfg_t MAINJS::from_json(String message) {
  main_cfg_t mcf;
  EEPROM.get(0, mcf);  // прочитали из адреса 0
  return mcf;
}

main_cfg_t MAINJS::def_conf() {
  main_cfg_t mcf;
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital main_cfg"));

#if defined _dacha
  strcpy(mcf.ch1_name, "В избе");
  strcpy(mcf.ch2_name, "На улице");
  strcpy(mcf.ch3_name, "Подпол");
#elif defined _work
  strcpy(mcf.ch1_name, "Внутри");
  strcpy(mcf.ch2_name, "На улице");
  memset(mcf.ch3_name, 0, 17);
#endif
  mcf.rus_lng = true;

#if defined _dacha
  mcf.vdrv_t = 2;
  mcf.dsp_t = 21;

#elif defined _work
  mcf.vdrv_t = 3;
  mcf.dsp_t = 23;

  mcf.type_snr1 = 10;
  mcf.type_snr2 = 4;
  mcf.type_snr3 = 0;
  mcf.type_snrp = 10;
#endif

  mcf.thermo_t = 0;
  mcf.src_thermo = 0;
  mcf.lb_thermo = 0;
  mcf.hb_thermo = 0;
  mcf.period = 10;
  mcf.man_br = 14;
  mcf.nmd_br = 2;
  mcf.esm = false;
  mcf.auto_br = true;

#if defined _dacha
  mcf.br_level[0] = 220;
  mcf.br_level[1] = 1;
  mcf.br_level[2] = 1;
  mcf.br_level[3] = 14;

#elif defined _work
  mcf.br_level[0] = 300;
  mcf.br_level[1] = 1;
  mcf.br_level[2] = 1;
  mcf.br_level[3] = 254;
#endif

  return mcf;
}

gpio_cfg_t GPIOJS::def_conf() {
  gpio_cfg_t gcf;
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital gpio_cfg"));

  gcf.snd_pola = false;
  gcf.led_pola = true;
  gcf.gpio_sda = 20;
  gcf.gpio_scl = 21;
  gcf.gpio_dio = 57;  //A3
  gcf.gpio_clk = 56;  //A2
  gcf.gpio_dcs = 55;  //A1
  gcf.gpio_dwr = 54;  //A0
  gcf.gpio_trm = 60;  //A6
  gcf.gpio_sqw = 19;
  gcf.gpio_snd = 61;  //A7
  gcf.gpio_led = 60;  //A6
  gcf.gpio_btn = 59;  //A5
  gcf.gpio_dht = 63;  //A9
  gcf.gpio_ana = 58;  //A4
  gcf.gpio_uar = 65;  //A11

  return gcf;
}

String MAINJS::to_json(main_cfg_t mcf) {
  EEPROM.put(0, mcf);  // записали по адресу 0
  String ret = "OK";
  return ret;
}
#endif
