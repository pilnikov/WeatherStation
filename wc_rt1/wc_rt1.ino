//------------------------------------------------------
//#include ".\headers\conf.h"
#include "constr.h"
#include "cfg.h"
#include "disp.h"
#include "web.h"
#include "conf.h"
#include "global_var.h"

#if defined(ESP8266)
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
#include <ESPmDNS.h>
#include <ESP32SSDP.h>
#endif

char            tstr[25];

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);


#if defined(ESP8266)
  //------------------------------------------------------  Get system information
  hwi.info();
#endif
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  //------------------------------------------------------  Запускаем LittleFileSystem
  lfs.begin();
  DBG_OUT_PORT.println(F("LittleFS started"));

  //------------------------------------------------------  Загружаем конфигурацию

  //------------------------------------------------------  Читаем установки WiFi из конфиг файла
  conf_f = "/conf_wifi.json";
  from_client = lfs.readFile(conf_f);
  wifi_data = wifi_cfg.from_json(from_client);

  //wifi_data = wifi_cfg.def_conf();
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);

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

  //------------------------------------------------------  Читаем установки RTC из конфиг файла
  conf_f = "/conf_rtc.json";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
#endif
  rtc_cfg = myrtccfg.from_json(from_client);

  //rtc_cfg = myrtccfg.def_conf();
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));


  //------------------------------------------------------  Загружаем настройки датчиков
  conf_f = "/conf_snr.json";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
#endif
  snr_cfg_data = mysnrcfg.from_json(from_client);

  //snr_cfg_data = mysnrcfg.def_conf();
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

    rtc_hw.a_type = rtc_cfg.c_type;
    hw_data = hw_chk.hw_present();
    hw_accept(hw_data, &snr_cfg_data, &conf_data.type_vdrv, &rtc_hw.a_type);
    //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
    disp_init(conf_data.type_vdrv, conf_data.type_disp, conf_data.gpio_uar, conf_data.gpio_dio, conf_data.gpio_clk, conf_data.gpio_dcs, conf_data.gpio_dwr, hw_data.ht_addr, hw_data.lcd_addr, screen, text_size, conf_data.rus_lng);
    //------------------------------------------------------  Инициализируем датчики
    if (hw_data.bh1750_present) lightMeter.begin();

    snr_cfg_data.gpio_dht = conf_data.gpio_dht;
    sensor_init(&snr_cfg_data);

    //------------------------------------------------------  Инициализируем GPIO

    if (!hw_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);
    if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
    if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

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

    //------------------------------------------------------  Инициализируем RTC
    unsigned long ttm = RtcDateTime(__DATE__, __TIME__) + 946684800;

    DBG_OUT_PORT.print(F("Type of rtc = "));
    DBG_OUT_PORT.println(rtc_hw.a_type);

    if (rtc_hw.a_type > 0) myrtc.rtc_init(rtc_hw);
    else myrtc.man_set_time(rtc_hw, ttm);

    //-------------------------------------------------------- Устанавливаем будильники
    rtc_time.ct = myrtc.GetTime(rtc_hw);
    myrtc.dt_from_unix(&rtc_time);
    myrtc.cur_time_str(rtc_time, conf_data.rus_lng, tstr);
    DBG_OUT_PORT.println(tstr);
    rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);

    //-------------------------------------------------------- Запускаем дополнительные сетевые сервисы
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data_cur.cli || wifi_data_cur.ap)
    {
      //------------------------------------------------------ Синхронизируем время с NTP
      if (wifi_data_cur.cli & rtc_cfg.auto_corr)
      {
        ttm = myrtc.GetNtp(rtc_cfg, rtc_time.ct);
        myrtc.man_set_time(rtc_hw, ttm);
        rtc_time.ct = myrtc.GetTime(rtc_hw);
        rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
      }
      //------------------------------------------------------ Получаем прогноз погоды
      handleUpdForeCast();
      //------------------------------------------------------ Запускаем SSDP
      nsys.ssdp_init();

      //------------------------------------------------------ Получаем новости от NewsApi
      if (conf_data.news_en & wifi_data_cur.cli)
      {
        newsClient = new NewsApiClient(conf_data.news_api_key, conf_data.news_source);
        handleUpdNews();
      }
    }
# endif
    //-------------------------------------------------------  Опрашиваем датчики
    bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    cli = wifi_data_cur.cli;
#endif

    snr_data_t sb = snr_data;
    snr_data = GetSnr(sb, snr_cfg_data, conf_data, rtc_hw.a_type, cli, wf_data_cur);

    //-------------------------------------------------------- Гасим светодиод
    if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);


    //-------------------------------------------------------- Регулируем яркость дисплея
    if (conf_data.auto_br)
    {
      snr_data.f = ff_dsp.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
      cur_br = ff_dsp.auto_br(snr_data.f, conf_data);
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
        send_uart(snr_data, wf_data, conf_data, rtc_time, rtc_alm, cur_br);
      }
    }
    //------------------------------------------------------ Радостно пищим по окончаниии подготовки к запуску
    rtc_alm.act = 15;
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
    //------------------------------------------------------ Распределяем системные ресурсы
    main_loop();
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
