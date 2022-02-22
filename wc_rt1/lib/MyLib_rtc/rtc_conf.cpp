#include "myrtc.h"

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

rtc_cfg_data_t RTCJS::from_json(String message)
{
	rtc_cfg_data_t _data;

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
		for (uint8_t j = 0; j <= 4; j++)
		{
			_data.alarms[0][j] = doc["al"]["0"][j];
			_data.alarms[1][j] = doc["al"]["1"][j];
			_data.alarms[2][j] = doc["al"]["2"][j];
			_data.alarms[3][j] = doc["al"]["3"][j];
			_data.alarms[4][j] = doc["al"]["4"][j];
			_data.alarms[5][j] = doc["al"]["5"][j];
			_data.alarms[6][j] = doc["al"]["6"][j];
		}
    }
	return _data;
}

String RTCJS::to_json(rtc_cfg_data_t _data)
{
	if (debug_level == 3)
	{
		DBG_OUT_PORT.print(F("Start forming wifi_cfg_data to json string"));
	}


	DynamicJsonDocument doc(3100);
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
	DynamicJsonDocument doc2(700);
	JsonObject json2 = doc2.to<JsonObject>();

	JsonArray al0 = json2.createNestedArray("0");
	JsonArray al1 = json2.createNestedArray("1");
	JsonArray al2 = json2.createNestedArray("2");
	JsonArray al3 = json2.createNestedArray("3");
	JsonArray al4 = json2.createNestedArray("4");
	JsonArray al5 = json2.createNestedArray("5");
	JsonArray al6 = json2.createNestedArray("6");
	for (uint8_t j = 0; j <= 4; j++)
	{
		al0.add(_data.alarms[0][j]);
		al1.add(_data.alarms[1][j]);
		al2.add(_data.alarms[2][j]);
		al3.add(_data.alarms[3][j]);
		al4.add(_data.alarms[4][j]);
		al5.add(_data.alarms[5][j]);
		al6.add(_data.alarms[6][j]);
	}

	JsonObject alarms = json.createNestedObject("al");
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
	for (uint8_t j = 0; j <= 4; j++) _data.alarms[i][j] = 0;

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
