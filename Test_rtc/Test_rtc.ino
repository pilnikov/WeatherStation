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
  wifi_data_cur = wifi.begin(wifi_data);
  myIP = wifi_data_cur.addr;
  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
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

  //------------------------------------------------------  Загружаем настройки
  conf_f = "/conf_rtc.json";
  rtc_cfg = myrtccfg.loadConfig(conf_f);

  //rtc_cfg = myrtccfg.defaultConfig();
  //myrtccfg.saveConfig(conf_f, rtc_cfg);
  DBG_OUT_PORT.println(F("config loaded"));

  //------------------------------------------------------  GPIO
#if defined(ESP8266)
  rtc_hw.gpio_dio = 13;
  rtc_hw.gpio_clk = 14;
  rtc_hw.gpio_dcs = 16;
  rtc_hw.gpio_sqw = 12;
  gpio_sda = 4;
  gpio_scl = 5;
  gpio_snd = 15;

#elif CONFIG_IDF_TARGET_ESP32
  rtc_hw.gpio_dio = 19;
  rtc_hw.gpio_clk = 18;
  rtc_hw.gpio_dcs = 5;
  rtc_hw.gpio_sqw = 34;
  gpio_sda = 23;
  gpio_scl = 22;
  gpio_snd = 2;
#endif
  //------------------------------------------------------  ТИП МС RTC
  rtc_hw.a_type = rtc_cfg.c_type;

  //------------------------------------------------------  Начинаем инициализацию Hardware
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  Wire.setPins(gpio_sda, gpio_scl);
# endif
  if (rtc_hw.a_type > 0)
  {
# if defined(ESP8266)
    Wire.begin(gpio_sda, gpio_scl);
# else
    Wire.begin();
# endif
  }
  
  //ram_data = fsys.i2c_scan(rtc_cfg);
  //------------------------------------------------------  Инициализируем RTC
  if (rtc_hw.a_type > 0) myrtc.rtc_init(rtc_hw);
  DBG_OUT_PORT.print(F("Type of rtc = "));
  DBG_OUT_PORT.println(rtc_hw.a_type);

  //------------------------------------------------------ Радостно пищим по окончаниии подготовки к запуску
  rtc_alm.muz = 15;
  play_snd = true;
  DBG_OUT_PORT.println(F("End of setup"));

}

void loop()
{
  // ----------------------------------------------------- Проигрываем звуки
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  Buzz.play(pgm_read_ptr(&songs[rtc_alm.muz ]), gpio_snd, play_snd, false);
#elif defined (__AVR__)
  Buzz.play(pgm_read_word(&songs[rtc_alm.muz]), gpio_snd, play_snd, false);
#endif
  play_snd = false;


  //-------------Refresh current time in rtc_cfg------------------
  myrtc.GetTime(rtc_hw, &rtc_time);

  myrtc.cur_time_str(rtc_time, true, tstr);

  rtc_alm = myrtc.set_alarm(rtc_hw, rtc_cfg, rtc_time);

  //------------------------------------------------------  Верифицируем ночной режим
  if (rtc_cfg.nm_start <  rtc_cfg.nm_stop) rtc_time.nm_is_on = (rtc_time.hour >= rtc_cfg.nm_start && rtc_time.hour < rtc_cfg.nm_stop);
  else rtc_time.nm_is_on = (rtc_time.hour >= rtc_cfg.nm_start || rtc_time.hour < rtc_cfg.nm_stop);
  server.handleClient();
  ArduinoOTA.handle();
}
