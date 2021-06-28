#if defined(__xtensa__)

conf_data_t loadConfig(const char *filename)
{
  conf_data_t data;

#if defined(ESP8266)
  File file = LittleFS.open(filename, "r");
#elif defined(ARDUINO_ARCH_ESP32)
  File file = LITTLEFS.open(filename, "r");
#endif

  if (!file)
  {
    DBG_OUT_PORT.println(F("Failed to open config file for reading"));
    data = defaultConfig();
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
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      return data;
    }

    file.close();

    if (!error)
    {
      DBG_OUT_PORT.println(F("Read configFile sucsses!!!"));

      // Get the root object in the document
      strncpy(data.sta_ssid,   " ", 17);
      strncpy(data.sta_pass,   " ", 17);
      strncpy(data.ap_ssid,    " ", 17);
      strncpy(data.ap_pass ,   " ", 17);
      strncpy(data.owm_key,    " ", 33);
      strncpy(data.esrv1_addr, " ", 17);
      strncpy(data.esrv2_addr, " ", 17);
      strncpy(data.radio_addr, " ", 17);
      strncpy(data.srudp_addr, " ", 17);
      strncpy(data.ch1_name,   " ", 17);
      strncpy(data.ch2_name,   " ", 17);
      strncpy(data.ch3_name,   " ", 17);
      strncpy(data.AKey_r,     " ", 17);
      strncpy(data.AKey_w,     " ", 17);

      //---Wifi.html----------------------------------------
      //---AP-----------------------------------------------
      strncpy(data.ap_ssid,    doc["ap_ssid"],  17);
      strncpy(data.ap_pass,    doc["ap_pass"],  17);

      //---STA----------------------------------------------
      strncpy(data.sta_ssid,   doc["sta_ssid"], 17);
      strncpy(data.sta_pass,   doc["sta_pass"], 17);

      data.wifi_off          = doc["wifi_off"];

      //---Clock.html----------------------------------------
      //---Options for clock---------------------------------
      data.time_zone         = doc["tzo"];
      data.auto_corr         = doc["auto_corr"];
      data.use_pm            = doc["upm"];
      data.every_hour_beep   = doc["e_h_b"];
      data.snd_pola          = doc["snd_pola"];
      data.led_pola          = doc["led_pola"];
      data.nm_start          = doc["nm_start"];
      data.nm_stop           = doc["nm_stop"];
      data.type_rtc          = doc["rtc_t"];

      //---GPIO-----------------------------------------------
      data.gpio_sda          = doc["sda"];
      data.gpio_scl          = doc["scl"];
      data.gpio_dio          = doc["dio"];
      data.gpio_clk          = doc["clk"];
      data.gpio_dcs          = doc["dcs"];
      data.gpio_dwr          = doc["dwr"];
      data.gpio_trm          = doc["trm"];
      data.gpio_sqw          = doc["sqw"];
      data.gpio_snd          = doc["snd"];
      data.gpio_led          = doc["led"];
      data.gpio_btn          = doc["btn"];
      data.gpio_dht          = doc["dht"];
      data.gpio_ana          = doc["ana"];
      data.gpio_uar          = doc["uar"];
      data.gpio_bz2          = doc["bz2"];

      //---Display.html--------------------------------------
      //---Options for display-------------------------------
      data.type_vdrv         = doc["vdrv_t"];
      data.type_disp         = doc["disp_t"];
      data.rus_lng           = doc["rus_lng"];
      data.type_font         = doc["type_font"];

      //---Brigthness----------------------------------------
      data.man_br            = doc["man_br"];
      data.nmd_br            = doc["nmd_br"];
      data.auto_br           = doc["auto_br"];

      //---Brigthness levels---------------------------------
      for (uint8_t i = 0; i <= 3; i++) data.br_level[i]  = doc["br_level"][i];

      //---Sensor.html---------------------------------------
      //---Options for sensor--------------------------------
      data.pp_city_id        = doc["pp_city_id"];
      strncpy(data.owm_key,    doc["owm_key"], 33);
      strncpy(data.esrv1_addr, doc["esrv1_a"], 17);
      strncpy(data.esrv2_addr, doc["esrv2_a"], 17);
      strncpy(data.radio_addr, doc["radio_a"], 17);
      strncpy(data.srudp_addr, doc["srudp_a"], 17);
      data.udp_mon           = doc["udm"];
      data.use_pp            = doc["upp"];

      //---Sensor type---------------------------------------
      data.type_snr1         = doc["snr1_t"];
      data.type_snr2         = doc["snr2_t"];
      data.type_snr3         = doc["snr3_t"];
      data.type_snrp         = doc["snrp_t"];
      data.period            = doc["period"]; // minutes

      //---Sensor actual value-------------------------------
      strncpy(data.ch1_name,   doc["ch1_name"], 17);
      strncpy(data.ch2_name,   doc["ch2_name"], 17);
      strncpy(data.ch3_name,   doc["ch3_name"], 17);

      //---TS Account----------------------------------------
      data.ts_ch_id          = doc["ts_ch_id"];
      strncpy(data.AKey_r,     doc["AKey_r"], 17);
      strncpy(data.AKey_w,     doc["AKey_w"], 17);

      //---TS sender-----------------------------------------
      data.use_tst1          = doc["utst1"];
      data.use_tst2          = doc["utst2"];
      data.use_tst3          = doc["utst3"];
      data.use_tsh1          = doc["utsh1"];
      data.use_tsh2          = doc["utsh2"];
      data.use_tsh3          = doc["utsh3"];
      data.use_tsp           = doc["utsp"];

      //---Thermo.html---------------------------------------
      //---Options for thermostat----------------------------
      data.type_thermo       = doc["trs_t"];
      data.src_thermo        = doc["src_trs"];
      data.lb_thermo         = doc["lb_trs"];
      data.hb_thermo         = doc["hb_trs"];

      //---Alarm.html----------------------------------------
      //---Options for alarms--------------------------------
      for (uint8_t j = 0; j <= 4; j++)
      {
        data.alarms[0][j] = doc["al"]["0"][j];
        data.alarms[1][j] = doc["al"]["1"][j];
        data.alarms[2][j] = doc["al"]["2"][j];
        data.alarms[3][j] = doc["al"]["3"][j];
        data.alarms[4][j] = doc["al"]["4"][j];
        data.alarms[5][j] = doc["al"]["5"][j];
        data.alarms[6][j] = doc["al"]["6"][j];
      }
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
   if (debug_level == 3) DBG_OUT_PORT.println(F("Start saving conf_data to config.json"));

  if ( data.type_vdrv    < 0  || data.type_vdrv  >  20) data.type_vdrv  = 0;
  if ( data.type_disp    < 0  || data.type_disp  >  50) data.type_disp  = 0;
  if ( data.type_snr1    < 0  || data.type_snr1  >  11) data.type_snr1  = 0;
  if ( data.type_snr2    < 0  || data.type_snr2  >  11) data.type_snr2  = 0;
  if ( data.type_snr3    < 0  || data.type_snr3  >  11) data.type_snr3  = 0;
  if ( data.type_snrp    < 0  || data.type_snrp  >  11) data.type_snrp  = 0;
  if ( data.ap_ssid[0] == ' ' || data.ap_ssid[0] ==  0) strncpy( data.ap_ssid, "Radio_Clock", 17);

  DynamicJsonDocument doc(3000);
  JsonObject json = doc.to<JsonObject>();

  //---Wifi.html----------------------------------------
  //---AP-----------------------------------------------
  json["ap_ssid"]             = data.ap_ssid;
  json["ap_pass"]             = data.ap_pass;

  //---STA----------------------------------------------
  json["sta_ssid"]            = data.sta_ssid;
  json["sta_pass"]            = data.sta_pass;

  json["wifi_off"]            = data.wifi_off;

  //---Clock.html----------------------------------------
  //---Options for clock---------------------------------
  json["tzo"]                 = data.time_zone;
  json["auto_corr"]           = data.auto_corr;
  json["upm"]                 = data.use_pm;
  json["e_h_b"]               = data.every_hour_beep;
  json["snd_pola"]            = data.snd_pola;
  json["led_pola"]            = data.led_pola;
  json["nm_start"]            = data.nm_start;
  json["nm_stop"]             = data.nm_stop;
  json["rtc_t"]               = data.type_rtc;

  //---GPIO-----------------------------------------------
  json["sda"]                 = data.gpio_sda;
  json["scl"]                 = data.gpio_scl;
  json["dio"]                 = data.gpio_dio;
  json["clk"]                 = data.gpio_clk;
  json["dcs"]                 = data.gpio_dcs;
  json["dwr"]                 = data.gpio_dwr;
  json["trm"]                 = data.gpio_trm;
  json["sqw"]                 = data.gpio_sqw;
  json["snd"]                 = data.gpio_snd;
  json["led"]                 = data.gpio_led;
  json["btn"]                 = data.gpio_btn;
  json["dht"]                 = data.gpio_dht;
  json["ana"]                 = data.gpio_ana;
  json["uar"]                 = data.gpio_uar;
  json["bz2"]                 = data.gpio_bz2;

  //---Display.html--------------------------------------
  //---Options for display-------------------------------
  json["vdrv_t"]              = data.type_vdrv;
  json["disp_t"]              = data.type_disp;
  json["rus_lng"]             = data.rus_lng;
  json["type_font"]           = data.type_font;

  //---Brigthness----------------------------------------
  json["man_br"]              = data.man_br;
  json["nmd_br"]              = data.nmd_br;
  json["auto_br"]             = data.auto_br;

  //---Brigthness levels---------------------------------
  JsonArray br_level = json.createNestedArray("br_level");
  for (uint8_t i = 0; i <= 3; i++) br_level.add(data.br_level[i]);

  //---Sensor.html---------------------------------------
  //---Options for sensor--------------------------------
  json["pp_city_id"]          = data.pp_city_id;
  json["owm_key"]             = data.owm_key;
  json["esrv1_a"]             = data.esrv1_addr;
  json["esrv2_a"]             = data.esrv2_addr;
  json["radio_a"]             = data.radio_addr;
  json["srudp_a"]             = data.srudp_addr;
  json["udm"]                 = data.udp_mon;
  json["upp"]                 = data.use_pp;

  //---Sensor type---------------------------------------
  json["snr1_t"]              = data.type_snr1;
  json["snr2_t"]              = data.type_snr2;
  json["snr3_t"]              = data.type_snr3;
  json["snrp_t"]              = data.type_snrp;
  json["period"]              = data.period; // minutes

  //---Sensor actual value-------------------------------
  json["ch1_name"]            = data.ch1_name;
  json["ch2_name"]            = data.ch2_name;
  json["ch3_name"]            = data.ch3_name;

  //---TS Account----------------------------------------
  json["ts_ch_id"]            = data.ts_ch_id;
  json["AKey_r"]              = data.AKey_r;
  json["AKey_w"]              = data.AKey_w;

  //---TS sender-----------------------------------------
  json["utst1"]               = data.use_tst1;
  json["utst2"]               = data.use_tst2;
  json["utst3"]               = data.use_tst3;
  json["utsh1"]               = data.use_tsh1;
  json["utsh2"]               = data.use_tsh2;
  json["utsh3"]               = data.use_tsh3;
  json["utsp"]                = data.use_tsp;

  //---Thermo.html---------------------------------------
  //---Options for thermostat----------------------------
  json["trs_t"]               = data.type_thermo;
  json["src_trs"]             = data.src_thermo;
  json["lb_trs"]              = data.lb_thermo;
  json["hb_trs"]              = data.hb_thermo;

  //---Alarm.html----------------------------------------
  //---Options for alarms--------------------------------
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

  JsonObject alarms = json.createNestedObject("al");
  alarms["0"] = al0;
  alarms["1"] = al1;
  alarms["2"] = al2;
  alarms["3"] = al3;
  alarms["4"] = al4;
  alarms["5"] = al5;
  alarms["6"] = al6;


  // Delete existing file, otherwise the configuration is appended to the file
#if defined(ESP8266)
  LittleFS.remove(filename);
  File configFile = LittleFS.open(filename, "w"); //Open config file for writing
#elif defined(ARDUINO_ARCH_ESP32)
  LITTLEFS.remove(filename);
  File configFile = LITTLEFS.open(filename, "w"); //Open config file for writing
#endif
  if (!configFile)
  {
    DBG_OUT_PORT.println(F("Failed to open config file for writing"));
    return;
  }
  if (serializeJson(doc, configFile) == 0) DBG_OUT_PORT.println(F("Failed write to file"));
  DBG_OUT_PORT.println(F("End write buffer to file"));
  configFile.close();
}
#endif

conf_data_t defaultConfig()
{
  conf_data_t data;

  // ---------------------------------------------------- WiFi Default

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

  strncpy(data.sta_ssid,  "MyWiFi", 17);
  strncpy(data.sta_pass,  "12345678", 17);
  strncpy(data.ap_ssid,   "Radio_Clock", 17);
  strncpy(data.ap_pass ,  "12345678", 17);
  strncpy(data.AKey_r,     " ", 17);
  strncpy(data.AKey_w,     " ", 17);
  strncpy(data.esrv1_addr, "192.168.1.100", 17);
  strncpy(data.esrv2_addr, "192.168.1.200", 17);
  strncpy(data.radio_addr, "192.168.1.33", 17);
  strncpy(data.srudp_addr, "192.168.1.30", 17);
  strncpy(data.owm_key,    " ", 33);
  strncpy(data.ch1_name,   "Внутри",  17);
  strncpy(data.ch2_name,   "Снаружи", 17);
  strncpy(data.ch3_name,   "В бане",  17);


  data.auto_corr        = true;
  data.use_pm           = false;
  data.every_hour_beep  = true;
  data.snd_pola         = false;
  data.led_pola         = false;
  data.rus_lng          = false;
  data.use_tst1         = false;
  data.use_tst2         = false;
  data.use_tst3         = false;
  data.use_tsh1         = false;
  data.use_tsh2         = false;
  data.use_tsh3         = false;
  data.use_tsp          = false;
  data.wifi_off         = false;
  data.udp_mon          = false;
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
  data.nm_start         = 23;
  data.nm_stop          = 7;
  data.ts_ch_id         = 0;
  data.pp_city_id       = 28438;
  data.period           = 10;
  data.man_br           = 7;
  data.nmd_br           = 2;
  data.auto_br          = false;

  data.gpio_sda         = 4;
  data.gpio_scl         = 5;
  data.gpio_dio         = 13;
  data.gpio_clk         = 14;
  data.gpio_dcs         = 16;
  data.gpio_dwr         = 2;
  data.gpio_trm         = 2;
  data.gpio_sqw         = 12;
  data.gpio_snd         = 15;
  data.gpio_led         = 2;
  data.gpio_btn         = 0;
  data.gpio_dht         = 0;
  data.gpio_ana         = A0;
  data.gpio_uar         = 16;
  data.gpio_bz2         = 32;


  data.br_level[0]      = 1;
  data.br_level[1]      = 200;
  data.br_level[2]      = 1;
  data.br_level[3]      = 15;

  data.type_font        = 0;

  for (uint8_t i = 0; i <= 6; i++)
    for (uint8_t j = 0; j <= 4; j++) data.alarms[i][j] = 0;

  return data;
}

#if defined(__AVR_ATmega2560__)

conf_data_t loadConfig(const char *filename)
{
  conf_data_t _data;

  EEPROM.get(0, _data);           // прочитали из адреса 0

  _data.led_pola         = true;

  return _data;
}

conf_data_t first_use()
{
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

  conf_data_t data;

  strncpy(data.sta_ssid,  "MyWiFi", 17);
  strncpy(data.sta_pass,  "12345678", 17);
  strncpy(data.ap_ssid,   "Radio_Clock", 17);
  strncpy(data.ap_pass ,  "12345678", 17);
  strncpy(data.AKey_r,     " ", 17);
  strncpy(data.AKey_w,     " ", 17);
  strncpy(data.esrv1_addr, "192.168.1.100", 17);
  strncpy(data.esrv2_addr, "192.168.1.200", 17);
  strncpy(data.radio_addr, "192.168.1.33", 17);
  strncpy(data.srudp_addr, "192.168.1.30", 17);
  strncpy(data.owm_key,    " ", 33);
  strncpy(data.ch1_name,   "Дома",  17);
  strncpy(data.ch2_name,   "На улице", 17);
  strncpy(data.ch3_name,   "В подполе",  17);


  data.auto_corr        = true;
  data.use_pm           = false;
  data.every_hour_beep  = true;
  data.snd_pola         = false;
  data.led_pola         = true;
  data.rus_lng          = true;
  data.use_tst1         = false;
  data.use_tst2         = false;
  data.use_tst3         = false;
  data.use_tsh1         = false;
  data.use_tsh2         = false;
  data.use_tsh3         = false;
  data.use_tsp          = false;
  data.wifi_off         = false;
  data.udp_mon          = false;
  data.use_pp           = 0;
  data.time_zone        = 5;
  data.type_vdrv        = 1;
  data.type_disp        = 1;
  data.type_snr1        = 4;
  data.type_snr2        = 6;
  data.type_snr3        = 10;
  data.type_snrp        = 10;
  data.type_rtc         = 1;
  data.type_thermo      = 0;
  data.src_thermo       = 0;
  data.lb_thermo        = 0;
  data.hb_thermo        = 0;
  data.nm_start         = 0;
  data.nm_stop          = 7;
  data.ts_ch_id         = 0;
  data.pp_city_id       = 28438;
  data.period           = 10;
  data.man_br           = 14;
  data.nmd_br           = 2;
  data.auto_br          = false;

  data.gpio_sda         = 20;
  data.gpio_scl         = 21;
  data.gpio_dio         = A3;
  data.gpio_clk         = A2;
  data.gpio_dcs         = A1;
  data.gpio_dwr         = A0;
  data.gpio_trm         = A6;
  data.gpio_sqw         = 19;
  data.gpio_snd         = A7;
  data.gpio_led         = A6;
  data.gpio_btn         = A5;
  data.gpio_dht         = A9;
  data.gpio_ana         = A4;
  data.gpio_uar         = A11;
  data.gpio_bz2         = A10;

  data.br_level[0]      = 200;
  data.br_level[1]      = 1;
  data.br_level[2]      = 1;
  data.br_level[3]      = 15;

  data.type_font        = 0;

  for (uint8_t i = 0; i <= 6; i++)
  {
    for (uint8_t j = 0; j <= 4; j++)
    {
      data.alarms[i][j] = 0;
    }
  }
  data.alarms[0][0] = 1;
  data.alarms[0][1] = 11;
  data.alarms[0][2] = 59;
  data.alarms[0][3] = 13;
  data.alarms[0][4] = 0;

  data.alarms[1][0] = 1;
  data.alarms[1][1] = 12;
  data.alarms[1][2] = 01;
  data.alarms[1][3] = 14;
  data.alarms[1][4] = 0;

  data.alarms[2][0] = 1;
  data.alarms[2][1] = 12;
  data.alarms[2][2] = 03;
  data.alarms[2][3] = 15;
  data.alarms[2][4] = 0;

  data.auto_br      = true;
  data.br_level[0] = 220;
  data.br_level[1] = 1;
  data.br_level[2] = 1;
  data.br_level[3] = 14;

  return data;
}

void saveConfig(const char *filename, conf_data_t _data)
{
  EEPROM.put(0, _data);           // записали по адресу 0
}
#endif
