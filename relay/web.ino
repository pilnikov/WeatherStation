// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/ch1_set",  handleSET_1);
  server.on("/ch1_auto", handleAuto_1);
  server.on("/ch2_set",  handleSET_2);
  server.on("/ch2_auto", handleAuto_2);

  server.on("/ch1_val",  handleVAL_1);
  server.on("/ch2_val",  handleVAL_2);

  server.on("/set_wifi", handleSetWiFi);
  server.on("/jwifi", handlejWiFi);

  server.on("/set_par", handleSetPar);
  server.on("/jpar", handlejPar);

  server.on("/jact", handlejAct);

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
  wifi.end(wifi_data_cur);
}

//-------------------------------------------------------------- handleSET_1
void handleSET_1()
{
  if (!bumpless)
  {
    if (ch1_set)
    {
      ch1_set = false;    // turn OFF the ch1
      ssta = "ch1_off";
      DBG_OUT_PORT.println(conf_data.ch1_off_code);
    }
    else
    {
      ch1_set = true;    // turn ON the ch1
      ssta = "ch1_on";
      DBG_OUT_PORT.println(conf_data.ch1_on_code);
    }
    ch1_auto = false;  // turn OFF the AUTO MODE for ch1

    server.send(200, "text/html", ssta);
    bumpless = true;
    setting_ms = millis();
  }
}

//-------------------------------------------------------------- handleAuto_1
void handleAuto_1()
{
  if (!bumpless)
  {
    if (ch1_auto)
    {
      ch1_auto = false;    // turn OFF the AUTO MODE for ch1
      ssta = "ch1_manual";
    }
    else
    {
      ch1_auto = true;    // turn ON the AUTO MODE for ch1
      ssta = "ch1_auto";
    }
    server.send(200, "text/html", ssta);
    bumpless = true;
    setting_ms = millis();
  }
}

//-------------------------------------------------------------- handleSET_2
void handleSET_2()
{
  if (!bumpless)
  {
    if (ch2_set)
    {
      ch2_set = false;    // turn OFF the ch2
      ssta = "ch2_off";
      DBG_OUT_PORT.println(conf_data.ch2_off_code);
    }
    else
    {
      ch2_set = true;    // turn ON the ch2
      ssta = "ch2_on";
      DBG_OUT_PORT.println(conf_data.ch2_on_code);
    }
    ch2_auto = false;  // turn OFF the AUTO MODE for ch2

    server.send(200, "text/html", ssta);
    bumpless = true;
    setting_ms = millis();
  }
}

//-------------------------------------------------------------- handleAuto_2
void handleAuto_2()
{
  if (!bumpless)
  {
    if (ch2_auto)
    {
      ch2_auto = false;    // turn OFF the AUTO MODE for ch2
      ssta = "ch2_manual";
    }
    else
    {
      ch2_auto = true;    // turn ON the AUTO MODE for ch2
      ssta = "ch2_auto";
    }

    server.send(200, "text/html", ssta);
    bumpless = true;
    setting_ms = millis();
  }
}

//-------------------------------------------------------------- handleVAL_1
void handleVAL_1()
{
  ch1_val = server.arg("val").toInt();
  ssta = String(ch1_val);
  server.send(200, "text/html", ssta);
}

//-------------------------------------------------------------- handleVAL_2
void handleVAL_2()
{
  ch2_val = server.arg("val").toInt();
  ssta = String(ch2_val);
  server.send(200, "text/html", ssta);
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

//-------------------------------------------------------------- handlejWiFi
void handlejWiFi()
{
  from_client = wifi_cfg.to_json(wifi_data);
  server.send(200, "text/json", from_client);
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

  conf_data.ch1_in ? pinMode(conf_data.ch1_gpio, INPUT) : pinMode(conf_data.ch1_gpio, OUTPUT);
  conf_data.ch2_in ? pinMode(conf_data.ch2_gpio, INPUT) : pinMode(conf_data.ch2_gpio, OUTPUT);
}

//-------------------------------------------------------------- handlejPar
void handlejPar()
{
  from_client = main_cfg.to_json(conf_data);
  server.send(200, "text/json", from_client);
}

//-------------------------------------------------------------- handlejAct
void handlejAct()
{
  from_client = String();
  DynamicJsonDocument jsonBuffer(200);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["ch1_auto"] = ch1_auto;
  json["ch2_auto"] = ch2_auto;

  json["ch1_state"] = ch1_set;
  json["ch2_state"] = ch2_set;

  json["ch1_value"] = ch1_val;
  json["ch2_value"] = ch2_val;

  json["ana_code"] = ft;

  if (serializeJson(jsonBuffer, from_client) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", from_client);
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
