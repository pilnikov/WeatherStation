#if defined(ESP8266) || defined(ESP32)

conf_data_t loadConfig()
{
  conf_data_t data;

  File configFile = SPIFFS.open("/config.json", "r");
  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  DynamicJsonDocument jsonBuffer;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(jsonBuffer, configFile);
  configFile.close();
  if (error)
  {
    DBG_OUT_PORT.println(F("Failed to read configFile, using default configuration"));

    data = defaultConfig();
    return data;
  }

  // Get the root object in the document
  JsonObject json = jsonBuffer.as<JsonObject>();

  strncpy(data.sta_ssid,   " ", 33);
  strncpy(data.sta_pass,   " ", 33);
  strncpy(data.ap_ssid,    " ", 17);
  strncpy(data.ap_pass ,   " ", 17);
  strncpy(data.AKey_r,     " ", 17);
  strncpy(data.AKey_w,     " ", 17);
  strncpy(data.esrv_addr,  " ", 17);
  strncpy(data.radio_addr, " ", 17);
  strncpy(data.owm_key,    " ", 35);

  strncpy(data.sta_ssid,  json["sta_ssid"],  33);
  strncpy(data.sta_pass,  json["sta_pass"],  33);
  strncpy(data.ap_ssid,   json["ap_ssid"],   17);
  strncpy(data.ap_pass,   json["ap_pass"],   17);
  strncpy(data.AKey_r,    json["AKey_r"],    17);
  strncpy(data.AKey_w,    json["AKey_w"],    17);
  strncpy(data.esrv_addr, json["esrv_addr"], 17);
  strncpy(data.radio_addr, json["radio_addr"], 17);
  strncpy(data.owm_key,   json["owm_key"], 32);

  data.auto_corr        = json["auto_corr"];
  data.use_pm           = json["use_pm"];
  data.every_hour_beep  = json["every_hour_beep"];
  data.rus_disp         = json["rus_disp"];
  data.use_ts_i         = json["use_ts_i"];
  data.use_ts_e         = json["use_ts_e"];
  data.use_ts_p         = json["use_ts_p"];
  data.use_es           = json["use_es"];
  data.use_pp           = json["use_pp"];
  data.time_zone        = json["time_zone"];
  data.type_disp        = json["type_disp"];
  data.type_int_snr     = json["type_int_snr"];
  data.type_ext_snr     = json["type_ext_snr"];
  data.type_prs_snr     = json["type_prs_snr"];
  data.type_rtc         = json["type_rtc"];
  data.night_mode_start = json["night_mode_start"];
  data.night_mode_stop  = json["night_mode_stop"];
  data.ts_ch_id         = json["ts_ch_id"];
  data.pp_city_id       = json["pp_city_id"];
  data.period           = json["period"]; // minutes
  data.man_br           = json["man_br"];
  data.auto_br          = json["auto_br"];

  for (uint8_t i = 0; i <= 3; i++) data.br_level[i]  = json["br_level"][i];

  data.type_font        = json["type_font"];

  for (uint8_t j = 0; j <= 4; j++)
  {
    data.alarms[0][j] = json["alarms"]["0"][j];
    data.alarms[1][j] = json["alarms"]["1"][j];
    data.alarms[2][j] = json["alarms"]["2"][j];
    data.alarms[3][j] = json["alarms"]["3"][j];
    data.alarms[4][j] = json["alarms"]["4"][j];
    data.alarms[5][j] = json["alarms"]["5"][j];
    data.alarms[6][j] = json["alarms"]["6"][j];
  }
  //  data = defaultConfig();

  return data;
}

void saveConfig(conf_data_t data)
{
  if (debug_level == 3) DBG_OUT_PORT.println( "Start saving conf_data to config.json");


  if ( data.type_disp < 0     || data.type_disp  >  9)    data.type_disp  = 0;
  if ( data.type_ext_snr < 0  || data.type_ext_snr  >  10) data.type_ext_snr  = 0;
  if ( data.type_int_snr < 0  || data.type_int_snr  >  10) data.type_int_snr  = 0;
  if ( data.type_prs_snr < 0  || data.type_prs_snr  >  10) data.type_prs_snr  = 0;
  if ( data.ap_ssid[0] == ' ' || data.ap_ssid[0] == 0) strncpy( data.ap_ssid, ap_ssid_def, sizeof(ap_ssid_def));

  DynamicJsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["sta_ssid"]            = data.sta_ssid;
  json["sta_pass"]            = data.sta_pass;
  json["ap_ssid"]             = data.ap_ssid;
  json["ap_pass"]             = data.ap_pass;
  json["auto_corr"]           = data.auto_corr;
  json["use_pm"]              = data.use_pm;
  json["every_hour_beep"]     = data.every_hour_beep;
  json["rus_disp"]            = data.rus_disp;
  json["use_ts_i"]            = data.use_ts_i;
  json["use_ts_e"]            = data.use_ts_e;
  json["use_ts_p"]            = data.use_ts_p;
  json["use_es"]              = data.use_es;
  json["use_pp"]              = data.use_pp;
  json["time_zone"]           = data.time_zone;
  json["type_disp"]           = data.type_disp;
  json["type_int_snr"]        = data.type_int_snr;
  json["type_ext_snr"]        = data.type_ext_snr;
  json["type_prs_snr"]        = data.type_prs_snr;
  json["type_rtc"]            = data.type_rtc;
  json["night_mode_start"]    = data.night_mode_start;
  json["night_mode_stop"]     = data.night_mode_stop;
  json["ts_ch_id"]            = data.ts_ch_id;
  json["AKey_r"]              = data.AKey_r;
  json["AKey_w"]              = data.AKey_w;
  json["esrv_addr"]           = data.esrv_addr;
  json["radio_addr"]          = data.radio_addr;
  json["pp_city_id"]          = data.pp_city_id;
  json["owm_key"]             = data.owm_key;
  json["period"]              = data.period; // minutes
  json["man_br"]              = data.man_br;
  json["auto_br"]             = data.auto_br;

  JsonArray br_level = json.createNestedArray("br_level");
  for (uint8_t i = 0; i <= 3; i++) br_level.add(data.br_level[i]);

  json["type_font"]           = data.type_font;

  DynamicJsonDocument jsonBuffer2;
  JsonObject json2 = jsonBuffer2.to<JsonObject>();

  JsonArray al0 = json2.createNestedArray("0");
  JsonArray al1 = json2.createNestedArray("1");
  JsonArray al2 = json2.createNestedArray("2");
  JsonArray al3 = json2.createNestedArray("3");
  JsonArray al4 = json2.createNestedArray("4");
  JsonArray al5 = json2.createNestedArray("5");
  JsonArray al6 = json2.createNestedArray("6");
  for (uint8_t j = 0; j <= 4; j++)
  {
    al0.add(data.alarms[0][j]);
    al1.add(data.alarms[1][j]);
    al2.add(data.alarms[2][j]);
    al3.add(data.alarms[3][j]);
    al4.add(data.alarms[4][j]);
    al5.add(data.alarms[5][j]);
    al6.add(data.alarms[6][j]);
  }

  JsonObject alarms = json.createNestedObject("alarms");
  alarms["0"] = al0;
  alarms["1"] = al1;
  alarms["2"] = al2;
  alarms["3"] = al3;
  alarms["4"] = al4;
  alarms["5"] = al5;
  alarms["6"] = al6;

  File configFile = SPIFFS.open("/config.json", "w");

  if (!configFile)
  {
    DBG_OUT_PORT.println("Failed to open config file for writing");
    return;
  }
  if (serializeJson(jsonBuffer, configFile) == 0) DBG_OUT_PORT.println(F("Failed to write to file"));
  DBG_OUT_PORT.println( "End write buffer to file");
  configFile.close();
}

conf_data_t defaultConfig()
{
  conf_data_t data;

  if (debug_level == 3) DBG_OUT_PORT.println( "Start inital conf_data with config.json");

  strncpy(data.sta_ssid,  "MyWiFi", 33);
  strncpy(data.sta_pass,  "12345678", 33);
  strncpy(data.ap_ssid,   "Radio_Clock", 17);
  strncpy(data.ap_pass ,  "12345678", 17);
  strncpy(data.AKey_r,     " ", 17);
  strncpy(data.AKey_w,     " ", 17);
  strncpy(data.esrv_addr, "192.168.1.100", 17);
  strncpy(data.radio_addr, "192.168.1.33", 17);
  strncpy(data.owm_key,    " ", 35);


  data.auto_corr        = true;
  data.use_pm           = false;
  data.every_hour_beep  = true;
  data.rus_disp         = false;
  data.use_ts_i         = false;
  data.use_ts_e         = false;
  data.use_ts_p         = false;
  data.use_es           = true;
  data.use_pp           = 0;
  data.time_zone        = 5;
  data.type_disp        = 0;
  data.type_int_snr     = 0;
  data.type_ext_snr     = 0;
  data.type_prs_snr     = 0;
  data.type_rtc         = 0;
  data.night_mode_start = 23;
  data.night_mode_stop  = 7;
  data.ts_ch_id         = 0;
  data.pp_city_id       = 28438;
  data.period           = 10;
  data.man_br           = 7;
  data.auto_br          = false;

  for (uint8_t i = 0; i <= 3; i++) data.br_level[i]  = 0;

  data.type_font        = 0;

  for (uint8_t i = 0; i <= 6; i++)
    for (uint8_t j = 0; j <= 4; j++) data.alarms[i][j] = 0;

  return data;
}
#endif
