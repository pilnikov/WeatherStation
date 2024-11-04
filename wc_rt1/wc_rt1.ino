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
conf_data_t conf_data;
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

MAINJS maincfg;

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
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    DBG_OUT_PORT.println(F("Failed to open conf_wifi.json for reading. Using default configuration!!!"));
    wifi_data = wifi_cfg.def_conf();
    from_client = wifi_cfg.to_json(wifi_data);
    lfs.writeFile(conf_f, from_client.c_str());
  } else {
    wifi_data = wifi_cfg.from_json(from_client);
  }
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем основные сетевые сервисы
  //--------------------------------------------------------  Запускаем WiFi
  wifi_data_cur = wifi.begin(wifi_data);

  if (wifi_data_cur.cli || wifi_data_cur.ap) {
    //------------------------------------------------------  Переопределяем консоль
    if (conf_data.udp_mon) {
      //DBG_OUT_PORT.end();
      //DBG_OUT_PORT = *udp_cons;
      //IP_Addr.fromString(conf_data.srudp_addr);
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
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    DBG_OUT_PORT.println(F("Failed to open conf_rtc.json for reading. Using default configuration!!!"));
    rtc_cfg = myrtccfg.def_conf();
    from_client = myrtccfg.to_json(rtc_cfg);
    lfs.writeFile(conf_f, from_client.c_str());
  } else {
    rtc_cfg = myrtccfg.from_json(from_client);
  }
#else
  rtc_cfg = myrtccfg.from_json(from_client);
#endif
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем настройки датчиков
  conf_f = "/conf_snr.json";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    DBG_OUT_PORT.println(F("Failed to open conf_snr.json for reading. Using default configuration!!!"));
    snr_cfg_data = mysnrcfg.def_conf();
    from_client = mysnrcfg.to_json(snr_cfg_data);
    lfs.writeFile(conf_f, from_client.c_str());
  } else {
    snr_cfg_data = mysnrcfg.from_json(from_client);
  }
#else
  snr_cfg_data = mysnrcfg.from_json(from_client);
#endif
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем общие настройки HW
  conf_f = "/config.json";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    DBG_OUT_PORT.println(F("Failed to open config.json for reading. Using default configuration!!!"));
    conf_data = maincfg.def_conf();
    from_client = maincfg.to_json(conf_data);
    lfs.writeFile(conf_f, from_client.c_str());
  } else {
    conf_data = maincfg.from_json(from_client);
  }
#else
  conf_data = maincfg.from_json(from_client);
#endif
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Инициализируем кнопку
  pinMode(conf_data.gpio_btn, INPUT_PULLUP);
  attachInterrupt(conf_data.gpio_btn, isr1, CHANGE);
  boot_mode = digitalRead(conf_data.gpio_btn);

  //boot_mode = 0;

  DBG_OUT_PORT.print(F("boot_mode..."));
  DBG_OUT_PORT.println(boot_mode);
  
   if (boot_mode == 1) {
    //------------------------------------------------------  Начинаем инициализацию Hardware
  
    if (conf_data.gpio_sda != 255 && conf_data.gpio_scl != 255)
    {
      #if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
          Wire.setPins(conf_data.gpio_sda, conf_data.gpio_scl);
      #endif

      #if defined(ESP8266)
          Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
      #else
          Wire.begin();
      #endif
    }
    
    rtc_hw.a_type = rtc_cfg.c_type;
    hw_data = hw_chk.hw_present();
    hw_accept(hw_data, &snr_cfg_data, &conf_data.type_vdrv, &rtc_hw.a_type);
    
    //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
    if (conf_data.type_disp > 0) mydsp.disp_init(conf_data.type_vdrv, conf_data.type_disp, conf_data.gpio_uar, conf_data.gpio_dio, conf_data.gpio_clk, conf_data.gpio_dcs, conf_data.gpio_dwr, hw_data.ht_addr, hw_data.lcd_addr, screen, text_size, conf_data.rus_lng);
    
    //------------------------------------------------------  Инициализируем датчики
    if (hw_data.bh1750_present) lightMeter.begin();

    snr_cfg_data.gpio_dht = conf_data.gpio_dht;
    sensor_init(&snr_cfg_data);

    //------------------------------------------------------  Инициализируем GPIO
    if (!hw_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);
    if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) pinMode(conf_data.gpio_led, OUTPUT);                                // Initialize the LED_PIN pin as an output
    if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

    if (conf_data.gpio_snd != 255) pinMode(conf_data.gpio_snd, OUTPUT);

    //------------------------------------------------------  GPIO for RTC
    rtc_hw.gpio_dio = conf_data.gpio_dio;
    rtc_hw.gpio_clk = conf_data.gpio_clk;
    rtc_hw.gpio_dcs = conf_data.gpio_dcs;
    rtc_hw.gpio_sqw = conf_data.gpio_sqw;
    if (rtc_hw.gpio_sqw != 255)
    {
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
    uint32_t ttm = compileDateTime.Epoch32Time();

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
      if (conf_data.use_pp > 0) handleUpdForeCast();
           
      //------------------------------------------------------ Запускаем SSDP
      nsys.ssdp_init();

      //------------------------------------------------------ Получаем новости от NewsApi
      if (conf_data.news_en & wifi_data_cur.cli) {
        newsClient = new NewsApiClient(conf_data.news_api_key, conf_data.news_source);
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
    snr_data = GetSnr(sb, snr_cfg_data, conf_data, rtc_hw.a_type, cli, wf_data_cur);

    //-------------------------------------------------------- Гасим светодиод
    if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);


    //-------------------------------------------------------- Регулируем яркость дисплея
    if (conf_data.auto_br && conf_data.type_disp > 0) {
      snr_data.f = f_dsp2.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
      cur_br = f_dsp2.auto_br(snr_data.f, conf_data);
    } else {
      if (rtc_time.nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
      else cur_br = conf_data.man_br;
      snr_data.f = cur_br;
    }
    DBG_OUT_PORT.print(F("brightness from sensor..."));
    DBG_OUT_PORT.println(snr_data.f);

    //------------------------------------------------------ Отправляем данные через UART
    if (conf_data.type_disp == 50) {
      if (!conf_data.udp_mon) {
        DBG_OUT_PORT.end();
        DBG_OUT_PORT.begin(19200);
        send_uart(snr_data, wf_data, conf_data, rtc_time, rtc_alm, cur_br);
      }
    }
 
    //------------------------------------------------------ Устанавливаем пищалку
    rtc_alm.act = 15;
    play_snd = true;
 
    //------------------------------------------------------ Окончание подготовки к запуску
    DBG_OUT_PORT.println(F("End of setup"));

    //------------------------------------------------------ Засыпаем
    if (conf_data.esm) {
#if defined(ESP8266)
      wifi._shutdown();
#endif
      DBG_OUT_PORT.flush();
#if defined(ESP8266)
      ESP.deepSleep(conf_data.period * 60e6, RF_DISABLED);  // deep-sleep. Засыпаем на period минут!
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
      ESP.deepSleep(conf_data.period * 60e6);  // deep-sleep. Засыпаем на period минут!
#endif
    }
  } else DBG_OUT_PORT.println(F("Safe mode!!! End of setup"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  if (boot_mode == 1) {
    //------------------------------------------------------ interrupts
    unsigned long t3 = conf_data.period * 2000L;
    const uint8_t irq_q = 9;
    static uint8_t _st = 0;
    static unsigned long buff_ms, _sum = 60002L;
    const unsigned long timers[irq_q] = { 120000L, 60000L, t3, 1800L, 180, 16, 6, 2, 1 }, base_t = 30L, _offset = base_t / (irq_q + 1);  // значения * base_t -> время в мс

    uint8_t irq = irq_q + 1;

    bool cli = false;
    bool ap = false;

    static uint8_t num_st = 1,
                   max_st = 6,  // макс кол-во прокручиваемых строк
                   disp_mode = 0,
                   hour_cnt = 0;
    static char    st1[254];

    static bool    end_run_st = false,
                   m32_8time_act = false,
                   disp_on = true,
                   blinkColon = false;

    static unsigned long alarm_time = millis();
    snr_data_t sb = snr_data;

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
          //myIP = start_wifi(conf_data.sta_ssid, conf_data.sta_pass, conf_data.ap_ssid, conf_data.ap_pass);
        }

        if (wifi_data_cur.cli) {
          if (hour_cnt % 12 && rtc_cfg.auto_corr) {
            unsigned long ttm = myrtc.GetNtp(rtc_cfg, rtc_time.ct);
            myrtc.man_set_time(rtc_hw, ttm);
            rtc_time.ct = myrtc.GetTime(rtc_hw);
            rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
          }

          if (hour_cnt % 6 == 0 && conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id, wifi_data_cur.cli));
          if (hour_cnt % 6 == 0 && conf_data.use_pp == 2) {
            //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
            wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);

            //------------------------------------------------------ Получаем прогноз погоды на сегодня от OpenWeatherMap
            wf_data_cur = getOWM_current(conf_data.pp_city_id, conf_data.owm_key);
          }
          if (conf_data.news_en) {
            newsClient->updateNewsClient(conf_data.news_api_key, conf_data.news_source);
            newsClient->updateNews();
          }
        }
#endif
        hour_cnt++;
        break;

      case 1:  // once per every 3 minute
        break;

      case 2:  // conf_data.period * 1 minute
        cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        cli = wifi_data_cur.cli;
#endif

        snr_data = GetSnr(sb, snr_cfg_data, conf_data, rtc_hw.a_type, cli, wf_data_cur);
        break;

      case 3:  // 55 sec
        max_st = 6;
        if ((conf_data.type_disp == 20) & end_run_st & !rtc_time.nm_is_on) {
          cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
          local_ip = wifi_data_cur.addr.toString();
          if (conf_data.news_en & wifi_data_cur.cli) ns = newsClient->getTitle(newsIndex);
          cli = wifi_data_cur.cli;
#endif
          mydsp.runing_string_start(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen);  // запуск бегущей строки для однострочных дисплеев
        }
        break;

      case 4:  // 5 sec
        disp_mode++;
        if (disp_mode > 12) disp_mode %= 12;
        break;

      case 5:  // 0.5 sec
        //-------------Refresh current time in rtc_data------------------
        rtc_time.ct = myrtc.GetTime(rtc_hw);
        myrtc.dt_from_unix(&rtc_time);
        //-------------Forming string version of current time ------------------
        if (disp_on && conf_data.type_disp > 0) {
          //-------------Brigthness------------------
          if (conf_data.auto_br) {
            snr_data.f = f_dsp2.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
            cur_br = f_dsp2.auto_br(snr_data.f, conf_data);
          } else {
            if (rtc_time.nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
            else cur_br = conf_data.man_br;
            snr_data.f = cur_br;
          }
          uint16_t ddd = cur_br;  // костыль!!!!!!!!!!!
          //-----------------------------------------
          // run slowely time displays here
          m32_8time_act = false;
          if (!((conf_data.type_disp == 20) & !end_run_st)) {
            m32_8time_act = mydsp.time_view(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, screen, conf_data, snr_data, ddd);  // break time view while string is running
            cur_br = ddd;
            mydsp.write_dsp(true, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen, text_size, conf_data.color_up, conf_data.color_dwn);
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
                            conf_data.type_vdrv, disp_on, play_snd, screen, conf_data.radio_addr);
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

        Thermo(snr_data, conf_data);
        blinkColon = !blinkColon;

        //------------------------------------------------------ Отправляем данные через UART
#if defined(ESP8266)
        if (conf_data.type_disp == 50 && !digitalRead(conf_data.gpio_uar)) send_uart(snr_data, wf_data, conf_data, rtc_time, rtc_alm, cur_br);
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

        mydsp.scroll_start(true, true, conf_data.type_vdrv, conf_data.type_disp, conf_data.time_up, end_run_st, st1, screen);
        if (conf_data.type_vdrv != 12) mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen, text_size, conf_data.color_up, conf_data.color_dwn);
        //divider = !divider;
        break;

      case 7:
        if (m32_8time_act) {
          uint8_t pos = 0;
          if (conf_data.type_disp > 20 && conf_data.type_disp < 29 && !conf_data.time_up) pos = 32;

          mydsp.scroll_disp(pos, screen);  // скроллинг вниз символов на экране
        }
        break;

      case 8:
        if (disp_on) {
          mydsp.scroll_start(false, false, conf_data.type_vdrv, conf_data.type_disp, conf_data.time_up, end_run_st, st1, screen);
          if ((conf_data.type_disp != 20) & end_run_st & !rtc_time.nm_is_on) {
            cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            local_ip = wifi_data_cur.addr.toString();
            if (conf_data.news_en & wifi_data_cur.cli) ns = newsClient->getTitle(newsIndex);
            cli = wifi_data_cur.cli;
#endif
            mydsp.runing_string_start(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen);  // перезапуск бегущей строки для двухстрочных дисплеев
          }
          mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen, text_size, conf_data.color_up, conf_data.color_dwn);  // передача видеобуфера (screen) на физический уровень
        }
        break;

      default:  // no IRQ
        break;
    }

    // ----------------------------------------------------- Проигрываем звуки
    uint8_t muz_n = 15;
    if (rtc_alm.act < 20) muz_n = rtc_alm.act;
    Buzz.play(muz_n, conf_data.gpio_snd, play_snd, conf_data.snd_pola);
    play_snd = false;

    // ----------------------------------------------------- Обрабатываем клавиатуру
    cli = false;
    ap = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    cli = wifi_data_cur.cli;
    ap = wifi_data_cur.ap;
#endif

    keyb_read(cli, ap, conf_data.gpio_btn, disp_mode, max_st,
              conf_data.type_thermo, conf_data.type_vdrv, conf_data.gpio_led, conf_data.led_pola, blinkColon, serv_ms, &conf_data, end_run_st);

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