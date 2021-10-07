#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

conf_data_t loadConfig(const char *filename)
{
  conf_data_t _data;

#if defined(ESP8266)
  File file = LittleFS.open(filename, "r");
#elif defined(ARDUINO_ARCH_ESP32)
  File file = LittleFS.open(filename, "r");
#endif

  if (!file)
  {
    DBG_OUT_PORT.println(F("Failed to open configFile, using default configuration"));
    _data = defaultConfig();
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
      DBG_OUT_PORT.print(F("deserializeJson() for configFile failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F("Using default configuration"));
      _data = defaultConfig();
      return _data;
    }

    file.close();

    if (!error)
    {
      DBG_OUT_PORT.println(F("Read configFile sucsses!!!"));

      _data.boot_mode               = doc["bm"];

      // Get the root object in the document
      memset(_data.sta_ssid,   0, 17);
      memset(_data.sta_pass,   0, 17);
      memset(_data.ap_ssid,    0, 17);
      memset(_data.ap_pass ,   0, 17);
      memset(_data.owm_key,    0, 33);
      memset(_data.esrv1_addr, 0, 17);
      memset(_data.esrv2_addr, 0, 17);
      memset(_data.radio_addr, 0, 17);
      memset(_data.srudp_addr, 0, 17);
      memset(_data.ch1_name,   0, 17);
      memset(_data.ch2_name,   0, 17);
      memset(_data.ch3_name,   0, 17);
      memset(_data.AKey_r,     0, 17);
      memset(_data.AKey_w,     0, 17);
      memset(_data.news_api_key, 0, 33);
      memset(_data.news_source,   0, 17);

      //---Wifi.html----------------------------------------
      //---AP-----------------------------------------------
      strcpy(_data.ap_ssid,    doc["ap_ssid"]);
      strcpy(_data.ap_pass,    doc["ap_pass"]);

      //---STA----------------------------------------------
      strcpy(_data.sta_ssid,   doc["sta_ssid"]);
      strcpy(_data.sta_pass,   doc["sta_pass"]);

      _data.wifi_off          = doc["wifi_off"];

      //---Clock.html----------------------------------------
      //---Options for clock---------------------------------
      _data.time_zone         = doc["tzo"];
      _data.auto_corr         = doc["auto_corr"];
      _data.use_pm            = doc["upm"];
      _data.every_hour_beep   = doc["e_h_b"];
      _data.snd_pola          = doc["snd_pola"];
      _data.led_pola          = doc["led_pola"];
      _data.nm_start          = doc["nm_start"];
      _data.nm_stop           = doc["nm_stop"];
      _data.type_rtc          = doc["rtc_t"];

      //---GPIO-----------------------------------------------
      _data.gpio_sda          = doc["sda"];
      _data.gpio_scl          = doc["scl"];
      _data.gpio_dio          = doc["dio"];
      _data.gpio_clk          = doc["clk"];
      _data.gpio_dcs          = doc["dcs"];
      _data.gpio_dwr          = doc["dwr"];
      _data.gpio_trm          = doc["trm"];
      _data.gpio_sqw          = doc["sqw"];
      _data.gpio_snd          = doc["snd"];
      _data.gpio_led          = doc["led"];
      _data.gpio_btn          = doc["btn"];
      _data.gpio_dht          = doc["dht"];
      _data.gpio_ana          = doc["ana"];
      _data.gpio_uar          = doc["uar"];

      //---Display.html--------------------------------------
      //---Options for display-------------------------------
      _data.type_vdrv         = doc["vdrv_t"];
      _data.type_disp         = doc["disp_t"];
      _data.rus_lng           = doc["rus_lng"];
      _data.type_font         = doc["type_font"];

      //---Brigthness----------------------------------------
      _data.man_br            = doc["man_br"];
      _data.nmd_br            = doc["nmd_br"];
      _data.auto_br           = doc["auto_br"];

      //---Brigthness levels---------------------------------
      for (uint8_t i = 0; i <= 3; i++) _data.br_level[i]  = doc["br_level"][i];

      //---Sensor.html---------------------------------------
      //---Options for sensor--------------------------------
      _data.pp_city_id        = doc["pp_city_id"];
      strcpy(_data.owm_key,     doc["owm_key"]);
      strcpy(_data.esrv1_addr,  doc["esrv1_a"]);
      strcpy(_data.esrv2_addr,  doc["esrv2_a"]);
      strcpy(_data.radio_addr,  doc["radio_a"]);
      strcpy(_data.srudp_addr,  doc["srudp_a"]);
      _data.udp_mon           = doc["udm"];
      _data.use_pp            = doc["upp"];

      //---Sensor type---------------------------------------
      _data.type_snr1         = doc["snr1_t"];
      _data.type_snr2         = doc["snr2_t"];
      _data.type_snr3         = doc["snr3_t"];
      _data.type_snrp         = doc["snrp_t"];
      _data.period            = doc["period"]; // minutes

      //---Sensor actual value-------------------------------
      strcpy(_data.ch1_name,   doc["ch1_name"]);
      strcpy(_data.ch2_name,   doc["ch2_name"]);
      strcpy(_data.ch3_name,   doc["ch3_name"]);

      //---TS Account----------------------------------------
      _data.ts_ch_id          = doc["ts_ch_id"];
      strcpy(_data.AKey_r,      doc["AKey_r"]);
      strcpy(_data.AKey_w,      doc["AKey_w"]);

      //---TS sender-----------------------------------------
      _data.use_tst1          = doc["utst1"];
      _data.use_tst2          = doc["utst2"];
      _data.use_tst3          = doc["utst3"];
      _data.use_tsh1          = doc["utsh1"];
      _data.use_tsh2          = doc["utsh2"];
      _data.use_tsh3          = doc["utsh3"];
      _data.use_tsp           = doc["utsp"];

      //---Thermo.html---------------------------------------
      //---Options for thermostat----------------------------
      _data.type_thermo       = doc["trs_t"];
      _data.src_thermo        = doc["src_trs"];
      _data.lb_thermo         = doc["lb_trs"];
      _data.hb_thermo         = doc["hb_trs"];

      //---News.html---------------------------------------
      //---Options for news----------------------------
      _data.news_en            = doc["news_en"];
      strcpy(_data.news_api_key, doc["news_api"]);
      strcpy(_data.news_source,  doc["news_src"]);

      //---Alarm.html----------------------------------------
      //---Options for alarms--------------------------------
      for (uint8_t j = 0; j <= 4; j++)
      {
        _data.alarms[0][j] = doc["al"]["0"][j];
        _data.alarms[1][j] = doc["al"]["1"][j];
        _data.alarms[2][j] = doc["al"]["2"][j];
        _data.alarms[3][j] = doc["al"]["3"][j];
        _data.alarms[4][j] = doc["al"]["4"][j];
        _data.alarms[5][j] = doc["al"]["5"][j];
        _data.alarms[6][j] = doc["al"]["6"][j];
      }
    }
    else
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F("Failed to read configFile, using default configuration"));
      _data = defaultConfig();
    }
  }
  return _data;
}

void saveConfig(const char *filename, conf_data_t _data)
{
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start saving conf_data to config.json"));

  if ( _data.type_vdrv    < 0  || _data.type_vdrv  >  20) _data.type_vdrv  = 0;
  if ( _data.type_disp    < 0  || _data.type_disp  >  50) _data.type_disp  = 0;
  if ( _data.type_snr1    < 0  || _data.type_snr1  >  11) _data.type_snr1  = 0;
  if ( _data.type_snr2    < 0  || _data.type_snr2  >  11) _data.type_snr2  = 0;
  if ( _data.type_snr3    < 0  || _data.type_snr3  >  11) _data.type_snr3  = 0;
  if ( _data.type_snrp    < 0  || _data.type_snrp  >  11) _data.type_snrp  = 0;
  if ( _data.ap_ssid[0] == ' ' || _data.ap_ssid[0] ==  0) strcpy( _data.ap_ssid, "Radio_Clock");

  DynamicJsonDocument doc(3000);
  JsonObject json = doc.to<JsonObject>();

  json["bm"]                  = _data.boot_mode;

  //---Wifi.html----------------------------------------
  //---AP-----------------------------------------------
  json["ap_ssid"]             = _data.ap_ssid;
  json["ap_pass"]             = _data.ap_pass;

  //---STA----------------------------------------------
  json["sta_ssid"]            = _data.sta_ssid;
  json["sta_pass"]            = _data.sta_pass;

  json["wifi_off"]            = _data.wifi_off;

  //---Clock.html----------------------------------------
  //---Options for clock---------------------------------
  json["tzo"]                 = _data.time_zone;
  json["auto_corr"]           = _data.auto_corr;
  json["upm"]                 = _data.use_pm;
  json["e_h_b"]               = _data.every_hour_beep;
  json["snd_pola"]            = _data.snd_pola;
  json["led_pola"]            = _data.led_pola;
  json["nm_start"]            = _data.nm_start;
  json["nm_stop"]             = _data.nm_stop;
  json["rtc_t"]               = _data.type_rtc;

  //---GPIO-----------------------------------------------
  json["sda"]                 = _data.gpio_sda;
  json["scl"]                 = _data.gpio_scl;
  json["dio"]                 = _data.gpio_dio;
  json["clk"]                 = _data.gpio_clk;
  json["dcs"]                 = _data.gpio_dcs;
  json["dwr"]                 = _data.gpio_dwr;
  json["trm"]                 = _data.gpio_trm;
  json["sqw"]                 = _data.gpio_sqw;
  json["snd"]                 = _data.gpio_snd;
  json["led"]                 = _data.gpio_led;
  json["btn"]                 = _data.gpio_btn;
  json["dht"]                 = _data.gpio_dht;
  json["ana"]                 = _data.gpio_ana;
  json["uar"]                 = _data.gpio_uar;

  //---Display.html--------------------------------------
  //---Options for display-------------------------------
  json["vdrv_t"]              = _data.type_vdrv;
  json["disp_t"]              = _data.type_disp;
  json["rus_lng"]             = _data.rus_lng;
  json["type_font"]           = _data.type_font;

  //---Brigthness----------------------------------------
  json["man_br"]              = _data.man_br;
  json["nmd_br"]              = _data.nmd_br;
  json["auto_br"]             = _data.auto_br;

  //---Brigthness levels---------------------------------
  JsonArray br_level = json.createNestedArray("br_level");
  for (uint8_t i = 0; i <= 3; i++) br_level.add(_data.br_level[i]);

  //---Sensor.html---------------------------------------
  //---Options for sensor--------------------------------
  json["pp_city_id"]          = _data.pp_city_id;
  json["owm_key"]             = _data.owm_key;
  json["esrv1_a"]             = _data.esrv1_addr;
  json["esrv2_a"]             = _data.esrv2_addr;
  json["radio_a"]             = _data.radio_addr;
  json["srudp_a"]             = _data.srudp_addr;
  json["udm"]                 = _data.udp_mon;
  json["upp"]                 = _data.use_pp;

  //---Sensor type---------------------------------------
  json["snr1_t"]              = _data.type_snr1;
  json["snr2_t"]              = _data.type_snr2;
  json["snr3_t"]              = _data.type_snr3;
  json["snrp_t"]              = _data.type_snrp;
  json["period"]              = _data.period; // minutes

  //---Sensor actual value-------------------------------
  json["ch1_name"]            = _data.ch1_name;
  json["ch2_name"]            = _data.ch2_name;
  json["ch3_name"]            = _data.ch3_name;

  //---TS Account----------------------------------------
  json["ts_ch_id"]            = _data.ts_ch_id;
  json["AKey_r"]              = _data.AKey_r;
  json["AKey_w"]              = _data.AKey_w;

  //---TS sender-----------------------------------------
  json["utst1"]               = _data.use_tst1;
  json["utst2"]               = _data.use_tst2;
  json["utst3"]               = _data.use_tst3;
  json["utsh1"]               = _data.use_tsh1;
  json["utsh2"]               = _data.use_tsh2;
  json["utsh3"]               = _data.use_tsh3;
  json["utsp"]                = _data.use_tsp;

  //---Thermo.html---------------------------------------
  //---Options for thermostat----------------------------
  json["trs_t"]               = _data.type_thermo;
  json["src_trs"]             = _data.src_thermo;
  json["lb_trs"]              = _data.lb_thermo;
  json["hb_trs"]              = _data.hb_thermo;


  //---News.html---------------------------------------
  //---Options for news----------------------------
  json["news_en"]             = _data.news_en;
  json["news_api"]            = _data.news_api_key;
  json["news_src"]            = _data.news_source;


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
    al0.add(_data.alarms[0][j]);
    al1.add(_data.alarms[1][j]);
    al2.add(_data.alarms[2][j]);
    al3.add(_data.alarms[3][j]);
    al4.add(_data.alarms[4][j]);
    al5.add(_data.alarms[5][j]);
    al6.add(_data.alarms[6][j]);
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
  LittleFS.remove(filename);
  File configFile = LittleFS.open(filename, FILE_WRITE); //Open config file for writing
  if (!configFile)
  {
    DBG_OUT_PORT.println(F("Failed to open config file for writing"));
    return;
  }
  if (serializeJson(doc, configFile) == 0) DBG_OUT_PORT.println(F("Failed write to file"));
  DBG_OUT_PORT.println(F("End write buffer to file"));
  configFile.close();
}

conf_data_t defaultConfig()
{
  conf_data_t _data;

  // ---------------------------------------------------- WiFi Default

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

  _data.boot_mode        = 2;

  strcpy(_data.sta_ssid,  "MyWiFi");
  strcpy(_data.sta_pass,  "12345678");
  strcpy(_data.ap_ssid,   "Radio_Clock");
  strcpy(_data.ap_pass ,  "12345678");
  memset(_data.AKey_r,    0, 17);
  memset(_data.AKey_w,    0, 17);
  strcpy(_data.esrv1_addr, "192.168.1.100");
  strcpy(_data.esrv2_addr, "192.168.1.200");
  strcpy(_data.radio_addr, "192.168.1.33");
  strcpy(_data.srudp_addr, "192.168.1.30");
  memset(_data.owm_key,    0, 33);
  strcpy(_data.ch1_name,   "Внутри");
  strcpy(_data.ch2_name,   "Снаружи");
  strcpy(_data.ch3_name,   "В бане");
  memset(_data.news_api_key, 0, 33);
  strcpy(_data.news_source, "lenta");


  _data.auto_corr        = true;
  _data.use_pm           = false;
  _data.every_hour_beep  = true;
  _data.snd_pola         = false;
  _data.led_pola         = false;
  _data.rus_lng          = false;
  _data.use_tst1         = false;
  _data.use_tst2         = false;
  _data.use_tst3         = false;
  _data.use_tsh1         = false;
  _data.use_tsh2         = false;
  _data.use_tsh3         = false;
  _data.use_tsp          = false;
  _data.wifi_off         = false;
  _data.udp_mon          = false;
  _data.use_pp           = 0;
  _data.time_zone        = 5;
  _data.type_vdrv        = 0;
  _data.type_disp        = 0;
  _data.type_snr1        = 0;
  _data.type_snr2        = 0;
  _data.type_snr3        = 0;
  _data.type_snrp        = 0;
  _data.type_rtc         = 0;
  _data.type_thermo      = 0;
  _data.src_thermo       = 0;
  _data.lb_thermo        = 0;
  _data.hb_thermo        = 0;
  _data.nm_start         = 23;
  _data.nm_stop          = 7;
  _data.ts_ch_id         = 0;
  _data.pp_city_id       = 28438;
  _data.period           = 10;
  _data.man_br           = 7;
  _data.nmd_br           = 2;
  _data.auto_br          = false;

#if defined(ESP8266)
  _data.gpio_sda         = 4;
  _data.gpio_scl         = 5;
  _data.gpio_dio         = 13;
  _data.gpio_clk         = 14;
  _data.gpio_dcs         = 16;
  _data.gpio_dwr         = 2;
  _data.gpio_trm         = 2;
  _data.gpio_sqw         = 12;
  _data.gpio_snd         = 15;
  _data.gpio_led         = 2;
  _data.gpio_btn         = 0;
  _data.gpio_dht         = 0;
  _data.gpio_ana         = 17;
  _data.gpio_uar         = 16;

#elif defined(ARDUINO_ARCH_ESP32)
  _data.gpio_sda         = 23;
  _data.gpio_scl         = 22;
  _data.gpio_dio         = 19;
  _data.gpio_clk         = 18;
  _data.gpio_dcs         = 5;
  _data.gpio_dwr         = 26;
  _data.gpio_trm         = 36;
  _data.gpio_sqw         = 34;
  _data.gpio_snd         = 2;
  _data.gpio_led         = 16;
  _data.gpio_btn         = 0;
  _data.gpio_dht         = 39;
  _data.gpio_ana         = 35;
  _data.gpio_uar         = 36;
#endif

  _data.br_level[0]      = 1;
  _data.br_level[1]      = 200;
  _data.br_level[2]      = 1;
  _data.br_level[3]      = 15;

  _data.type_font        = 0;

  _data.news_en          = false;

  for (uint8_t i = 0; i <= 6; i++)
    for (uint8_t j = 0; j <= 4; j++) _data.alarms[i][j] = 0;

  return _data;
}

#elif defined(__AVR_ATmega2560__)

conf_data_t loadConfig(const char *filename)
{
  conf_data_t _data;
  EEPROM.get(0, _data);           // прочитали из адреса 0
  return _data;
}

conf_data_t defaultConfig()
{
  conf_data_t _data;
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

#if defined _dacha
  strcpy(_data.ch1_name,   "В избе");
  strcpy(_data.ch2_name,   "На улице");
  strcpy(_data.ch3_name,   "Подпол");
#elif defined _work
  strcpy(_data.ch1_name,   "Внутри");
  strcpy(_data.ch2_name,   "На улице");
  memset(_data.ch3_name,    0,  17);
#endif
  _data.boot_mode        = 2;
  _data.auto_corr        = true;
  _data.use_pm           = false;
  _data.every_hour_beep  = true;
  _data.snd_pola         = false;
  _data.led_pola         = true;
  _data.rus_lng          = true;
  _data.time_zone        = 5;

#if defined _dacha
  _data.type_vdrv        = 2;
  _data.type_disp        = 21;

  _data.type_snr1        = 4;
  _data.type_snr2        = 6;
  _data.type_snr3        = 10;
  _data.type_snrp        = 10;

#elif defined _work
  _data.type_vdrv        = 3;
  _data.type_disp        = 23;

  _data.type_snr1        = 10;
  _data.type_snr2        = 4;
  _data.type_snr3        = 0;
  _data.type_snrp        = 10;
#endif

  _data.type_rtc         = 1;
  _data.type_thermo      = 0;
  _data.src_thermo       = 0;
  _data.lb_thermo        = 0;
  _data.hb_thermo        = 0;
  _data.nm_start         = 0;
  _data.nm_stop          = 7;
  _data.period           = 10;
  _data.man_br           = 14;
  _data.nmd_br           = 2;
  _data.auto_br          = true;

  _data.gpio_sda         = 20;
  _data.gpio_scl         = 21;
  _data.gpio_dio         = 57; //A3
  _data.gpio_clk         = 56; //A2
  _data.gpio_dcs         = 55; //A1
  _data.gpio_dwr         = 54; //A0
  _data.gpio_trm         = 60; //A6
  _data.gpio_sqw         = 19;
  _data.gpio_snd         = 61; //A7
  _data.gpio_led         = 60; //A6
  _data.gpio_btn         = 59; //A5
  _data.gpio_dht         = 63; //A9
  _data.gpio_ana         = 58; //A4
  _data.gpio_uar         = 65; //A11

#if defined _dacha
  _data.br_level[0]      = 220;
  _data.br_level[1]      = 1;
  _data.br_level[2]      = 1;
  _data.br_level[3]      = 14;

#elif defined _work
  _data.br_level[0]      = 300;
  _data.br_level[1]      = 1;
  _data.br_level[2]      = 1;
  _data.br_level[3]      = 254;
#endif

  for (uint8_t i = 0; i <= 6; i++)
  {
    for (uint8_t j = 0; j <= 4; j++)
    {
      _data.alarms[i][j] = 0;
    }
  }

#if defined _dacha
  _data.alarms[0][0] = 2;
  _data.alarms[0][1] = 06;
  _data.alarms[0][2] = 30;
  _data.alarms[0][3] = 13;
  _data.alarms[0][4] = 0;

#elif defined _work
  _data.alarms[0][0] = 2;
  _data.alarms[0][1] = 16;
  _data.alarms[0][2] = 30;
  _data.alarms[0][3] = 13;
  _data.alarms[0][4] = 0;

  _data.alarms[1][0] = 2;
  _data.alarms[1][1] = 16;
  _data.alarms[1][2] = 38;
  _data.alarms[1][3] = 7;
  _data.alarms[1][4] = 0;

  _data.alarms[2][0] = 2;
  _data.alarms[2][1] = 11;
  _data.alarms[2][2] = 59;
  _data.alarms[2][3] = 8;
  _data.alarms[2][4] = 0;

  _data.alarms[3][0] = 2;
  _data.alarms[3][1] = 7;
  _data.alarms[3][2] = 30;
  _data.alarms[3][3] = 8;
  _data.alarms[3][4] = 3;

  _data.alarms[4][0] = 1;
  _data.alarms[4][1] = 16;
  _data.alarms[4][2] = 50;
  _data.alarms[4][3] = 8;
  _data.alarms[4][4] = 4;
#endif

  return _data;
}

void saveConfig(const char *filename, conf_data_t _data)
{
  EEPROM.put(0, _data);           // записали по адресу 0
}
#endif
