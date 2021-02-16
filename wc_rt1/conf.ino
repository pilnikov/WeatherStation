#if defined(ESP8266) || defined(ESP32)


conf_data_t loadConfig(const char *filename)
{
  conf_data_t data;

  File file = SPIFFS.open(filename, "r");

  if (!file)
  {
    DBG_OUT_PORT.println("Failed to open config file for reading");
    data = defaultConfig();
  }
  else
  {

    // Allocate the document on the stack.
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.


    DynamicJsonDocument doc(3000);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      return data;
    }

    file.close();

    if (!error)
    {
      DBG_OUT_PORT.println(F("Read configFile sucsses!!!"));

      // Get the root object in the document
      //JsonObject json = jsonBuffer.as<JsonObject>();

      strncpy(data.sta_ssid,   " ", 33);
      strncpy(data.sta_pass,   " ", 33);
      strncpy(data.ap_ssid,    " ", 17);
      strncpy(data.ap_pass ,   " ", 17);
      strncpy(data.AKey_r,     " ", 17);
      strncpy(data.AKey_w,     " ", 17);
      strncpy(data.esrv1_addr, " ", 17);
      strncpy(data.esrv2_addr, " ", 17);
      strncpy(data.radio_addr, " ", 17);
      strncpy(data.owm_key,    " ", 35);
      strncpy(data.ch1_name,   " ", 17);
      strncpy(data.ch2_name,   " ", 17);
      strncpy(data.ch3_name,   " ", 17);

      strncpy(data.sta_ssid,   doc["sta_ssid"],   33);
      strncpy(data.sta_pass,   doc["sta_pass"],   33);
      strncpy(data.ap_ssid,    doc["ap_ssid"],    17);
      strncpy(data.ap_pass,    doc["ap_pass"],    17);
      strncpy(data.AKey_r,     doc["AKey_r"],     17);
      strncpy(data.AKey_w,     doc["AKey_w"],     17);
      strncpy(data.esrv1_addr, doc["esrv1_addr"], 17);
      strncpy(data.esrv2_addr, doc["esrv2_addr"], 17);
      strncpy(data.radio_addr, doc["radio_addr"], 17);
      strncpy(data.owm_key,    doc["owm_key"],    32);
      strncpy(data.ch1_name,   doc["ch1_name"],   17);
      strncpy(data.ch2_name,   doc["ch2_name"],   17);
      strncpy(data.ch3_name,   doc["ch3_name"],   17);

      data.wifi_off         = doc["wifi_off"];
      data.auto_corr        = doc["auto_corr"];
      data.use_pm           = doc["use_pm"];
      data.every_hour_beep  = doc["every_hour_beep"];
      data.rus_lng          = doc["rus_lng"];
      data.use_tst1         = doc["use_tst1"];
      data.use_tst2         = doc["use_tst2"];
      data.use_tst3         = doc["use_tst3"];
      data.use_tsh1         = doc["use_tsh1"];
      data.use_tsh2         = doc["use_tsh2"];
      data.use_tsh3         = doc["use_tsh3"];
      data.use_tsp          = doc["use_tsp"];
      data.use_pp           = doc["use_pp"];
      data.time_zone        = doc["time_zone"];
      data.type_vdrv        = doc["type_vdrv"];
      data.type_disp        = doc["type_disp"];
      data.type_snr1        = doc["type_snr1"];
      data.type_snr2        = doc["type_snr2"];
      data.type_snr3        = doc["type_snr3"];
      data.type_snrp        = doc["type_snrp"];
      data.type_rtc         = doc["type_rtc"];
      data.type_thermo      = doc["type_trs"];
      data.src_thermo       = doc["src_trs"];
      data.lb_thermo        = doc["lb_trs"];
      data.hb_thermo        = doc["hb_trs"];
      data.nm_start = doc["nm_start"];
      data.nm_stop  = doc["nm_stop"];
      data.ts_ch_id         = doc["ts_ch_id"];
      data.pp_city_id       = doc["pp_city_id"];
      data.period           = doc["period"]; // minutes
      data.man_br           = doc["man_br"];
      data.auto_br          = doc["auto_br"];

      for (uint8_t i = 0; i <= 3; i++) data.br_level[i]  = doc["br_level"][i];

      data.type_font        = doc["type_font"];

      for (uint8_t j = 0; j <= 4; j++)
      {
        data.alarms[0][j] = doc["alarms"]["0"][j];
        data.alarms[1][j] = doc["alarms"]["1"][j];
        data.alarms[2][j] = doc["alarms"]["2"][j];
        data.alarms[3][j] = doc["alarms"]["3"][j];
        data.alarms[4][j] = doc["alarms"]["4"][j];
        data.alarms[5][j] = doc["alarms"]["5"][j];
        data.alarms[6][j] = doc["alarms"]["6"][j];
      }
      //  data = defaultConfig();
    }
    else
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F("Failed to read configFile, using default configuration"));

      data = defaultConfig();
    }
  }
  return data;
}

void saveConfig(const char *filename, conf_data_t data)
{
  if (debug_level == 3) DBG_OUT_PORT.println( "Start saving conf_data to config.json");


  if ( data.type_vdrv    < 0  || data.type_vdrv  >  20) data.type_vdrv  = 0;
  if ( data.type_disp    < 0  || data.type_disp  >  40) data.type_disp  = 0;
  if ( data.type_snr1    < 0  || data.type_snr1  >  11) data.type_snr1  = 0;
  if ( data.type_snr2    < 0  || data.type_snr2  >  11) data.type_snr2  = 0;
  if ( data.type_snr3    < 0  || data.type_snr3  >  11) data.type_snr3  = 0;
  if ( data.type_snrp    < 0  || data.type_snrp  >  11) data.type_snrp  = 0;
  if ( data.ap_ssid[0] == ' ' || data.ap_ssid[0] ==  0) strncpy( data.ap_ssid, ap_ssid_def, sizeof(ap_ssid_def));

  DynamicJsonDocument doc(3000);
  JsonObject json = doc.to<JsonObject>();

  json["sta_ssid"]            = data.sta_ssid;
  json["sta_pass"]            = data.sta_pass;
  json["ap_ssid"]             = data.ap_ssid;
  json["ap_pass"]             = data.ap_pass;
  json["wifi_off"]            = data.wifi_off;
  json["auto_corr"]           = data.auto_corr;
  json["use_pm"]              = data.use_pm;
  json["every_hour_beep"]     = data.every_hour_beep;
  json["rus_lng"]             = data.rus_lng;
  json["use_tst1"]            = data.use_tst1;
  json["use_tst2"]            = data.use_tst2;
  json["use_tst3"]            = data.use_tst3;
  json["use_tsh1"]            = data.use_tsh1;
  json["use_tsh2"]            = data.use_tsh2;
  json["use_tsh3"]            = data.use_tsh3;
  json["use_tsp"]             = data.use_tsp;
  json["use_pp"]              = data.use_pp;
  json["time_zone"]           = data.time_zone;
  json["type_vdrv"]           = data.type_vdrv;
  json["type_disp"]           = data.type_disp;
  json["type_snr1"]           = data.type_snr1;
  json["type_snr2"]           = data.type_snr2;
  json["type_snr3"]           = data.type_snr3;
  json["type_snrp"]           = data.type_snrp;
  json["type_rtc"]            = data.type_rtc;
  json["type_trs"]            = data.type_thermo;
  json["src_trs"]             = data.src_thermo;
  json["lb_trs"]              = data.lb_thermo;
  json["hb_trs"]              = data.hb_thermo;
  json["nm_start"]    = data.nm_start;
  json["nm_stop"]     = data.nm_stop;
  json["ts_ch_id"]            = data.ts_ch_id;
  json["AKey_r"]              = data.AKey_r;
  json["AKey_w"]              = data.AKey_w;
  json["esrv1_addr"]          = data.esrv1_addr;
  json["esrv2_addr"]          = data.esrv2_addr;
  json["radio_addr"]          = data.radio_addr;
  json["pp_city_id"]          = data.pp_city_id;
  json["owm_key"]             = data.owm_key;
  json["ch1_name"]            = data.ch1_name;
  json["ch2_name"]            = data.ch2_name;
  json["ch3_name"]            = data.ch3_name;
  json["period"]              = data.period; // minutes
  json["man_br"]              = data.man_br;
  json["auto_br"]             = data.auto_br;

  JsonArray br_level = json.createNestedArray("br_level");
  for (uint8_t i = 0; i <= 3; i++) br_level.add(data.br_level[i]);

  json["type_font"]           = data.type_font;

  //DynamicJsondocument jsonBuffer2(128);
  // JsonObject json2 = doc.to<JsonObject>();
  DynamicJsonDocument doc2(700);
  JsonObject json2 = doc2.to<JsonObject>();

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

  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  File configFile = SPIFFS.open(filename, "w"); //Open config file for writing

  if (!configFile)
  {
    DBG_OUT_PORT.println("Failed to open config file for writing");
    return;
  }
  if (serializeJson(doc, configFile) == 0) DBG_OUT_PORT.println(F("Failed write to file"));
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
  strncpy(data.esrv1_addr, "192.168.1.100", 17);
  strncpy(data.esrv2_addr, "192.168.1.200", 17);
  strncpy(data.radio_addr, "192.168.1.33", 17);
  strncpy(data.owm_key,    " ", 35);
  strncpy(data.ch1_name,   "Внутри",  17);
  strncpy(data.ch2_name,   "Снаружи", 17);
  strncpy(data.ch3_name,   "В бане",  17);


  data.auto_corr        = true;
  data.use_pm           = false;
  data.every_hour_beep  = true;
  data.rus_lng          = false;
  data.use_tst1         = false;
  data.use_tst2         = false;
  data.use_tst3         = false;
  data.use_tsh1         = false;
  data.use_tsh2         = false;
  data.use_tsh3         = false;
  data.use_tsp          = false;
  data.wifi_off         = false;
  data.use_pp           = 0;
  data.time_zone        = 5;
  data.type_vdrv        = 0;
  data.type_disp        = 0;
  data.type_snr1        = 0;
  data.type_snr2        = 0;
  data.type_snr3        = 0;
  data.type_snrp        = 0;
  data.type_rtc         = 0;
  data.type_thermo      = 0;
  data.src_thermo       = 0;
  data.lb_thermo        = 0;
  data.hb_thermo        = 0;
  data.nm_start = 23;
  data.nm_stop  = 7;
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
