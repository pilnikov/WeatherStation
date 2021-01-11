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

void configModeCallback (WiFiManager *myWiFiManager)
{
  st1 = "Please reboot and connect to " + String(conf_data.ap_ssid) + " with pass " + String(conf_data.ap_pass) + " for configure your WiFi connection! ";
  if (conf_data.rus_disp)  st1 = "Перезагрузитесь и подключитесь к " + String(conf_data.ap_ssid) + " с паролем " + String(conf_data.ap_pass) + " для настройки подключения к вашей WiFi";
}

//-------------------------------------------------------------- Start_wifi
void start_wifi()
{
  WiFiManager wm;
//  wm.resetSettings();
  wm.setConfigPortalBlocking(false);
  wm.setAPCallback(configModeCallback);
  wm.setConfigPortalTimeout(30);
  wm.autoConnect(conf_data.ap_ssid, conf_data.ap_pass);

  /* if  (!wm.autoConnect(conf_data.ap_ssid, conf_data.ap_pass))
    {
     DBG_OUT_PORT.println("failed to connect and hit timeout");
     //reset and try again, or maybe put it to deep sleep
     ESP.restart();
     delay(1000);
    }
  */
  if (WiFi.status() == 3)
  {
    web_cli = true;
    st1 = "Your IP: " + WiFi.localIP().toString() + " ";
    if (conf_data.rus_disp) st1 = "Ваш IP: " + WiFi.localIP().toString() + " ";
  }
  else web_ap = true;
}
