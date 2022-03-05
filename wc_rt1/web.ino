#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
//#include "conf.h"
#include "cfg.h"

File fsUploadFile; //Should be placed here as global

// ----------------------------------- Web updater
#if defined(ESP8266)
ESP8266HTTPUpdateServer httpUpdater;
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
HTTPUpdateServer httpUpdater;
#endif

// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/ntp",         handleNTP);
  server.on("/set_time",    handleSetTime);
  server.on("/set_part",    handleSetPart);
  server.on("/set_wifi",    handleSetWiFi);
  server.on("/set_ip1",     handleSetIp1);
  server.on("/set_ip2",     handleSetIp2);
  server.on("/end_set_wifi", handleEndSetWiFi);
  server.on("/exit",        handleExit);
# ifdef MATRIX
  server.on("/set_font",    handleSetFont);
# endif //MATRIX
  server.on("/set_pard",    handleSetPard);
  server.on("/upd_snr",     handleUpdSnr);
  server.on("/upd_wfc",     handleUpdForeCast);
  server.on("/set_pars1",   handleSetPars1);
  server.on("/set_pars2",   handleSetPars2);
  server.on("/set_pars3",   handleSetPars3);
  server.on("/set_parc",    handleSetParc);
  server.on("/set_partrm",  handleSetPartrm);
  server.on("/set_news",    handleSetNews);
  server.on("/upd_news",    handleUpdNews);

  server.on("/jactt",       handlejActT);
  server.on("/jacta",       handlejActA);
  server.on("/jtime",       handlejTime);
  server.on("/jwifi",       handlejWiFi);
  server.on("/jclock",      handlejParc);
  server.on("/jactb",       handlejActB);
  server.on("/jdisp",       handlejPard);
  server.on("/jsens",       handlejPars);
  server.on("/jts",         handlejTS);
  server.on("/jsnr",        handlejSnr);
  server.on("/rcv_snr",     handleRcvSnr);
  server.on("/juart",       handlejUart);
  server.on("/jtrm",        handlejTrm);
  server.on("/jnews",       handlejNews);
  server.on("/jnewst",      handlejNewsT);

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
void start_serv()
{
  if (!wifi_data_cur.cli & !wifi_data_cur.ap)
  {
    wifi_data_cur = wifi.begin(wifi_data);
  }

  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
    server.begin();

    DBG_OUT_PORT.println(F("Server started"));
  }
}

//-------------------------------------------------------------- Stop_serv
void stop_serv()
{
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


//-------------------------------------------------------------- handlejTime
void handlejTime()
{
  from_client = myrtccfg.to_json(rtc_cfg);
  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handleSetTime
void handleSetTime()
{
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

//-------------------------------------------------------------- handleSetPart
void handleSetPart()
{
  from_client = server.arg("in");
  server.send(200, "text/html", "OK!");

  conf_f = "/conf_rtc.json";
  lfs.writeFile(conf_f, from_client.c_str());
  rtc_cfg = myrtccfg.from_json(from_client);
  rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
}

//-------------------------------------------------------------- handleNTP
void handleNTP()
{
  server.send(200, "text/html", "OK!");
  if (wifi_data_cur.cli)
  {
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

//-------------------------------------------------------------- handlejWiFi
void handlejWiFi()
{
  from_client = wifi_cfg.to_json(wifi_data);
  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  from_client = server.arg("in");
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleSetIp1
void handleSetIp1()
{
  from_client += server.arg("in");
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleSetIp2
void handleSetIp2()
{
  from_client += server.arg("in");
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleEndSetWiFi
void handleEndSetWiFi()
{
  from_client.replace("}{", ",");

  conf_f = "/conf_wifi.json";
  lfs.writeFile(conf_f, from_client.c_str());
  wifi_data = wifi_cfg.from_json(from_client);
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handlejActT
void handlejActT()
{
  DynamicJsonDocument jsonBuffer(100);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["boot"] = boot_mode;
  json["actw"] = wasAlarm;
  json["ct"]   = rtc_time.ct;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejActA
void handlejActA()
{
  DynamicJsonDocument jsonBuffer(300);
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["actn"] = rtc_alm.num;
  json["acta"] = rtc_alm.time;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejPard
void handlejPard()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  //	json["font"] = conf_data.type_font;
  json["ctyp"] = conf_data.type_vdrv;
  json["dtyp"] = conf_data.type_disp;
  json["rlng"] = conf_data.rus_lng;
  json["tup"]  = conf_data.time_up;
  json["colu"] = conf_data.color_up;
  json["cold"] = conf_data.color_dwn;
  json["abrd"] = conf_data.auto_br;
  json["mbrd"] = conf_data.man_br;
  json["nbrd"] = conf_data.nmd_br;

  JsonArray br_level = json.createNestedArray("brlevel");
  for (uint8_t i = 0; i <= 3; i++) br_level.add(conf_data.br_level[i]);

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetFont
# ifdef MATRIX
void handleSetFont()
{
  //url='/set_font?tfnt='+tfnt;

  conf_data.type_font = server.arg("tfnt").toInt();
  if (debug_level == 14) DBG_OUT_PORT.printf("font is.... %u", conf_data.type_font);

  conf_f = "/config.json";
  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}
# endif //MATRIX

//-------------------------------------------------------------- handler Set Parameter for display
void handleSetPard()
{
  //url='/set_pard?ctyp='+ctyp+'&dtyp='+dtyp+'&rlng='+rlng+'&ttup='+ttup+'&mcou='+mcou+'&mcod='+mcod+'&abrd='+abrd+'&mbrd='+mbrd+'&nbrd='+nbrd+'&brd1='+brd1+'&brd2='+brd2+'&brd3='+brd3+'&brd4='+brd4;

  uint8_t vdrv_buf = conf_data.type_vdrv;

  conf_data.type_vdrv = server.arg("ctyp").toInt();
  conf_data.type_disp = server.arg("dtyp").toInt();
  conf_data.rus_lng = server.arg("rlng") == "1";
  conf_data.time_up = server.arg("ttup") == "1";
  conf_data.color_up = server.arg("mcou").toInt();
  conf_data.color_dwn = server.arg("mcod").toInt();
  conf_data.auto_br = server.arg("abrd") == "1";
  uint16_t val = server.arg("mbrd").toInt();
  conf_data.man_br = constrain(val, 0, 254);;
  val = server.arg("nbrd").toInt();
  conf_data.nmd_br = constrain(val, 0, 254);;
  conf_data.br_level[0] = server.arg("brd1").toInt();
  conf_data.br_level[1] = server.arg("brd2").toInt();
  conf_data.br_level[2] = server.arg("brd3").toInt();
  conf_data.br_level[3] = server.arg("brd4").toInt();

  conf_f = "/config.json";
  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
  if (vdrv_buf != conf_data.type_vdrv) handleExit();
}

//-------------------------------------------------------------- handler Get Parameter from TS
void handlejTS()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tsid"] = conf_data.ts_ch_id;
  json["tsar"] = conf_data.AKey_r;
  json["tsaw"] = conf_data.AKey_w;
  json["tss"]  = conf_data.use_ts;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler Get Parameter from sensor
void handlejPars()
{
  DynamicJsonDocument jsonBuffer(700);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["cyid"]  = conf_data.pp_city_id;
  json["owmk"]  = conf_data.owm_key;
  json["srve1"] = conf_data.esrv1_addr;
  json["srve2"] = conf_data.esrv2_addr;
  json["srvr"]  = conf_data.radio_addr;
  json["udp"]   = conf_data.srudp_addr;
  json["udm"]   = conf_data.udp_mon;
  json["prgp"]  = conf_data.use_pp;
  json["s1"]    = snr_cfg_data.type_snr1;
  json["s2"]    = snr_cfg_data.type_snr2;
  json["s3"]    = snr_cfg_data.type_snr3;
  json["sp"]    = snr_cfg_data.type_snrp;
  json["nc1"]   = conf_data.ch1_name;
  json["nc2"]   = conf_data.ch2_name;
  json["nc3"]   = conf_data.ch3_name;
  json["peri"]  = conf_data.period;
  json["esm"]   = conf_data.esm;
  json["ess"]   = conf_data.use_es;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handler force set data for sensor from ext source e.g. ext sensor
void handleRcvSnr()
{
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
  if (server.arg("est1") != 0)
  {
    t = constrain(server.arg("est1").toInt(), -99, 99);
    if ((t < 99) & (t > -99)) snr_data.t1 = t;
  }
  t = 99;
  if (server.arg("est2") != 0)
  {
    t = constrain(server.arg("est2").toInt(), -99, 99);
    if ((t < 99) & (t > -99)) snr_data.t2 = t;
  }
  t = 99;
  if (server.arg("est3") != 0)
  {
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

//-------------------------------------------------------------- handler Get data from sensor
void handlejSnr()
{
  DynamicJsonDocument jsonBuffer(150);
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
void handleUpdSnr()
{
  snr_data_t sb = snr_data;
  snr_data = GetSnr(sb, snr_cfg_data, conf_data, rtc_hw.a_type, wifi_data_cur.cli, wf_data_cur);

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

void handleUpdForeCast()
{
  //------------------------------------------------------ Получаем прогноз погоды от GisMeteo
  if ((conf_data.use_pp == 1) & wifi_data_cur.cli) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id, wifi_data_cur.cli));

  //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
  if ((conf_data.use_pp == 2) & wifi_data_cur.cli)
  {
    wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);

    //------------------------------------------------------ Получаем прогноз погоды на сегодня от OpenWeatherMap
    wf_data_cur = getOWM_current(conf_data.pp_city_id, conf_data.owm_key);
  }
}

//-------------------------------------------------------------- handler Set Parameter for sensor part one
void handleSetPars1()
{
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

  conf_data.pp_city_id = constrain(server.arg("cid").toInt(), 0, 999999);
  strcpy(conf_data.owm_key, server.arg("owmk").c_str());
  strcpy(conf_data.esrv1_addr, server.arg("esa1").c_str());
  strcpy(conf_data.esrv2_addr, server.arg("esa2").c_str());
  strcpy(conf_data.radio_addr, server.arg("rda").c_str());
  strcpy(conf_data.srudp_addr, server.arg("udp").c_str());
  conf_data.use_pp  = server.arg("upp").toInt();
  conf_data.udp_mon = server.arg("udm") == "1";

  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handler Set Parameter for sensor part two
void handleSetPars2()
{
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

  conf_f = "/conf_snr.json";
  from_client = mysnrcfg.to_json(snr_cfg_data);
  lfs.writeFile(conf_f, from_client.c_str());
  snr_cfg_data = mysnrcfg.from_json(from_client);

  conf_data.period = constrain(server.arg("period").toInt(), 1, 59);
  conf_data.use_es = server.arg("ues").toInt();
  conf_data.esm = server.arg("esm") == "1";

  strcpy(conf_data.ch1_name, server.arg("nc1").c_str());
  strcpy(conf_data.ch2_name, server.arg("nc2").c_str());
  strcpy(conf_data.ch3_name, server.arg("nc3").c_str());

  server.send(200, "text/html", "OK!");

  sens_f.init(&snr_cfg_data);
}

//-------------------------------------------------------------- handle Set Parameter for sensor part tree
void handleSetPars3()
{
  /*
  	  url='/set_pars3?tschan=' + tschan_t
  		 + '&tsapir=' + tsapir_t
  		 + '&tsapiw=' + tsapiw_t
  		 + '&uts='    + uts_t;
  */

  conf_data.ts_ch_id = constrain(server.arg("tschan").toInt(), 0, 999999);
  strcpy(conf_data.AKey_r, server.arg("tsapir").c_str());
  strcpy(conf_data.AKey_w, server.arg("tsapiw").c_str());
  conf_data.use_ts = server.arg("uts").toInt();

  conf_f = "/config.json";
  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");

  handleExit();
}

//-------------------------------------------------------------- handlejParc
void handlejParc()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["sndpol"] = conf_data.snd_pola;
  json["ledpol"] = conf_data.led_pola;

  json["sda"] = conf_data.gpio_sda;
  json["scl"] = conf_data.gpio_scl;
  json["dio"] = conf_data.gpio_dio;
  json["clk"] = conf_data.gpio_clk;
  json["dcs"] = conf_data.gpio_dcs;
  json["dwr"] = conf_data.gpio_dwr;
  json["trm"] = conf_data.gpio_trm;
  json["sqw"] = conf_data.gpio_sqw;
  json["snd"] = conf_data.gpio_snd;
  json["led"] = conf_data.gpio_led;
  json["btn"] = conf_data.gpio_btn;
  json["dht"] = conf_data.gpio_dht;
  json["ana"] = conf_data.gpio_ana;
  json["uar"] = conf_data.gpio_uar;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetParc
void handleSetParc()
{
  //url = '/set_parc?sndpol='+sndpol+'&ledpol='+ledpol+
  //'&sda='+sda+'&scl='+scl+'&dio='+dio+'&clk='+clk+'&dcs='+dcs+'&dwr='+dwr+'&trm='+trm+'&sqw='+sqw+'&snd='+snd+'&led='+led+'&btn='+btn+
  //'&dht='+dht+'&ana='+ana+'&uar='+uar;

  conf_data.snd_pola = (server.arg("sndpol") == "1");
  conf_data.led_pola = (server.arg("ledpol") == "1");

  conf_data.gpio_sda = constrain(server.arg("sda").toInt(), 0, 255);
  conf_data.gpio_scl = constrain(server.arg("scl").toInt(), 0, 255);
  conf_data.gpio_dio = constrain(server.arg("dio").toInt(), 0, 255);
  conf_data.gpio_clk = constrain(server.arg("clk").toInt(), 0, 255);
  conf_data.gpio_dcs = constrain(server.arg("dcs").toInt(), 0, 255);
  conf_data.gpio_dwr = constrain(server.arg("dwr").toInt(), 0, 255);
  conf_data.gpio_trm = constrain(server.arg("trm").toInt(), 0, 255);
  conf_data.gpio_sqw = constrain(server.arg("sqw").toInt(), 0, 255);
  conf_data.gpio_snd = constrain(server.arg("snd").toInt(), 0, 255);
  conf_data.gpio_led = constrain(server.arg("led").toInt(), 0, 255);
  conf_data.gpio_btn = constrain(server.arg("btn").toInt(), 0, 255);
  conf_data.gpio_dht = constrain(server.arg("dht").toInt(), 0, 255);
  conf_data.gpio_ana = constrain(server.arg("ana").toInt(), 0, 255);
  conf_data.gpio_uar = constrain(server.arg("uar").toInt(), 0, 255);

  conf_f = "/config.json";
  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
  handleExit();
}

//-------------------------------------------------------------- handlejUart
void handlejUart()
{
  server.send(200, "text/json", uart_st(snr_data, wf_data, conf_data, rtc_time, rtc_alm, cur_br));
}

//-------------------------------------------------------------- handlejTrm
void handlejTrm()
{
  DynamicJsonDocument jsonBuffer(64);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["ttrm"] = conf_data.type_thermo;
  json["tsrc"] = conf_data.src_thermo;
  json["dsta"] = conf_data.lb_thermo;
  json["dstp"] = conf_data.hb_thermo;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetpartrm
void handleSetPartrm()
{
  //url = '/set_partrm?ttrm='+sttrm+'&tsrc='+stsrs+'&dsta='+sdsta+'&dstp='+sdstp;

  conf_data.type_thermo = server.arg("ttrm").toInt();
  conf_data.src_thermo = server.arg("tsrc").toInt();
  conf_data.lb_thermo = server.arg("dsta").toInt();
  conf_data.hb_thermo = server.arg("dstp").toInt();

  server.send(200, "text/html", "Ok!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejActB
void handlejActB()
{
  DynamicJsonDocument jsonBuffer(100);
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["brig"] = snr_data.f;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleExit
void handleExit()
{
  stop_serv();
  if (debug_level == 14)  DBG_OUT_PORT.println(F("Reboot esp..."));
  ESP.restart();
}

//-------------------------------------------------------------- for FS
bool handleFileRead(String path)
{
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

void handleFileUpload()
{
  if (server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    DBG_OUT_PORT.print(F("handleFileUpload Name: ")); DBG_OUT_PORT.println(filename);
    fsUploadFile = LittleFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUT_PORT.print("handleFileUpload Data: "); DBG_OUT_PORT.println(F(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
    DBG_OUT_PORT.printf("handleFileUpload Size: %u\n", upload.totalSize);
  }
  serv_ms = millis();
}

void handleFileDelete()
{
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

void handleFileCreate()
{
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

void handleFileList()
{
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

String getContentType(String filename)
{
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

void handleSetNews()
{
  //url = '/set_news?displaynews='+sdisplaynews'&newsApiKey='+snewsApiKey+'&newssource='+snewssource;
  char src_buff[17];
  strcpy(src_buff, conf_data.news_source);

  conf_data.news_en = (server.arg("displaynews") == "1");

  strcpy(conf_data.news_api_key, server.arg("newsApiKey").c_str());
  strcpy(conf_data.news_source, server.arg("newssource").c_str());

  conf_f = "/config.json";
  saveConfig(conf_f, conf_data);

  if (strcmp(src_buff, conf_data.news_source) != 0) handleUpdNews();
  server.send(200, "text/html", "Ok!");
  serv_ms = millis();
}

void handleUpdNews()
{
  if (conf_data.news_en)
  {
    newsClient -> updateNewsClient(conf_data.news_api_key, conf_data.news_source);
    newsClient -> updateNews();
  }
}


//-------------------------------------------------------------- handler Get Parameter from sensor
void handlejNews()
{
  String st = "";
  DynamicJsonDocument jsonBuffer(200);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["displaynews"] = conf_data.news_en;
  json["newsApiKey"]  = conf_data.news_api_key;
  json["newssource"]  = conf_data.news_source;

  String st1 = String();
  if (serializeJson(jsonBuffer, st1) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  server.send(200, "text/json", st1);
}

//-------------------------------------------------------------- handler Get Parameter from sensor
void handlejNewsT()
{
  String st = "";
  DynamicJsonDocument jsonBuffer(4096);
  JsonObject json = jsonBuffer.to<JsonObject>();

  uint8_t inx = 0;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_1"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_2"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_3"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_4"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_5"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_6"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_7"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_8"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_9"] = st;
  inx++;
  st = newsClient -> getUrl(inx) + newsClient -> getTitle(inx);
  st += newsClient -> getDescription(inx);
  json["st_10"] = st;

  String st1 = String();
  if (serializeJson(jsonBuffer, st1) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  server.send(200, "text/json", st1);
}

# endif
