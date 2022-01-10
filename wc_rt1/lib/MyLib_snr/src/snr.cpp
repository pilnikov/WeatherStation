#include "Snr.h"

ram_data_t SNR::init(ram_data_t in_data)
{
	
	ram_data_t out_data = in_data;
	for(uint8_t i = 0; i < 4; i++)
	{	
		uint8_t type_snr = 0;
		switch (i)
		{
			case 0:
				type_snr = in_data.type_snr1;
				break;
			case 1:
				type_snr = in_data.type_snr2;
				break;
			case 2:
				type_snr = in_data.type_snr3;
				break;
			case 3:
				type_snr = in_data.type_snrp;
				break;
			default: 
				break;
		}
		
		switch (type_snr)
		{
			case 4:
				dht->begin();

				#ifdef _debug
					sensor_t sensor;
					dht->temperature().getSensor(&sensor);
					DBG_OUT_PORT.println(F("------------------------------------"));
					DBG_OUT_PORT.println(F("Temperature"));
					DBG_OUT_PORT.print(F("Sensor:       ")); DBG_OUT_PORT.println(sensor.name);
					DBG_OUT_PORT.print(F("Driver Ver:   ")); DBG_OUT_PORT.println(sensor.version);
					DBG_OUT_PORT.print(F("Unique ID:    ")); DBG_OUT_PORT.println(sensor.sensor_id);
					DBG_OUT_PORT.print(F("Max Value:    ")); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(F(" °C"));
					DBG_OUT_PORT.print(F("Min Value:    ")); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(F(" °C"));
					DBG_OUT_PORT.print(F("Resolution:   ")); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(F(" °C"));
					DBG_OUT_PORT.println(F("------------------------------------"));
					// Print humidity sensor details.
					dht->humidity().getSensor(&sensor);
					DBG_OUT_PORT.println(F("------------------------------------"));
					DBG_OUT_PORT.println(F("Humidity"));
					DBG_OUT_PORT.print(F("Sensor:       ")); DBG_OUT_PORT.println(sensor.name);
					DBG_OUT_PORT.print(F("Driver Ver:   ")); DBG_OUT_PORT.println(sensor.version);
					DBG_OUT_PORT.print(F("Unique ID:    ")); DBG_OUT_PORT.println(sensor.sensor_id);
					DBG_OUT_PORT.print(F("Max Value:    ")); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(F("%"));
					DBG_OUT_PORT.print(F("Min Value:    ")); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(F("%"));
					DBG_OUT_PORT.print(F("Resolution:   ")); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(F("%"));
					DBG_OUT_PORT.println(F("------------------------------------"));
				#endif
				delayMS = 2000;
				//DBG_OUT_PORT.print  (F("Delay:   ")); DBG_OUT_PORT.print(delayMS); DBG_OUT_PORT.println(F(" ms"));
				break;
			case 6:
				if (!si.begin(0x40, &Wire))
				{
					DBG_OUT_PORT.println(F("Couldn't find a valid Si7021 sensor, check wiring!"));
					type_snr = 0;
				}
				break;
			case 7:
				if (!am23.begin())
				{
					DBG_OUT_PORT.println(F("Couldn't find a valid AM2320 sensor, check wiring!"));
					type_snr = 0;
				}
				break;
			case 8:
				if (!bmp.begin())
				{
					DBG_OUT_PORT.println(F("There was a problem detecting the BMP085/180 ... check your connections!"));
					type_snr = 0;
				}
				else
				{
					#ifdef _debug
						sensor_t sensor;
						bmp.getSensor(&sensor);

						DBG_OUT_PORT.println(F("------------------------------------"));
						DBG_OUT_PORT.print(F("Sensor:       ")); DBG_OUT_PORT.println(sensor.name);
						DBG_OUT_PORT.print(F("Driver Ver:   ")); DBG_OUT_PORT.println(sensor.version);
						DBG_OUT_PORT.print(F("Unique ID:    ")); DBG_OUT_PORT.println(sensor.sensor_id);
						DBG_OUT_PORT.print(F("Max Value:    ")); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(F(" hPa"));
						DBG_OUT_PORT.print(F("Min Value:    ")); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(F(" hPa"));
						DBG_OUT_PORT.print(F("Resolution:   ")); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(F(" hPa"));
						DBG_OUT_PORT.println(F("------------------------------------"));
					#endif //_debug 
				}
				break;
			case 9:
				if (!bmp2.begin(in_data.bm_addr, 0x58))
				{
					DBG_OUT_PORT.println(F("There was a problem detecting the BMP280 ... check your connections!"));
					type_snr = 0;
				}
				break;
			case 10:
				if (!bme.begin(in_data.bm_addr, &Wire))
				{
					DBG_OUT_PORT.println(F("There was a problem detecting the BME280 ... check your connections!"));
					type_snr = 0;
				}
				break;
			case 13:
				SNR::ds_init(in_data.gpio_dht);
				break;
			default:
				break;
		}
		
		switch (i)
		{
			case 0:
				out_data.type_snr1 = type_snr;
				break;
			case 1:
				out_data.type_snr2 = type_snr;
				break;
			case 2:
				out_data.type_snr3 = type_snr;
				break;
			case 3:
				out_data.type_snrp = type_snr;
				break;
			default: 
				break;
		}

		
	}
	return out_data;
}


float SNR::humi_read(uint8_t type_snr)
{
	float ret = 0; 
	switch (type_snr)
	{
		case 4:
			delay(delayMS);
			#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
				ret = dht->readHumidity();
				# ifdef _debug
					DBG_OUT_PORT.print(F("DHT Humidity: "));
				# endif //_debug 
			#elif defined(__xtensa__) || defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32C3
				sensors_event_t event;

				// Get humidity event.
				dht->humidity().getEvent(&event);

				if (isnan(event.relative_humidity))
				{
					DBG_OUT_PORT.println(F("DHT Error reading humidity!"));
				}
				else
				{
					ret = event.relative_humidity;
					# ifdef _debug
						DBG_OUT_PORT.print(F("DHT Humidity: "));
					# endif //_debug 
				}
			#endif
			break;
		case 6:
			ret = si.readHumidity();
			#ifdef _debug
				DBG_OUT_PORT.print(F("Si7021 Humidity: "));
			# endif //_debug 
			break;
		case 7:
			delay(600);
			switch (am23.Read()) 
			{
				case 2:
					#ifdef _debug
						DBG_OUT_PORT.println(F("AM2320 CRC failed"));
					#endif //_debug 
					break;
				case 1:
					#ifdef _debug
						DBG_OUT_PORT.println(F("AM2320 offline"));
					#endif //_debug 
					break;
				case 0:
					#ifdef _debug
						DBG_OUT_PORT.print(F("AM2320 Humidity: "));
					#endif //_debug 
					ret = am23.h;
					break;
			}
			break;
		case 10:
			ret = bme.readHumidity();
			#ifdef _debug
				DBG_OUT_PORT.print(F("BME280 Humidity: "));
			#endif //_debug 
			break;
		default:
			break;
	}
	#ifdef _debug
		DBG_OUT_PORT.print(ret);
		DBG_OUT_PORT.println(F(" %"));
	#endif //_debug 
	return ret;
}	

float SNR::temp_read(uint8_t type_snr)
{
	float ret = 99; 
	switch (type_snr)
	{
		case 4:
			delay(delayMS);
			#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
				ret = dht->readTemperature();
				# ifdef _debug
					DBG_OUT_PORT.print(F("DHT Temperature: "));
				# endif //_debug 
			#elif defined(__xtensa__) || defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32C3
				sensors_event_t event;

				// Get temperature event.
				dht->temperature().getEvent(&event);

				if (isnan(event.temperature))
				{
					DBG_OUT_PORT.println(F("DHT Error reading temperature!"));
				}
				else
				{
					ret = event.temperature;
					# ifdef _debug
						DBG_OUT_PORT.print(F("DHT Temperature: "));
					# endif //_debug 
				}
			#endif
			break;
		case 6:
			ret = si.readTemperature();
			#ifdef _debug
				DBG_OUT_PORT.print(F("Si7021 Temperature: "));
			# endif //_debug 
			break;
		case 7:
			delay(600);
			switch (am23.Read()) 
			{
				case 2:
					#ifdef _debug
						DBG_OUT_PORT.println(F("AM2320 CRC failed"));
					#endif //_debug 
					break;
				case 1:
					#ifdef _debug
						DBG_OUT_PORT.println(F("AM2320 offline"));
					#endif //_debug 
					break;
				case 0:
					#ifdef _debug
						DBG_OUT_PORT.print(F("AM2320 Temperature: "));
					#endif //_debug 
					ret = am23.t;
					break;
			}
			break;
		case 8:
			sensors_event_t event1;
			bmp.getEvent(&event1);
			if (event1.pressure)
			{
				bmp.getTemperature(&ret);
				#ifdef _debug
					DBG_OUT_PORT.print(F("BMP180 Temperature: "));
				#endif //_debug 
			}
			else
			{
				DBG_OUT_PORT.println(F("BMP180 read error"));
			}
			break;
		case 9:
			ret = bmp2.readTemperature();
			#ifdef _debug
				DBG_OUT_PORT.print(F("BMP280 Temperature: "));
			#endif //_debug 
			break;
		case 10:
			ret = bme.readTemperature();
			#ifdef _debug
				DBG_OUT_PORT.print(F("BME280 Temperature: "));
			#endif //_debug 
			break;
		case 13:
			ret = SNR::ds_read();
			#ifdef _debug
				DBG_OUT_PORT.print(F("DS18B20 Temperature: "));
			#endif //_debug 
			break;
		default:
			break;
	}
	#ifdef _debug
		DBG_OUT_PORT.print(ret);
		DBG_OUT_PORT.println(F(" °C"));
	#endif //_debug 
	return ret;
}	

float SNR::pres_read(uint8_t type_snr)
{
	float ret = 700; 
	switch (type_snr)
	{
		case 8:
			sensors_event_t event;
			bmp.getEvent(&event);
			if (event.pressure)
			{
				ret = event.pressure / 1.33322;
				#ifdef _debug
					DBG_OUT_PORT.print(F("BMP180 Pressure:  "));
				#endif //_debug 
			}
			else
			{
				DBG_OUT_PORT.println(F("BMP180 read error"));
			}
			break;
		case 9:
			ret = bmp2.readPressure() / 133.3;
			#ifdef _debug
				DBG_OUT_PORT.print(F("BMP280 Pressure:  "));
			#endif //_debug 
			break;
		case 10:
			ret = bme.readPressure() / 133.3;
			#ifdef _debug
				DBG_OUT_PORT.print(F("BME280 Pressure:  "));
			#endif //_debug 
			break;
		default:
			break;
	}
	#ifdef _debug
		DBG_OUT_PORT.print(ret);
		DBG_OUT_PORT.println(F(" mmHg"));
	#endif //_debug 
	return ret;
}	


void SNR::dht_preset(uint8_t pin, uint8_t type) // установка пинов
{
#if defined(__xtensa__) || defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32C3
	dht = new DHT_Unified(pin, type);
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
	dht = new DHT(pin, type);
#endif
}

void SNR::ds_init(uint8_t pin)
{
	#ifdef PWR_CTRL_PIN
	# ifndef CONFIG_PWR_CTRL_ENABLED
	#  error "CONFIG_PWR_CTRL_ENABLED needs to be enabled"
	# endif
		new (&_ow) OneWireNg_CurrentPlatform(pin, PWR_CTRL_PIN, false);
	#else
		new (&_ow) OneWireNg_CurrentPlatform(pin, false);
	#endif

	DSTherm drv(_ow);

	#if (CONFIG_MAX_SRCH_FILTERS > 0)
		static_assert(CONFIG_MAX_SRCH_FILTERS >= DSTherm::SUPPORTED_SLAVES_NUM,
		"CONFIG_MAX_SRCH_FILTERS too small");

		drv.filterSupportedSlaves();
	#endif

	#ifdef COMMON_RES
		drv.writeScratchpadAll(0, 0, COMMON_RES);
		drv.copyScratchpadAll(PARASITE_POWER);
	#endif
}

int SNR::ds_read()
{
	int ret = 99;
	DSTherm drv(_ow);
	Placeholder<DSTherm::Scratchpad> _scrpd;

	/* convert temperature on all sensors connected... */
	drv.convertTempAll(DSTherm::SCAN_BUS, PARASITE_POWER);
	
	/* ...and read them one-by-one */
 	ret = SNR::printScratchpad(_scrpd);
	DBG_OUT_PORT.print(F(" DS READ TEMP 3!!! "));
	DBG_OUT_PORT.println(ret);
	return ret;
}			

/* returns false if not supported */
bool SNR::printId(const OneWireNg::Id& id)
{
    const char *name = DSTherm::getFamilyName(id);

    DBG_OUT_PORT.print(id[0], HEX);
    for (size_t i = 1; i < sizeof(OneWireNg::Id); i++) {
        DBG_OUT_PORT.print(':');
        DBG_OUT_PORT.print(id[i], HEX);
    }
    if (name) {
        DBG_OUT_PORT.print(" -> ");
        DBG_OUT_PORT.print(name);
    }
    DBG_OUT_PORT.println();

    return (name != NULL);
}

int SNR::printScratchpad(const DSTherm::Scratchpad& scrpd)
{
    int ret = 99;
	const uint8_t *scrpd_raw = scrpd.getRaw();
    long temp = scrpd.getTemp();
	DBG_OUT_PORT.print(F("ds temp is.."));
	DBG_OUT_PORT.println(temp);
    ret = temp / 1000;
	return ret;
}


snr_data_t SNR::read_snr(uint8_t type_snr1, uint8_t type_snr2, uint8_t type_snr3, uint8_t type_snrp, uint8_t rtc_temp, snr_data_t t_data, snr_data_t e_data1, snr_data_t e_data2, wf_data_t w_data)
{
	snr_data_t data;

	for(uint8_t i = 0; i < 4; i++)
	{	
		uint8_t type_snr = 0; 
		float t_data_h = 0, t_data_t = 99,  t_data_p = 700, e_data1_h = 0, e_data1_t = 99, e_data1_p = 700, e_data2_h = 0, e_data2_t = 99, e_data2_p =700;

		switch (i)
		{
			case 0:
				type_snr = type_snr1;
				data.h1 = 0; 
				data.t1 = 99;

				t_data_h = t_data.h1;
				t_data_t = t_data.t1;

				e_data1_h = e_data1.h1;
				e_data1_t = e_data1.t1;

				e_data2_h = e_data2.h1;
				e_data2_t = e_data2.t1;
				break;
			case 1:
				type_snr = type_snr2;
				data.h2 = 0; 
				data.t2 = 99;

				t_data_h = t_data.h2;
				t_data_t = t_data.t2;

				e_data1_h = e_data1.h2;
				e_data1_t = e_data1.t2;

				e_data2_h = e_data2.h2;
				e_data2_t = e_data2.t2;
				break;
			case 2:
				type_snr = type_snr3;
				data.h3 = 0; 
				data.t3 = 99;

				t_data_h = t_data.h3;
				t_data_t = t_data.t3;

				e_data1_h = e_data1.h3;
				e_data1_t = e_data1.t3;

				e_data2_h = e_data2.h3;
				e_data2_t = e_data2.t3;
				break;
			case 3:
				data.p = 700; 
				type_snr = type_snrp;

				t_data_p = t_data.p;

				e_data1_p = e_data1.p;

				e_data2_p = e_data2.p;
				break;
			default: 
				break;
		}
		
		float h = 0, t = 99, p = 700;

		switch (type_snr)
		{
			case 1:
				h = t_data_h;
				t = t_data_t;
				p = t_data_p;
				break;
			case 2:
				h = e_data1_h;
				t = e_data1_t;
				p = e_data1_p;
				break;
			case 3:
				h = e_data2_h;
				t = e_data2_t;
				p = e_data2_p;
				break;
			case 4:
				h = humi_read(type_snr);
				t = temp_read(type_snr);
				break;
			case 5:
				t = rtc_temp;
				break;
			case 6:
				h = humi_read(type_snr);
				t = temp_read(type_snr);
				break;
			case 7:
				h = humi_read(type_snr);
				t = temp_read(type_snr);
				break;
			case 8:
				t = temp_read(type_snr);
				p = pres_read(type_snr);
				break;
			case 9:
				t = temp_read(type_snr);
				p = pres_read(type_snr);
				break;
			case 10:
				h = humi_read(type_snr);
				t = temp_read(type_snr);
				p = pres_read(type_snr);
				break;
			case 11:
				h = w_data.hum_min;
				t = w_data.temp_min;
				p = w_data.press_min;
				break;
			case 13:
				t = temp_read(type_snr);
				break;
			default:
				break;
		}
		
		switch (i)
		{
			case 0:
				//------------------------------------------- channel 1
				if (type_snr >0)
				{
					DBG_OUT_PORT.print(F("Snr type on ch1.."));
					DBG_OUT_PORT.println(type_snr);
					data.t1 = constrain(t, -99, 99);
					data.h1 = constrain(h, 0, 99);

					DBG_OUT_PORT.print(F("TC1.."));
					DBG_OUT_PORT.println(data.t1);
					DBG_OUT_PORT.print(F("HC1.."));
					DBG_OUT_PORT.println(data.h1);
				}
				break;
			case 1:
				//------------------------------------------- channel 2
				if (type_snr >0)
				{
					DBG_OUT_PORT.print(F("Snr type on ch2.."));
					DBG_OUT_PORT.println(type_snr);
					data.t2 = constrain(t, -99, 99);
					data.h2 = constrain(h, 0, 99);

					DBG_OUT_PORT.print(F("TC2.."));
					DBG_OUT_PORT.println(data.t2);
					DBG_OUT_PORT.print(F("HC2.."));
					DBG_OUT_PORT.println(data.h2);
				}
				break;
			case 2:
				//------------------------------------------- channel 3
				if (type_snr >0)
				{
					DBG_OUT_PORT.print(F("Snr type on ch3.."));
					DBG_OUT_PORT.println(type_snr);
					data.t3 = constrain(t, -99, 99);
					data.h3 = constrain(h, 0, 99);

					DBG_OUT_PORT.print(F("TC3.."));
					DBG_OUT_PORT.println(data.t3);
					DBG_OUT_PORT.print(F("HC3.."));
					DBG_OUT_PORT.println(data.h3);
				}
				break;
			case 3:
				//------------------------------------------- channel p
				if (type_snr >0)
				{
					DBG_OUT_PORT.print(F("Snr type on chp.."));
					DBG_OUT_PORT.println(type_snr);
					data.p = constrain(p, 700, 800);

					DBG_OUT_PORT.print(F("P.."));
					DBG_OUT_PORT.println(data.p);
				}
				break;
			default: 
				break;
		}
	}
	return data;
}
