#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

File fsUploadFile;  //Should be placed here as global

// ----------------------------------- Web updater
#if defined(ESP8266)
ESP8266HTTPUpdateServer httpUpdater;
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
HTTPUpdateServer httpUpdater;
#endif

// ---------------------------------------------------------------------- setup
void web_setup() {
  server.on("/set_ntp",        handler_SET_Time_from_NTP);
  server.on("/set_time",       handler_SET_Time);
  server.on("/get_time_cfg",   handler_GET_Time_Cfg);
  server.on("/get_wifi_cfg",   handler_GET_WiFi_Cfg_Start);
  server.on("/get_ip1",        handler_GET_IP_1);
  server.on("/get_ip2",        handler_GET_IP_2);
  server.on("/get_wifi_end",   handler_GET_WiFi_Cfg_End);
  server.on("/exit",           handleExit);
#ifdef MATRIX
  server.on("/set_font",       handler_SET_Font);
#endif  //MATRIX
  server.on("/get_disp_cfg",   handler_GET_Disp_Cfg);
  server.on("/upd_snr_data",   handler_UPD_Snr_Data);
  server.on("/upd_wfc_data",   handler_UPD_WFC_Data);
  server.on("/get_snr_cfg1",   handler_GET_Snr_Cfg_1);
  server.on("/get_snr_cfg2",   handler_GET_Snr_Cfg_2);
  server.on("/get_snr_cfg3",   handler_GET_Snr_Cfg_3);
  server.on("/get_gpio_cfg",   handler_GET_GPIO_Cfg);
  server.on("/get_news_cfg",   handler_GET_News_Cfg);
  server.on("/upd_news",       handler_UPD_News);
  server.on("/get_tmst_cfg",   handler_GET_Tmst_Cfg);

  server.on("/send_actt",      handler_SEND_Act_Time);
  server.on("/send_acta",      handler_SEND_Act_Alarm);
  server.on("/send_actb",      handler_SEND_BR_Act);

  server.on("/send_time_cfg",  handler_SEND_Time_Cfg);
  server.on("/send_wifi_cfg",  handler_SEND_WiFi_Cfg);
  server.on("/send_disp_cfg",  handler_SEND_Disp_Cfg);
  server.on("/send_sens_cfg",  handler_SEND_Snr_Cfg);
  server.on("/send_tsnr_cfg",  handler_SEND_TSnr_Cfg);
  server.on("/send_snr_data",  handler_SEND_Snr_Data);
  server.on("/get_snr_data",   handler_GET_Snr_Data);
  server.on("/send_uart",      handler_SEND_UART);
  server.on("/send_gpio_cfg",  handler_SEND_GPIO_Cfg);
  server.on("/send_tmst_cfg",  handler_SEND_Tmst_Cfg);
  server.on("/send_news_cfg",  handler_SEND_News_Cfg);
  server.on("/send_newsf",     handler_SEND_News_Full);

  //-------------------------------------------------------------- for LittleFS
  //list directory
  server.on("/list", HTTP_GET, handleFileList);
  //load editor
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from LittleFS
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
  //-------------------------------------------------------------- http update

  httpUpdater.setup(&server);
}

//-------------------------------------------------------------- Start_serv
void start_serv() {
  if (!wifi_data_cur.cli & !wifi_data_cur.ap) {
    wifi_data_cur = wifi.begin(wifi_data);
  }

  if (wifi_data_cur.cli || wifi_data_cur.ap) {
    server.begin();

    DBG_OUT_PORT.println(F("Server started"));
  }
}

//-------------------------------------------------------------- Stop_serv
void stop_serv() {
  server.stop();
  DBG_OUT_PORT.println(F("Server stopped...."));
#if defined(ESP8266)
  ;
#else
  ArduinoOTA.end();
#endif
  DBG_OUT_PORT.println(F("OTA stopped...."));
  MDNS.end();
  DBG_OUT_PORT.println(F("MDNS stopped...."));
  wifi.end(wifi_data_cur);
}


//-------------------------------------------------------------- handler_SEND_Time_Cfg
void handler_SEND_Time_Cfg() {
  from_client = myrtccfg.to_json(rtc_cfg);
  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handler_SET_Time
void handler_SET_Time() {
  unsigned long ttm = server.arg("in").toInt();
  server.send(200, "text/html", "OK!");

  DBG_OUT_PORT.print(F("Time from web...."));
  DBG_OUT_PORT.println(ttm);

  myrtc.man_set_time(rtc_hw, ttm);
  rtc_time.ct = myrtc.GetTime(rtc_hw);
  rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);

  DBG_OUT_PORT.print(F("Current Time...."));
  DBG_OUT_PORT.println(rtc_time.ct);
}

//-------------------------------------------------------------- handler_GET_Time_Cfg
void handler_GET_Time_Cfg() {
  from_client = server.arg("in");
  server.send(200, "text/html", "OK!");

  conf_f = "/conf_rtc.json";
  lfs.writeFile(conf_f, from_client.c_str());
  rtc_cfg = myrtccfg.from_json(from_client);
  rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
}

//-------------------------------------------------------------- handler_SET_Time_from_NTP
void handler_SET_Time_from_NTP() {
  server.send(200, "text/html", "OK!");
  if (wifi_data_cur.cli) {
    unsigned long ttm = myrtc.GetNtp(rtc_cfg, rtc_time.ct);
    DBG_OUT_PORT.print(F("Time after NTP...."));
    DBG_OUT_PORT.println(ttm);
    myrtc.man_set_time(rtc_hw, ttm);
    rtc_time.ct = myrtc.GetTime(rtc_hw);
    rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
    DBG_OUT_PORT.print(F("Current Time...."));
    DBG_OUT_PORT.println(rtc_time.ct);
  }
}

//-------------------------------------------------------------- handler_SEND_WiFi_Cfg
void handler_SEND_WiFi_Cfg() {
  from_client = wifi_cfg.to_json(wifi_data);
  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handler_GET_WiFi_Cfg_Start
void handler_GET_WiFi_Cfg_Start() {
  from_client = server.arg("in");
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler_GET_IP_1
void handler_GET_IP_1() {
  from_client += server.arg("in");
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler_GET_IP_2
void handler_GET_IP_2() {
  from_client += server.arg("in");
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler_GET_WiFi_Cfg_End
void handler_GET_WiFi_Cfg_End() {
  from_client.replace("}{", ",");

  conf_f = "/conf_wifi.json";
  lfs.writeFile(conf_f, from_client.c_str());
  wifi_data = wifi_cfg.from_json(from_client);
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler_SEND_Act_Time
void handler_SEND_Act_Time() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["boot"] = boot_mode;
  json["actw"] = wasAlarm;
  json["ct"] = rtc_time.ct;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler_SEND_Act_Alarm
void handler_SEND_Act_Alarm() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["actn"] = rtc_alm.num;
  json["acta"] = rtc_alm.time;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler_SEND_Disp_Cfg
void handler_SEND_Disp_Cfg() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  //	json["font"] = main_cfg.font_t;
  json["ctyp"] = mcf.vdrv_t;
  json["dtyp"] = mcf.dsp_t;
  json["rlng"] = mcf.rus_lng;
  json["tup"] = mcf.time_up;
  json["colu"] = mcf.color_up;
  json["cold"] = mcf.color_dwn;
  json["abrd"] = mcf.auto_br;
  json["mbrd"] = mcf.man_br;
  json["nbrd"] = mcf.nmd_br;

  JsonArray br_level = jsonBuffer["br_level"].to<JsonArray>();
  for (uint8_t i = 0; i <= 3; i++) br_level.add(mcf.br_level[i]);

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler_SET_Font
#ifdef MATRIX
void handler_SET_Font() {
  //url='/set_font?tfnt='+tfnt;

  main_cfg.font_t = server.arg("tfnt").toInt();
  if (debug_level == 14) DBG_OUT_PORT.printf("font is.... %u", main_cfg.font_t);

  from_client = maincfg.to_json(main_cfg);

  conf_f = "/config.json";
  lfs.writeFile(conf_f, from_client.c_str());

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}
#endif  //MATRIX

//-------------------------------------------------------------- handler GET Parameter for display
void handler_GET_Disp_Cfg() {
  //url='/set_pard?ctyp='+ctyp+'&dtyp='+dtyp+'&rlng='+rlng+'&ttup='+ttup+'&mcou='+mcou+'&mcod='+mcod+'&abrd='+abrd+'&mbrd='+mbrd+'&nbrd='+nbrd+'&brd1='+brd1+'&brd2='+brd2+'&brd3='+brd3+'&brd4='+brd4;

  uint8_t vdrv_buf = mcf.vdrv_t;

  mcf.vdrv_t = server.arg("ctyp").toInt();
  mcf.dsp_t = server.arg("dtyp").toInt();
  mcf.rus_lng = server.arg("rlng") == "1";
  mcf.time_up = server.arg("ttup") == "1";
  mcf.color_up = server.arg("mcou").toInt();
  mcf.color_dwn = server.arg("mcod").toInt();
  mcf.auto_br = server.arg("abrd") == "1";
  uint16_t val = server.arg("mbrd").toInt();
  mcf.man_br = constrain(val, 0, 254);
  ;
  val = server.arg("nbrd").toInt();
  mcf.nmd_br = constrain(val, 0, 254);
  ;
  mcf.br_level[0] = server.arg("brd1").toInt();
  mcf.br_level[1] = server.arg("brd2").toInt();
  mcf.br_level[2] = server.arg("brd3").toInt();
  mcf.br_level[3] = server.arg("brd4").toInt();

  from_client = main_cfg.to_json(mcf);

  conf_f = "/conf_main.json";
  lfs.writeFile(conf_f, from_client.c_str());

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
  if (vdrv_buf != mcf.vdrv_t) handleExit();
}

//-------------------------------------------------------------- handler SEND ThingSpeak Cfg
void handler_SEND_TSnr_Cfg() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tsid"] = mcf.ts_ch_id;
  json["tsar"] = mcf.AKey_r;
  json["tsaw"] = mcf.AKey_w;
  json["tss"] = mcf.use_ts;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler Send Sensor Config
void handler_SEND_Snr_Cfg() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["cyid"] = mcf.pp_city_id;
  json["owmk"] = mcf.owm_key;
  json["srve1"] = mcf.esrv1_addr;
  json["srve2"] = mcf.esrv2_addr;
  json["srvr"] = mcf.radio_addr;
  json["udp"] = mcf.srudp_addr;
  json["udm"] = mcf.udp_mon;
  json["prgp"] = mcf.use_pp;
  json["s1"] = snr_cfg_data.type_snr1;
  json["s2"] = snr_cfg_data.type_snr2;
  json["s3"] = snr_cfg_data.type_snr3;
  json["sp"] = snr_cfg_data.type_snrp;
  json["nc1"] = mcf.ch1_name;
  json["nc2"] = mcf.ch2_name;
  json["nc3"] = mcf.ch3_name;
  json["peri"] = mcf.period;
  json["esm"] = mcf.esm;
  json["ess"] = mcf.use_es;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler GET data for sensor from ext source e.g. ext sensor
void handler_GET_Snr_Data() {
  /*
      url='/rcv_snr?
       + '&est1='  + est1_t
       + '&est2='  + est2_t
       + '&est3='  + est3_t
       + '&esh1='  + esh1_t
       + '&esh2='  + esh2_t
       + '&esh3='  + esh3_t
       + '&esp='   + esp_t;
  */
  float t = 99, h = 0, p = 700;
  if (server.arg("est1") != 0) {
    t = constrain(server.arg("est1").toInt(), -99, 99);
    if ((t < 99) & (t > -99)) snr_data.t1 = t;
  }
  t = 99;
  if (server.arg("est2") != 0) {
    t = constrain(server.arg("est2").toInt(), -99, 99);
    if ((t < 99) & (t > -99)) snr_data.t2 = t;
  }
  t = 99;
  if (server.arg("est3") != 0) {
    t = constrain(server.arg("est3").toInt(), -99, 99);
    if ((t < 99) & (t > -99)) snr_data.t3 = t;
  }
  h = constrain(server.arg("esh1").toInt(), 0, 101);
  if ((h < 101) & (h > 0)) snr_data.h1 = h;
  h = 0;
  h = constrain(server.arg("esh2").toInt(), 0, 101);
  if ((h < 101) & (h > 0)) snr_data.h2 = h;
  h = 0;
  h = constrain(server.arg("esh3").toInt(), 0, 101);
  if ((h < 101) & (h > 0)) snr_data.h3 = h;

  p = constrain(server.arg("esp").toInt(), 700, 800);
  if ((p < 800) & (p > 700)) snr_data.p = p;

  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler SEND sensor data
void handler_SEND_Snr_Data() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["t1"] = snr_data.t1;
  json["t2"] = snr_data.t2;
  json["t3"] = snr_data.t3;
  json["h1"] = snr_data.h1;
  json["h2"] = snr_data.h2;
  json["h3"] = snr_data.h3;
  json["pres"] = snr_data.p;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler Update data from sensor
void handler_UPD_Snr_Data() {
  snr_data_t sb = snr_data;
  snr_data = GetSnr(sb, snr_cfg_data, mcf, rtc_hw.a_type, wifi_data_cur.cli, wf_data_cur);

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handler Update Weather ForeCast data
void handler_UPD_WFC_Data() {
  //------------------------------------------------------ Получаем прогноз погоды от GisMeteo
  if ((mcf.use_pp == 1) & wifi_data_cur.cli) wf_data = e_srv.get_gm(gs_rcv(mcf.pp_city_id, wifi_data_cur.cli));

  //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
  if ((mcf.use_pp == 2) & wifi_data_cur.cli) {
    //------------------------------------------------------ на завтра
    String fore_st = getOWM_forecast(mcf.pp_city_id, mcf.owm_key);
    wf_data = forecast_decode(fore_st, 2);

    //------------------------------------------------------ на сегодня
    wf_data_cur = forecast_decode(fore_st, 1);
  }
}

//-------------------------------------------------------------- handler GET sensor Cfg part one
void handler_GET_Snr_Cfg_1() {
  /*
        url='/set_pars1?cid='+cid_t
           + '&owmk='+owmk_t
           + '&esa1='+esa1_t
           + '&esa2='+esa2_t
           + '&rda=' +rda_t
           + '&udp=' +udp_t
           + '&upp=' +upp_t
           + '&udm=' +udm_t;
  */

  mcf.pp_city_id = constrain(server.arg("cid").toInt(), 0, 999999);
  strcpy(mcf.owm_key, server.arg("owmk").c_str());
  strcpy(mcf.esrv1_addr, server.arg("esa1").c_str());
  strcpy(mcf.esrv2_addr, server.arg("esa2").c_str());
  strcpy(mcf.radio_addr, server.arg("rda").c_str());
  strcpy(mcf.srudp_addr, server.arg("udp").c_str());
  mcf.use_pp = server.arg("upp").toInt();
  mcf.udp_mon = server.arg("udm") == "1";

  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler GET Sensor Cfg part two
void handler_GET_Snr_Cfg_2() {
  /*
  	  url='/set_pars2?snr1='   + snr1_t
  		 + '&snr2='   + snr2_t
  		 + '&snr3='   + snr3_t
  		 + '&snrp='   + snrp_t
  		 + '&period=' + period_t
       + '&ues='    + ues_t
       + '&esm='    + esm_t
       + '&nc1='    + nc1_t
  		 + '&nc2='    + nc2_t
  		 + '&nc3='    + nc3_t;
  */

  snr_cfg_data.type_snr1 = server.arg("snr1").toInt();
  snr_cfg_data.type_snr2 = server.arg("snr2").toInt();
  snr_cfg_data.type_snr3 = server.arg("snr3").toInt();
  snr_cfg_data.type_snrp = server.arg("snrp").toInt();

  from_client = mysnrcfg.to_json(snr_cfg_data);

  conf_f = "/conf_snr.json";
  lfs.writeFile(conf_f, from_client.c_str());

  mcf.period = constrain(server.arg("period").toInt(), 1, 59);
  mcf.use_es = server.arg("ues").toInt();
  mcf.esm = server.arg("esm") == "1";

  strcpy(mcf.ch1_name, server.arg("nc1").c_str());
  strcpy(mcf.ch2_name, server.arg("nc2").c_str());
  strcpy(mcf.ch3_name, server.arg("nc3").c_str());

  server.send(200, "text/html", "OK!");

  sens_f.init(&snr_cfg_data);
}

//-------------------------------------------------------------- handle GET sensor Cfg part tree
void handler_GET_Snr_Cfg_3() {
  /*
  	  url='/set_pars3?tschan=' + tschan_t
  		 + '&tsapir=' + tsapir_t
  		 + '&tsapiw=' + tsapiw_t
  		 + '&uts='    + uts_t;
  */

  mcf.ts_ch_id = constrain(server.arg("tschan").toInt(), 0, 999999);
  strcpy(mcf.AKey_r, server.arg("tsapir").c_str());
  strcpy(mcf.AKey_w, server.arg("tsapiw").c_str());
  mcf.use_ts = server.arg("uts").toInt();

  from_client = main_cfg.to_json(mcf);

  conf_f = "/conf_main.json";
  lfs.writeFile(conf_f, from_client.c_str());

  server.send(200, "text/html", "OK!");

  handleExit();
}

//-------------------------------------------------------------- handler_SEND_GPIO_Cfg
void handler_SEND_GPIO_Cfg() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["sndpol"] = gcf.snd_pola;
  json["ledpol"] = gcf.led_pola;

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

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed forming json string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler_GET_GPIO_Cfg
void handler_GET_GPIO_Cfg() {
  //url = '/set_parc?sndpol='+sndpol+'&ledpol='+ledpol+
  //'&sda='+sda+'&scl='+scl+'&dio='+dio+'&clk='+clk+'&dcs='+dcs+'&dwr='+dwr+'&trm='+trm+'&sqw='+sqw+'&snd='+snd+'&led='+led+'&btn='+btn+
  //'&dht='+dht+'&ana='+ana+'&uar='+uar;

  gcf.snd_pola = (server.arg("sndpol") == "1");
  gcf.led_pola = (server.arg("ledpol") == "1");

  gcf.gpio_sda = constrain(server.arg("sda").toInt(), 0, 255);
  gcf.gpio_scl = constrain(server.arg("scl").toInt(), 0, 255);
  gcf.gpio_dio = constrain(server.arg("dio").toInt(), 0, 255);
  gcf.gpio_clk = constrain(server.arg("clk").toInt(), 0, 255);
  gcf.gpio_dcs = constrain(server.arg("dcs").toInt(), 0, 255);
  gcf.gpio_dwr = constrain(server.arg("dwr").toInt(), 0, 255);
  gcf.gpio_trm = constrain(server.arg("trm").toInt(), 0, 255);
  gcf.gpio_sqw = constrain(server.arg("sqw").toInt(), 0, 255);
  gcf.gpio_snd = constrain(server.arg("snd").toInt(), 0, 255);
  gcf.gpio_led = constrain(server.arg("led").toInt(), 0, 255);
  gcf.gpio_btn = constrain(server.arg("btn").toInt(), 0, 255);
  gcf.gpio_dht = constrain(server.arg("dht").toInt(), 0, 255);
  gcf.gpio_ana = constrain(server.arg("ana").toInt(), 0, 255);
  gcf.gpio_uar = constrain(server.arg("uar").toInt(), 0, 255);

  from_client = gpio_cfg.to_json(gcf);

  conf_f = "/conf_gpio.json";
  lfs.writeFile(conf_f, from_client.c_str());

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
  handleExit();
}

//-------------------------------------------------------------- handler_SEND_UART
void handler_SEND_UART() {
  server.send(200, "text/json", uart_st(snr_data, wf_data, mcf, rtc_time, rtc_alm, cur_br));
}

//-------------------------------------------------------------- handler_SEND_Tmst_Cfg
void handler_SEND_Tmst_Cfg() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["ttrm"] = mcf.thermo_t;
  json["tsrc"] = mcf.src_thermo;
  json["dsta"] = mcf.lb_thermo;
  json["dstp"] = mcf.hb_thermo;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler_GET_Tmst_Cfg
void handler_GET_Tmst_Cfg() {
  //url = '/set_partrm?ttrm='+sttrm+'&tsrc='+stsrs+'&dsta='+sdsta+'&dstp='+sdstp;

  mcf.thermo_t = server.arg("ttrm").toInt();
  mcf.src_thermo = server.arg("tsrc").toInt();
  mcf.lb_thermo = server.arg("dsta").toInt();
  mcf.hb_thermo = server.arg("dstp").toInt();

  server.send(200, "text/html", "Ok!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handler_SEND_BR_Act
void handler_SEND_BR_Act() {
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["brig"] = snr_data.f;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleExit
void handleExit() {
  stop_serv();
  if (debug_level == 14) DBG_OUT_PORT.println(F("Reboot esp..."));
  ESP.restart();
}

//-------------------------------------------------------------- for FS
bool handleFileRead(String path) {
  serv_ms = millis();
  DBG_OUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
    if (LittleFS.exists(pathWithGz))
      path += ".gz";
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload() {
  if (server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    DBG_OUT_PORT.print(F("handleFileUpload Name: "));
    DBG_OUT_PORT.println(filename);
    fsUploadFile = LittleFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUT_PORT.print("handleFileUpload Data: "); DBG_OUT_PORT.println(F(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
    DBG_OUT_PORT.printf("handleFileUpload Size: %u\n", upload.totalSize);
  }
  serv_ms = millis();
}

void handleFileDelete() {
  if (server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  DBG_OUT_PORT.println("handleFileDelete: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!LittleFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  LittleFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
  serv_ms = millis();
}

void handleFileCreate() {
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  DBG_OUT_PORT.println("handleFileCreate: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (LittleFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");

  File file = LittleFS.open(path, "w");
  if (file)
    file.close();
  else
    return server.send(500, "text/plain", "CREATE FAILED");
  server.send(200, "text/plain", "");
  path = String();
  serv_ms = millis();
}

void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    serv_ms = millis();
    return;
  }

  String path = server.arg("dir");
  DBG_OUT_PORT.println("handleFileList: " + path);

#if defined(ESP8266)
  Dir dir = LittleFS.openDir(path);
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name());
    output += "\"}";
    entry.close();
  }
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  File root = LittleFS.open(path);
  String output = "[";
  if (root.isDirectory()) {
    File file = root.openNextFile();
    while (file) {
      if (output != "[") output += ',';
      output += "{\"type\":\"";
      output += (file.isDirectory()) ? "dir" : "file";
      output += "\",\"name\":\"";
      output += String(file.name());
      output += "\"}";
      file = root.openNextFile();
    }
  }
#endif
  path = String();
  output += "]";
  server.send(200, "text/json", output);

  serv_ms = millis();
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

//-------------------------------------------------------------- handler GET news Cfg
void handler_GET_News_Cfg() {
  //url = '/set_news?displaynews='+sdisplaynews'&newsApiKey='+snewsApiKey+'&newssource='+snewssource;
  char src_buff[17];
  strcpy(src_buff, mcf.news_source);

  mcf.news_en = (server.arg("displaynews") == "1");

  strcpy(mcf.news_api_key, server.arg("newsApiKey").c_str());
  strcpy(mcf.news_source, server.arg("newssource").c_str());

  from_client = main_cfg.to_json(mcf);

  conf_f = "/conf_main.json";
  lfs.writeFile(conf_f, from_client.c_str());

  if (strcmp(src_buff, mcf.news_source) != 0) handler_UPD_News();
  server.send(200, "text/html", "Ok!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handler Updater news
void handler_UPD_News() {
  if (mcf.news_en) {
    newsClient->updateNewsClient(mcf.news_api_key, mcf.news_source);
    newsClient->updateNews();
  }
}

//-------------------------------------------------------------- handler SEND news cfg
void handler_SEND_News_Cfg() {
  String st = "";
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["displaynews"] = mcf.news_en;
  json["newsApiKey"] = mcf.news_api_key;
  json["newssource"] = mcf.news_source;

  String st1 = String();
  if (serializeJson(jsonBuffer, st1) == 0) DBG_OUT_PORT.println(F("Failed forming json string"));
  server.send(200, "text/json", st1);
}

//-------------------------------------------------------------- handler SEND full text from news string
void handler_SEND_News_Full() {
  String st = "";
  JsonDocument jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  uint8_t inx = 0;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_1"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_2"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_3"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_4"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_5"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_6"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_7"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_8"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_9"] = st;
  inx++;
  st = newsClient->getUrl(inx) + newsClient->getTitle(inx);
  st += newsClient->getDescription(inx);
  json["st_10"] = st;

  String st1 = String();
  if (serializeJson(jsonBuffer, st1) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  server.send(200, "text/json", st1);
}

#endif
