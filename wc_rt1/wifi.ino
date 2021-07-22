# if defined(__xtensa__)

//-------------------------------------------------------------- Stop_wifi
void stop_wifi()
{
  if (web_cli)     //Останавливаем клиента
  {
    if (debug_level == 15) DBG_OUT_PORT.println(F( "Client stopped"));
  }
  if (web_ap)     //Останавливаем АР
  {
    if (debug_level == 15) DBG_OUT_PORT.println(F( "AP stopped"));
  }
  web_ap   = false;
  web_cli  = false;
  WiFi.mode(WIFI_OFF);
  WiFi.disconnect();
}

//-------------------------------------------------------------- Start_wifi
void start_wifi()
{
  DBG_OUT_PORT.print(F("Connecting to "));
  DBG_OUT_PORT.println(conf_data.sta_ssid);

  WiFi.persistent(false);

  WiFi.disconnect();

  WiFi.mode(WIFI_STA);

  WiFi.begin(conf_data.sta_ssid, conf_data.sta_pass);

  uint8_t tru = 0;

  while ((WiFi.status() != WL_CONNECTED) & (tru < 20))
  {
    delay(500);
    DBG_OUT_PORT.print(F("."));
    tru ++;
  }

  if (tru < 18)
  {
    DBG_OUT_PORT.println(F("\nWiFi connected"));
    DBG_OUT_PORT.print(F("IP address: "));
    DBG_OUT_PORT.println(WiFi.localIP());

    web_cli = true;

    st1 = "Your IP:" + WiFi.localIP().toString();
    if (conf_data.rus_lng)
    {
      st1 = "Ваш IP:" + WiFi.localIP().toString();
      f_dsp.utf8rus(st1);
    }
  }
  else
  {
    DBG_OUT_PORT.print(F("Configuring access point..."));
    WiFi.mode(WIFI_AP);

    WiFi.softAP(conf_data.ap_ssid, conf_data.ap_pass);

    IPAddress myIP = WiFi.softAPIP();
    DBG_OUT_PORT.print(F("AP IP address: "));
    DBG_OUT_PORT.println(myIP);
    web_ap = true;
  }
}
#endif
