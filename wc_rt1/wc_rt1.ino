//------------------------------------------------------
//#include ".\headers\conf.h"
#include "conf.h"


char tstr[25];

// ---------------------------------------------------- Common
const char *conf_f = "/config.json";  // config file name

String from_client = String();

int boot_mode = 1;

uint32_t serv_ms = 60000;

static bool wasAlarm = false;
static bool play_snd = false;

static uint16_t cur_br = 0;

//---------------------------------------------------------------------------------------------------
static byte screen[64];        // display buffer
static uint8_t text_size = 1;  // размер текста
static uint8_t newsIndex;
String local_ip = "192.168.0.0", ns = String();

//---------------------------------------------------------------------------------------------------
snr_data_t snr_data;
snr_cfg_t snr_cfg_data;
main_cfg_t mcf;
gpio_cfg_t gcf;
hw_data_t hw_data;
wf_data_t wf_data_cur;
wf_data_t wf_data;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;
#endif
rtc_hw_data_t rtc_hw;
rtc_cfg_data_t rtc_cfg;
rtc_time_data_t rtc_time;
rtc_alm_data_t rtc_alm;

// ---------------------------------------------------- Constructors
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
ES e_srv;
NF nsys;
WF wifi;
WFJS wifi_cfg;
LFS lfs;

udp_cons print_console_udp;
#endif

MAINJS main_cfg;
GPIOJS gpio_cfg;

// ----------------------------------- Web server
#if defined(ESP8266)
ESP8266WebServer server(80);
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
WebServer server(80);
#endif

// ---------------------------------------------------- HW info
#if defined(ESP8266)
ESP8266HWInfo hwi;
#endif

// ---------------------------------------------------- LM
BH1750 lightMeter;

// ---------------------------------------------------- Other
FD f_dsp2;  //For Display
MyDsp mydsp;

Synt Buzz;  //Конструктор пищалки

CT myrtc;        //For RTC Common
RTCJS myrtccfg;  //For RTC Config
SF hw_chk;       //For HW Check
SNR sens_f;      //For Sensor Common
SNRJS mysnrcfg;  //For Sensor Config

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
// ---------------------------------------------------- News Client
NewsApiClient *newsClient;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
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
  wifi_data = wifi_cfg.def_conf();

  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    from_client = wifi_cfg.to_json(wifi_data);
    lfs.writeFile(conf_f, from_client.c_str());
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(conf_f);
    DBG_OUT_PORT.println(F(" for reading. Write new with default configuration!!!"));
  }
  wifi_data = wifi_cfg.from_json(from_client);

  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);

  if (wifi_data_cur.cli || wifi_data_cur.ap) {
    //------------------------------------------------------  Переопределяем консоль
    if (mcf.udp_mon) {
      //DBG_OUT_PORT.end();
      //DBG_OUT_PORT = *udp_cons;
      //IP_Addr.fromString(main_cfg.srudp_addr);
      //DBG_OUT_PORT.begin(4023, IP_Addr);
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
  rtc_cfg = myrtccfg.def_conf();

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    from_client = myrtccfg.to_json(rtc_cfg);
    lfs.writeFile(conf_f, from_client.c_str());
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(conf_f);
    DBG_OUT_PORT.println(F(" for reading. Write new with default configuration!!!"));
  }
  rtc_cfg = myrtccfg.from_json(from_client);
#endif

  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем настройки датчиков
  conf_f = "/conf_snr.json";
  snr_cfg_data = mysnrcfg.def_conf();

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    from_client = mysnrcfg.to_json(snr_cfg_data);
    lfs.writeFile(conf_f, from_client.c_str());
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(conf_f);
    DBG_OUT_PORT.println(F(" for reading. Write new with default configuration!!!"));
  }
  snr_cfg_data = mysnrcfg.from_json(from_client);
#endif

  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем общие настройки HW
  conf_f = "/main_cfg.json";
  mcf = main_cfg.def_conf();

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    from_client = main_cfg.to_json(mcf);
    lfs.writeFile(conf_f, from_client.c_str());
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(conf_f);
    DBG_OUT_PORT.println(F(" for reading. Write new with default configuration!!!"));
  }
  mcf = main_cfg.from_json(from_client);
#endif

  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем настройки GPIO
  conf_f = "/gpio_cfg.json";
  gcf = gpio_cfg.def_conf();

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    from_client = gpio_cfg.to_json(gcf);
    lfs.writeFile(conf_f, from_client.c_str());
    DBG_OUT_PORT.print(F("Failed to open "));
    DBG_OUT_PORT.print(conf_f);
    DBG_OUT_PORT.println(F(" for reading. Write new with default configuration!!!"));
  }
  gcf = gpio_cfg.from_json(from_client);
#endif

  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Инициализируем кнопку
  pinMode(gcf.gpio_btn, INPUT_PULLUP);
  attachInterrupt(gcf.gpio_btn, isr1, CHANGE);
  boot_mode = digitalRead(gcf.gpio_btn);

  //boot_mode = 0;

  DBG_OUT_PORT.print(F("boot_mode..."));
  DBG_OUT_PORT.println(boot_mode);

  if (boot_mode == 1) {
    //------------------------------------------------------  Начинаем инициализацию Hardware

    if (gcf.gpio_sda != 255 && gcf.gpio_scl != 255) {
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
      Wire.setPins(gcf.gpio_sda, gcf.gpio_scl);
#endif

#if defined(ESP8266)
      Wire.begin(gcf.gpio_sda, gcf.gpio_scl);
#else
      Wire.begin();
#endif
    }

    rtc_hw.a_type = rtc_cfg.c_type;
    hw_data = hw_chk.hw_present();
    hw_accept(hw_data, &snr_cfg_data, &mcf.vdrv_t, &rtc_hw.a_type);

    //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
    if (mcf.dsp_t > 0) mydsp.disp_init(mcf.vdrv_t, mcf.dsp_t, gcf.gpio_uar, gcf.gpio_dio, gcf.gpio_clk, gcf.gpio_dcs, gcf.gpio_dwr, hw_data.ht_addr, hw_data.lcd_addr, screen, text_size, mcf.rus_lng);

    //------------------------------------------------------  Инициализируем датчики
    if (hw_data.bh1750_present) lightMeter.begin();

    snr_cfg_data.gpio_dht = gcf.gpio_dht;
    sensor_init(&snr_cfg_data);

    //------------------------------------------------------  Инициализируем GPIO
    if (!hw_data.bh1750_present) pinMode(gcf.gpio_ana, INPUT);
    if ((mcf.thermo_t == 0) & (mcf.vdrv_t != 5)) pinMode(gcf.gpio_led, OUTPUT);                          // Initialize the LED_PIN pin as an output
    if ((mcf.thermo_t == 0) & (mcf.vdrv_t != 5)) digitalWrite(gcf.gpio_led, gcf.led_pola ? HIGH : LOW);  //Включаем светодиод

    if (gcf.gpio_snd != 255) pinMode(gcf.gpio_snd, OUTPUT);

    //------------------------------------------------------  GPIO for RTC
    rtc_hw.gpio_dio = gcf.gpio_dio;
    rtc_hw.gpio_clk = gcf.gpio_clk;
    rtc_hw.gpio_dcs = gcf.gpio_dcs;
    rtc_hw.gpio_sqw = gcf.gpio_sqw;
    if (rtc_hw.gpio_sqw != 255) {
      // set the interupt pin to input mode
      pinMode(rtc_hw.gpio_sqw, INPUT);
      // setup external interupt
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
      attachInterrupt(rtc_hw.gpio_sqw, isr0, FALLING);
#else
      attachInterrupt(4, isr0, FALLING);
#endif
    }

    DBG_OUT_PORT.println(F("GPIO inital done !!!"));
    //------------------------------------------------------  Инициализируем RTC
    RtcDateTime compileDateTime(__DATE__, __TIME__);
    uint32_t ttm = compileDateTime.Unix32Time();

    DBG_OUT_PORT.print(F("Type of rtc = "));
    DBG_OUT_PORT.println(rtc_hw.a_type);

    if (rtc_hw.a_type > 0) myrtc.rtc_init(rtc_hw);
    else myrtc.man_set_time(rtc_hw, ttm);

    //-------------------------------------------------------- Устанавливаем будильники
    rtc_time.ct = myrtc.GetTime(rtc_hw);
    myrtc.dt_from_unix(&rtc_time);
    myrtc.cur_time_str(rtc_time, mcf.rus_lng, tstr);
    DBG_OUT_PORT.println(tstr);
    rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);

    //-------------------------------------------------------- Запускаем дополнительные сетевые сервисы
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data_cur.cli || wifi_data_cur.ap) {
      //------------------------------------------------------ Синхронизируем время с NTP
      if (wifi_data_cur.cli & rtc_cfg.auto_corr) {
        ttm = myrtc.GetNtp(rtc_cfg, rtc_time.ct);
        myrtc.man_set_time(rtc_hw, ttm);
        rtc_time.ct = myrtc.GetTime(rtc_hw);
        rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
      }

      //------------------------------------------------------ Получаем прогноз погоды
      if (mcf.use_pp > 0) handleUpdForeCast();

      //------------------------------------------------------ Запускаем SSDP
      nsys.ssdp_init();

      //------------------------------------------------------ Получаем новости от NewsApi
      if (mcf.news_en & wifi_data_cur.cli) {
        newsClient = new NewsApiClient(mcf.news_api_key, mcf.news_source);
        handleUpdNews();
      }
    }
#endif

    //-------------------------------------------------------  Опрашиваем датчики
    bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    cli = wifi_data_cur.cli;
#endif

    snr_data_t sb = snr_data;
    snr_data = GetSnr(sb, snr_cfg_data, mcf, rtc_hw.a_type, cli, wf_data_cur);

    //-------------------------------------------------------- Гасим светодиод
    if ((mcf.thermo_t == 0) & (mcf.vdrv_t != 5)) digitalWrite(gcf.gpio_led, gcf.led_pola ? LOW : HIGH);


    //-------------------------------------------------------- Регулируем яркость дисплея
    if (mcf.auto_br && mcf.dsp_t > 0) {
      snr_data.f = f_dsp2.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
      cur_br = f_dsp2.auto_br(snr_data.f, mcf);
    } else {
      if (rtc_time.nm_is_on) cur_br = mcf.nmd_br;  // Man brigthness
      else cur_br = mcf.man_br;
      snr_data.f = cur_br;
    }
    DBG_OUT_PORT.print(F("brightness from sensor..."));
    DBG_OUT_PORT.println(snr_data.f);

    //------------------------------------------------------ Отправляем данные через UART
    if (mcf.dsp_t == 50) {
      if (!mcf.udp_mon) {
        DBG_OUT_PORT.end();
        DBG_OUT_PORT.begin(19200);
        send_uart(snr_data, wf_data, mcf, rtc_time, rtc_alm, cur_br);
      }
    }

    //------------------------------------------------------ Устанавливаем пищалку
    rtc_alm.act = 15;
    play_snd = true;

    //------------------------------------------------------ Окончание подготовки к запуску
    DBG_OUT_PORT.println(F("End of setup"));

    //------------------------------------------------------ Засыпаем
    if (mcf.esm) {
#if defined(ESP8266)
      wifi._shutdown();
#endif
      DBG_OUT_PORT.flush();
#if defined(ESP8266)
      ESP.deepSleep(mcf.period * 60e6, RF_DISABLED);  // deep-sleep. Засыпаем на period минут!
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
      ESP.deepSleep(mcf.period * 60e6);  // deep-sleep. Засыпаем на period минут!
#endif
    }
  } else DBG_OUT_PORT.println(F("Safe mode!!! End of setup"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  if (boot_mode == 1) {
    //------------------------------------------------------ interrupts
    unsigned long t3 = mcf.period * 2000L;
    const uint8_t irq_q = 9;
    static uint8_t _st = 0;
    static unsigned long buff_ms, _sum = 60002L;
    const unsigned long timers[irq_q] = { 120000L, 60000L, t3, 1800L, 180, 16, 6, 2, 1 }, base_t = 30L, _offset = base_t / (irq_q + 1);  // значения * base_t -> время в мс

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    uint8_t irq = irq_q + 1;

    bool cli = false;
    bool ap = false;

    static uint8_t num_st = 1,
                   max_st = 6,  // макс кол-во прокручиваемых строк
      disp_mode = 0,
                   hour_cnt = 0;
    static char st1[254];

    static bool end_run_st = false,
                m32_8time_act = false,
                disp_on = true,
                blinkColon = false;

    static unsigned long alarm_time = millis();
    snr_data_t sb = snr_data;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (millis() >= buff_ms) {
      _st++;
      if (_st >= irq_q) {
        _st = 0;
        _sum++;
        if (_sum > timers[0]) _sum = 1;
      }

      if (timers[_st] != 0) {
        if (_sum % timers[_st] == 0) irq = _st;
      }
      buff_ms = millis() + _offset;
    }

    switch (irq) {
      case 0:  // once per every hour
        if (hour_cnt > 23) hour_cnt = 0;

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        if (!wifi_data.wifi_off) {
          //stop_wifi();
          //myIP = start_wifi(main_cfg.sta_ssid, main_cfg.sta_pass, main_cfg.ap_ssid, main_cfg.ap_pass);
        }

        if (wifi_data_cur.cli) {
          if (hour_cnt % 12 && rtc_cfg.auto_corr) {
            unsigned long ttm = myrtc.GetNtp(rtc_cfg, rtc_time.ct);
            myrtc.man_set_time(rtc_hw, ttm);
            rtc_time.ct = myrtc.GetTime(rtc_hw);
            rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
          }

          if (hour_cnt % 6 == 0 && mcf.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(mcf.pp_city_id, wifi_data_cur.cli));
          if (hour_cnt % 6 == 0 && mcf.use_pp == 2) {
            //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
            String fore_st = getOWM_forecast(mcf.pp_city_id, mcf.owm_key);
            wf_data = forecast_decode(fore_st, 2);

            //------------------------------------------------------ Получаем прогноз погоды на сегодня от OpenWeatherMap
            wf_data_cur = forecast_decode(fore_st, 1);
          }
          if (mcf.news_en) {
            newsClient->updateNewsClient(mcf.news_api_key, mcf.news_source);
            newsClient->updateNews();
          }
        }
#endif
        hour_cnt++;
        break;

      case 1:  // once per every 3 minute
        break;

      case 2:  // main_cfg.period * 1 minute
        cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        cli = wifi_data_cur.cli;
#endif

        snr_data = GetSnr(sb, snr_cfg_data, mcf, rtc_hw.a_type, cli, wf_data_cur);
        break;

      case 3:  // 55 sec
        max_st = 6;
        if ((mcf.dsp_t == 20) & end_run_st & !rtc_time.nm_is_on) {
          cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
          local_ip = wifi_data_cur.addr.toString();
          if (mcf.news_en & wifi_data_cur.cli) ns = newsClient->getTitle(newsIndex);
          cli = wifi_data_cur.cli;
#endif
          mydsp.runing_string_start(num_st, max_st, mcf, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen);  // запуск бегущей строки для однострочных дисплеев
        }
        break;

      case 4:  // 5 sec
        disp_mode++;
        if (disp_mode > 12) disp_mode %= 12;
        break;

      case 5:  // 0.5 sec
        //-------------Refresh current time in rtcmcf------------------
        rtc_time.ct = myrtc.GetTime(rtc_hw);
        myrtc.dt_from_unix(&rtc_time);
        //-------------Forming string version of current time ------------------
        if (disp_on && mcf.dsp_t > 0) {
          //-------------Brigthness------------------
          if (mcf.auto_br) {
            snr_data.f = f_dsp2.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
            cur_br = f_dsp2.auto_br(snr_data.f, mcf);
          } else {
            if (rtc_time.nm_is_on) cur_br = mcf.nmd_br;  // Man brigthness
            else cur_br = mcf.man_br;
            snr_data.f = cur_br;
          }
          uint16_t ddd = cur_br;  // костыль!!!!!!!!!!!
          //-----------------------------------------
          // run slowely time displays here
          m32_8time_act = false;
          if (!((mcf.dsp_t == 20) & !end_run_st)) {
            m32_8time_act = mydsp.time_view(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, screen, mcf, snr_data, ddd);  // break time view while string is running
            cur_br = ddd;
            mydsp.write_dsp(true, mcf.vdrv_t, mcf.dsp_t, cur_br, mcf.time_up, screen, text_size, mcf.color_up, mcf.color_dwn);
          }
        } else cur_br = 0;
        if (!wasAlarm)  //Проверка будильников
        {
          bool aaaa = !digitalRead(rtc_hw.gpio_sqw);
          if (myrtc.Alarmed(aaaa, rtc_hw, &rtc_alm, rtc_time.ct)) {
            wasAlm_reset();
            if (rtc_alm.al1_on) {
              cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
              cli = wifi_data_cur.cli;
#endif
              alarm1_action(cli, rtc_cfg.alarms[rtc_alm.num].act, rtc_alm.act, rtc_alm.num, &rtc_cfg, rtc_cfg.alarms[rtc_alm.num].type, rtc_time.nm_is_on,
                            mcf.vdrv_t, disp_on, play_snd, screen, mcf.radio_addr);
            }
            if (rtc_alm.al2_on & !rtc_time.nm_is_on & rtc_cfg.every_hour_beep) {
              rtc_alm.act = 15;
              play_snd = true;
            }
            wasAlarm = true;
            alarm_time = millis() + 2000;
          }
        }

        if (wasAlarm & (millis() > alarm_time))  //Перезапуск будильников
        {
          rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
          wasAlarm = false;
        }

        Thermo(snr_data, mcf, gcf);
        blinkColon = !blinkColon;

        //------------------------------------------------------ Отправляем данные через UART
#if defined(ESP8266)
        if (mcf.dsp_t == 50 && !digitalRead(gcf.gpio_uar)) send_uart(snr_data, wf_data, mcf, rtc_time, rtc_alm, cur_br);
#endif
        break;

      case 6:
        //bool divider = true;

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        if (wifi_data_cur.cli || wifi_data_cur.ap) {
          server.handleClient();
          if (debug_level == 2) {
            uint16_t a = (millis() - serv_ms) / 1000;
            DBG_OUT_PORT.printf("Serv sec %u\n", a);
          }
          ArduinoOTA.handle();
          if (((millis() - serv_ms) > 300000L) & wifi_data.wifi_off) stop_serv();  // Истек таймер неактивности - останавливаем вебморду
        }
#endif

        mydsp.scroll_start(true, true, mcf.vdrv_t, mcf.dsp_t, mcf.time_up, end_run_st, st1, screen);
        if (mcf.vdrv_t != 12) mydsp.write_dsp(false, mcf.vdrv_t, mcf.dsp_t, cur_br, mcf.time_up, screen, text_size, mcf.color_up, mcf.color_dwn);
        //divider = !divider;
        break;

      case 7:
        if (m32_8time_act) {
          uint8_t pos = 0;
          if (mcf.dsp_t > 20 && mcf.dsp_t < 29 && !mcf.time_up) pos = 32;

          mydsp.scroll_disp(pos, screen);  // скроллинг вниз символов на экране
        }
        break;

      case 8:
        if (disp_on) {
          mydsp.scroll_start(false, false, mcf.vdrv_t, mcf.dsp_t, mcf.time_up, end_run_st, st1, screen);
          if ((mcf.dsp_t != 20) & end_run_st & !rtc_time.nm_is_on) {
            cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            local_ip = wifi_data_cur.addr.toString();
            if (mcf.news_en & wifi_data_cur.cli) ns = newsClient->getTitle(newsIndex);
            cli = wifi_data_cur.cli;
#endif
            mydsp.runing_string_start(num_st, max_st, mcf, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen);  // перезапуск бегущей строки для двухстрочных дисплеев
          }
          mydsp.write_dsp(false, mcf.vdrv_t, mcf.dsp_t, cur_br, mcf.time_up, screen, text_size, mcf.color_up, mcf.color_dwn);  // передача видеобуфера (screen) на физический уровень
        }
        break;

      default:  // no IRQ
        break;
    }

    // ----------------------------------------------------- Проигрываем звуки
    uint8_t muz_n = 15;
    if (rtc_alm.act < 20) muz_n = rtc_alm.act;
    Buzz.play(muz_n, gcf.gpio_snd, play_snd, gcf.snd_pola);
    play_snd = false;

    // ----------------------------------------------------- Обрабатываем клавиатуру
    cli = false;
    ap = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    cli = wifi_data_cur.cli;
    ap = wifi_data_cur.ap;
#endif

    keyb_read(cli, ap, gcf.gpio_btn, disp_mode, max_st,
              mcf.thermo_t, mcf.vdrv_t, gcf.gpio_led, gcf.led_pola, blinkColon, serv_ms, &mcf, end_run_st);

    //------------------------------------------------------  Верифицируем ночной режим
    rtc_time.nm_is_on = myrtc.nm_act(rtc_time.ct, rtc_cfg.nm_start, rtc_cfg.nm_stop);
  } else  //-------------------------------------------------- Minimal boot
  {
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    if (wifi_data_cur.cli || wifi_data_cur.ap) {
      server.handleClient();
      ArduinoOTA.handle();
    }
#endif
  }
}
