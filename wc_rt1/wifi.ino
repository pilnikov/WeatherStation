#//-------------------------------------------------------------- Stop_wifi
void stop_wifi()
{
  if (web_cli)     //Останавливаем клиента
  {
    if (debug_level == 15) DBG_OUT_PORT.println( "Client stopped");
  }
  if (web_ap)     //Останавливаем АР
  {
    if (debug_level == 15) DBG_OUT_PORT.println( "AP stopped");
  }
  web_ap   = false;
  web_cli  = false;
# if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_OFF);
# endif
  WiFi.disconnect();
}

/*void configModeCallback (WiFiManager *myWiFiManager)
  {
  st1 = "Please reboot and connect to " + String(conf_data.ap_ssid) + " with pass " + String(conf_data.ap_pass) + " for configure your WiFi connection! ";
  if (conf_data.rus_lng)  st1 = "Перезагрузитесь и подключитесь к " + String(conf_data.ap_ssid) + " с паролем " + String(conf_data.ap_pass) + " для настройки подключения к вашей WiFi";
  }
*/
//-------------------------------------------------------------- Start_wifi
void start_wifi()
{
  /* WiFiManager wm;
    //wm.resetSettings();
    wm.setConfigPortalBlocking(false);
    wm.setAPCallback(configModeCallback);
    wm.setConfigPortalTimeout(30);
    //  wm.autoConnect(conf_data.ap_ssid, conf_data.ap_pass);
    //  wm.autoConnect(conf_data.ap_ssid, conf_data.ap_pass);

    if  (!wm.autoConnect(conf_data.ap_ssid, conf_data.ap_pass))
     {
      DBG_OUT_PORT.println("failed to connect and hit timeout");
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(1000);
     }
  */

  DBG_OUT_PORT.print("Connecting to ");
  DBG_OUT_PORT.println(conf_data.sta_ssid);

  WiFi.persistent(false);

  WiFi.disconnect();

  WiFi.begin(conf_data.sta_ssid, conf_data.sta_pass);

  uint8_t tru = 0;

  while (WiFi.status() > 3 && tru < 10)
  {
    delay(500);
    DBG_OUT_PORT.print(".");
    tru ++;
  }

  if (tru < 10)
  {
    DBG_OUT_PORT.println("\nWiFi connected");
    DBG_OUT_PORT.println("IP address: ");
    DBG_OUT_PORT.println(WiFi.localIP());

    web_cli = true;
    st1 = "Your IP: " + WiFi.localIP().toString() + " ";
    if (conf_data.rus_lng) st1 = "Ваш IP: " + WiFi.localIP().toString() + " ";

    f_dsp.utf8rus(st1);
    strcpy(tstr1, st1.c_str());
  }
  else
  {
    DBG_OUT_PORT.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(conf_data.ap_ssid, conf_data.ap_pass);

    IPAddress myIP = WiFi.softAPIP();
    DBG_OUT_PORT.print("AP IP address: ");
    DBG_OUT_PORT.println(myIP);
    web_ap = true;
  }
}
