snr_ch_t snr_init(snr_ch_t snr_ch)
{

  for (uint8_t i = 0; i < 3; i++)
  {
    switch (snr_ch.ch[i])
    {
      case 3:
        dht_init();
        break;
      case 5:
        if (!sht.begin())
        {
          Serial.println("Couldn't find SHT21 sensor, check wiring!");
          snr_ch.ch[i] = 0;
        }
        break;
      case 8:
        if (!bmp2.begin(0x76))
        {
          Serial.println("Couldn't find BMP280 sensor, check wiring!");
          snr_ch.ch[i] = 0;
        }
        break;
      case 9:
        if (!bme.begin(0x76))
        {
          Serial.println("Couldn't find BME280 sensor, check wiring!");
          snr_ch.ch[i] = 0;
        }
        break;
    }
  }
  snr_ch_t snr_ch_o = snr_ch;
  return snr_ch_o;
}


snr_para_t sht_read()
{
  snr_para_t sp;

#ifdef _debug
  Serial.print("SHT Temperature: ");
  Serial.print(sht.readTemperature());
  Serial.println(" *C");
  Serial.print("SHT Humidity: ");
  Serial.println(sht.readHumidity());
  Serial.println("%");
# endif //_debug 

  sp.t = sht.readTemperature();
  sp.h = sht.readHumidity();

  return sp;
}

snr_para_t bmp280_read()
{
  snr_para_t sp;
#ifdef _debug
  Serial.print("Temperature = ");
  Serial.print(bmp2.readTemperature());
  Serial.println(" *C");
#endif //_debug 
  sp.t = bmp2.readTemperature();

#ifdef _debug
  Serial.print("Pressure = ");
  Serial.print(bmp2.readPressure());
  Serial.println(" Pa");

  Serial.print("Approx altitude = ");
  Serial.print(bmp2.readAltitude(SEALEVELPRESSURE_HPA)); // this should be adjusted to your local forcase
  Serial.println(" m");

  Serial.print("Pressure = ");
  Serial.print(bmp2.readPressure() / 133.3, 1);
  Serial.println(" mm.rt.st.");
#endif //_debug 
  sp.p = bmp2.readPressure() / 133.3;

  return sp;
}

snr_para_t bme280_read()
{
  snr_para_t sp;
#ifdef _debug
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
#endif //_debug 
  sp.t = bme.readTemperature();

#ifdef _debug
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
#endif //_debug 
  sp.h = bme.readHumidity();

#ifdef _debug
  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
#endif //_debug 
  sp.p = bme.readPressure() / 133.3;

  return sp;
}

void dht_init()
{
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

snr_para_t dht_read()
{
  snr_para_t sp;
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    sp.t = event.temperature;
#ifdef _debug
    Serial.print("Temperature: ");
    Serial.print(sp.t);
    Serial.println(" *C");
#endif
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    sp.h = event.relative_humidity + 21.0;
#ifdef _debug
    Serial.print("Humidity: ");
    Serial.print(sp.h);
    Serial.println("%");
#endif
  }

  return sp;
}

void read_snr(snr_ch_t snr_ch)
{
  snr_para_t sp;

  for (uint8_t i = 0; i < 3; i++)
  {
    Serial.print("Ch#");
    Serial.print(i);
    Serial.print(" sensor..");
    Serial.println(snr_ch.ch[i]);
    switch (snr_ch.ch[i])
    {
      case 0:
        Serial.println(" no sensor! ");
        sp.t = -99;
        sp.h = 0;
        break;
      case 3:
        sp = dht_read();
        break;
      case 5:
        sp = sht_read();
        break;
      case 8:
        sp = bmp280_read();
        break;
      case 9:
        sp = bme280_read();
        break;
    }

    st[i] = constrain(sp.t, -99, 99);
    sh[i] = constrain(sp.h,   0, 99);

    Serial.print("Ch#");
    Serial.print(i);
    Serial.print(" temperature..");
    Serial.println(st[i]);
    Serial.print("Ch#");
    Serial.print(i);
    Serial.print(" humidity..");
    Serial.println(sh[i]);
  }

  p1 = constrain(sp.p, 700, 800);

  Serial.print("Pressure..");
  Serial.println(p1);
}
