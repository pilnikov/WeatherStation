// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/set_wifi", handleSetWiFi);
  server.on("/set_ip1",  handleSetIp1);
  server.on("/set_ip2",  handleSetIp2);
  server.on("/exit",     handleExit);
  server.on("/jwifi",    handlejWiFi);

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
  if (!conf_data.cli & !conf_data.ap) myIP = start_wifi(conf_data.sta_ssid1, conf_data.sta_pass1, conf_data.ap_ssid, conf_data.ap_pass);

  if (conf_data.cli || conf_data.ap)
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
  stop_wifi();
}


//-------------------------------------------------------------- handlejWiFi
void handlejWiFi()
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["apid"]    = conf_data.ap_ssid;
  json["appas"]   = conf_data.ap_pass;
  json["staid1"]  = conf_data.sta_ssid1;
  json["staid2"]  = conf_data.sta_ssid2;
  json["stapas1"] = conf_data.sta_pass1;
  json["stapas2"] = conf_data.sta_pass2;

  json["iap"]   = conf_data.ap_ip;
  json["map"]   = conf_data.ap_ma;

  json["sst1"]    = conf_data.st_ip1;
  json["sst2"]    = conf_data.st_ip2;

  if (conf_data.st_ip1)
  {
    json["ipst1"]   = conf_data.sta_ip1;
    json["mast1"]   = conf_data.sta_ma1;
    json["gwst1"]   = conf_data.sta_ma1;
    json["dns1st1"] = conf_data.sta_dns11;
    json["dns2st1"] = conf_data.sta_dns21;
  }
  if (conf_data.st_ip1)
  {
    json["ipst2"]   = conf_data.sta_ip2;
    json["mast2"]   = conf_data.sta_ma2;
    json["gwst2"]   = conf_data.sta_ma2;
    json["dns1st2"] = conf_data.sta_dns12;
    json["dns2st2"] = conf_data.sta_dns22;
  }
  json["wof"]    = conf_data.wifi_off;


  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  //url='/set_wifi?as='+as+'&ap='+ap+'&ss1='+ss1+'&sp1='+sp1+'&ss2='+ss2+'&sp2='+sp2+'&st1='+st1+'&st2='+st2+'&iap='+iap+'&map='+map+'&wof='+wof_t;

  strcpy(conf_data.ap_ssid, server.arg("as").c_str());
  strcpy(conf_data.ap_pass, server.arg("ap").c_str());
  strcpy(conf_data.sta_ssid1, server.arg("ss1").c_str());
  strcpy(conf_data.sta_pass1, server.arg("sp1").c_str());
  strcpy(conf_data.sta_ssid2, server.arg("ss2").c_str());
  strcpy(conf_data.sta_pass2, server.arg("sp2").c_str());

  conf_data.st_ip1 = server.arg("st1") == "1";
  conf_data.st_ip2 = server.arg("st2") == "1";

  strcpy(conf_data.ap_ip, server.arg("iap").c_str());
  strcpy(conf_data.ap_ma, server.arg("map").c_str());

  conf_data.wifi_off = server.arg("wof") == "1";

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSetIp1
void handleSetIp1()
{
  //url='/set_ip1?ip='+ip1+'&ma='+ma1+'&gw='+gw1+'&d1='+d11+'&d2='+d21;

  strcpy(conf_data.sta_ip1, server.arg("ip").c_str());
  strcpy(conf_data.sta_ma1, server.arg("ma").c_str());
  strcpy(conf_data.sta_gw1, server.arg("gw").c_str());
  strcpy(conf_data.sta_dns11, server.arg("d1").c_str());
  strcpy(conf_data.sta_dns21, server.arg("d2").c_str());

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSetIp2
void handleSetIp2()
{
  //rl='/set_ip2?ip='+ip2+'&ma='+ma2+'&gw='+gw2+'&d1='+d12+'&d2='+d22;

  strcpy(conf_data.sta_ip2, server.arg("ip").c_str());
  strcpy(conf_data.sta_ma2, server.arg("ma").c_str());
  strcpy(conf_data.sta_gw2, server.arg("gw").c_str());
  strcpy(conf_data.sta_dns12, server.arg("d1").c_str());
  strcpy(conf_data.sta_dns22, server.arg("d2").c_str());

  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
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


void printFile(const char* filename) {
  // Open file for reading
  File file = LittleFS.open(filename, "r");

  if (!file)
  {
    DBG_OUT_PORT.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) DBG_OUT_PORT.print((char)file.read());
  DBG_OUT_PORT.println();

  // Close the file
  file.close();
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
