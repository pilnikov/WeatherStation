// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/q1_set",  handleSET_1);
  server.on("/q1_auto", handleAuto_1);
  server.on("/q2_set",  handleSET_2);
  server.on("/q2_auto", handleAuto_2);
  server.on("/set_wifi", handleSetWiFi);
  server.on("/set_par", handleSetPar);
  server.on("/jwifi", handlejWiFi);
  server.on("/jpar", handlejPar);
  server.on("/jact", handlejAct);
  server.on("/rc_cmd", handleRCmd);
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

//-------------------------------------------------------------- handleSET_1
void handleSET_1()
{
  if (!pin1_t & !bumpless)
  {
    pin1_t = true;    // turn ON the pin1
    bumpless = true;
    DBG_OUT_PORT.println(conf_data.str1_on);
  }
  if (pin1_t & !bumpless)
  {
    pin1_t = false;    // turn OFF the pin1
    bumpless = true;
    setting_ms = millis();
    DBG_OUT_PORT.println(conf_data.str1_off);
  }
  pin1_a = false;  // turn OFF the AUTO MODE for pin1
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleAuto_1
void handleAuto_1()
{
  if (!pin1_a & !bumpless)
  {
    pin1_a = true;    // turn ON the AUTO MODE for pin1
    bumpless = true;
    setting_ms = millis();
  }
  if (pin1_a & !bumpless)
  {
    pin1_a = false;    // turn OFF the AUTO MODE for pin1
    bumpless = true;
    setting_ms = millis();
  }
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleSET_2
void handleSET_2()
{
  if (!pin2_t & !bumpless)
  {
    pin2_t = true;    // turn ON the pin2
    bumpless = true;
    setting_ms = millis();
    DBG_OUT_PORT.println(conf_data.str2_on);
  }
  if (pin2_t & !bumpless)
  {
    pin2_t = false;    // turn OFF the pin2
    bumpless = true;
    setting_ms = millis();
    DBG_OUT_PORT.println(conf_data.str2_off);
  }
  pin2_a = false;  // turn OFF the AUTO MODE for pin2
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleAuto_2
void handleAuto_2()
{
  if (!pin2_a & !bumpless)
  {
    pin2_a = true;    // turn ON the AUTO MODE for pin2
    bumpless = true;
    setting_ms = millis();
  }
  if (pin2_a & !bumpless)
  {
    pin2_a = false;    // turn OFF the AUTO MODE for pin2
    bumpless = true;
    setting_ms = millis();
  }
  server.send(200, "text/html", "OK!");
}
//-------------------------------------------------------------- handlejWiFi
void handlejWiFi()
{
  from_client = wifi_cfg.to_json(wifi_data);
  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handlejPar
void handlejPar()
{
  from_client = main_cfg.to_json(conf_data);
  server.send(200, "text/json", from_client);
}
//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  from_client = server.arg("in");
  from_client.replace("}{", ",");
  conf_f = "/conf_wifi.json";
  lfs.writeFile(conf_f, from_client.c_str());
  wifi_data = wifi_cfg.from_json(from_client);
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handleSetPar
void handleSetPar()
{
  from_client = server.arg("in");
  from_client.replace("}{", ",");
  conf_f = "/conf_main.json";
  lfs.writeFile(conf_f, from_client.c_str());
  conf_data = main_cfg.from_json(from_client);
  server.send(200, "text/html", "OK!");
}

//-------------------------------------------------------------- handlejAct
void handlejAct()
{
  from_client = String();
  DynamicJsonDocument jsonBuffer(200);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["out1_auto"] = pin1_a;
  json["out2_auto"] = pin2_a;

  json["out1_state"] = pin1_t;
  json["out2_state"] = pin2_t;

  json["ana_code"] = ft;

  if (serializeJson(jsonBuffer, from_client) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handler for receive data from remote controller
void handleRCmd()
{
  /*
      url='/send_data?
       + '&data1='  + data1_t
       + '&data2='  + data2_t
       + '&button1='  + btn1_t
       + '&button2='  + btn2_t
  */
  uint8_t data1, data2; bool button1, button2;
  data1 = 0;
  data2 = 0;
  button1 = false;
  button2 = false;

  if (server.arg("data1") != 0)
  {
    data1 = constrain(server.arg("data1").toInt(), 0, 255);
  }

  if (server.arg("data2") != 0)
  {
    data2 = constrain(server.arg("data2").toInt(), 0, 255);
  }

  button1 = server.arg("button1") == "1";
  button2 = server.arg("button2") == "1";

  server.send(200, "text/html", "OK!");

  analogWrite (4, data1);
  analogWrite (5, data2);
  digitalWrite (18, button1);
  digitalWrite (19, button2);
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
