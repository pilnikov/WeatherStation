#include "my_wifi.h"


wifi_data_t WF::loadConfig(const char *filename)
{
  wifi_data_t _data;

  File file = LittleFS.open(filename, "r");

  if (!file)
  {
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(filename);
    DBG_OUT_PORT.println(F(", using default configuration"));
    _data = defaultConfig();
    saveConfig(filename, _data);
  }
  else
  {
    // Allocate the document on the stack.
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    DynamicJsonDocument doc(1000);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
      DBG_OUT_PORT.print(F("deserializeJson() for"));
      DBG_OUT_PORT.print(filename);
      DBG_OUT_PORT.print(F("failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F("Using default configuration"));
      _data = defaultConfig();
      saveConfig(filename, _data);
      return _data;
    }
    file.close();

    if (!error)
    {
      DBG_OUT_PORT.println(F("Read configFile sucsses!!!"));

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

      strcpy(_data.ap_ssid,  doc["apid"]);
      strcpy(_data.ap_pass , doc["appas"]);

      strcpy(_data.sta_ssid1, doc["staid1"]);
      strcpy(_data.sta_pass1, doc["stapas1"]);

      strcpy(_data.sta_ssid2, doc["staid2"]);
      strcpy(_data.sta_pass2, doc["stapas2"]);

      strcpy(_data.ap_ip, doc["iap"]);
      strcpy(_data.ap_ma, doc["map"]);

      _data.st_ip1            = doc["sst1"];
      _data.st_ip2            = doc["sst2"];
      if (_data.st_ip1)
      {

        strcpy(_data.sta_ip1, doc["ipst1"]);
        strcpy(_data.sta_ma1, doc["mast1"]);
        strcpy(_data.sta_gw1, doc["gwst1"]);
        strcpy(_data.sta_dns11, doc["dns1st1"]);
        strcpy(_data.sta_dns21, doc["dns2st1"]);
      }
      if (_data.st_ip2)
      {

        strcpy(_data.sta_ip2, doc["ipst2"]);
        strcpy(_data.sta_ma2, doc["mast2"]);
        strcpy(_data.sta_gw2, doc["gwst2"]);
        strcpy(_data.sta_dns12, doc["dns1st2"]);
        strcpy(_data.sta_dns22, doc["dns2st2"]);
      }
      _data.wifi_off          = doc["wof"];
    }
    else
    {
      DBG_OUT_PORT.print(F("deserializeJson() failed: "));
      DBG_OUT_PORT.println(error.c_str());
      DBG_OUT_PORT.println(F("Failed to read configFile, using default configuration"));

      _data = defaultConfig();
      saveConfig(filename, _data);
    }
  }
  return _data;
}

void WF::saveConfig(const char * filename, wifi_data_t _data)
{
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start saving wifi_data to config.json"));

  if ( _data.ap_ssid[0] == ' ' || _data.ap_ssid[0] ==  0) strcpy( _data.ap_ssid, "Radio_Clock");

  DynamicJsonDocument doc(1000);
  JsonObject json = doc.to<JsonObject>();

  json["apid"]    = _data.ap_ssid;
  json["appas"]   = _data.ap_pass;
  json["staid1"]  = _data.sta_ssid1;
  json["staid2"]  = _data.sta_ssid2;
  json["stapas1"] = _data.sta_pass1;
  json["stapas2"] = _data.sta_pass2;

  json["iap"]     = _data.ap_ip;
  json["map"]     = _data.ap_ma;

  json["sst1"]  = _data.st_ip1;
  json["sst2"]  = _data.st_ip2;

  if (_data.st_ip1)
  {
    json["ipst1"]   = _data.sta_ip1;
    json["mast1"]   = _data.sta_ma1;
    json["gwst1"]   = _data.sta_gw1;
    json["dns1st1"] = _data.sta_dns11;
    json["dns2st1"] = _data.sta_dns21;
  }
  if (_data.st_ip2)
  {
    json["ipst2"]   = _data.sta_ip2;
    json["mast2"]   = _data.sta_ma2;
    json["gwst2"]   = _data.sta_gw2;
    json["dns1st2"] = _data.sta_dns12;
    json["dns2st2"] = _data.sta_dns22;
  }
  json["wof"]       = _data.wifi_off;

  // Delete existing file, otherwise the configuration is appended to the file
  LittleFS.remove(filename);
  File configFile = LittleFS.open(filename, "w"); //Open config file for writing
  if (!configFile)
  {
    DBG_OUT_PORT.println(F("Failed to open config file for writing"));
    return;
  }
  if (serializeJson(doc, configFile) == 0) DBG_OUT_PORT.println(F("Failed write to file"));
  else DBG_OUT_PORT.println(F("End write buffer to file"));
  configFile.close();
}

wifi_data_t WF::defaultConfig()
{
  wifi_data_t _data;

  // ---------------------------------------------------- WiFi Default

  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital wifi_data with config.json"));

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

  strcpy(_data.ap_ssid,   "Radio_Clock");
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
