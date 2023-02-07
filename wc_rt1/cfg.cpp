
#include "conf.h"
#include <ArduinoJson.h>

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
#define ARDUINOJSON_USE_LONG_LONG 1

conf_data_t MAINJS::from_json(String message)
{
	conf_data_t _data;

	// Allocate the document on the stack.
	// Don't forget to change the capacity to match your requirements.
	// Use arduinojson.org/assistant to compute the capacity.
	DynamicJsonDocument doc(3100);

	// Deserialize the JSON document
	DeserializationError error = deserializeJson(doc, message);
	if (error)
	{
		DBG_OUT_PORT.print(F("deserializeJson() for configFile failed: "));
		DBG_OUT_PORT.println(error.c_str());
		DBG_OUT_PORT.println(F(". Using default configuration"));
		_data = def_conf();
		return _data;
	}
	else
	{
		DBG_OUT_PORT.println(F("Read msg sucsses!!!"));

		// Get the root object in the document
		memset(_data.owm_key, 0, 33);
		memset(_data.esrv1_addr, 0, 17);
		memset(_data.esrv2_addr, 0, 17);
		memset(_data.radio_addr, 0, 17);
		memset(_data.srudp_addr, 0, 17);
		memset(_data.ch1_name, 0, 17);
		memset(_data.ch2_name, 0, 17);
		memset(_data.ch3_name, 0, 17);
		memset(_data.AKey_r, 0, 17);
		memset(_data.AKey_w, 0, 17);
		memset(_data.news_api_key, 0, 33);
		memset(_data.news_source, 0, 17);

		//---gpio.html----------------------------------------
		//---Options for HW---------------------------------
		_data.snd_pola = doc["snd_pola"];
		_data.led_pola = doc["led_pola"];

		//---GPIO-----------------------------------------------
		_data.gpio_sda = doc["sda"];
		_data.gpio_scl = doc["scl"];
		_data.gpio_dio = doc["dio"];
		_data.gpio_clk = doc["clk"];
		_data.gpio_dcs = doc["dcs"];
		_data.gpio_dwr = doc["dwr"];
		_data.gpio_trm = doc["trm"];
		_data.gpio_sqw = doc["sqw"];
		_data.gpio_snd = doc["snd"];
		_data.gpio_led = doc["led"];
		_data.gpio_btn = doc["btn"];
		_data.gpio_dht = doc["dht"];
		_data.gpio_ana = doc["ana"];
		_data.gpio_uar = doc["uar"];

		//---Display.html--------------------------------------
		//---Options for display-------------------------------
		_data.type_vdrv = doc["vdrv_t"];
		_data.type_disp = doc["disp_t"];
		_data.rus_lng = doc["rus_lng"];
		_data.time_up = doc["t_up"];
		_data.color_up = doc["colu"];
		_data.color_dwn = doc["cold"];
		_data.type_font = doc["type_font"];

		//---Brigthness----------------------------------------
		_data.man_br = doc["man_br"];
		_data.nmd_br = doc["nmd_br"];
		_data.auto_br = doc["auto_br"];

		//---Brigthness levels---------------------------------
		for (uint8_t i = 0; i <= 3; i++) _data.br_level[i] = doc["br_level"][i];

		//---Sensor.html---------------------------------------
		//---Options for sensor--------------------------------
		_data.pp_city_id = doc["pp_city_id"];
		strcpy(_data.owm_key, doc["owm_key"]);
		strcpy(_data.esrv1_addr, doc["esrv1_a"]);
		strcpy(_data.esrv2_addr, doc["esrv2_a"]);
		strcpy(_data.radio_addr, doc["radio_a"]);
		strcpy(_data.srudp_addr, doc["srudp_a"]);
		_data.udp_mon = doc["udm"];
		_data.use_pp = doc["upp"];

		//---Sensor type---------------------------------------
		_data.period = doc["period"]; // minutes
		_data.use_es = doc["ues"];
		_data.esm = doc["esm"];

		//---Sensor actual value-------------------------------
		strcpy(_data.ch1_name, doc["ch1_name"]);
		strcpy(_data.ch2_name, doc["ch2_name"]);
		strcpy(_data.ch3_name, doc["ch3_name"]);

		//---TS Account----------------------------------------
		_data.ts_ch_id = doc["ts_ch_id"];
		strcpy(_data.AKey_r, doc["AKey_r"]);
		strcpy(_data.AKey_w, doc["AKey_w"]);

		//---TS sender-----------------------------------------
		_data.use_ts = doc["uts"];

		//---Thermo.html---------------------------------------
		//---Options for thermostat----------------------------
		_data.type_thermo = doc["trs_t"];
		_data.src_thermo = doc["src_trs"];
		_data.lb_thermo = doc["lb_trs"];
		_data.hb_thermo = doc["hb_trs"];

		//---News.html---------------------------------------
		//---Options for news----------------------------
		_data.news_en = doc["news_en"];
		strcpy(_data.news_api_key, doc["news_api"]);
		strcpy(_data.news_source, doc["news_src"]);
	}
	return _data;
}

String MAINJS::to_json(conf_data_t _data)
{
	if (debug_level == 3) DBG_OUT_PORT.println(F("Start saving conf_data to config.json"));

	if (_data.type_vdrv > 20) _data.type_vdrv = 0;
	if (_data.type_disp > 50) _data.type_disp = 0;

	DynamicJsonDocument doc(3000);
	JsonObject json = doc.to<JsonObject>();

	//---gpio.html----------------------------------------
	//---Options for HW---------------------------------
	json["snd_pola"] = _data.snd_pola;
	json["led_pola"] = _data.led_pola;

	//---GPIO-----------------------------------------------
	json["sda"] = _data.gpio_sda;
	json["scl"] = _data.gpio_scl;
	json["dio"] = _data.gpio_dio;
	json["clk"] = _data.gpio_clk;
	json["dcs"] = _data.gpio_dcs;
	json["dwr"] = _data.gpio_dwr;
	json["trm"] = _data.gpio_trm;
	json["sqw"] = _data.gpio_sqw;
	json["snd"] = _data.gpio_snd;
	json["led"] = _data.gpio_led;
	json["btn"] = _data.gpio_btn;
	json["dht"] = _data.gpio_dht;
	json["ana"] = _data.gpio_ana;
	json["uar"] = _data.gpio_uar;

	//---Display.html--------------------------------------
	//---Options for display-------------------------------
	json["vdrv_t"] = _data.type_vdrv;
	json["disp_t"] = _data.type_disp;
	json["rus_lng"] = _data.rus_lng;
	json["t_up"] = _data.time_up;
	json["colu"] = _data.color_up;
	json["cold"] = _data.color_dwn;
	json["type_font"] = _data.type_font;

	//---Brigthness----------------------------------------
	json["man_br"] = _data.man_br;
	json["nmd_br"] = _data.nmd_br;
	json["auto_br"] = _data.auto_br;

	//---Brigthness levels---------------------------------
	JsonArray br_level = json.createNestedArray("br_level");
	for (uint8_t i = 0; i <= 3; i++) br_level.add(_data.br_level[i]);

	//---Sensor.html---------------------------------------
	//---Options for sensor--------------------------------
	json["pp_city_id"] = _data.pp_city_id;
	json["owm_key"] = _data.owm_key;
	json["esrv1_a"] = _data.esrv1_addr;
	json["esrv2_a"] = _data.esrv2_addr;
	json["radio_a"] = _data.radio_addr;
	json["srudp_a"] = _data.srudp_addr;
	json["udm"] = _data.udp_mon;
	json["upp"] = _data.use_pp;

	//---Sensor type---------------------------------------
	json["period"] = _data.period; // minutes
	json["ues"] = _data.use_es;
	json["esm"] = _data.esm;

	//---Sensor channel name -------------------------------
	json["ch1_name"] = _data.ch1_name;
	json["ch2_name"] = _data.ch2_name;
	json["ch3_name"] = _data.ch3_name;

	//---TS Account----------------------------------------
	json["ts_ch_id"] = _data.ts_ch_id;
	json["AKey_r"] = _data.AKey_r;
	json["AKey_w"] = _data.AKey_w;

	//---TS sender-----------------------------------------
	json["uts"] = _data.use_ts;

	//---Thermo.html---------------------------------------
	//---Options for thermostat----------------------------
	json["trs_t"] = _data.type_thermo;
	json["src_trs"] = _data.src_thermo;
	json["lb_trs"] = _data.lb_thermo;
	json["hb_trs"] = _data.hb_thermo;


	//---News.html---------------------------------------
	//---Options for news----------------------------
	json["news_en"] = _data.news_en;
	json["news_api"] = _data.news_api_key;
	json["news_src"] = _data.news_source;

	String msg = String();
	if (serializeJson(doc, msg) == 0) DBG_OUT_PORT.println(F("Failed write json to string"));
	return msg;
}

conf_data_t MAINJS::def_conf()
{
	conf_data_t _data;

	// ---------------------------------------------------- WiFi Default

	if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

	memset(_data.AKey_r, 0, 17);
	memset(_data.AKey_w, 0, 17);
	strcpy(_data.esrv1_addr, "192.168.1.100");
	strcpy(_data.esrv2_addr, "192.168.1.200");
	strcpy(_data.radio_addr, "192.168.1.33");
	strcpy(_data.srudp_addr, "192.168.1.30");
	memset(_data.owm_key, 0, 33);
	strcpy(_data.ch1_name, "Внутри");
	strcpy(_data.ch2_name, "Снаружи");
	strcpy(_data.ch3_name, "В бане");
	memset(_data.news_api_key, 0, 33);
	strcpy(_data.news_source, "lenta");


	_data.snd_pola = false;
	_data.led_pola = false;
	_data.rus_lng = false;
	_data.time_up = false;
	_data.udp_mon = false;
	_data.esm = false;
	_data.auto_br = false;
	_data.color_up = 0;
	_data.color_dwn = 0;
	_data.use_pp = 0;
	_data.type_vdrv = 0;
	_data.type_disp = 0;
	_data.use_es = 0;
	_data.use_ts = 0;
	_data.type_thermo = 0;
	_data.src_thermo = 0;
	_data.lb_thermo = 0;
	_data.hb_thermo = 0;
	_data.ts_ch_id = 0;
	_data.pp_city_id = 28438;
	_data.period = 10;
	_data.man_br = 7;
	_data.nmd_br = 2;

#if defined(ESP8266)
	_data.gpio_sda = 4;
	_data.gpio_scl = 5;
	_data.gpio_dio = 13;
	_data.gpio_clk = 14;
	_data.gpio_dcs = 16;
	_data.gpio_dwr = 2;
	_data.gpio_trm = 2;
	_data.gpio_sqw = 12;
	_data.gpio_snd = 15;
	_data.gpio_led = 2;
	_data.gpio_btn = 0;
	_data.gpio_dht = 0;
	_data.gpio_ana = 17;
	_data.gpio_uar = 16;

#elif CONFIG_IDF_TARGET_ESP32
	_data.gpio_sda = 23;
	_data.gpio_scl = 22;
	_data.gpio_dio = 19;
	_data.gpio_clk = 18;
	_data.gpio_dcs = 5;
	_data.gpio_dwr = 26;
	_data.gpio_trm = 36;
	_data.gpio_sqw = 34;
	_data.gpio_snd = 2;
	_data.gpio_led = 16;
	_data.gpio_btn = 0;
	_data.gpio_dht = 39;
	_data.gpio_ana = 35;
	_data.gpio_uar = 36;

#elif CONFIG_IDF_TARGET_ESP32C3
	_data.gpio_sda = 5;
	_data.gpio_scl = 6;
	_data.gpio_dio = 7;
	_data.gpio_clk = 8;
	_data.gpio_dcs = 5;
	_data.gpio_dwr = 0;
	_data.gpio_trm = 10;
	_data.gpio_sqw = 1;
	_data.gpio_snd = 2;
	_data.gpio_led = 3;
	_data.gpio_btn = 9;
	_data.gpio_dht = 10;
	_data.gpio_ana = 10;
	_data.gpio_uar = 10;
#endif

	_data.br_level[0] = 1;
	_data.br_level[1] = 200;
	_data.br_level[2] = 1;
	_data.br_level[3] = 15;

	_data.type_font = 0;

	_data.news_en = false;

	return _data;
}

#elif defined(__AVR_ATmega2560__)

#include <EEPROM.h>

conf_data_t MAINJS::from_json(String message)
{
	conf_data_t _data;
	EEPROM.get(0, _data);           // прочитали из адреса 0
	return _data;
}

conf_data_t MAINJS::def_conf()
{
	conf_data_t _data;
	if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

#if defined _dacha
	strcpy(_data.ch1_name, "В избе");
	strcpy(_data.ch2_name, "На улице");
	strcpy(_data.ch3_name, "Подпол");
#elif defined _work
	strcpy(_data.ch1_name, "Внутри");
	strcpy(_data.ch2_name, "На улице");
	memset(_data.ch3_name, 0, 17);
#endif
	_data.snd_pola = false;
	_data.led_pola = true;
	_data.rus_lng = true;

#if defined _dacha
	_data.type_vdrv = 2;
	_data.type_disp = 21;

#elif defined _work
	_data.type_vdrv = 3;
	_data.type_disp = 23;

	_data.type_snr1 = 10;
	_data.type_snr2 = 4;
	_data.type_snr3 = 0;
	_data.type_snrp = 10;
#endif

	_data.type_thermo = 0;
	_data.src_thermo = 0;
	_data.lb_thermo = 0;
	_data.hb_thermo = 0;
	_data.period = 10;
	_data.man_br = 14;
	_data.nmd_br = 2;
	_data.esm = false;
	_data.auto_br = true;

	_data.gpio_sda = 20;
	_data.gpio_scl = 21;
	_data.gpio_dio = 57; //A3
	_data.gpio_clk = 56; //A2
	_data.gpio_dcs = 55; //A1
	_data.gpio_dwr = 54; //A0
	_data.gpio_trm = 60; //A6
	_data.gpio_sqw = 19;
	_data.gpio_snd = 61; //A7
	_data.gpio_led = 60; //A6
	_data.gpio_btn = 59; //A5
	_data.gpio_dht = 63; //A9
	_data.gpio_ana = 58; //A4
	_data.gpio_uar = 65; //A11

#if defined _dacha
	_data.br_level[0] = 220;
	_data.br_level[1] = 1;
	_data.br_level[2] = 1;
	_data.br_level[3] = 14;

#elif defined _work
	_data.br_level[0] = 300;
	_data.br_level[1] = 1;
	_data.br_level[2] = 1;
	_data.br_level[3] = 254;
#endif

	return _data;
}

String MAINJS::to_json(conf_data_t _data)
{
	EEPROM.put(0, _data);           // записали по адресу 0
	String ret = "OK";
	return ret;
}
#endif
