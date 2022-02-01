/////////////////////////////////////////////////////////////////////////////////////////
//#include ".\headers\conf.h"
#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);


#if defined(ESP8266)
  //------------------------------------------------------  Get system information
  hwi.info();
#endif

  //------------------------------------------------------  Инициализируем встроенную файловую систему LittleFS
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  fs_setup();
  DBG_OUT_PORT.println(F("file system started"));
#endif

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки из EEPROM
  conf_f = "/conf_wifi.json";
  wifi_data = wifi.loadConfig(conf_f);

  //wifi_data = defaultConfig();
  //wifi.saveConfig(conf_f, wifi_data);
  DBG_OUT_PORT.println(F("WiFi config loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data = wifi.begin(wifi_data);
  myIP = wifi_data.cur_addr;
  if (wifi_data.cli || wifi_data.ap)
  {
    //------------------------------------------------------  Переопределяем консоль
    if (conf_data.udp_mon)
    {
      //DBG_OUT_PORT.end();

      //    DBG_OUT_PORT = *udp_cons;
      //IP_Addr.fromString(conf_data.srudp_addr);
      //    DBG_OUT_PORT.begin(4023, IP_Addr);
      //DBG_OUT_PORT.setDebugOutput(true);
    }

    //------------------------------------------------------  Запускаем сервер, ОТА, MDNS
    nsys.OTA_init(wifi_data.ap_ssid, wifi_data.ap_pass);

    MDNS.begin(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

    web_setup();
    start_serv();
  }
  strcpy(tstr, "Safe Mode");
#endif


  conf_f = "/config.json";
  conf_data = loadConfig(conf_f);

  //conf_data = defaultConfig();
  //saveConfig(conf_f, conf_data);
  DBG_OUT_PORT.println(F("config loaded"));

  //------------------------------------------------------  Инициализируем кнопку
  pinMode(conf_data.gpio_btn, INPUT_PULLUP);
  boot_mode = digitalRead(conf_data.gpio_btn);
  DBG_OUT_PORT.print(F("boot_mode..."));
  DBG_OUT_PORT.println(boot_mode);

  if (boot_mode == 1)
  {
    //------------------------------------------------------  Начинаем инициализацию Hardware
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
    Wire.setPins(conf_data.gpio_sda, conf_data.gpio_scl);
# endif

# if defined(ESP8266)
    Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
# else
    Wire.begin();
# endif

    ram_data = fsys.i2c_scan(conf_data);

    //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
    memset(st1, 0, 254);
    memset(screen, 0, 64);

    switch (ram_data.type_vdrv)
    {
      case 0:
#if defined(ESP8266)
        pinMode(conf_data.gpio_uar, INPUT_PULLUP);
#endif
        break;
      case 1:
        tm1637_init();
        break;
      case 2:
#if defined(ESP8266)
        SPI.pins(conf_data.gpio_clk, -1, conf_data.gpio_dio, conf_data.gpio_dcs);
        SPI.begin();
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
        SPI.begin(conf_data.gpio_clk, -1, conf_data.gpio_dio, conf_data.gpio_dcs);
#else
        SPI.begin();
#endif
        m7219_init();
        break;
      case 3:
        a595_init();
        break;
      case 4:
        ht1621_init();
        break;
      case 5:
#if defined(ESP8266)
        SPI.pins(conf_data.gpio_clk, -1, conf_data.gpio_dio, conf_data.gpio_dcs);
        SPI.begin();
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
        SPI.begin(conf_data.gpio_clk, -1, conf_data.gpio_dio, conf_data.gpio_dcs);
#else
        SPI.begin();
#endif
        ht1632_init();
        break;
      case 6:
#if defined(ESP8266)
        SPI.pins(conf_data.gpio_clk, -1, conf_data.gpio_dio, conf_data.gpio_dcs);
        SPI.begin();
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
        SPI.begin(conf_data.gpio_clk, -1, conf_data.gpio_dio, conf_data.gpio_dcs);
#else
        SPI.begin();
#endif
        ili9341_init();
        break;
      case 11:
        ht1633_init();
        break;
      case 12:
        pcf8574_init();
        break;
    }
    DBG_OUT_PORT.print(F("Type chip driver of display = "));
    DBG_OUT_PORT.println(ram_data.type_vdrv);

    //------------------------------------------------------  Инициализируем датчики
    if (ram_data.bh1750_present) lightMeter.begin();

    if (ram_data.type_snr1 > 0 || ram_data.type_snr2 > 0 || ram_data.type_snr3 > 0)
    {
      if (ram_data.type_snr1 == 4 || ram_data.type_snr2 == 4 || ram_data.type_snr3 == 4)
      {
        pinMode(conf_data.gpio_dht, INPUT_PULLUP);
        sens.dht_preset(conf_data.gpio_dht, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)
      }

      if (ram_data.type_snr1 == 13 || ram_data.type_snr2 == 13 || ram_data.type_snr3 == 13)
        ram_data.gpio_dht = conf_data.gpio_dht; //Тут устанавливается GPIO для DS18B20

      ram_data_t sens_data = ram_data;

      sens_data = sens.init(ram_data);
      ram_data = sens_data;

      DBG_OUT_PORT.print(F("Snr type on channel 1 = "));
      DBG_OUT_PORT.println(ram_data.type_snr1);
      DBG_OUT_PORT.print(F("Snr type on channel 2 = "));
      DBG_OUT_PORT.println(ram_data.type_snr2);
      DBG_OUT_PORT.print(F("Snr type on channel 3 = "));
      DBG_OUT_PORT.println(ram_data.type_snr3);
      DBG_OUT_PORT.print("Snr type on pressure = ");
      DBG_OUT_PORT.println(ram_data.type_snrp);
      DBG_OUT_PORT.println(F("sensor inital"));
    }

    //------------------------------------------------------  Инициализируем GPIO
    attachInterrupt(conf_data.gpio_btn, isr1, CHANGE);

    if (!ram_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);
    if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
    if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

    pinMode(conf_data.gpio_snd, OUTPUT);


    DBG_OUT_PORT.println(F("GPIO inital"));

    //------------------------------------------------------  Инициализируем RTC
    if (ram_data.type_rtc > 0) rtc_init();
    DBG_OUT_PORT.print(F("Type of rtc = "));
    DBG_OUT_PORT.println(ram_data.type_rtc);

    //-------------------------------------------------------- Запускаем дополнительные сетевые сервисы
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data.cli || wifi_data.ap)
    {
      //------------------------------------------------------ Синхронизируем время с нтп если нету RTC
      if ((ram_data.type_rtc == 0) & wifi_data.cli & conf_data.auto_corr) GetNtp();

      //------------------------------------------------------ Получаем прогноз погоды от GisMeteo
      if ((conf_data.use_pp == 1) & wifi_data.cli) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));

      //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
      if ((conf_data.use_pp == 2) & wifi_data.cli) wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);

      //------------------------------------------------------ Запускаем SSDP
      nsys.ssdp_init();

      //------------------------------------------------------ Получаем новости от NewsApi
      if (conf_data.news_en & wifi_data.cli)
      {
        newsClient.updateNewsClient(conf_data.news_api_key, conf_data.news_source);
        newsClient.updateNews();
      }
    }
# endif
    //-------------------------------------------------------  Опрашиваем датчики
    snr_data = GetSnr(ram_data, conf_data);

    //-------------------------------------------------------- Гасим светодиод
    if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);

    //-------------------------------------------------------- Устанавливаем будильники
    set_alarm();

    //-------------------------------------------------------- Регулируем яркость дисплея
    if (conf_data.auto_br)
    {
      snr_data.f = f_dsp.ft_read(ram_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
      cur_br = f_dsp.auto_br(snr_data.f, conf_data);
    }
    else
    {
      if (nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
      else cur_br = conf_data.man_br;
      snr_data.f = cur_br;
    }
    DBG_OUT_PORT.print(F("brightness from sensor..."));
    DBG_OUT_PORT.println(snr_data.f);

    //------------------------------------------------------ Отправляем данные через UART
    if (conf_data.type_disp == 50)
    {
      if (!conf_data.udp_mon)
      {
        DBG_OUT_PORT.end();
        DBG_OUT_PORT.begin(19200);
        send_uart();
      }
    }

    //------------------------------------------------------ Радостно пищим по окончаниии подготовки к запуску
    rtc_data.a_muz = 15;
    play_snd = true;
    DBG_OUT_PORT.println(F("End of setup"));

    //------------------------------------------------------ Засыпаем
    if (conf_data.esm)
    {
#if defined(ESP8266)
      wifi._shutdown();
#endif
      DBG_OUT_PORT.flush();
#if defined(ESP8266)
      ESP.deepSleep(conf_data.period * 60e6, RF_DISABLED); // deep-sleep. Засыпаем на period минут!
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
      ESP.deepSleep(conf_data.period * 60e6); // deep-sleep. Засыпаем на period минут!
#endif
    }
  }
  else DBG_OUT_PORT.println(F("Safe mode!!! End of setup"));
}

void loop()
{
  if (boot_mode == 1)
  {
    // ----------------------------------------------------- Проигрываем звуки
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    Buzz.play(pgm_read_ptr(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#elif defined (__AVR__)
    Buzz.play(pgm_read_word(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#endif
    play_snd = false;

    //------------------------------------------------------ Распределяем системные ресурсы
    irq_set();

    // ----------------------------------------------------- Обрабатываем клавиатуру
    keyb_read();

    //------------------------------------------------------  Верифицируем ночной режим
    if (conf_data.nm_start <  conf_data.nm_stop) nm_is_on = (rtc_data.hour >= conf_data.nm_start && rtc_data.hour < conf_data.nm_stop);
    else nm_is_on = (rtc_data.hour >= conf_data.nm_start || rtc_data.hour < conf_data.nm_stop);
  }
  else //-------------------------------------------------- Minimal boot
  {
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data.cli || wifi_data.ap)
    {
      server.handleClient();
      ArduinoOTA.handle();
    }
# endif
  }
}
