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

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки из EEPROM
  conf_f = "/conf_wifi.json";
  wifi_data = wifi.loadConfig(conf_f);

  //wifi_data = defaultConfig();
  //wifi.saveConfig(conf_f, wifi_data);
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);
  myIP = wifi_data_cur.addr;
  if (wifi_data_cur.cli || wifi_data_cur.ap)
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

  //------------------------------------------------------  Загружаем настройки RTC
  conf_f = "/conf_rtc.json";
  rtc_cfg = myrtccfg.loadConfig(conf_f);

  //rtc_cfg = myrtccfg.defaultConfig();
  //myrtccfg.saveConfig(conf_f, rtc_cfg);
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем настройки датчиков
  conf_f = "/conf_snr.json";
  snr_cfg_data = mysnrcfg.loadCfgSnr(conf_f);

  //snr_cfg_data = mysnrcfg.defaultCfgSnr();
  //mysnrcfg.saveCfg(conf_f, snr_cfg_data);
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем общие настройки HW
  conf_f = "/config.json";
  conf_data = loadConfig(conf_f);

  //conf_data = defaultConfig();
  //saveConfig(conf_f, conf_data);
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Инициализируем кнопку
  pinMode(conf_data.gpio_btn, INPUT_PULLUP);
  attachInterrupt(conf_data.gpio_btn, isr1, CHANGE);
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
    
    hw_data = hw_chk.hw_present();
    hw_accept(hw_data);
    //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
    memset(st1, 0, 254);
    memset(screen, 0, 64);

    switch (type_vdrv)
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
    DBG_OUT_PORT.println(type_vdrv);

    //------------------------------------------------------  Инициализируем датчики
    if (hw_data.bh1750_present) lightMeter.begin();

    snr_cur_data.bm_addr = hw_data.bm_addr;
    snr_cur_data.gpio_dht = conf_data.gpio_dht;

    if (snr_cur_data.type_snr1 > 0 || snr_cur_data.type_snr2 > 0 || snr_cur_data.type_snr3 > 0)
    {
      if (snr_cur_data.type_snr1 == 4 || snr_cur_data.type_snr2 == 4 || snr_cur_data.type_snr3 == 4)
      {
        sens.dht_preset(snr_cur_data.gpio_dht, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)
      }

      snr_cur_data = sens.init(snr_cfg_data);

      DBG_OUT_PORT.print(F("Snr type on channel 1 = "));
      DBG_OUT_PORT.println(snr_cur_data.type_snr1);
      DBG_OUT_PORT.print(F("Snr type on channel 2 = "));
      DBG_OUT_PORT.println(snr_cur_data.type_snr2);
      DBG_OUT_PORT.print(F("Snr type on channel 3 = "));
      DBG_OUT_PORT.println(snr_cur_data.type_snr3);
      DBG_OUT_PORT.print("Snr type on pressure = ");
      DBG_OUT_PORT.println(snr_cur_data.type_snrp);
      DBG_OUT_PORT.println(F("sensor inital"));
    }

    //------------------------------------------------------  Инициализируем GPIO

    if (!hw_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);
    if ((conf_data.type_thermo == 0) & (type_vdrv != 5)) pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
    if ((conf_data.type_thermo == 0) & (type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

    pinMode(conf_data.gpio_snd, OUTPUT);
    DBG_OUT_PORT.println(F("GPIO inital"));

    //------------------------------------------------------  GPIO for RTC
    rtc_hw.gpio_dio = conf_data.gpio_dio;
    rtc_hw.gpio_clk = conf_data.gpio_clk;
    rtc_hw.gpio_dcs = conf_data.gpio_dcs;
    rtc_hw.gpio_sqw = conf_data.gpio_sqw;

    // set the interupt pin to input mode
    pinMode(rtc_hw.gpio_sqw, INPUT);
    // setup external interupt
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    attachInterrupt(rtc_hw.gpio_sqw, isr0, FALLING);
#else
    attachInterrupt(4, isr0, FALLING);
#endif

    //------------------------------------------------------  ТИП МС RTC

    //hw_data = fsys.i2c_scan(rtc_cfg);
    //------------------------------------------------------  Инициализируем RTC
    if (rtc_hw.a_type > 0) myrtc.rtc_init(rtc_hw);
    DBG_OUT_PORT.print(F("Type of rtc = "));
    DBG_OUT_PORT.println(rtc_hw.a_type);


    //-------------------------------------------------------- Устанавливаем будильники
    myrtc.GetTime(rtc_hw, &rtc_time);
    rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);

    //-------------------------------------------------------- Запускаем дополнительные сетевые сервисы
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data_cur.cli || wifi_data_cur.ap)
    {
      //------------------------------------------------------ Синхронизируем время с NTP
      if (wifi_data_cur.cli & rtc_cfg.auto_corr)
      {
        RtcDateTime c_time = myrtc.GetNtp(rtc_cfg, rtc_time);
        rtc_time.ct = myrtc.man_set_time(rtc_hw, c_time);
      }
      //------------------------------------------------------ Получаем прогноз погоды от GisMeteo
      if ((conf_data.use_pp == 1) & wifi_data_cur.cli) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));

      //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
      if ((conf_data.use_pp == 2) & wifi_data_cur.cli) wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);

      //------------------------------------------------------ Запускаем SSDP
      nsys.ssdp_init();

      //------------------------------------------------------ Получаем новости от NewsApi
      if (conf_data.news_en & wifi_data_cur.cli)
      {
        newsClient.updateNewsClient(conf_data.news_api_key, conf_data.news_source);
        newsClient.updateNews();
      }
    }
# endif
    //-------------------------------------------------------  Опрашиваем датчики
    snr_data = GetSnr(snr_cur_data, conf_data, rtc_hw.a_type);

    //-------------------------------------------------------- Гасим светодиод
    if ((conf_data.type_thermo == 0) & (type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);


    //-------------------------------------------------------- Регулируем яркость дисплея
    if (conf_data.auto_br)
    {
      snr_data.f = f_dsp.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
      cur_br = f_dsp.auto_br(snr_data.f, conf_data);
    }
    else
    {
      if (rtc_time.nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
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
    rtc_alm.muz = 15;
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
    Buzz.play(pgm_read_ptr(&songs[rtc_alm.muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#elif defined (__AVR__)
    Buzz.play(pgm_read_word(&songs[rtc_alm.muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#endif
    play_snd = false;

    //------------------------------------------------------ Распределяем системные ресурсы
    irq_set();

    // ----------------------------------------------------- Обрабатываем клавиатуру
    keyb_read();

    //------------------------------------------------------  Верифицируем ночной режим
    if (rtc_cfg.nm_start <  rtc_cfg.nm_stop) rtc_time.nm_is_on = (rtc_time.hour >= rtc_cfg.nm_start && rtc_time.hour < rtc_cfg.nm_stop);
    else rtc_time.nm_is_on = (rtc_time.hour >= rtc_cfg.nm_start || rtc_time.hour < rtc_cfg.nm_stop);
  }
  else //-------------------------------------------------- Minimal boot
  {
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data_cur.cli || wifi_data_cur.ap)
    {
      server.handleClient();
      ArduinoOTA.handle();
    }
# endif
  }
}
