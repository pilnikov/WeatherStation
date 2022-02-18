#include "my_wifi.h"


wifi_cfg_data_t WFJS::from_json(String message)
{
  wifi_cfg_data_t _data;

    // Allocate the document on the stack.
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    DynamicJsonDocument doc(3000);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F(". Using default configuration!!!"));
      _data = def_conf();
      return _data;
	}
	else
    {
      DBG_OUT_PORT.println(F("Read msg sucsses!!!"));

      memset(_data.ap_ssid,   0, 20);
      memset(_data.ap_pass,   0, 20);

      memset(_data.sta_ssid1, 0, 20);
      memset(_data.sta_pass1, 0, 20);

      memset(_data.sta_ssid2, 0, 20);
      memset(_data.sta_pass2, 0, 20);

      memset(_data.ap_ip,     0, 17);
      memset(_data.ap_ma,     0, 17);

      memset(_data.sta_ip1,   0, 17);
      memset(_data.sta_ma1,   0, 17);
      memset(_data.sta_gw1,   0, 17);
      memset(_data.sta_dns11, 0, 17);
      memset(_data.sta_dns21, 0, 17);

      memset(_data.sta_ip2,   0, 17);
      memset(_data.sta_ma2,   0, 17);
      memset(_data.sta_gw2,   0, 17);
      memset(_data.sta_dns12, 0, 17);
      memset(_data.sta_dns22, 0, 17);

      strcpy(_data.ap_ssid,  doc["as"] | "N/A");
      strcpy(_data.ap_pass , doc["ap"] | "N/A");

      strcpy(_data.ap_ip, doc["iap"] | "N/A");
      strcpy(_data.ap_ma, doc["map"] | "N/A");

      strcpy(_data.sta_ssid1, doc["ss1"] | "N/A");
      strcpy(_data.sta_pass1, doc["sp1"] | "N/A");

      strcpy(_data.sta_ssid2, doc["ss2"] | "N/A");
      strcpy(_data.sta_pass2, doc["sp2"] | "N/A");


      _data.st_ip1            = doc["st1"];
      _data.st_ip2            = doc["st2"];

      _data.wifi_off          = doc["wof"];

      if (_data.st_ip1)
      {
        strcpy(_data.sta_ip1, doc["ip1"] | "N/A");
        strcpy(_data.sta_ma1, doc["ma1"] | "N/A");
        strcpy(_data.sta_gw1, doc["gw1"] | "N/A");
        strcpy(_data.sta_dns11, doc["d11"] | "N/A");
        strcpy(_data.sta_dns21, doc["d21"] | "N/A");
      }
      if (_data.st_ip2)
      {
        strcpy(_data.sta_ip2, doc["ip2"] | "N/A");
        strcpy(_data.sta_ma2, doc["ma2"] | "N/A");
        strcpy(_data.sta_gw2, doc["gw2"] | "N/A");
        strcpy(_data.sta_dns12, doc["d12"] | "N/A");
        strcpy(_data.sta_dns22, doc["d22"] | "N/A");
      }
	 return _data;
    }
}

String WFJS::to_json(wifi_cfg_data_t _data)
{
  if (debug_level == 3)
  {
    DBG_OUT_PORT.print(F("Start forming wifi_cfg_data to json string"));
  }

  if ( _data.ap_ssid[0] == ' ' || _data.ap_ssid[0] ==  0) strcpy( _data.ap_ssid, "Radio_Clock");

  DynamicJsonDocument doc(1000);
  JsonObject json = doc.to<JsonObject>();

  json["as"]  = _data.ap_ssid;
  json["ap"]  = _data.ap_pass;
  json["ss1"] = _data.sta_ssid1;
  json["ss2"] = _data.sta_ssid2;
  json["sp1"] = _data.sta_pass1;
  json["sp2"] = _data.sta_pass2;

  json["iap"] = _data.ap_ip;
  json["map"] = _data.ap_ma;

  json["st1"] = _data.st_ip1;
  json["st2"] = _data.st_ip2;

  json["wof"] = _data.wifi_off;

  if (_data.st_ip1)
  {
    json["ip1"] = _data.sta_ip1;
    json["ma1"] = _data.sta_ma1;
    json["gw1"] = _data.sta_gw1;
    json["d11"] = _data.sta_dns11;
    json["d21"] = _data.sta_dns21;
  }
  if (_data.st_ip2)
  {
    json["ip2"] = _data.sta_ip2;
    json["ma2"] = _data.sta_ma2;
    json["gw2"] = _data.sta_gw2;
    json["d12"] = _data.sta_dns12;
    json["d22"] = _data.sta_dns22;
  }
  String msg = String();
  if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
  return msg;
}

wifi_cfg_data_t WFJS::def_conf()
{
  wifi_cfg_data_t _data;

  // ---------------------------------------------------- WiFi Default
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital wifi_cfg_data"));

  memset(_data.ap_ssid,   0, 20);
  memset(_data.ap_pass,   0, 20);

  memset(_data.sta_ssid1, 0, 20);
  memset(_data.sta_pass1, 0, 20);

  memset(_data.sta_ssid2, 0, 20);
  memset(_data.sta_pass2, 0, 20);

  memset(_data.ap_ip,     0, 17);
  memset(_data.ap_ma,     0, 17);

  memset(_data.sta_ip1,   0, 17);
  memset(_data.sta_ma1,   0, 17);
  memset(_data.sta_gw1,   0, 17);
  memset(_data.sta_dns11, 0, 17);
  memset(_data.sta_dns21, 0, 17);

  memset(_data.sta_ip2,   0, 17);
  memset(_data.sta_ma2,   0, 17);
  memset(_data.sta_gw2,   0, 17);
  memset(_data.sta_dns12, 0, 17);
  memset(_data.sta_dns22, 0, 17);

  strcpy(_data.ap_ssid,   "MyWiFi_AP");
  strcpy(_data.ap_pass ,  "12345678");

  strcpy(_data.sta_ssid1,  "MyWiFi_1");
  strcpy(_data.sta_pass1,  "12345678");

  strcpy(_data.sta_ssid2,  "MyWiFi_2");
  strcpy(_data.sta_pass2,  "12345678");

  strcpy(_data.ap_ip, "192.168.4.1");
  strcpy(_data.ap_ma, "255.255.255.0");

  strcpy(_data.sta_ip1, "192.168.1.2");
  strcpy(_data.sta_ma1, "255.255.255.0");
  strcpy(_data.sta_gw1, "192.168.1.1");
  strcpy(_data.sta_dns11, "8.8.8.8");
  strcpy(_data.sta_dns21, "8.8.4.4");

  strcpy(_data.sta_ip2, "192.168.0.2");
  strcpy(_data.sta_ma2, "255.255.255.0");
  strcpy(_data.sta_gw2, "192.168.0.1");
  strcpy(_data.sta_dns12, "8.8.8.8");
  strcpy(_data.sta_dns22, "8.8.4.4");

  _data.st_ip1 = false;
  _data.st_ip2 = false;

  _data.wifi_off = false;

  return _data;
}
