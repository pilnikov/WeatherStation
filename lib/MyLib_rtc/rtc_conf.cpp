#include "myrtc.h"

#if defined(__AVR__)
#include <EEPROM.h>
#endif

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

rtc_cfg_data_t RTCJS::from_json(String message)
{
	rtc_cfg_data_t _data;

	// Allocate the document on the stack.
	// Don't forget to change the capacity to match your requirements.
	// Use arduinojson.org/assistant to compute the capacity.
	JsonDocument doc;

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

		//---Clock.html----------------------------------------
		//---Options for clock---------------------------------
		_data.time_zone         = doc["tzo"];
		_data.auto_corr         = doc["auto_corr"];
		_data.use_pm            = doc["upm"];
		_data.every_hour_beep   = doc["e_h_b"];
		_data.nm_start          = doc["nm_start"];
		_data.nm_stop           = doc["nm_stop"];
		_data.c_type            = doc["rtc_t"];

		strcpy(_data.ntp_srv[0], doc["ntp1"] | "N/A");
		strcpy(_data.ntp_srv[1], doc["ntp2"] | "N/A");
		strcpy(_data.ntp_srv[2], doc["ntp3"] | "N/A");

		//---Alarm.html----------------------------------------
		//---Options for alarms--------------------------------
		_data.alarms[0].type = doc["al"]["0"][0];
		_data.alarms[0].time = doc["al"]["0"][1];
		_data.alarms[0].act  = doc["al"]["0"][2];
		_data.alarms[1].type = doc["al"]["1"][0];
		_data.alarms[1].time = doc["al"]["1"][1];
		_data.alarms[1].act  = doc["al"]["1"][2];
		_data.alarms[2].type = doc["al"]["2"][0];
		_data.alarms[2].time = doc["al"]["2"][1];
		_data.alarms[2].act  = doc["al"]["2"][2];
		_data.alarms[3].type = doc["al"]["3"][0];
		_data.alarms[3].time = doc["al"]["3"][1];
		_data.alarms[3].act  = doc["al"]["3"][2];
		_data.alarms[4].type = doc["al"]["4"][0];
		_data.alarms[4].time = doc["al"]["4"][1];
		_data.alarms[4].act  = doc["al"]["4"][2];
		_data.alarms[5].type = doc["al"]["5"][0];
		_data.alarms[5].time = doc["al"]["5"][1];
		_data.alarms[5].act  = doc["al"]["5"][2];
		_data.alarms[6].type = doc["al"]["6"][0];
		_data.alarms[6].time = doc["al"]["6"][1];
		_data.alarms[6].act  = doc["al"]["6"][2];
    }
	return _data;
}

String RTCJS::to_json(rtc_cfg_data_t _data)
{
	if (debug_level == 3)
	{
		DBG_OUT_PORT.print(F("Start forming wifi_cfg_data to json string"));
	}


	JsonDocument doc;
	JsonObject json = doc.to<JsonObject>();

	//---Clock.html----------------------------------------
	//---Options for clock---------------------------------
	json["tzo"]                 = _data.time_zone;
	json["auto_corr"]           = _data.auto_corr;
	json["upm"]                 = _data.use_pm;
	json["e_h_b"]               = _data.every_hour_beep;
	json["nm_start"]            = _data.nm_start;
	json["nm_stop"]             = _data.nm_stop;
	json["rtc_t"]               = _data.c_type;

	json["ntp1"]                = _data.ntp_srv[0];
	json["ntp2"]                = _data.ntp_srv[1];
	json["ntp3"]                = _data.ntp_srv[2];

	//---Alarm.html----------------------------------------
	//---Options for alarms--------------------------------

	JsonArray al0 = doc["0"].to<JsonArray>();
	JsonArray al1 = doc["1"].to<JsonArray>();
	JsonArray al2 = doc["2"].to<JsonArray>();
	JsonArray al3 = doc["3"].to<JsonArray>();
	JsonArray al4 = doc["4"].to<JsonArray>();
	JsonArray al5 = doc["5"].to<JsonArray>();
	JsonArray al6 = doc["6"].to<JsonArray>();

	al0.add(_data.alarms[0].type);
	al0.add(_data.alarms[0].time);
	al0.add(_data.alarms[0].act);
	al1.add(_data.alarms[1].type);
	al1.add(_data.alarms[1].time);
	al1.add(_data.alarms[1].act);
	al2.add(_data.alarms[2].type);
	al2.add(_data.alarms[2].time);
	al2.add(_data.alarms[2].act);
	al3.add(_data.alarms[3].type);
	al3.add(_data.alarms[3].time);
	al3.add(_data.alarms[3].act);
	al4.add(_data.alarms[4].type);
	al4.add(_data.alarms[4].time);
	al4.add(_data.alarms[4].act);
	al5.add(_data.alarms[5].type);
	al5.add(_data.alarms[5].time);
	al5.add(_data.alarms[5].act);
	al6.add(_data.alarms[6].type);
	al6.add(_data.alarms[6].time);
	al6.add(_data.alarms[6].act);

	JsonObject alarms = doc["al"].to<JsonObject>();
	alarms["0"] = al0;
	alarms["1"] = al1;
	alarms["2"] = al2;
	alarms["3"] = al3;
	alarms["4"] = al4;
	alarms["5"] = al5;
	alarms["6"] = al6;

	String msg = String();
	if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
	return msg;
}

rtc_cfg_data_t RTCJS::def_conf()
{
	rtc_cfg_data_t _data;

	// ---------------------------------------------------- WiFi Default

	if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital rtc_cfg_data"));

	_data.auto_corr        = true;
	_data.use_pm           = false;
	_data.every_hour_beep  = true;
	_data.time_zone        = 5;
	_data.c_type           = 0;
	_data.nm_start         = 23;
	_data.nm_stop          = 7;

	strcpy(_data.ntp_srv[0], "89.109.251.21");
	strcpy(_data.ntp_srv[1], "10.98.34.10");
	strcpy(_data.ntp_srv[2], "88.212.196.95");

	for (uint8_t i = 0; i <= 6; i++)
	{
		_data.alarms[i].type = 0;
		_data.alarms[i].time = 0;
		_data.alarms[i].act  = 0;
	}
	return _data;
}

#elif defined(__AVR_ATmega2560__)
rtc_cfg_data_t RTCJS::from_json(String message)
{
	rtc_cfg_data_t _data;
	EEPROM.get(0, _data);           // прочитали из адреса 0
	return _data;
}

rtc_cfg_data_t RTCJS::def_conf()
{
	rtc_cfg_data_t _data;
	if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital rtc_cfg_data"));

	_data.auto_corr        = true;
	_data.use_pm           = false;
	_data.every_hour_beep  = true;
	_data.time_zone        = 5;
	_data.c_type           = 1;
	_data.nm_start         = 0;
	_data.nm_stop          = 7;

	for (uint8_t i = 0; i <= 6; i++)
	{
		_data.alarms[i].type = 0;
		_data.alarms[i].time = 0;
		_data.alarms[i].act = 0;
	}

#if defined _dacha
	_data.alarms[0].type = 2;
	_data.alarms[0].time = 06;
	_data.alarms[0].act = 13;

#elif defined _work
	_data.alarms[0].type = 2;
	_data.alarms[0].time = 06;
	_data.alarms[0].act = 13;
#endif
	return _data;
}

String RTCJS::to_json(rtc_cfg_data_t _data)
{
	EEPROM.put(0, _data);           // записали по адресу 0
	String msg = "OK";
	return msg;
}
#endif
