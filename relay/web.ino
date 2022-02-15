// ---------------------------------------------------------------------- setup
void web_setup()
{
  server.on("/q1_set",  handleSET_1);
  server.on("/q1_auto", handleAuto_1);
  server.on("/q2_set",  handleSET_2);
  server.on("/q2_auto", handleAuto_2);
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
  DynamicJsonDocument jsonBuffer(512);
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["apid"]   = wifi_data.ap_ssid;
  json["appas"]  = wifi_data.ap_pass;
  json["staid"]  = wifi_data.sta_ssid1;
  json["stapas"] = wifi_data.sta_pass1;

  json["pin1_name"] = conf_data.pin1;
  json["pin2_name"] = conf_data.pin2;

  json["on1_code"] = conf_data.str1_on;
  json["off1_code"] = conf_data.str1_off;
  json["on2_code"] = conf_data.str2_on;
  json["off2_code"] = conf_data.str2_off;

  json["lim_l"] = conf_data.lim_l;
  json["lim_h"] = conf_data.lim_h;

  json["out1_auto"] = pin1_a;
  json["out2_auto"] = pin2_a;

  json["out1_state"] = pin1_t;
  json["out2_state"] = pin2_t;

  json["ana_code"] = ft;

  String st = String();
  if (serializeJson(jsonBuffer, st) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));

  server.send(200, "text/json", st);
  st = String();
}

//-------------------------------------------------------------- handleSetWiFi
void handleSetWiFi()
{
  //url='/set_wifi?as='+as+'&ap='+ap+'&ss='+ss+'&sp='+sp+
  //'&p1='+p1+'&p2='+p2'+'&on1='+on1+'&of1='+of1+'&on2='+on2+'&of2='+of2+'&ll='+ll+'&lh='+lh;

  /*
    strcpy(wifi_data.ap_ssid, server.arg("as").c_str());
    strcpy(wifi_data.ap_pass, server.arg("ap").c_str());
    strcpy(wifi_data.sta_ssid1, server.arg("ss").c_str());
    strcpy(wifi_data.sta_pass1, server.arg("sp").c_str());

    uint8_t __pin = constrain(server.arg("p1").toInt(), 0, 255);
    conf_data.pin1 = selector(__pin);
    __pin = constrain(server.arg("p2").toInt(), 0, 255);
    conf_data.pin2 = selector(__pin);

    conf_data.str1_on  = constrain(server.arg("on1").toInt(), 0, 255);
    conf_data.str1_off = constrain(server.arg("of1").toInt(), 0, 255);

    conf_data.str2_on  = constrain(server.arg("on2").toInt(), 0, 255);
    conf_data.str2_off = constrain(server.arg("of2").toInt(), 0, 255);

    conf_data.lim_l = constrain(server.arg("ll").toInt(), 0, 255);
    conf_data.lim_h = constrain(server.arg("lh").toInt(), 0, 255);
  */
  char buf[255];
  memset (buf , 0 , 255);
  strcpy(buf, server.arg("in").c_str());

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  DynamicJsonDocument doc(1000);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, buf);
  if (error)
  {
    DBG_OUT_PORT.print(F("deserializeJson() failed: "));
    DBG_OUT_PORT.println(error.c_str());
  }
  else
  {
    DBG_OUT_PORT.println(F("Read sucsses!!!"));

    memset(wifi_data.ap_ssid,   0, 20);
    memset(wifi_data.ap_pass,   0, 20);

    memset(wifi_data.sta_ssid1, 0, 20);
    memset(wifi_data.sta_pass1, 0, 20);

    strcpy(wifi_data.ap_ssid,  doc["as"]);
    strcpy(wifi_data.ap_pass , doc["ap"]);

    strcpy(wifi_data.sta_ssid1, doc["ss"]);
    strcpy(wifi_data.sta_pass1, doc["sp"]);


    uint8_t __pin = doc["p1"];
    conf_data.pin1 = selector(__pin);
    __pin = doc["p2"];
    conf_data.pin2 = selector(__pin);

    conf_data.str1_on  = doc["on1"];
    conf_data.str1_off = doc["of1"];

    conf_data.str2_on  = doc["on2"];
    conf_data.str2_off = doc["of2"];

    conf_data.lim_l = doc["ll"];
    conf_data.lim_h = doc["lh"];
  }

  conf_f = "/conf_wifi.json";
  wifi.saveConfig(conf_f, wifi_data);

  conf_f = "/conf_main.json";
  saveConfig(conf_f, conf_data);
  server.send(200, "text/html", "OK!");
}

uint8_t selector (uint8_t _pin)
{
  const uint8_t gpio[6] = {0, 2, 12, 13, 14, 15};
  bool valid = false;
  for (uint8_t i = 0; i < 6; i++)
  {
    if (_pin == gpio[i]) valid = true;
  }
  if (!valid) _pin = 255;
  return _pin;
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
