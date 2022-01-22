// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/set_q1", handleSET_1);
  server.on("/reset_q1", handleRESET_1);
  server.on("/set_q2", handleSET_2);
  server.on("/reset_q2", handleRESET_2);
  server.on("/set_wifi", handleSetWiFi);
  server.on("/jwifi", handlejWiFi);
  server.on("/exit", handleExit);

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
  if (!web_cli & !web_ap) myIP = start_wifi(conf_data.sta_ssid, conf_data.sta_pass, conf_data.ap_ssid, conf_data.ap_pass);

  if (web_cli || web_ap)
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

//-------------------------------------------------------------- handleSET_1
void handleSET_1()
{
  pin1_t = true;    // turn the RELAY on by making the voltage HIGH
  pin1_on_force = true;
  DBG_OUT_PORT.println(conf_data.str1_on);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleRESET_1
void handleRESET_1()
{
  pin1_t = false;    // turn the RELAY off by making the voltage LOW
  pin1_on_force = false;
  DBG_OUT_PORT.println(conf_data.str1_off);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleSET_2
void handleSET_2()
{
  pin2_t = true ;    // turn the RELAY on by making the voltage HIGH
  pin2_on_force = true;
  DBG_OUT_PORT.println(conf_data.str2_on);
  server.send(200, "text/html", "OK!");
  serv_ms = millis();
}

//-------------------------------------------------------------- handleRESET_2
void handleRESET_2()
{
  pin2_t = false;    // turn the RELAY off by making the voltage LOW
  pin2_on_force = false;
  DBG_OUT_PORT.println(conf_data.str2_off);
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

  json["pin1_name"] = conf_data.pin1;
  json["pin2_name"] = conf_data.pin2;

  json["pin1_state"] = pin1_t;
  json["pin2_state"] = pin2_t;

  json["on1_code"] = conf_data.str1_on;
  json["off1_code"] = conf_data.str1_off;
  json["on2_code"] = conf_data.str2_on;
  json["off2_code"] = conf_data.str2_off;

  json["lim_l"] = conf_data.lim_l;
  json["lim_h"] = conf_data.lim_h;

  json["ana_code"] = ft;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  //url='/set_wifi?as='+as+'&ap='+ap+'&ss='+ss+'&sp='+sp+'&p1='+p1
  //    +'&p2='+p2+'&on1='+on1+'&of1='+of1+'&on2='+on2+'&of2='+of2+'&ll='+ll+'&lh='+lh;

  strcpy(conf_data.ap_ssid, server.arg("as").c_str());
  strcpy(conf_data.ap_pass, server.arg("ap").c_str());
  strcpy(conf_data.sta_ssid, server.arg("ss").c_str());
  strcpy(conf_data.sta_pass, server.arg("sp").c_str());

  conf_data.pin1 = constrain(server.arg("p1").toInt(), 0, 255);
  conf_data.pin2 = constrain(server.arg("p2").toInt(), 0, 255);

  conf_data.str1_on  = constrain(server.arg("on1").toInt(), 0, 255);
  conf_data.str1_off = constrain(server.arg("of1").toInt(), 0, 255);

  conf_data.str2_on  = constrain(server.arg("on2").toInt(), 0, 255);
  conf_data.str2_off = constrain(server.arg("of2").toInt(), 0, 255);

  conf_data.lim_l = constrain(server.arg("ll").toInt(), 0, 255);
  conf_data.lim_h = constrain(server.arg("lh").toInt(), 0, 255);

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
