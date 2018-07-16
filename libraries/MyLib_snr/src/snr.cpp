#include "Snr.h"

ram_data_t SNR::init(ram_data_t in_data)
{
  ram_data_t out_data = in_data;

  switch (in_data.type_int_snr)
  {
    case 3:
      dht_init();
      break;
    case 5:
        if (!sht.begin())
        {
         DBG_OUT_PORT.println("Couldn't find a valid SHT21 sensor, check wiring!");
         out_data.type_int_snr = 0;
        }
      break;
    case 6:
        if (!am23.begin())
        {
         DBG_OUT_PORT.println("Couldn't find a valid AM2320 sensor, check wiring!");
         out_data.type_int_snr = 0;
        }
      break;
    case 7:
        if (!bmp.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
         out_data.type_int_snr = 0;
        }
      break;
    case 8:
        if (!bmp2.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
         out_data.type_int_snr = 0;
        }
      break;
    case 9:
        if (!bme.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
         out_data.type_int_snr = 0;
        }
      break;
  }
  switch (in_data.type_ext_snr)
  {
    case 3:
      dht_init();
      break;
    case 5:
        if (!sht.begin())
        {
         DBG_OUT_PORT.println("Couldn't find a valid SHT21 sensor, check wiring!");
         out_data.type_ext_snr = 0;
        }
      break;
    case 6:
        if (!am23.begin())
        {
         DBG_OUT_PORT.println("Couldn't find a valid AM2320 sensor, check wiring!");
         out_data.type_int_snr = 0;
        }
      break;
    case 7:
        if (!bmp.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
         out_data.type_ext_snr = 0;
        }
      break;
    case 8:
        if (!bmp2.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
         out_data.type_ext_snr = 0;
        }
      break;
    case 9:
        if (!bme.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
         out_data.type_ext_snr = 0;
        }
      break;
  }
  
  switch (in_data.type_prs_snr)
  {
    case 7:
        if (!bmp.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BMP085/180 ... check your connections!");
         out_data.type_prs_snr = 0;
        }
      break;
    case 8:
        if (!bmp2.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BMP280 ... check your connections!");
         out_data.type_prs_snr = 0;
        }
      break;
    case 9:
        if (!bme.begin())
        {
         DBG_OUT_PORT.println("There was a problem detecting the BME280 ... check your connections!");
         out_data.type_prs_snr = 0;
        }
      break;
  }
  
  if (in_data.bh1750_present)
  {
    lightMeter.begin();
  }
  
  delay (50);
  return out_data; 
}

float SNR::am_read_h()
{
  delay(600);
  switch (am23.Read()) {
    case 2:
# ifdef _debug
      DBG_OUT_PORT.println("CRC failed");
# endif
      break;
      return -1;
    case 1:
# ifdef _debug
      DBG_OUT_PORT.println("Sensor offline");
# endif
      return -1;
      break;
    case 0:
      return am23.h;
      break;
  }
}

float SNR::am_read_t()
{
  delay(600);
  switch (am23.Read()) {
    case 2:
# ifdef _debug
      DBG_OUT_PORT.println("CRC failed");
# endif
      return 99;
      break;
    case 1:
# ifdef _debug
      DBG_OUT_PORT.println("Sensor offline");
# endif
      return 99;
      break;
    case 0:
      return am23.t;
      break;
  }
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
  DBG_OUT_PORT.print  ("Sensor:       "); DBG_OUT_PORT.println(sensor.name);
  DBG_OUT_PORT.print  ("Driver Ver:   "); DBG_OUT_PORT.println(sensor.version);
  DBG_OUT_PORT.print  ("Unique ID:    "); DBG_OUT_PORT.println(sensor.sensor_id);
  DBG_OUT_PORT.print  ("Max Value:    "); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(" hPa");
  DBG_OUT_PORT.print  ("Min Value:    "); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(" hPa");
  DBG_OUT_PORT.print  ("Resolution:   "); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(" hPa");
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
  DBG_OUT_PORT.print  ("Sensor:       "); DBG_OUT_PORT.println(sensor.name);
  DBG_OUT_PORT.print  ("Driver Ver:   "); DBG_OUT_PORT.println(sensor.version);
  DBG_OUT_PORT.print  ("Unique ID:    "); DBG_OUT_PORT.println(sensor.sensor_id);
  DBG_OUT_PORT.print  ("Max Value:    "); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println(" *C");
  DBG_OUT_PORT.print  ("Min Value:    "); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println(" *C");
  DBG_OUT_PORT.print  ("Resolution:   "); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println(" *C");
  DBG_OUT_PORT.println("------------------------------------");
  // Print humidity sensor details.
  dht->humidity().getSensor(&sensor);
  DBG_OUT_PORT.println("------------------------------------");
  DBG_OUT_PORT.println("Humidity");
  DBG_OUT_PORT.print  ("Sensor:       "); DBG_OUT_PORT.println(sensor.name);
  DBG_OUT_PORT.print  ("Driver Ver:   "); DBG_OUT_PORT.println(sensor.version);
  DBG_OUT_PORT.print  ("Unique ID:    "); DBG_OUT_PORT.println(sensor.sensor_id);
  DBG_OUT_PORT.print  ("Max Value:    "); DBG_OUT_PORT.print(sensor.max_value); DBG_OUT_PORT.println("%");
  DBG_OUT_PORT.print  ("Min Value:    "); DBG_OUT_PORT.print(sensor.min_value); DBG_OUT_PORT.println("%");
  DBG_OUT_PORT.print  ("Resolution:   "); DBG_OUT_PORT.print(sensor.resolution); DBG_OUT_PORT.println("%");
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

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
 float t = dht->readTemperature();
 return t;
#endif

#if defined(ESP8266)
  sensors_event_t event;
  dht->temperature().getEvent(&event);

  if (isnan(event.temperature))
  {
    DBG_OUT_PORT.println("Error reading dht temperature!");
    return 99;
  }
  else
  {
# ifdef _debug
    DBG_OUT_PORT.print("DHT Temperature: ");
    DBG_OUT_PORT.print(event.temperature);
    DBG_OUT_PORT.println(" *C");
# endif //_debug 

    return event.temperature;
  }
#endif
}

float SNR::dht_read_h()
{
  delay(delayMS);

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
  float h = dht->readHumidity();
  return h;
#endif

#if defined(ESP8266)
  sensors_event_t event;

  // Get humidity event and print its value.
  dht->humidity().getEvent(&event);

  if (isnan(event.relative_humidity))
  {
    DBG_OUT_PORT.println("Error reading dht humidity!");
    return -1;
  }
  else
  {
# ifdef _debug
    DBG_OUT_PORT.print("DHT Humidity: ");
    DBG_OUT_PORT.print(event.relative_humidity);
    DBG_OUT_PORT.println("%");
# endif //_debug 
    return event.relative_humidity;
  }
#endif
}

uint16_t SNR::ft_read(bool d_pres)
{
  uint16_t ft = 7;
  if (d_pres) ft = lightMeter.readLightLevel();
  else 
  {
#if defined(ESP8266) || defined(ESP32)
	  ft = analogRead(A0);
#endif
  }
  return ft;
}

snr_data_t SNR::read_snr(uint8_t type_int, uint8_t type_ext, uint8_t type_press, uint8_t rtc_temp, snr_data_t t_data, snr_data_t e_data, wf_data_t w_data)
{
  snr_data_t data;

  float h = 0, t = 0, p = 700;   

  data.h1 = 0; data.h2 = 0; data.t1 = 99; data.t2 = 99; data.p = 700;

  //-------------------------------------------int sensor
  DBG_OUT_PORT.print("Type internal sensor..");
  DBG_OUT_PORT.println(type_int);

  switch (type_int)
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
      t = dht_read_t();
      h = dht_read_h();
      break;
    case 4:
      t = rtc_temp;
      h = 0;
      break;
    case 5:
      t = sht_read_t();
      h = sht_read_h();
      break;
    case 6:
      t = am_read_t();
      h = am_read_h();
      break;
    case 7:
      t = bmp180_read_t();
      h = 0;
      break;
    case 8:
      t = bmp280_read_t();
      h = 0;
      break;
    case 9:
      t = bme280_read_t();
      h = bme280_read_h();
      break;
    case 10:
      t = w_data.temp_min;
      h = w_data.hum_min;
      break;
  }

  data.t1 = constrain(t, -99, 99);
  data.h1 = constrain(h,   0, 99);

  DBG_OUT_PORT.print("Internal temperature..");
  DBG_OUT_PORT.println(data.t1);
  DBG_OUT_PORT.print("Internal humidity..");
  DBG_OUT_PORT.println(data.h1);


//  data.t1 = t;
//  data.h1 = h;

  //-------------------------------------------ext sensor
  DBG_OUT_PORT.print("Type external sensor..");
  DBG_OUT_PORT.println(type_ext);

  t = 0;
  h = 0;

  switch (type_ext)
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
      t = dht_read_t();
      h = dht_read_h();
      break;
    case 4:
      t = rtc_temp;
      h = 0;
      break;
    case 5:
      t = sht_read_t();
      h = sht_read_h();
      break;
    case 6:
      t = am_read_t();
      h = am_read_h();
      break;
    case 7:
      t = bmp180_read_t();
      h = 0;
      break;
    case 8:
      t = bmp280_read_t();
      h = 0;
      break;
    case 9:
      t = bme280_read_t();
      h = bme280_read_h();
      break;
    case 10:
      t = w_data.temp_min;
      h = w_data.hum_min;
      break;
  }

  data.t2 = constrain(t, -99, 99);
  data.h2 = constrain(h,   0, 99);

  DBG_OUT_PORT.print("External temperature..");
  DBG_OUT_PORT.println(data.t2);
  DBG_OUT_PORT.print("External humidity..");
  DBG_OUT_PORT.println(data.h2);

  //-------------------------------------------pressure sensor
  DBG_OUT_PORT.print("Type pressure sensor..");
  DBG_OUT_PORT.println(type_press);

  switch (type_press)
  {
    case 1:
      p = t_data.p;
      break;
    case 2:
      p = e_data.p;
      break;
    case 7:
      p = bmp180_read_p();
      break;
    case 8:
      p = bmp280_read_p();
      break;
    case 9:
      p = bme280_read_p();
      break;
    case 10:
      p = w_data.press_min;
      break;
  }
  data.p = constrain(p, 700, 800);

  DBG_OUT_PORT.print("Pressure..");
  DBG_OUT_PORT.println(data.p);

  return data;
}