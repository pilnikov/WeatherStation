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
  server.on("/end_set_wifi",  handleEndSetWiFi);
  server.on("/exit",      handleExit);
  server.on("/jactt",     handlejActT);
  server.on("/jacta",     handlejActA);
  server.on("/jtime1",    handlejTime1);
  server.on("/jtime2",    handlejTime2);
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
  from_client = myrtccfg.to_json(rtc_cfg);
  server.send(200, "text/json", from_client);
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
}

//-------------------------------------------------------------- handleSetTime2
void handleSetTime2()
{
  from_client = server.arg("in");
  server.send(200, "text/html", "OK!");

  rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);
}

//-------------------------------------------------------------- handleSetAlarm
void handleSetAlarm()
{
  from_client += server.arg("in");
  from_client.replace("}{", ",");

  conf_f = "/conf_rtc.json";
  lfs.writeFile(conf_f, from_client.c_str());
  rtc_cfg = myrtccfg.from_json(from_client);
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleNTP
void handleNTP()
{
  RtcDateTime c_time;
  if (wifi_data_cur.cli)
  {
    c_time = myrtc.GetNtp(rtc_cfg, rtc_time);
    rtc_time.ct = myrtc.man_set_time(rtc_hw, c_time);
    rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);
  }
  server.send(200, "text/html", "OK!");
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
}

void handleFileList()
{
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
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

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void hard_restart()
{
  esp_task_wdt_init(1, true);
  esp_task_wdt_add(NULL);
  while (true);
}
#endif

# endif
