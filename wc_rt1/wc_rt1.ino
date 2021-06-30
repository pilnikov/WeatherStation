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

# if defined(__xtensa__)
  fs_setup();
  DBG_OUT_PORT.println(F("file system started"));
#endif

  //------------------------------------------------------  Читаем установки из EEPROM

#if defined(__AVR_ATmega2560__)
  conf_data = defaultConfig();
#endif

# if defined(__xtensa__)
  conf_data = loadConfig(conf_f);

  //conf_data = defaultConfig();
  //saveConfig(conf_f, conf_data);
#endif

  DBG_OUT_PORT.println(F("config loaded"));

  //------------------------------------------------------  Запускаем I2C и проверяем наличие клиентов

# if defined(__xtensa__)
  Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
# endif

# if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM) || defined(__AVR_ATmega2560__)
  Wire.begin();
# endif

  ram_data = fsys.i2c_scan(conf_data);
  //------------------------------------------------------  Инициализируем датчики

  if (ram_data.bh1750_present) lightMeter.begin();


  if (ram_data.type_snr1 > 0 || ram_data.type_snr2 > 0 || ram_data.type_snr3 > 0)
  {
    sens.dht_preset(conf_data.gpio_dht, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)

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
  pinMode(conf_data.gpio_btn, INPUT_PULLUP);
  if (!ram_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);
  pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
  if (conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

  pinMode(conf_data.gpio_snd, OUTPUT);

# if defined(ARDUINO_ARCH_ESP32)
  pinMode(conf_data.gpio_bz2, OUTPUT);
  ledcAttachPin(conf_data.gpio_bz2, conf_data.gpio_snd);
  ledcSetup(conf_data.gpio_bz2, 2000, 8); // 2 kHz PWM, 8-bit resolution
# endif

  DBG_OUT_PORT.println(F("GPIO inital"));

  //------------------------------------------------------  Инициализируем RTC
  if (ram_data.type_rtc > 0) rtc_init();
  DBG_OUT_PORT.print(F("Type of rtc = "));
  DBG_OUT_PORT.println(ram_data.type_rtc);

  //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
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
      m7219_init();
      break;
    case 3:
      a595_init();
      break;
    case 4:
      ht1621_init();
      break;
    case 5:
      ht1632_init();
      break;
    case 6:
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

  //-------------------------------------------------------- Запускаем сетевые сервисы
# if defined(__xtensa__)
  start_wifi();

  //------------------------------------------------------  Переопределяем консоль
  if (conf_data.udp_mon)
  {
    DBG_OUT_PORT.end();

//    DBG_OUT_PORT = *udp_cons;
    IP_Addr.fromString(conf_data.srudp_addr);
//    DBG_OUT_PORT.begin(4023, IP_Addr);
    //DBG_OUT_PORT.setDebugOutput(true);
  }


  if (web_cli || web_ap)
  {
    //------------------------------------------------------ Синхронизируем время с нтп если нету RTC
    if (ram_data.type_rtc == 0 && web_cli)
    {
      GetNtp();
    }

    //------------------------------------------------------ Получаем прогноз погоды от GisMeteo
    if (conf_data.use_pp == 1 && web_cli)
    {
      wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));
    }


    //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
    if (conf_data.use_pp == 2 && web_cli)
    {
      wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);
    }

    //------------------------------------------------------ Подключаем OTA, MDNS
    nsys.OTA_init(conf_data.ap_ssid, conf_data.ap_pass);

    MDNS.begin(conf_data.ap_ssid);
    DBG_OUT_PORT.printf("Open http://%s", conf_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

    //------------------------------------------------------ Запускаем сервер и SSDP
    web_setup();
    start_serv();
    nsys.ssdp_init();
  }
# endif

  //-------------------------------------------------------  Опрашиваем датчики
  snr_data = GetSnr(ram_data, conf_data);

  //-------------------------------------------------------- Гасим светодиод
  if (conf_data.type_thermo == 0 && ram_data.type_vdrv != 5)   digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);

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
}

void loop()
{
  // ----------------------------------------------------- Проигрываем звуки
#if defined(__xtensa__)
  //inital sound card
  Buzz.play(pgm_read_ptr(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#elif defined (__AVR__)
  Buzz.play(pgm_read_word(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);   //inital sound card
#endif
  play_snd = false;

  //------------------------------------------------------ Распределяем системные ресурсы
  irq_set();

  // ----------------------------------------------------- Обрабатываем клавиатуру
  keyb_read();

  //------------------------------------------------------  Верифицируем ночной режим
  if (conf_data.nm_start <  conf_data.nm_stop) nm_is_on = (hour() >= conf_data.nm_start && hour() < conf_data.nm_stop);
  else nm_is_on = (hour() >= conf_data.nm_start || hour() < conf_data.nm_stop);
}
