#include "my_wifi.h"

//-------------------------------------------------------------- Stop_wifi
void WF::end(wifi_data_t _data)
{
  if (_data.cli)     //Останавливаем клиента
  {
    DBG_OUT_PORT.println(F( "True stop the Client"));
    _data.cli  = false;
  }
  if (_data.ap)     //Останавливаем АР
  {
    DBG_OUT_PORT.println(F( "True stop the AP"));
    _data.ap   = false;
  }
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  DBG_OUT_PORT.println(F("WiFi stopped...."));
}

//-------------------------------------------------------------- Start_wifi
IPAddress WF::begin(wifi_data_t _data)
{
  IPAddress my_IP;
  char * ssid = _data.sta_ssid1;

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
  ESP.rtcUserMemoryRead(RTC_USER_DATA_SLOT_WIFI_STATE, reinterpret_cast<uint32_t *>(&_state), sizeof(_state));
#endif

  unsigned long start = millis();
  bool _resume = false;

#if defined(ESP8266)
  _resume = (!WiFi.resumeFromShutdown(_state) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED));
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
    IPAddress _ip = str_to_ip(_data.sta_ip1),
              _gw = str_to_ip(_data.sta_gw1),
              _ma = str_to_ip(_data.sta_ma1),
              _d1 = str_to_ip(_data.sta_dns11),
              _d2 = str_to_ip(_data.sta_dns21);

    if (_data.st_ip1)
    {
      DBG_OUT_PORT.println(F("Trying to configure STA1"));
      WiFi.hostname(_data.ap_ssid);
      if (!WiFi.config(_ip, // адрес
                       _gw, // шлюз
                       _ma, // маска
                       _d1, // DNS1
                       _d2))// DNS2
      {
        DBG_OUT_PORT.println(F("STA1 Failed to configure"));
      }
    }
    if (!WiFi.mode(WIFI_STA) || !WiFi.begin(_data.sta_ssid1, _data.sta_pass1) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
    {
      DBG_OUT_PORT.print(F("Cannot connect to..."));
      DBG_OUT_PORT.println(ssid);

      DBG_OUT_PORT.print(F("Trying to connect to next AP..."));
      DBG_OUT_PORT.println(_data.sta_ssid2);

      if (_data.st_ip2)
      {
        _ip =  str_to_ip(_data.sta_ip2);
        _gw = str_to_ip(_data.sta_gw2);
        _ma = str_to_ip(_data.sta_ma2);
        _d1 = str_to_ip(_data.sta_dns12);
        _d2 = str_to_ip(_data.sta_dns22);

        DBG_OUT_PORT.println(F("Trying to configure STA2"));
        WiFi.hostname(_data.ap_ssid);
        if (!WiFi.config(_ip, // адрес
                         _gw, // шлюз
                         _ma, // маска
                         _d1, // DNS1
                         _d2))// DNS2
        {
          DBG_OUT_PORT.println(F("STA2 Failed to configure"));
        }
      }
    }
    if (!WiFi.mode(WIFI_STA) || !WiFi.begin(_data.sta_ssid2, _data.sta_pass2) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
    {
      ssid = _data.sta_ssid2;
      DBG_OUT_PORT.print(F("Cannot connect to..."));
      DBG_OUT_PORT.println(ssid);

      DBG_OUT_PORT.print(F("Trying to start access point..."));
      DBG_OUT_PORT.println(_data.ap_ssid);

      WiFi.mode(WIFI_AP);

      WiFi.softAPConfig(str_to_ip(_data.ap_ip), str_to_ip(_data.ap_ip), str_to_ip(_data.ap_ma));
      WiFi.softAP(_data.ap_ssid, _data.ap_pass);

      my_IP = WiFi.softAPIP();

      _data.ap = true;

      DBG_OUT_PORT.print(F("AP IP address: "));
      DBG_OUT_PORT.println(my_IP);
      return my_IP;
    }
  }

  DBG_OUT_PORT.print(ssid);
  DBG_OUT_PORT.println(F(" is connected!"));

  unsigned long duration = millis() - start;
  DBG_OUT_PORT.printf("Duration: %f\n", duration * 0.001);

  _data.cli = true;

  my_IP = WiFi.localIP();

  DBG_OUT_PORT.print(F("Client IP address: "));
  DBG_OUT_PORT.println(my_IP);
  return my_IP;
}

IPAddress WF::str_to_ip(char* strIP)
{
  uint16_t k = strlen(strIP);// source length
  int Parts[4] = {0, 0, 0, 0};
  int Part = 0;

  for ( int i = 0; i < k; i++ )
  {
    char c = strIP[i];
    if ( c == '.' )
    {
      Part++;
      continue;
    }
    Parts[Part] *= 10;
    Parts[Part] += c - '0';
  }
  IPAddress ip( Parts[0], Parts[1], Parts[2], Parts[3] );
  return ip;
}
