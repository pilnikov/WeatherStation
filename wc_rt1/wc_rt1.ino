//------------------------------------------------------
//#include ".\headers\conf.h"
#include "Scr_frm.h"

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
static byte time_buff[32];      // display buffer for time part
static byte scroll_buff[32];    // display buffer for scrolling
static uint8_t text_size = 1;   // размер текста
static uint8_t newsIndex;
String local_ip = "192.168.0.0", ns = String();

static byte dsp_c; // Class of displays

//_color _symbol _fast _double    1
//_mono  _digit  _slow _single    0


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

// ------------------------------------------------------- HW Level of displays
MyDspHW mydsp_hw;

//-------------------------------------------------------- Bright control
MyDspBCF mydsp_bcf;

//-------------------------------------------------------- MatrixScreenContentFormer
MSCF mscf;

//-------------------------------------------------------- SegmentScreenContentFormer
SSCF sscf;

Synt Buzz;  //Конструктор пищалки

CT myrtc;        //For RTC Common
RTCJS myrtccfg;  //For RTC Config
SF hw_chk;       //For HW Check
SNR sens_f;      //For Sensor Common
SNRJS mysnrcfg;  //For Sensor Config

KBT keybrd;

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
    MDNS.begin(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F("Open http://"));
    DBG_OUT_PORT.print(wifi_data.ap_ssid);
    DBG_OUT_PORT.print(F(".local/edit to see the file browser\n"));

    nsys.OTA_init(wifi_data.ap_ssid, wifi_data.ap_pass);

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
  conf_f = "/conf_main.json";
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
  conf_f = "/conf_gpio.json";
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
    //if (mcf.dsp_t > 0 && mcf.vdrv_t > 0) mydsp_hw._init(mcf.vdrv_t, mcf.dsp_t, gcf, hw_data.ht_addr, hw_data.lcd_addr, text_size);
    if ((mcf.dsp_t > 0) && (mcf.vdrv_t > 0)) dsp_c = mydsp_hw._init(mcf.vdrv_t, mcf.dsp_t, gcf, hw_data.ht_addr, hw_data.lcd_addr, text_size);

    //------------------------------------------------------  Инициализируем датчики
    if (hw_data.bh1750_present) lightMeter.begin();

    snr_cfg_data.gpio_dht = gcf.gpio_dht;
    sensor_init(&snr_cfg_data);

    //------------------------------------------------------  Инициализируем GPIO
    if (!hw_data.bh1750_present) pinMode(gcf.gpio_ana, INPUT);
    if (gcf.gpio_led != 255) pinMode(gcf.gpio_led, OUTPUT);                          // Initialize the LED_PIN pin as an output
    if (gcf.gpio_led != 255) digitalWrite(gcf.gpio_led, gcf.led_pola ? HIGH : LOW);  //Включаем светодиод

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
      if (mcf.use_pp > 0) handler_UPD_WFC_Data();

      //------------------------------------------------------ Запускаем SSDP
      nsys.ssdp_init();

      //------------------------------------------------------ Получаем новости от NewsApi
      if (mcf.news_en & wifi_data_cur.cli) {
        newsClient = new NewsApiClient(mcf.news_api_key, mcf.news_source);
        handler_UPD_News();
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
    if (gcf.gpio_led != 255) digitalWrite(gcf.gpio_led, gcf.led_pola ? LOW : HIGH);


    //-------------------------------------------------------- Регулируем яркость дисплея
    if (mcf.auto_br && mcf.dsp_t > 0 && mcf.vdrv_t > 0) {
      snr_data.f = mydsp_bcf.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
      cur_br = mydsp_bcf.auto_br(snr_data.f, mcf);
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
//    mcf.esm = false;
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
    byte _screen[64];               // display buffer
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
        if (dsp_c && 0b0100 && !(dsp_c & 0b0001) && end_run_st && !rtc_time.nm_is_on) {
          cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
          local_ip = wifi_data_cur.addr.toString();
          if (mcf.news_en & wifi_data_cur.cli) ns = newsClient->getTitle(newsIndex);
          cli = wifi_data_cur.cli;
#endif
          mscf.roll_string_part_sw(num_st, max_st, mcf, snr_data, wf_data, wf_data_cur,
                                   rtc_time, rtc_alm, local_ip, cur_br, cli, ns,
                                   newsIndex, end_run_st, st1, scroll_buff);  //смена фрагмента строки прокрутки для однострочных символьных дисплеев
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


        //-------------Forming current time frame ---------------------------------------------------------------------------------------------------------------------------
        // run slowely time displays here

        m32_8time_act = false;
        if (!((mcf.dsp_t == 20) & !end_run_st))
        {
          if ((mcf.dsp_t > 0 && mcf.dsp_t < 14) || mcf.dsp_t == 30 || mcf.dsp_t == 31)
          {
            memset(time_buff, 0, sizeof(time_buff));
            sscf.seg_scr_frm(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, time_buff, mcf, snr_data, cur_br);
          }
          else
          {
            // Формирует часть фрейма с отображением текущего времени на символьных (матричных и LCD1602) дисплеях
            memset(time_buff, 0, sizeof(time_buff));
            m32_8time_act = mscf.symbol_time_part_view(rtc_cfg.use_pm, end_run_st, mcf.rus_lng, mcf.dsp_t, rtc_time, rtc_alm, time_buff);
          }
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
                            mcf.vdrv_t, disp_on, play_snd, _screen, mcf.radio_addr);
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

        //------------------------------------------------------ Управляем яркостью
        if (disp_on && mcf.dsp_t > 0 && mcf.vdrv_t > 0)
        {
          if (mcf.auto_br) {
            snr_data.f = mydsp_bcf.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
            cur_br = mydsp_bcf.auto_br(snr_data.f, mcf);
          }
          else
          {
            if (rtc_time.nm_is_on) cur_br = mcf.nmd_br;  // Man brigthness
            else cur_br = mcf.man_br;
            snr_data.f = cur_br;
          }
        }
        else cur_br = 0;
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
        if (disp_on) mscf.scroll_string_ran_selector(mcf.vdrv_t, mcf.dsp_t, true, end_run_st, st1, scroll_buff); // Бегущая строка для медленных
        break;

      case 7:
        if (m32_8time_act) {
          mscf.v_scroll_all(0, time_buff);  // вертикальный скроллинг изменившихся позиций
        }
        break;

      case 8:
        if (disp_on)
        {
          mscf.scroll_string_ran_selector(mcf.vdrv_t, mcf.dsp_t, false, end_run_st, st1, scroll_buff); //Бегущая строка для быстрых

          // --------------------------------------------------------------------------перезапуск бегущей строки для двухстрочных дисплеев
          if ((mcf.dsp_t != 20) & end_run_st & !rtc_time.nm_is_on)
          {
            cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
            local_ip = wifi_data_cur.addr.toString();
            if (mcf.news_en & wifi_data_cur.cli) ns = newsClient->getTitle(newsIndex);
            cli = wifi_data_cur.cli;
#endif
            mscf.roll_string_part_sw(num_st, max_st, mcf, snr_data, wf_data, wf_data_cur, rtc_time,
                                     rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, scroll_buff);
          }

          //-----------------------------------------------------------------------------------------------/
          memset(_screen, 0, 64);

          if (mcf.time_up)
          {
            memcpy (_screen,            // цель
                    time_buff,          // источник
                    32); // объем
            memcpy(_screen + 32,        // цель
                   scroll_buff,         // источник
                   32);// объем
          }
          else
          {
            memcpy(_screen,             // цель
                   scroll_buff,         // источник
                   32);// объем
            memcpy (_screen + 32,       // цель
                    time_buff,          // источник
                    32); // объем
          }
 
          mydsp_hw._write(mcf.vdrv_t, mcf.dsp_t, cur_br, text_size, mcf.color_up, mcf.color_dwn, _screen);  // передача видеобуфера (_screen) на физический уровень
          //          mydsp_hw._write(mcf.vdrv_t, mcf.dsp_t, cur_br, text_size, mcf.color_up, mcf.color_dwn, _screen);  // передача видеобуфера (_screen) на физический уровень
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

    keybrd._read(cli, ap, gcf.gpio_btn, gcf.gpio_led, gcf.led_pola, disp_mode, max_st, blinkColon, serv_ms, end_run_st, &mcf);

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
