/////////////////////////////////////////////////////////////////////////////////////////
//#include ".\headers\conf.h"
#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
#ifdef DEBUG_UDP
  DBG_OUT_PORT.begin(4023, IPAddress(192, 168, 111, 132));

#else
  DBG_OUT_PORT.begin(115200);
#if defined(__xtensa__)
  DBG_OUT_PORT.setDebugOutput(true);
#endif
#endif


#if defined(ESP8266)
  //------------------------------------------------------  Get system information
  hwi.info();
#endif

  //------------------------------------------------------  Инициализируем встроенную файловую систему SPIFFS

# if defined(__xtensa__)
  fs_setup();
  DBG_OUT_PORT.println(F("file system started"));
#endif

  //------------------------------------------------------  Читаем установки из EEPROM

  //conf_data = defaultConfig();
  conf_data = loadConfig(conf_f);

  DBG_OUT_PORT.println(F("config loaded"));

  //------------------------------------------------------  Запускаем I2C и проверяем наличие клиентов

# if defined(__xtensa__)
  Wire.begin(SDA_PIN, SCL_PIN);
# endif

# if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A)  || defined(BOARD_RTL8711AM) || defined(__AVR_ATmega2560__)
  Wire.begin();
# endif

  ram_data = fsys.i2c_scan(conf_data);
  //------------------------------------------------------  Инициализируем датчики

  if (ram_data.bh1750_present) lightMeter.begin();


  if (ram_data.type_snr1 > 0 || ram_data.type_snr2 > 0 || ram_data.type_snr3 > 0)
  {
    sens.dht_preset(DHT_PIN, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)

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
  pinMode(setting_pin, INPUT_PULLUP);
  if (!ram_data.bh1750_present) pinMode(ANA_SNR, INPUT);
  pinMode(LED_PIN, OUTPUT);     // Initialize the LED_PIN pin as an output
  if (conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(LED_PIN, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

  pinMode(BUZ_PIN, OUTPUT);

# if defined(ARDUINO_ARCH_ESP32)
  pinMode(BUZ2_PIN, OUTPUT);
  ledcAttachPin(BUZ2_PIN, BUZ_PIN);
  ledcSetup(BUZ2_PIN, 2000, 8); // 2 kHz PWM, 8-bit resolution
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
      pinMode(uart_pin, INPUT_PULLUP);
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
  GetSnr();

  //-------------------------------------------------------- Гасим светодиод
  if (conf_data.type_thermo == 0 && ram_data.type_vdrv != 5)   digitalWrite(LED_PIN, conf_data.led_pola ? LOW : HIGH);

  //-------------------------------------------------------- Устанавливаем будильники
  set_alarm();

  //-------------------------------------------------------- Регулируем яркость дисплея
  if (conf_data.auto_br)
  {
    snr_data.f = ft_read(ram_data.bh1750_present);
    cur_br = auto_br(snr_data.f);
  }
  else
  {
    cur_br = conf_data.man_br;  // Man brigthness
    snr_data.f = cur_br;
  }
  DBG_OUT_PORT.print(F("brightness from sensor..."));
  DBG_OUT_PORT.println(snr_data.f);

  //------------------------------------------------------ Отправляем данные через UART
  if (conf_data.type_disp == 50)
  {
    DBG_OUT_PORT.end();
    DBG_OUT_PORT.begin(19200);
    send_uart();
  }


  //------------------------------------------------------ Радостно пищим по окончаниии подготовки к запуску
  rtc_data.a_muz = 15;
  play_snd = true;

  DBG_OUT_PORT.println(F("End of setup"));
}

void loop()
{

  //------------------------------------------------------ Распределяем системные ресурсы
  irq_set();

  // ----------------------------------------------------- Обрабатываем клавиатуру
  keyb_read();

  // ----------------------------------------------------- Доп для будильника
  char b1[440];
  printFromPGM(&songs[rtc_data.a_muz], b1);

  Buzz.play(b1, BUZ_PIN, play_snd, conf_data.snd_pola);   //inital sound card


  //------------------------------------------------------  Верифицируем ночной режим
  if (conf_data.nm_start <  conf_data.nm_stop) nm_is_on = (hour() >= conf_data.nm_start && hour() < conf_data.nm_stop);
  else nm_is_on = (hour() >= conf_data.nm_start || hour() < conf_data.nm_stop);
}
