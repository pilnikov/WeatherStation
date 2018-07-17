#if defined(ESP8266) || defined(ESP32)
//-------------------------------------------------------------- sta_init
bool sta_init()
{
  if (debug_level == 15) DBG_OUT_PORT.println( "Client initialized");

  bool result = WiFi.getMode() == WIFI_STA;;
  if (!result)
  {
    wifi_conn(1, 0, ram_data.type_disp); //сообщение на индикатор
    WiFi.disconnect();
    WiFi.mode( WIFI_STA );
    while ( WiFi.getMode() != WIFI_STA );
    WiFi.begin( conf_data.sta_ssid, (conf_data.sta_pass[0] == 0) ? NULL : conf_data.sta_pass);
    result = WiFi.getMode() == WIFI_STA;
  }
  return result;
}

bool sta_check()
//-------------------------------------------------------------- Установка соединения с точкой доступа WIFi
{
  if (debug_level == 15)  DBG_OUT_PORT.println( "Connect established");

  bool result = WiFi.status() == WL_CONNECTED;
  if (!result)
  {
    uint8_t j = 0;

    while (j < 6 && !result)
    {
      result = WiFi.status() == 3;
      wifi_conn(3, WiFi.status(), ram_data.type_disp); //сообщение на индикатор
      if (!result)
      {
        delay(2000);
        j++;
      }
    }

    if (result)
    {
      IP_Addr = WiFi.localIP();
      wifi_conn(4, 0, ram_data.type_disp); //выдать радостную новость на индикатор
      WiFi.setAutoConnect(true);
      WiFi.setAutoReconnect(true);
      serv_ms = millis();
    }
    else wifi_conn(6, 0, ram_data.type_disp); //выдать нерадостную новость на индикатор
  }
  return result;
}

//-------------------------------------------------------------- ap_init
bool ap_init()
{
  if (debug_level == 15) DBG_OUT_PORT.println( "AP mode started");

  bool result = WiFi.getMode() == WIFI_AP;
  if (!result)
  {
    wifi_conn(2, 0, ram_data.type_disp); //сообщение на индикатор

    WiFi.disconnect();
    WiFi.mode( WIFI_AP );
    while ( WiFi.getMode() != WIFI_AP );

    WiFi.softAP(conf_data.ap_ssid, (conf_data.ap_pass[0] == 0) ? NULL : conf_data.ap_pass);

    if  (WiFi.getMode() == WIFI_AP)
    {
      IP_Addr = WiFi.softAPIP();
      wifi_conn(5, 0, ram_data.type_disp); //выдать радостную новость на индикатор
      serv_ms = millis();
    }
    else wifi_conn(6, 0, ram_data.type_disp); //выдать нерадостную новость на индикатор

    result = WiFi.getMode() == WIFI_AP;
    if (result && web_cli)  web_cli = false;
  }
  return result;
}
# endif

//------------------------------------------------------------ Start_client
bool start_client()
{
  if (debug_level == 15) DBG_OUT_PORT.printf( "\r\nTrue connection as client\n" );

  bool result = sta_init() && sta_check();
  if (result && web_ap)  web_ap = false;

  return result;
}

//-------------------------------------------------------------- Stop_wifi
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
  if (conf_data.rus_disp)  st1 = fsys.utf8rus("Перезагрузитесь и подключитесь к " + String(conf_data.ap_ssid) + " с паролем " + String(conf_data.ap_pass) + " для настройки подключения к вашей WiFi");
}

//-------------------------------------------------------------- Start_wifi
void start_wifi()
{
  /*#if defined(ESP32)
    if (debug_level == 15)  DBG_OUT_PORT.println( "Start wifi");
      if (!web_ap && !web_cli) web_cli = start_client();  // Запускаем клиента и пытаемся подключиться к АР
      if (!web_ap && !web_cli) web_ap  = ap_init();       // Не удалось подключиться клиентом - создаем АР
    #endif
  */
  WiFiManager wm;
  //wm.resetSettings();
  //  wm.setConfigPortalBlocking(false);
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
  }
  else web_ap = true;
}

# if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
//-------------------------------------------------------------- sta_init
bool sta_init()
{
  int status = WL_IDLE_STATUS;     // the Wifi radio's status


  wifi_conn(1, 0, ram_data.type_disp); //сообщение на индикатор
  WiFi.disconnect();
  status = WiFi.begin( conf_data.sta_ssid, (conf_data.sta_pass[0] == 0) ? NULL : conf_data.sta_pass);
  return status > 0;
}

bool sta_check()
{
  bool cCon = false;
  byte j = 0;

  while ( j < 6 && !cCon )
  {
    cCon = WiFi.status() == 3;
    delay(2000);
    wifi_conn(3, WiFi.status(), ram_data.type_disp); //сообщение на индикатор
    j++;
  }

  if ( cCon )
  {
    IP_Addr = WiFi.localIP();
    wifi_conn(4, 0, ram_data.type_disp); //выдать радостную новость на индикатор
  }
  else wifi_conn(6, 0, ram_data.type_disp); //выдать нерадостную новость на индикатор

  return cCon;
}

//-------------------------------------------------------------- ap_init
bool ap_init()
{
  int status = WL_IDLE_STATUS;     // the Wifi radio's status

  wifi_conn(2, 0, ram_data.type_disp); //сообщение на индикатор

  WiFi.disconnect();

  status = WiFi.apbegin(conf_data.ap_ssid, (conf_data.ap_pass[0] == 0) ? NULL : conf_data.ap_pass);

  if  (status > 0)
  {
    IP_Addr = WiFi.localIP();
    wifi_conn(5, 0, ram_data.type_disp); //выдать радостную новость на индикатор
  }
  else wifi_conn(6, 0, ram_data.type_disp); //выдать нерадостную новость на индикатор

  return status > 0;
}

# endif
