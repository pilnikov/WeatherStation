#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/ntp", handleNTP);
  server.on("/set_time1", handleSetTime1);
  server.on("/set_time2", handleSetTime2);
  server.on("/set_alarm", handleSetAlarm);
  server.on("/set_wifi",  handleSetWiFi);
  server.on("/set_ip1",   handleSetIp1);
  server.on("/set_ip2",   handleSetIp2);
  server.on("/exit",      handleExit);
  server.on("/jactt",     handlejActT);
  server.on("/jacta",     handlejActA);
  server.on("/jtime1",    handlejTime1);
  server.on("/jtime2",    handlejTime2);
  server.on("/jalarm",    handlejAlarm);
  server.on("/jwifi",     handlejWiFi);

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
    myIP = wifi_data_cur.addr;
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

//-------------------------------------------------------------- handlejTime1
void handlejTime1()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["hour"]  = rtc_time.hour;
  json["min"]   = rtc_time.min;
  json["day"]   = rtc_time.day;
  json["month"] = rtc_time.month;
  json["year"]  = rtc_time.year;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejTime2
void handlejTime2()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tzon"]  = rtc_cfg.time_zone;
  json["acor"]  = rtc_cfg.auto_corr;
  json["uspm"]  = rtc_cfg.use_pm;
  json["nstr"]  = rtc_cfg.nm_start;
  json["nend"]  = rtc_cfg.nm_stop;
  json["evhb"]  = rtc_cfg.every_hour_beep;
  json["trts"]  = rtc_cfg.c_type;
  json["antp1"] = rtc_cfg.ntp_srv[0];
  json["antp2"] = rtc_cfg.ntp_srv[1];
  json["antp3"] = rtc_cfg.ntp_srv[2];

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejAlarm
void handlejAlarm()
{
  DynamicJsonDocument jsonBuffer(700);
  JsonObject json = jsonBuffer.to<JsonObject>();

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
    al0.add(rtc_cfg.alarms[0][j]);
    al1.add(rtc_cfg.alarms[1][j]);
    al2.add(rtc_cfg.alarms[2][j]);
    al3.add(rtc_cfg.alarms[3][j]);
    al4.add(rtc_cfg.alarms[4][j]);
    al5.add(rtc_cfg.alarms[5][j]);
    al6.add(rtc_cfg.alarms[6][j]);
  }

  JsonObject alarms = json.createNestedObject("al");
  alarms["0"] = al0;
  alarms["1"] = al1;
  alarms["2"] = al2;
  alarms["3"] = al3;
  alarms["4"] = al4;
  alarms["5"] = al5;
  alarms["6"] = al6;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetTime1
void handleSetTime1()
{
  //url='/set_time1?h='+h+'&m='+m+'&d='+d+'&mm='+mm+'&y='+y;

  uint8_t  hr = server.arg("h").toInt();
  uint8_t  mn = server.arg("m").toInt();
  uint8_t  dy = server.arg("d").toInt();
  uint8_t  mo = server.arg("mm").toInt();
  uint16_t yr = server.arg("y").toInt();

  RtcDateTime dt1 = RtcDateTime(constrain(yr, 2021, 2036), constrain(mo, 1, 12), constrain(dy, 1, 31), constrain(hr, 0, 23), constrain(mn, 0, 59), 0);

  if (debug_level == 14) DBG_OUT_PORT.printf("set time = %02d.%02d.%04d %02d:%02d:%02d\n",
        dt1.Day(), dt1.Month(), dt1.Year(), dt1.Hour(), dt1.Minute(), dt1.Second());

  rtc_time.ct = myrtc.man_set_time(rtc_hw, dt1);
  rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSetTime2
void handleSetTime2()
{
  //url = '/set_time2?tzone='+tzone+'&acorr='+acorr+'&upm='+upm+'&nmstart='+nmstart+'&nmstop='+nmstop+'&ehb='+ehb+'&srtyp='+srtyp+'&antp1='+antp1+'&antp2='+antp2+'&antp3='+antp3;

  rtc_cfg.time_zone = constrain(server.arg("tzone").toInt(), -12, 12);
  rtc_cfg.auto_corr = (server.arg("acorr") == "1");
  rtc_cfg.use_pm = (server.arg("upm") == "1");
  rtc_cfg.nm_start = constrain(server.arg("nmstart").toInt(), 0, 23);
  rtc_cfg.nm_stop  = constrain(server.arg("nmstop" ).toInt(), 0, 23);
  rtc_cfg.every_hour_beep = (server.arg("ehb") == "1");
  rtc_cfg.c_type = server.arg("srtyp").toInt();
  strcpy(rtc_cfg.ntp_srv[0], server.arg("antp1").c_str());
  strcpy(rtc_cfg.ntp_srv[1], server.arg("antp2").c_str());
  strcpy(rtc_cfg.ntp_srv[2], server.arg("antp3").c_str());

  conf_f = "/conf_rtc.json";
  myrtccfg.saveConfig(conf_f, rtc_cfg);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();

  rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);
}

//-------------------------------------------------------------- handleSetAlarm
void handleSetAlarm()
{
  //url = '/set_alarm?sanum='+sanum+'&satyp='+satyp+'&ahour='+ahour+'&amin='+amin+'&samel='+samel+'&saon='+saon;

  uint8_t alm_num = server.arg("sanum").toInt(); // номер будильника
  rtc_cfg.alarms[alm_num][0] = server.arg("satyp").toInt();
  uint8_t val = server.arg("ahour").toInt(); // час
  rtc_cfg.alarms[alm_num][1] = constrain(val, 0, 23);
  val = server.arg("amin").toInt(); // минута
  rtc_cfg.alarms[alm_num][2] = constrain(val, 0, 59);
  rtc_cfg.alarms[alm_num][3] = server.arg("samel").toInt();
  rtc_cfg.alarms[alm_num][4] = server.arg("saon").toInt();

  if (debug_level == 14)
  {
    DBG_OUT_PORT.print(alm_num); DBG_OUT_PORT.print(F(" alarm is...."));
    for (int n = 0; n < 5; n++)
    {
      DBG_OUT_PORT.print(rtc_cfg.alarms[alm_num][n]); DBG_OUT_PORT.print(F(","));
    }
    DBG_OUT_PORT.println();
  }

  conf_f = "/conf_rtc.json";
  myrtccfg.saveConfig(conf_f, rtc_cfg);
  rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleNTP
void handleNTP()
{
  RtcDateTime c_time;
  if (wifi_data_cur.cli)
  {
    c_time = myrtc.GetNtp(rtc_cfg);
    rtc_time.ct = myrtc.man_set_time(rtc_hw, c_time);
    rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);
  }

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejWiFi
void handlejWiFi()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["apid"]    = wifi_data.ap_ssid;
  json["appas"]   = wifi_data.ap_pass;
  json["staid1"]  = wifi_data.sta_ssid1;
  json["staid2"]  = wifi_data.sta_ssid2;
  json["stapas1"] = wifi_data.sta_pass1;
  json["stapas2"] = wifi_data.sta_pass2;

  json["iap"]   = wifi_data.ap_ip;
  json["map"]   = wifi_data.ap_ma;

  json["sst1"]    = wifi_data.st_ip1;
  json["sst2"]    = wifi_data.st_ip2;

  if (wifi_data.st_ip1)
  {
    json["ipst1"]   = wifi_data.sta_ip1;
    json["mast1"]   = wifi_data.sta_ma1;
    json["gwst1"]   = wifi_data.sta_gw1;
    json["dns1st1"] = wifi_data.sta_dns11;
    json["dns2st1"] = wifi_data.sta_dns21;
  }
  if (wifi_data.st_ip1)
  {
    json["ipst2"]   = wifi_data.sta_ip2;
    json["mast2"]   = wifi_data.sta_ma2;
    json["gwst2"]   = wifi_data.sta_gw2;
    json["dns1st2"] = wifi_data.sta_dns12;
    json["dns2st2"] = wifi_data.sta_dns22;
  }
  json["wof"]    = wifi_data.wifi_off;


  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  //url='/set_wifi?as='+as+'&ap='+ap+'&ss1='+ss1+'&sp1='+sp1+'&ss2='+ss2+'&sp2='+sp2+'&st1='+st1+'&st2='+st2+'&iap='+iap+'&map='+map+'&wof='+wof_t;

  strcpy(wifi_data.ap_ssid, server.arg("as").c_str());
  strcpy(wifi_data.ap_pass, server.arg("ap").c_str());
  strcpy(wifi_data.sta_ssid1, server.arg("ss1").c_str());
  strcpy(wifi_data.sta_pass1, server.arg("sp1").c_str());
  strcpy(wifi_data.sta_ssid2, server.arg("ss2").c_str());
  strcpy(wifi_data.sta_pass2, server.arg("sp2").c_str());

  wifi_data.st_ip1 = server.arg("st1") == "1";
  wifi_data.st_ip2 = server.arg("st2") == "1";

  strcpy(wifi_data.ap_ip, server.arg("iap").c_str());
  strcpy(wifi_data.ap_ma, server.arg("map").c_str());

  wifi_data.wifi_off = server.arg("wof") == "1";

  conf_f = "/conf_wifi.json";
  wifi.saveConfig(conf_f, wifi_data);

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSetIp1
void handleSetIp1()
{
  //url='/set_ip1?ip='+ip1+'&ma='+ma1+'&gw='+gw1+'&d1='+d11+'&d2='+d21;

  strcpy(wifi_data.sta_ip1, server.arg("ip").c_str());
  strcpy(wifi_data.sta_ma1, server.arg("ma").c_str());
  strcpy(wifi_data.sta_gw1, server.arg("gw").c_str());
  strcpy(wifi_data.sta_dns11, server.arg("d1").c_str());
  strcpy(wifi_data.sta_dns21, server.arg("d2").c_str());

  conf_f = "/conf_wifi.json";
  wifi.saveConfig(conf_f, wifi_data);

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSetIp2
void handleSetIp2()
{
  //rl='/set_ip2?ip='+ip2+'&ma='+ma2+'&gw='+gw2+'&d1='+d12+'&d2='+d22;

  strcpy(wifi_data.sta_ip2, server.arg("ip").c_str());
  strcpy(wifi_data.sta_ma2, server.arg("ma").c_str());
  strcpy(wifi_data.sta_gw2, server.arg("gw").c_str());
  strcpy(wifi_data.sta_dns12, server.arg("d1").c_str());
  strcpy(wifi_data.sta_dns22, server.arg("d2").c_str());
  conf_f = "/conf_wifi.json";
  wifi.saveConfig(conf_f, wifi_data);

  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handlejActT
void handlejActT()
{
  DynamicJsonDocument jsonBuffer(100);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["tstr"] = tstr;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handlejActA
void handlejActA()
{
  DynamicJsonDocument jsonBuffer(100);
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["acth"] = rtc_alm.hour;
  json["actm"] = rtc_alm.min;

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

void fs_setup()
{
#if defined(ESP8266)
  if (!LittleFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LittleFS.format();
  }
  else
  {
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUT_PORT.printf(" FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
  }
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  if (!LittleFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LittleFS.format();
  }
  else
  {
    File root = LittleFS.open("/");

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
      DBG_OUT_PORT.println(output);
    }
  }
# endif
}

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void hard_restart()
{
  esp_task_wdt_init(1, true);
  esp_task_wdt_add(NULL);
  while (true);
}
#endif

String formatBytes(uint32_t bytes)
{
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

# endif
