#include "my_wifi.h"

#if defined(ESP8266)
#include <include/WiFiState.h> // WiFiState structure details
WiFiState state;
#ifndef RTC_USER_DATA_SLOT_WIFI_STATE
#define RTC_USER_DATA_SLOT_WIFI_STATE 33u
#endif
#endif

//-------------------------------------------------------------- Stop_wifi
void WF::end(wifi_cur_data_t _data)
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
wifi_cur_data_t WF::begin(wifi_cfg_data_t _data)
{
  wifi_cur_data_t c_data;
  
  memset(c_data.ssid,   0, 20);
  strcpy(c_data.ssid, _data.sta_ssid1);

  DBG_OUT_PORT.print(F("Trying to connect a..."));
  DBG_OUT_PORT.println(c_data.ssid);

  if (WiFi.getAutoReconnect() != true) WiFi.setAutoReconnect(true);  //on power-on automatically connects to last used hwAP
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
    IPAddress local_IP,
              gateway,
              subnet,
              primaryDNS,   //optional
              secondaryDNS; //optional
			  
	local_IP.fromString(_data.sta_ip1);
    gateway.fromString(_data.sta_gw1);
    subnet.fromString(_data.sta_ma1);
    primaryDNS.fromString(_data.sta_dns11);   //optional
    secondaryDNS.fromString(_data.sta_dns21); //optional

    if (_data.st_ip1)
    {
      // Configures static IP address
      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))   DBG_OUT_PORT.println("STA1 Failed to configure");
    }

    if (!WiFi.mode(WIFI_STA) || !WiFi.begin(c_data.ssid, _data.sta_pass1) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
    {
      DBG_OUT_PORT.print(F("Cannot connect to..."));
      DBG_OUT_PORT.println(c_data.ssid);

	  WiFi.disconnect(true);
	  WiFi.mode(WIFI_OFF);
	  
	  memset(c_data.ssid,   0, 20);
	  strcpy(c_data.ssid, _data.sta_ssid2);

      DBG_OUT_PORT.print(F("Trying to connect to next access point..."));
      DBG_OUT_PORT.println(c_data.ssid);

 
	  local_IP.fromString(_data.sta_ip2);
      gateway.fromString(_data.sta_gw2);
      subnet.fromString(_data.sta_ma2);
      primaryDNS.fromString(_data.sta_dns12);   //optional
      secondaryDNS.fromString(_data.sta_dns22); //optional

      if (_data.st_ip2)
      {
        // Configures static IP address
        if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))   DBG_OUT_PORT.println("STA2 Failed to configure");
      }

      if (!WiFi.mode(WIFI_STA) || !WiFi.begin(c_data.ssid, _data.sta_pass2) || (WiFi.waitForConnectResult(10000) != WL_CONNECTED))
      {
        DBG_OUT_PORT.print(F("Cannot connect to..."));
        DBG_OUT_PORT.println(c_data.ssid);

		memset(c_data.ssid,   0, 20);
		strcpy(c_data.ssid, _data.ap_ssid);

        DBG_OUT_PORT.print(F("Trying to start access point..."));
        DBG_OUT_PORT.println(c_data.ssid);

 	    local_IP.fromString(_data.ap_ip);
        gateway.fromString(_data.ap_ip);
        subnet.fromString(_data.ap_ma);
        
        WiFi.mode(WIFI_AP);

        if (!WiFi.softAPConfig(local_IP, gateway, subnet))  DBG_OUT_PORT.println("AP Config Failed");

        WiFi.softAP(c_data.ssid, _data.ap_pass);

        c_data.addr = WiFi.softAPIP();

        c_data.ap = true;

        DBG_OUT_PORT.print(F("AP IP address: "));
        DBG_OUT_PORT.println(c_data.addr);
        return c_data;
      }
    }
  }
  DBG_OUT_PORT.print(c_data.ssid);
  DBG_OUT_PORT.println(F(" is connected!"));

  unsigned long duration = millis() - start;
  DBG_OUT_PORT.printf("Duration: %f\n", duration * 0.001);

  c_data.cli = true;

  c_data.addr = WiFi.localIP();

  DBG_OUT_PORT.print(F("Client IP address: "));
  DBG_OUT_PORT.println(c_data.addr);
  return c_data;
}

void WF::_shutdown()
{
#if defined(ESP8266)
	WiFi.shutdown(state);
    ESP.rtcUserMemoryWrite(RTC_USER_DATA_SLOT_WIFI_STATE, reinterpret_cast<uint32_t *>(&state), sizeof(state));
#endif
}