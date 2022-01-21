#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

//-------------------------------------------------------------- Stop_wifi
void stop_wifi()
{
  if (web_cli)     //Останавливаем клиента
  {
    DBG_OUT_PORT.println(F( "True stop the Client"));
    web_cli  = false;
  }
  if (web_ap)     //Останавливаем АР
  {
    DBG_OUT_PORT.println(F( "True stop the AP"));
    web_ap   = false;
  }
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  DBG_OUT_PORT.println(F("WiFi stopped...."));
}

//-------------------------------------------------------------- Start_wifi
IPAddress start_wifi(const char* ssid, const char* ssipass, const char* apid, const char* appass)
{
  IPAddress my_IP;

  DBG_OUT_PORT.print(F("Trying to connect a "));
  DBG_OUT_PORT.println(ssid);

  if (WiFi.getAutoConnect() != true) WiFi.setAutoConnect(true);  //on power-on automatically connects to last used hwAP
  WiFi.setAutoReconnect(true);                                   //automatically reconnects to hwAP in case it is disconnected
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  // May be necessary after deepSleep. Otherwise you may get "error: pll_cal exceeds 2ms!!!" when trying to connect
  delay(1);

  // ---
  // Here you can do whatever you need to do that doesn't need a WiFi connection.
  // ---

#if defined(ESP8266)
  ESP.rtcUserMemoryRead(RTC_USER_DATA_SLOT_WIFI_STATE, reinterpret_cast<uint32_t *>(&state), sizeof(state));
#endif

  unsigned long start = millis();
  bool _resume = false;

#if defined(ESP8266)
  _resume = (!WiFi.resumeFromShutdown(state) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED));
#else
  _resume = (WiFi.waitForConnectResult(10000) != WL_CONNECTED);
#endif
  if (_resume)
  {
    DBG_OUT_PORT.println(F("Cannot resume WiFi connection, connecting via begin..."));
#if defined(ESP8266)
    if (WiFi.getPersistent() == true) WiFi.persistent(false);   //disable saving wifi config into SDK flash area
#else
    WiFi.persistent(false);
#endif

    if (!WiFi.mode(WIFI_STA) || !WiFi.begin(ssid, ssipass) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
    {
      DBG_OUT_PORT.print(F("Cannot connect to "));
      DBG_OUT_PORT.println(ssid);

      DBG_OUT_PORT.print(F("Trying to start access point "));
      DBG_OUT_PORT.println(apid);

      WiFi.mode(WIFI_AP);

      WiFi.softAP(apid, appass);

      my_IP = WiFi.softAPIP();

      web_ap = true;

      DBG_OUT_PORT.print(F("AP IP address: "));
      DBG_OUT_PORT.println(my_IP);
      return my_IP;
    }
  }

  DBG_OUT_PORT.print(ssid);
  DBG_OUT_PORT.println(F(" is connected!"));

  unsigned long duration = millis() - start;
  DBG_OUT_PORT.printf("Duration: %f\n", duration * 0.001);

  web_cli = true;

  my_IP = WiFi.localIP();

  DBG_OUT_PORT.print(F("Client IP address: "));
  DBG_OUT_PORT.println(my_IP);
  return my_IP;
}
#endif
