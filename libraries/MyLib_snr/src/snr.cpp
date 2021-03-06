#include "Snr.h"

ram_data_t SNR::init(ram_data_t in_data)
{
	ram_data_t out_data = in_data;

	switch (in_data.type_snr1)
	{
	case 4:
		dht_init();
		break;
	case 6:
		if (!sht.begin())
		{
			DBG_OUT_PORT.println("Couldn't find a valid SHT21 sensor, check wiring!");
			out_data.type_snr1 = 0;
		}
		break;
	case 7:
		if (!am23.begin())
		{
			DBG_OUT_PORT.println("Couldn't find a valid AM2320 sensor, check wiring!");
			out_data.type_snr1 = 0;
		}
		break;
	case 8:
		if (!bmp.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
			out_data.type_snr1 = 0;
		}
		break;
	case 9:
		if (!bmp2.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
			out_data.type_snr1 = 0;
		}
		break;
	case 10:
		if (!bme.begin(0x76, &Wire))
		{
			DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
			out_data.type_snr1 = 0;
		}
		break;
	default:
		break;
	}

	switch (in_data.type_snr2)
	{
	case 4:
		dht_init();
		break;
	case 6:
		if (!sht.begin())
		{
			DBG_OUT_PORT.println("Couldn't find a valid SHT21 sensor, check wiring!");
			out_data.type_snr2 = 0;
		}
		break;
	case 7:
		if (!am23.begin())
		{
			DBG_OUT_PORT.println("Couldn't find a valid AM2320 sensor, check wiring!");
			out_data.type_snr2 = 0;
		}
		break;
	case 8:
		if (!bmp.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
			out_data.type_snr2 = 0;
		}
		break;
	case 9:
		if (!bmp2.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
			out_data.type_snr2 = 0;
		}
		break;
	case 10:
		if (!bme.begin(0x76, &Wire))
		{
			DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
			out_data.type_snr2 = 0;
		}
		break;
	default:
		break;
	}

	switch (in_data.type_snr3)
	{
	case 4:
		dht_init();
		break;
	case 6:
		if (!sht.begin())
		{
			DBG_OUT_PORT.println("Couldn't find a valid SHT21 sensor, check wiring!");
			out_data.type_snr3 = 0;
		}
		break;
	case 7:
		if (!am23.begin())
		{
			DBG_OUT_PORT.println("Couldn't find a valid AM2320 sensor, check wiring!");
			out_data.type_snr3 = 0;
		}
		break;
	case 8:
		if (!bmp.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
			out_data.type_snr3 = 0;
		}
		break;
	case 9:
		if (!bmp2.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
			out_data.type_snr3 = 0;
		}
		break;
	case 10:
		if (!bme.begin(0x76, &Wire))
		{
			DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
			out_data.type_snr3 = 0;
		}
		break;
	default:
		break;
	}

	switch (in_data.type_snrp)
	{
	case 8:
		if (!bmp.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
			out_data.type_snrp = 0;
		}
		break;
	case 9:
		if (!bmp2.begin())
		{
			DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
			out_data.type_snrp = 0;
		}
		break;
	case 10:
		if (!bme.begin(0x76, &Wire))
		{
			DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
			out_data.type_snrp = 0;
		}
		break;
	default:
		break;
	}
	return out_data;
}

float SNR::am_read_h()
{
	delay(600);

	float ret = -1.0;


	switch (am23.Read()) {
	case 2:
# ifdef _debug
		DBG_OUT_PORT.println("CRC failed");
# endif
		break;
	case 1:
# ifdef _debug
		DBG_OUT_PORT.println("Sensor offline");
# endif
		break;
	case 0:
		ret = am23.h;
		break;
	}
	return ret;
}

float SNR::am_read_t()
{
	delay(600);

	float ret = 99.0;

	switch (am23.Read()) {
	case 2:
# ifdef _debug
		DBG_OUT_PORT.println("CRC failed");
# endif
		break;
	case 1:
# ifdef _debug
		DBG_OUT_PORT.println("Sensor offline");
# endif
		break;
	case 0:
		ret = am23.t;
		break;
	}
	return ret;
}


float SNR::sht_read_t()
{
#ifdef _debug
	DBG_OUT_PORT.print("SHT Temperature: ");
	DBG_OUT_PORT.print(sht.readTemperature());
	DBG_OUT_PORT.println(" *C");
# endif //_debug 
	return sht.readTemperature();
}

float SNR::sht_read_h()
{
#ifdef _debug
	DBG_OUT_PORT.print("SHT Humidity: ");
	DBG_OUT_PORT.println(sht.readHumidity());
	DBG_OUT_PORT.println("%");
# endif //_debug 
	return sht.readHumidity();
}

void SNR::dispsnrDetails(void)
{
	sensor_t sensor;
	bmp.getSensor(&sensor);

	DBG_OUT_PORT.println("------------------------------------");
	DBG_OUT_PORT.print("Sensor:       "); DBG_OUT_PORT.println(sensor.name);
	DBG_OUT_PORT.print("Driver Ver:   "); DBG_OUT_PORT.println(sensor.version);
	DBG_OUT_PORT.print("Unique ID:    "); DBG_OUT_PORT.println(sensor.sensor_id);
	DBG_OUT_PORT.print("Max Value:    "); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(" hPa");
	DBG_OUT_PORT.print("Min Value:    "); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(" hPa");
	DBG_OUT_PORT.print("Resolution:   "); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(" hPa");
	DBG_OUT_PORT.println("------------------------------------");
}

float SNR::bmp180_read_t()
{
#ifdef _debug
	displaySensorDetails();
#endif //_debug 
	sensors_event_t event;
	bmp.getEvent(&event);
	if (event.pressure)
	{
		float bmp_t;
		bmp.getTemperature(&bmp_t);
		return bmp_t - 18.0;
	}
	else
	{
		DBG_OUT_PORT.println("Sensor error");
		return 99;
	}
}

float SNR::bmp180_read_p()
{
#ifdef _debug
	dispsnrDetails();
#endif //_debug 
	sensors_event_t event;
	bmp.getEvent(&event);
	if (event.pressure)
	{
		return event.pressure / 1.33322 - 18.0;
	}
	else
	{
		DBG_OUT_PORT.println("Sensor error");
		return -1;
	}
}

float SNR::bmp280_read_t()
{
#ifdef _debug
	DBG_OUT_PORT.print("Temperature = ");
	DBG_OUT_PORT.print(bmp2.readTemperature());
	DBG_OUT_PORT.println(" *C");
#endif //_debug 
	return bmp2.readTemperature();
}

float SNR::bmp280_read_p()
{
#ifdef _debug
	DBG_OUT_PORT.print("Pressure = ");
	DBG_OUT_PORT.print(bmp2.readPressure());
	DBG_OUT_PORT.println(" Pa");

	DBG_OUT_PORT.print("Approx altitude = ");
	DBG_OUT_PORT.print(bmp2.readAltitude(SEALEVELPRESSURE_HPA)); // this should be adjusted to your local forcase
	DBG_OUT_PORT.println(" m");

	DBG_OUT_PORT.print("Pressure = ");
	DBG_OUT_PORT.print(bmp2.readPressure() / 133.3, 1);
	DBG_OUT_PORT.println(" mm.rt.st.");
#endif //_debug 
	return bmp2.readPressure() / 133.3;
}

float SNR::bme280_read_t()
{
#ifdef _debug
	DBG_OUT_PORT.print("Temperature = ");
	DBG_OUT_PORT.print(bme.readTemperature());
	DBG_OUT_PORT.println(" *C");
#endif //_debug 
	return bme.readTemperature();
}

float SNR::bme280_read_h()
{
#ifdef _debug
	DBG_OUT_PORT.print("Humidity = ");
	DBG_OUT_PORT.print(bme.readHumidity());
	DBG_OUT_PORT.println(" %");
#endif //_debug 
	return bme.readHumidity();
}

float SNR::bme280_read_p()
{
#ifdef _debug
	DBG_OUT_PORT.print("Pressure = ");

	DBG_OUT_PORT.print(bme.readPressure() / 100.0F);
	DBG_OUT_PORT.println(" hPa");

	DBG_OUT_PORT.print("Approx. Altitude = ");
	DBG_OUT_PORT.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
	DBG_OUT_PORT.println(" m");
#endif //_debug 
	return bme.readPressure() / 133.3;
}

void SNR::dht_preset(uint8_t pin, uint8_t type) // установка пинов
{
#if defined(ESP8266)
	dht = new DHT_Unified(pin, type);
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
	dht = new DHT(pin, type);
#endif
}

void SNR::dht_init()
{

	dht->begin();

#if defined(ESP8266)
	sensor_t sensor;
#ifdef _debug
	dht->temperature().getSensor(&sensor);
	DBG_OUT_PORT.println("------------------------------------");
	DBG_OUT_PORT.println("Temperature");
	DBG_OUT_PORT.print("Sensor:       "); DBG_OUT_PORT.println(sensor.name);
	DBG_OUT_PORT.print("Driver Ver:   "); DBG_OUT_PORT.println(sensor.version);
	DBG_OUT_PORT.print("Unique ID:    "); DBG_OUT_PORT.println(sensor.sensor_id);
	DBG_OUT_PORT.print("Max Value:    "); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(" *C");
	DBG_OUT_PORT.print("Min Value:    "); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(" *C");
	DBG_OUT_PORT.print("Resolution:   "); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(" *C");
	DBG_OUT_PORT.println("------------------------------------");
	// Print humidity sensor details.
	dht->humidity().getSensor(&sensor);
	DBG_OUT_PORT.println("------------------------------------");
	DBG_OUT_PORT.println("Humidity");
	DBG_OUT_PORT.print("Sensor:       "); DBG_OUT_PORT.println(sensor.name);
	DBG_OUT_PORT.print("Driver Ver:   "); DBG_OUT_PORT.println(sensor.version);
	DBG_OUT_PORT.print("Unique ID:    "); DBG_OUT_PORT.println(sensor.sensor_id);
	DBG_OUT_PORT.print("Max Value:    "); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println("%");
	DBG_OUT_PORT.print("Min Value:    "); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println("%");
	DBG_OUT_PORT.print("Resolution:   "); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println("%");
	DBG_OUT_PORT.println("------------------------------------");
#endif
	// Set delay between sensor readings based on sensor details.
	delayMS = sensor.min_delay / 1000;
#endif


#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
	delayMS = 2000;
#endif
	//DBG_OUT_PORT.print  ("Delay:   "); DBG_OUT_PORT.print(delayMS); DBG_OUT_PORT.println(" ms");
}

float SNR::dht_read_t()
{
	delay(delayMS);

	float ret = 99.0;

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
	ret = dht->readTemperature();
#endif

#if defined(ESP8266)
	sensors_event_t event;
	dht->temperature().getEvent(&event);

	if (isnan(event.temperature))
	{
		DBG_OUT_PORT.println("Error reading dht temperature!");
		ret = 99;
	}
	else
	{
# ifdef _debug
		DBG_OUT_PORT.print("DHT Temperature: ");
		DBG_OUT_PORT.print(event.temperature);
		DBG_OUT_PORT.println(" *C");
# endif //_debug 

		ret = event.temperature;
	}
#endif

	return ret;
}

float SNR::dht_read_h()
{
	delay(delayMS);

	float ret = -1.0;

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
	ret = dht->readHumidity();
#endif

#if defined(ESP8266)
	sensors_event_t event;

	// Get humidity event and print its value.
	dht->humidity().getEvent(&event);

	if (isnan(event.relative_humidity))
	{
		DBG_OUT_PORT.println("Error reading dht humidity!");
		ret = -1;
	}
	else
	{
# ifdef _debug
		DBG_OUT_PORT.print("DHT Humidity: ");
		DBG_OUT_PORT.print(event.relative_humidity);
		DBG_OUT_PORT.println("%");
# endif //_debug 
		ret = event.relative_humidity;
	}
#endif

	return ret;
}

snr_data_t SNR::read_snr(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t type_press, uint8_t rtc_temp, snr_data_t t_data, snr_data_t e_data, wf_data_t w_data)
{
	snr_data_t data;

	float h = 0, t = 99, p = 700;

	data.h1 = 0; data.h2 = 0; data.h3 = 0; data.t1 = 99; data.t2 = 99; data.t3 = 99; data.p = 700;

	//------------------------------------------- channel 1
	DBG_OUT_PORT.print("Snr type on ch1..");
	DBG_OUT_PORT.println(channel1);

	switch (channel1)
	{
	case 1:
		h = t_data.h1;
		t = t_data.t1;
		break;
	case 2:
		h = e_data.h1;
		t = e_data.t1;
		break;
	case 3:
		h = e_data.h1;
		t = e_data.t1;
		break;
	case 4:
		t = dht_read_t();
		h = dht_read_h();
		break;
	case 5:
		t = rtc_temp;
		h = 0;
		break;
	case 6:
		t = sht_read_t();
		h = sht_read_h();
		break;
	case 7:
		t = am_read_t();
		h = am_read_h();
		break;
	case 8:
		t = bmp180_read_t();
		h = 0;
		break;
	case 9:
		t = bmp280_read_t();
		h = 0;
		break;
	case 10:
		t = bme280_read_t();
		h = bme280_read_h();
		break;
	case 11:
		t = w_data.temp_min;
		h = w_data.hum_min;
		break;
	}

	data.t1 = constrain(t, -99, 99);
	data.h1 = constrain(h, 0, 99);

	DBG_OUT_PORT.print("TC1..");
	DBG_OUT_PORT.println(data.t1);
	DBG_OUT_PORT.print("HC1..");
	DBG_OUT_PORT.println(data.h1);


	//------------------------------------------- channel 2
	DBG_OUT_PORT.print("Snr type on ch2..");
	DBG_OUT_PORT.println(channel2);

	t = 99;
	h = 0;

	switch (channel2)
	{
	case 1:
		t = t_data.t2;
		h = t_data.h2;
		break;
	case 2:
		t = e_data.t2;
		h = e_data.h2;
		break;
	case 3:
		t = e_data.t2;
		h = e_data.h2;
		break;
	case 4:
		t = dht_read_t();
		h = dht_read_h();
		break;
	case 5:
		t = rtc_temp;
		h = 0;
		break;
	case 6:
		t = sht_read_t();
		h = sht_read_h();
		break;
	case 7:
		t = am_read_t();
		h = am_read_h();
		break;
	case 8:
		t = bmp180_read_t();
		h = 0;
		break;
	case 9:
		t = bmp280_read_t();
		h = 0;
		break;
	case 10:
		t = bme280_read_t();
		h = bme280_read_h();
		break;
	case 11:
		t = w_data.temp_min;
		h = w_data.hum_min;
		break;
	}

	data.t2 = constrain(t, -99, 99);
	data.h2 = constrain(h, 0, 99);

	DBG_OUT_PORT.print("TC2..");
	DBG_OUT_PORT.println(data.t2);
	DBG_OUT_PORT.print("HC2..");
	DBG_OUT_PORT.println(data.h2);

	//------------------------------------------- channel 3
	DBG_OUT_PORT.print("Snr type on ch3..");
	DBG_OUT_PORT.println(channel3);

	t = 99;
	h = 0;

	switch (channel3)
	{
	case 1:
		t = t_data.t3;
		h = t_data.h3;
		break;
	case 2:
		t = e_data.t3;
		h = e_data.h3;
		break;
	case 3:
		t = e_data.t3;
		h = e_data.h3;
		break;
	case 4:
		t = dht_read_t();
		h = dht_read_h();
		break;
	case 5:
		t = rtc_temp;
		h = 0;
		break;
	case 6:
		t = sht_read_t();
		h = sht_read_h();
		break;
	case 7:
		t = am_read_t();
		h = am_read_h();
		break;
	case 8:
		t = bmp180_read_t();
		h = 0;
		break;
	case 9:
		t = bmp280_read_t();
		h = 0;
		break;
	case 10:
		t = bme280_read_t();
		h = bme280_read_h();
		break;
	case 11:
		t = w_data.temp_min;
		h = w_data.hum_min;
		break;
	}

	data.t3 = constrain(t, -99, 99);
	data.h3 = constrain(h, 0, 99);

	DBG_OUT_PORT.print("TC3..");
	DBG_OUT_PORT.println(data.t3);
	DBG_OUT_PORT.print("HC3..");
	DBG_OUT_PORT.println(data.h3);

	//-------------------------------------------pressure sensor
	DBG_OUT_PORT.print("Type press snr..");
	DBG_OUT_PORT.println(type_press);

	switch (type_press)
	{
	case 1:
		p = t_data.p;
		break;
	case 2:
		p = e_data.p;
		break;
	case 3:
		p = e_data.p;
		break;
	case 8:
		p = bmp180_read_p();
		break;
	case 9:
		p = bmp280_read_p();
		break;
	case 10:
		p = bme280_read_p();
		break;
	case 11:
		p = w_data.press_min;
		break;
	}
	data.p = constrain(p, 700, 800);

	DBG_OUT_PORT.print("Press..");
	DBG_OUT_PORT.println(data.p);

	return data;
}
