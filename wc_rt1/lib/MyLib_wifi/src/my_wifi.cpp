#include "my_wifi.h"

#if defined(ESP8266)
#include <include/WiFiState.h> // WiFiState structure details
WiFiState state;
#ifndef RTC_USER_DATA_SLOT_WIFI_STATE
#define RTC_USER_DATA_SLOT_WIFI_STATE 33u
#endif
#endif

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
  DBG_OUT_PORT.println(F("WiFi stopped..."));
}

//-------------------------------------------------------------- Start_wifi
wifi_data_t WF::begin(wifi_data_t _data)
{
  //const char* ssid, const char* ssipass, const char* apid, const char* appass
  char *ssid = _data.sta_ssid1;

  DBG_OUT_PORT.print(F("Trying to connect a..."));
  DBG_OUT_PORT.println(_data.sta_ssid1);

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
    IPAddress local_IP     = str_to_ip(_data.sta_ip1),
              gateway      = str_to_ip(_data.sta_gw1),
              subnet       = str_to_ip(_data.sta_ma1),
              primaryDNS   = str_to_ip(_data.sta_dns11),   //optional
              secondaryDNS = str_to_ip(_data.sta_dns21); //optional

    if (_data.st_ip1)
    {
      // Configures static IP address
      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))   DBG_OUT_PORT.println("STA1 Failed to configure");
    }

    if (!WiFi.mode(WIFI_STA) || !WiFi.begin(_data.sta_ssid1, _data.sta_pass1) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
    {
      DBG_OUT_PORT.print(F("Cannot connect to..."));
      DBG_OUT_PORT.println(_data.sta_ssid1);

      DBG_OUT_PORT.print(F("Trying to connect to next access point..."));
      DBG_OUT_PORT.println(_data.sta_ssid2);

      ssid = _data.sta_ssid2;

      local_IP     = str_to_ip(_data.sta_ip2);
      gateway      = str_to_ip(_data.sta_gw2);
      subnet       = str_to_ip(_data.sta_ma2);
      primaryDNS   = str_to_ip(_data.sta_dns12);   //optional
      secondaryDNS = str_to_ip(_data.sta_dns22); //optional

      if (_data.st_ip2)
      {
        // Configures static IP address
        if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))   DBG_OUT_PORT.println("STA2 Failed to configure");
      }

      if (!WiFi.mode(WIFI_STA) || !WiFi.begin(_data.sta_ssid2, _data.sta_pass2) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
      {
        DBG_OUT_PORT.print(F("Cannot connect to..."));
        DBG_OUT_PORT.println(_data.sta_ssid2);

        DBG_OUT_PORT.print(F("Trying to start access point..."));
        DBG_OUT_PORT.println(_data.ap_ssid);

        ssid = _data.ap_ssid;
        local_IP     = str_to_ip(_data.ap_ip);
        gateway      = str_to_ip(_data.ap_ip);
        subnet       = str_to_ip(_data.ap_ma);

        WiFi.mode(WIFI_AP);

        if (!WiFi.softAPConfig(local_IP, gateway, subnet))  DBG_OUT_PORT.println("AP Config Failed");

        WiFi.softAP(_data.ap_ssid, _data.ap_pass);

        _data.cur_addr = WiFi.softAPIP();

        _data.ap = true;

        DBG_OUT_PORT.print(F("AP IP address: "));
        DBG_OUT_PORT.println(_data.cur_addr);
        return _data;
      }
    }
  }
  DBG_OUT_PORT.print(ssid);
  DBG_OUT_PORT.println(F(" is connected!"));

  unsigned long duration = millis() - start;
  DBG_OUT_PORT.printf("Duration: %f\n", duration * 0.001);

  _data.cli = true;

  _data.cur_addr = WiFi.localIP();

  DBG_OUT_PORT.print(F("Client IP address: "));
  DBG_OUT_PORT.println(_data.cur_addr);
  return _data;
}

IPAddress WF::str_to_ip(char *strIP)
{
  uint8_t Part = 0, Parts[4] = {0, 0, 0, 0}, k = strlen(strIP);
  for ( uint8_t i = 0; i < k; i++ )
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
  IPAddress ip( Parts[0], Parts[1], Parts[2], Parts[3]);
  return ip;
}
