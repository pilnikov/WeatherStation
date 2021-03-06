#if defined(ESP8266) || defined(ESP32)
// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/ntp", handleNTP);
  server.on("/set_time", handleSetTime);
  server.on("/set_wifi", handleSetWiFi);
  server.on("/exit", handleExit);
# ifdef MATRIX
  server.on("/set_font", handleSetFont);
# endif //MATRIX
  server.on("/sel_anum", handleSelNum);
  server.on("/set_pard", handleSetPard);
  server.on("/set_pars1", handleSetPars1);
  server.on("/set_pars2", handleSetPars2);
  server.on("/set_pars3", handleSetPars3);
  server.on("/set_parc", handleSetParc);
  server.on("/set_alm", handleSetAlarm);
  server.on("/set_partrm", handleSetPartrm);
  server.on("/jact", handlejAct);
  server.on("/jtime", handlejTime);
  server.on("/jwifi", handlejWiFi);
  server.on("/jclock", handlejParc);
  server.on("/jdisp", handlejPard);
  server.on("/jsens", handlejPars);
  server.on("/jts", handlejTS);
  server.on("/jalarm", handlejAlarm);
  server.on("/jsnr", handlejSnr);
  server.on("/juart", handlejUart);
  server.on("/jtrm", handlejTrm);
  //server.on("/all",       handleAll  );

  //-------------------------------------------------------------- for SPIFFS
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
  //use it to load content from SPIFFS
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
  /*
  	//get heap status, analog input value and all GPIO statuses in one json call
  	server.on("/all", HTTP_GET, []() {
  	  String json = "{";
  	  json += "\"heap\":" + String(ESP.getFreeHeap());
  	  json += ", \"analog\":" + String(analogRead(A0));
  	  json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
  	  json += "}";
  	  server.send(200, "text/json", json);
  	  json = String();
  	});
    //    server.begin();
    //    DBG_OUT_PORT.println("HTTP server started");
    }
    }

    void fsb_run(void) {
    server.handleClient();
  */

  //-------------------------------------------------------------- http update

#if defined(ESP8266)
  httpUpdater.setup(&server);
#endif

#if defined(ESP32)
  HTTPUpload& upload = server.upload();
#endif
  //  server.begin();
}

//-------------------------------------------------------------- Start_serv
void start_serv()
{
  if (web_cli || web_ap)
  {
    server.begin();
    DBG_OUT_PORT.println("Server started");
  }
}

//-------------------------------------------------------------- Stop_serv
void stop_serv()
{
  server.stop();
  if (conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(LED_BUILTIN, HIGH); //Гасим светодиод
  if (debug_level == 14) DBG_OUT_PORT.println("Server stopped");
  stop_wifi();
}

//-------------------------------------------------------------- cur_time_str
String cur_time_str()
{
  char buf[25];

  snprintf(buf, 25, "%s %02u.%02u.%04u %02u:%02u:%02u", name_week[weekday()],
          day(), month(), year(), hour(), minute(), second());
return String(buf);
}

//-------------------------------------------------------------- handlejTime
void handlejTime()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["jhour"] = hour();
  json["jmin"] = minute();
  json["jday"] = day();
  json["jmonth"] = month();
  json["jyear"] = year();
  json["tstr"] = cur_time_str();;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetTime
void handleSetTime()
{
  //url='/set_time?h='+h+'&m='+m+'&d='+d+'&mm='+mm+'&y='+y;

  uint8_t  hr = server.arg("h").toInt();
  uint8_t  mn = server.arg("m").toInt();
  uint8_t  dy = server.arg("d").toInt();
  uint8_t  mo = server.arg("mm").toInt();
  uint16_t yr = server.arg("y").toInt();

  ds_tm_stru.Hour = constrain(hr, 0, 23);
  ds_tm_stru.Minute = constrain(mn, 0, 59);
  ds_tm_stru.Day = constrain(dy, 1, 31);
  ds_tm_stru.Month = constrain(mo, 1, 12);
  ds_tm_stru.Year = yr - 1970;

  ds_tm_stru.Wday = 0;
  ds_tm_stru.Second = 0;

  if (debug_level == 14) DBG_OUT_PORT.printf("set time = %d %02d.%02d.%04d %02d:%02d:%02d\n",
        ds_tm_stru.Wday, ds_tm_stru.Day, ds_tm_stru.Month, ds_tm_stru.Year + 1970, ds_tm_stru.Hour, ds_tm_stru.Minute, ds_tm_stru.Second);

  man_set_time();

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleNTP
void handleNTP()
{
  GetNtp();

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejWiFi
void handlejWiFi()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["apid"]   = conf_data.ap_ssid;
  json["appas"]  = conf_data.ap_pass;
  json["staid"]  = conf_data.sta_ssid;
  json["stapas"] = conf_data.sta_pass;
  json["wof"]    = conf_data.wifi_off;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  //url='/set_wifi?as='+as+'&ap='+ap+'&ss='+ss+'&sp='+sp+'&wof='+wof_t;
 
  strcpy(conf_data.ap_ssid, server.arg("as").c_str());
  strcpy(conf_data.ap_pass, server.arg("ap").c_str());
  strcpy(conf_data.sta_ssid, server.arg("ss").c_str());
  strcpy(conf_data.sta_pass, server.arg("sp").c_str());
  conf_data.wifi_off = server.arg("wof") == "1";

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
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
  json["abrd"] = conf_data.auto_br;
  json["mbrd"] = conf_data.man_br;

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
  strcpy(conf_data.test, "ok");
  saveConfig(conf_f, conf_data);
  if (debug_level == 14) DBG_OUT_PORT.printf("font is.... %u", conf_data.type_font);

  set_type_font();

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}
# endif //MATRIX

//-------------------------------------------------------------- handleSetPard
void handleSetPard()
{
  //url='/set_pard?dtyp='+dtyp+'&rdsp='+rdsp+'&abrd='+abrd+'&brd='+brd+'&brd1='+brd1+'&brd2='+brd2+'&brd3='+brd3+'&brd4='+brd4;

  uint8_t vdrv_buf = conf_data.type_vdrv;

  conf_data.type_vdrv = server.arg("ctyp").toInt();
  conf_data.type_disp = server.arg("dtyp").toInt();
  conf_data.rus_lng = server.arg("rlng") == "1";
  conf_data.auto_br = server.arg("abrd") == "1";
  uint16_t val = server.arg("brd").toInt();
  conf_data.man_br = constrain(val, 0, 15);;
  conf_data.br_level[0] = server.arg("brd1").toInt();
  conf_data.br_level[1] = server.arg("brd2").toInt();
  val = server.arg("brd3").toInt();
  if (val == conf_data.br_level[1]) conf_data.br_level[2] = val + 1;
  else conf_data.br_level[2] = val;
  val = server.arg("brd4").toInt();
  if (val == conf_data.br_level[0]) conf_data.br_level[3] = val + 1;
  else conf_data.br_level[3] = val;

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
  if (vdrv_buf != conf_data.type_vdrv) handleExit();
}

//-------------------------------------------------------------- handlejTS
void handlejTS()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tsid"] = conf_data.ts_ch_id;
  json["tsar"] = conf_data.AKey_r;
  json["tsaw"] = conf_data.AKey_w;
  json["tsst1"] = conf_data.use_tst1;
  json["tsst2"] = conf_data.use_tst2;
  json["tsst3"] = conf_data.use_tst3;
  json["tssh1"] = conf_data.use_tsh1;
  json["tssh2"] = conf_data.use_tsh2;
  json["tssh3"] = conf_data.use_tsh3;
  json["tssp"] = conf_data.use_tsp;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejPars
void handlejPars()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["cyid"] = conf_data.pp_city_id;
  json["owmk"] = conf_data.owm_key;
  json["srve1"] = conf_data.esrv1_addr;
  json["srve2"] = conf_data.esrv2_addr;
  json["srvr"] = conf_data.radio_addr;
  json["prgp"] = conf_data.use_pp;
  json["s1"] = conf_data.type_snr1;
  json["s2"] = conf_data.type_snr2;
  json["s3"] = conf_data.type_snr3;
  json["sp"] = conf_data.type_snrp;
  json["nc1"] = conf_data.ch1_name;
  json["nc2"] = conf_data.ch2_name;
  json["nc3"] = conf_data.ch3_name;
  json["peri"] = conf_data.period;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejSnr
void handlejSnr()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["t1"] = snr_data.t1;
  json["t2"] = snr_data.t2;
  json["t3"] = snr_data.t3;
  json["h1"] = snr_data.h1;
  json["h2"] = snr_data.h2;
  json["h3"] = snr_data.h3;
  json["pres"] = snr_data.p;
  json["brig"] = snr_data.f;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handle Set Parameter for sensor
void handleSetPars1()
{
  /*
        url='/set_pars1?cid='+cid_t
           + '&owmk='+owmk_t
           + '&esa1='+esa1_t
           + '&esa2='+esa2_t
           + '&rda=' +rda_t
           + '&upp=' +upp_t;
  */

  conf_data.pp_city_id = constrain(server.arg("cid").toInt(), 0, 999999);
  strcpy(conf_data.owm_key, server.arg("owmk").c_str());
  strcpy(conf_data.esrv1_addr, server.arg("esa1").c_str());
  strcpy(conf_data.esrv2_addr, server.arg("esa2").c_str());
  strcpy(conf_data.radio_addr, server.arg("rda").c_str());
  conf_data.use_pp = server.arg("upp").toInt();

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();

  if (conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));
  if (conf_data.use_pp == 2) {
    wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);
  }
}

//-------------------------------------------------------------- handle Set Parameter for sensor
void handleSetPars2()
{
  /*
  	  url='/set_pars2?snr1='   + snr1_t
  		 + '&snr2='   + snr2_t
  		 + '&snr3='   + snr3_t
  		 + '&snrp='   + snrp_t
  		 + '&period=' + period_t
  		 + '&nc1='    + nc1_t
  		 + '&nc2='    + nc2_t
  		 + '&nc3='    + nc3_t;
  */

  conf_data.type_snr1 = server.arg("snr1").toInt();
  conf_data.type_snr2 = server.arg("snr2").toInt();
  conf_data.type_snr3 = server.arg("snr3").toInt();
  conf_data.type_snrp = server.arg("snrp").toInt();
  conf_data.period = constrain(server.arg("period").toInt(), 1, 59);
  strcpy(conf_data.ch1_name, server.arg("nc1").c_str());
  strcpy(conf_data.ch2_name, server.arg("nc2").c_str());
  strcpy(conf_data.ch3_name, server.arg("nc3").c_str());

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();

  ram_data.type_snr1 = conf_data.type_snr1;
  ram_data.type_snr2 = conf_data.type_snr2;
  ram_data.type_snr2 = conf_data.type_snr2;
  ram_data.type_snrp = conf_data.type_snrp;

  GetSnr();
}

//-------------------------------------------------------------- handle Set Parameter for sensor
void handleSetPars3()
{
  /*
  	  url='/set_pars3?tschan=' + tschan_t
  		 + '&tsapir=' + tsapir_t
  		 + '&tsapiw=' + tsapiw_t
  		 + '&utst1='  + utst1_t
  		 + '&utst2='  + utst2_t
  		 + '&utst3='  + utst3_t
  		 + '&utsh1='  + utsh1_t
  		 + '&utsh2='  + utsh2_t
  		 + '&utsh3='  + utsh3_t
  		 + '&utsp='   + utsp_t;
  */

  conf_data.ts_ch_id = constrain(server.arg("tschan").toInt(), 0, 999999);
  strcpy(conf_data.AKey_r, server.arg("tsapir").c_str());
  strcpy(conf_data.AKey_w, server.arg("tsapiw").c_str());
  conf_data.use_tst1 = server.arg("utst1") == "1";
  conf_data.use_tst2 = server.arg("utst2") == "1";
  conf_data.use_tst3 = server.arg("utst3") == "1";
  conf_data.use_tsh1 = server.arg("utsh1") == "1";
  conf_data.use_tsh2 = server.arg("utsh2") == "1";
  conf_data.use_tsh3 = server.arg("utsh3") == "1";
  conf_data.use_tsp  = server.arg("utsp") == "1";

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejParc
void handlejParc()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tzon"] = conf_data.time_zone;
  json["acor"] = conf_data.auto_corr;
  json["uspm"] = conf_data.use_pm;
  json["nstr"] = conf_data.nm_start;
  json["nend"] = conf_data.nm_stop;
  json["evhb"] = conf_data.every_hour_beep;
  json["trts"] = conf_data.type_rtc;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetParc
void handleSetParc()
{
  //url = '/set_parc?tzone='+tzone+'&acorr='+acorr+'&upm='+upm+'&nmstart='+nmstart+'&nmstop='+nmstop+'&ehb='+ehb+'&srtyp='+srtyp;

  conf_data.time_zone = constrain(server.arg("tzone").toInt(), -12, 12);
  conf_data.auto_corr = (server.arg("acorr") == "1");
  conf_data.use_pm = (server.arg("upm") == "1");
  conf_data.nm_start = constrain(server.arg("nmstart").toInt(), 0, 23);
  conf_data.nm_stop  = constrain(server.arg("nmstop" ).toInt(), 0, 23);
  conf_data.every_hour_beep = (server.arg("ehb") == "1");
  conf_data.type_rtc = server.arg("srtyp").toInt();

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejAlarm
void handlejAlarm()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["anum"] = rtc_data.a_num;
  json["atyp"] = conf_data.alarms[rtc_data.a_num][0];
  json["ahou"] = conf_data.alarms[rtc_data.a_num][1];
  json["amin"] = conf_data.alarms[rtc_data.a_num][2];
  json["amel"] = conf_data.alarms[rtc_data.a_num][3];
  json["aact"] = conf_data.alarms[rtc_data.a_num][4];
  json["acth"] = rtc_data.a_hour;
  json["actm"] = rtc_data.a_min;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetAlarm
void handleSetAlarm()
{
  //url='/set_alm?satyp='+satyp+'&ahour='+ahour+'&amin='+amin+'&samel='+samel+'&saon='+saon;

  conf_data.alarms[rtc_data.a_num][0] = server.arg("satyp").toInt();
  uint8_t val = server.arg("ahour").toInt(); // час
  conf_data.alarms[rtc_data.a_num][1] = constrain(val, 0, 23);
  val = server.arg("amin").toInt(); // минута
  conf_data.alarms[rtc_data.a_num][2] = constrain(val, 0, 59);
  conf_data.alarms[rtc_data.a_num][3] = server.arg("samel").toInt();
  conf_data.alarms[rtc_data.a_num][4] = server.arg("saon").toInt();

  if (debug_level == 14)
  {
    DBG_OUT_PORT.print(rtc_data.a_num); DBG_OUT_PORT.print(" alarm is....");
    for (int n = 0; n <= 4; n++) DBG_OUT_PORT.print(conf_data.alarms[rtc_data.a_num][n]);
    DBG_OUT_PORT.println();
  }

  saveConfig(conf_f, conf_data);
  set_alarm();
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSelectNumber
void handleSelNum()
{
  //url='/sel_anum?sanum='+sanum;

  uint8_t val = server.arg("sanum").toInt();
  rtc_data.a_num = constrain(val, 0, 6);
  if (debug_level == 14) DBG_OUT_PORT.printf("number of alarm is....%u", rtc_data.a_num);

  server.send(200, "text/html", "Ok!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejUart
void handlejUart()
{
  server.send(200, "text/json", uart_st());
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

  saveConfig(conf_f, conf_data);

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

//-------------------------------------------------------------- handlejAct
void handlejAct()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tstr"] = cur_time_str();
  json["acth"] = rtc_data.a_hour;
  json["actm"] = rtc_data.a_min;
  json["t1"] = snr_data.t1;
  json["t2"] = snr_data.t2;
  json["t3"] = snr_data.t3;
  json["h1"] = snr_data.h1;
  json["h2"] = snr_data.h2;
  json["h3"] = snr_data.h3;
  json["pres"] = snr_data.p;
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
  if (debug_level == 14)  DBG_OUT_PORT.println("Reboot esp...");
  delay(100);
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
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
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
    DBG_OUT_PORT.print("handleFileUpload Name: "); DBG_OUT_PORT.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUT_PORT.print("handleFileUpload Data: "); DBG_OUT_PORT.println(upload.currentSize);
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
  if (!SPIFFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
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
  if (SPIFFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
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

  Dir dir = SPIFFS.openDir(path);
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
#endif

#if defined(ESP32)

  File root = SPIFFS.open(path);
  String output = "[";
  if (root.isDirectory()) {
    File file = root.openNextFile();
    while (file) {
      if (output != "[") {
        output += ',';
      }
      output += "{\"type\":\"";
      output += (file.isDirectory()) ? "dir" : "file";
      output += "\",\"name\":\"";
      output += String(file.name()).substring(1);
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

# endif
