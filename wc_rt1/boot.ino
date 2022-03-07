//#include ".\headers\fonts.h"
//#include "conf.h"
#include "fonts.h"

static uint8_t  num_st = 1;
static uint8_t  max_st = 6;                             // макс кол-во прокручиваемых строк
static uint8_t  disp_mode  = 0;
static char     st1[254];

//const uint8_t   q_dig = 6;                              // количество цифр на дисплее
//static uint8_t  cur_sym_pos[3] = {0, 0, 0};
//static bool     d_notequal[q_dig];
//const uint8_t   digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
//static unsigned char oldDigit[q_dig];                   // убегающая цифра
//static uint16_t buffud[64];

static bool     end_run_st = false,
                m32_8time_act = false,
                disp_on  = true,
                blinkColon = false;

static uint8_t  hour_cnt  = 0;
static unsigned long alarm_time = millis();


void main_loop()
{
  //------------------------------------------------------ interrupts
  unsigned long t3 = conf_data.period * 2000L;
  const uint8_t irq_q = 9;
  static uint8_t _st = 0;
  static unsigned long buff_ms, _sum = 60002L;
  const unsigned long timers[irq_q] = {120000L, 60000L, t3, 1800L, 180, 16, 6, 2, 1}, base_t = 30L, _offset = base_t / (irq_q + 1); // значения * base_t -> время в мс

  uint8_t irq = irq_q + 1;

  if (millis() >= buff_ms)
  {
    _st++;
    if (_st >= irq_q)
    {
      _st = 0;
      _sum++;
      if (_sum > timers[0]) _sum = 1;
    }

    if (timers[_st] != 0)
    {
      if (_sum % timers[_st] == 0) irq = _st;
    }
    buff_ms = millis() + _offset;
  }

  switch (irq)
  {
    case 0: // once per every hour
      firq0();
      break;

    case 1: // once per every 3 minute
      break;

    case 2: // conf_data.period * 1 minute
      firq2();
      break;

    case 3: // 55 sec
      max_st = 6;
      if ((conf_data.type_disp == 20) & end_run_st & !rtc_time.nm_is_on)
      {
        bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        local_ip = wifi_data_cur.addr.toString();
        if (conf_data.news_en & wifi_data_cur.cli) ns = newsClient -> getTitle(newsIndex);
        cli = wifi_data_cur.cli;
#endif
        mydsp.runing_string_start(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen); // запуск бегущей строки для однострочных дисплеев
      }
      break;

    case 4: // 5 sec
      disp_mode++;
      if (disp_mode > 12) disp_mode %= 12;
      break;

    case 5: // 0.5 sec
      firq5();
      break;

    case 6:
      firq6();
      break;

    case 7:
      if (m32_8time_act) firq7();
      break;

    case 8:
      if (disp_on) firq8();
      break;

    default: // no IRQ
      break;
  }

  // ----------------------------------------------------- Проигрываем звуки
  uint8_t muz_n = 15;
  if  (rtc_alm.act < 20)  muz_n = rtc_alm.act;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  Buzz.play(pgm_read_ptr(&songs[ muz_n]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#elif defined (__AVR__)
  Buzz.play(pgm_read_word(&songs[muz_n]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#endif
  play_snd = false;

  // ----------------------------------------------------- Обрабатываем клавиатуру
  bool cli = false;
  bool ap  = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  cli = wifi_data_cur.cli;
  ap  = wifi_data_cur.ap;
#endif

  keyb_read(cli, ap, conf_data.gpio_btn, disp_mode, max_st,
            conf_data.type_thermo, conf_data.type_vdrv, conf_data.gpio_led, conf_data.led_pola, blinkColon, serv_ms, &conf_data, end_run_st);

  //------------------------------------------------------  Верифицируем ночной режим
  rtc_time.nm_is_on = myrtc.nm_act(rtc_time.ct, rtc_cfg.nm_start, rtc_cfg.nm_stop);
}

void firq0() // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

  if (!wifi_data.wifi_off)
  {
    //stop_wifi();
    //myIP = start_wifi(conf_data.sta_ssid, conf_data.sta_pass, conf_data.ap_ssid, conf_data.ap_pass);
  }

  if (wifi_data_cur.cli)
  {
    if (hour_cnt % 12 && rtc_cfg.auto_corr)
    {
      unsigned long ttm = myrtc.GetNtp(rtc_cfg, rtc_time.ct);
      myrtc.man_set_time(rtc_hw, ttm);
      rtc_time.ct = myrtc.GetTime(rtc_hw);
      rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
    }

    if (hour_cnt % 6 == 0 && conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id, wifi_data_cur.cli));
    if (hour_cnt % 6 == 0 && conf_data.use_pp == 2)
    {
      //------------------------------------------------------ Получаем прогноз погоды от OpenWeatherMap
      wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);

      //------------------------------------------------------ Получаем прогноз погоды на сегодня от OpenWeatherMap
      wf_data_cur = getOWM_current(conf_data.pp_city_id, conf_data.owm_key);
    }
    if (conf_data.news_en)
    {
      newsClient -> updateNewsClient(conf_data.news_api_key, conf_data.news_source);
      newsClient -> updateNews();
    }
  }
#endif
  hour_cnt++;
}

void firq2()
{
  bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  cli = wifi_data_cur.cli;
#endif

  snr_data_t sb = snr_data;
  snr_data = GetSnr(sb, snr_cfg_data, conf_data, rtc_hw.a_type, cli, wf_data_cur);
}

void firq5() // 0.5 sec main cycle
{
  //-------------Refresh current time in rtc_data------------------
  rtc_time.ct = myrtc.GetTime(rtc_hw);
  myrtc.dt_from_unix(&rtc_time);
  //-------------Forming string version of current time ------------------
  if (disp_on)
  {
    //-------------Brigthness------------------
    if (conf_data.auto_br)
    {
      snr_data.f = f_dsp2.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
      cur_br = f_dsp2.auto_br(snr_data.f, conf_data);
    }
    else
    {
      if (rtc_time.nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
      else cur_br = conf_data.man_br;
      snr_data.f = cur_br;
    }
    uint16_t ddd = cur_br; // костыль!!!!!!!!!!!
    //-----------------------------------------
    // run slowely time displays here
    m32_8time_act = false;
    if (!((conf_data.type_disp == 20) & !end_run_st))
    {
      m32_8time_act = mydsp.time_view(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, screen, conf_data, snr_data, ddd); // break time view while string is running
      cur_br = ddd;
      mydsp.write_dsp(true, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen, text_size, conf_data.color_up, conf_data.color_dwn);
    }
  }
  else cur_br = 0;
  if (!wasAlarm) //Проверка будильников
  {
    bool aaaa = !digitalRead(rtc_hw.gpio_sqw);
    if (myrtc.Alarmed(aaaa, rtc_hw, &rtc_alm, rtc_time.ct))
    {
      wasAlm_reset();
      if (rtc_alm.al1_on)
      {
        bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        cli = wifi_data_cur.cli;
#endif
        alarm1_action(cli, rtc_cfg.alarms[rtc_alm.num].act, rtc_alm.act, rtc_alm.num, &rtc_cfg, rtc_cfg.alarms[rtc_alm.num].type, rtc_time.nm_is_on,
                      conf_data.type_vdrv, conf_data.type_disp, disp_on, play_snd, screen, conf_data.radio_addr);
      }
      if (rtc_alm.al2_on & !rtc_time.nm_is_on & rtc_cfg.every_hour_beep)
      {
        rtc_alm.act = 15;
        play_snd = true;
      }
      wasAlarm = true;
      alarm_time = millis() + 2000;
    }
  }

  if (wasAlarm & (millis() > alarm_time)) //Перезапуск будильников
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
}

void firq6() // 0.180 sec Communications with server
{
  static bool divider;

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  if (wifi_data_cur.cli || wifi_data_cur.ap)
  {
    server.handleClient();
    if (debug_level == 2)
    {
      uint16_t a = (millis() - serv_ms) / 1000;
      DBG_OUT_PORT.printf("Serv sec %u\n", a);
    }
    ArduinoOTA.handle();
    if (((millis() - serv_ms) > 300000L) & wifi_data.wifi_off) stop_serv(); // Истек таймер неактивности - останавливаем вебморду
  }
# endif

  mydsp.scroll_start(true, divider, conf_data.type_vdrv, conf_data.type_disp, conf_data.time_up, end_run_st, st1, screen);
  if (conf_data.type_vdrv != 12) mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen, text_size, conf_data.color_up, conf_data.color_dwn);
  divider = !divider;
}

void firq7() // 0.060 sec
{
  uint8_t pos = 0;
  if (conf_data.type_disp > 20 && conf_data.type_disp < 29 && !conf_data.time_up) pos = 32;

  mydsp.scroll_disp(pos, screen); // скроллинг вниз символов на экране
}

void firq8() //0.030 sec running string is out switch to time view
{
  mydsp.scroll_start(false, false, conf_data.type_vdrv, conf_data.type_disp, conf_data.time_up, end_run_st, st1, screen);
  if ((conf_data.type_disp != 20) & end_run_st & !rtc_time.nm_is_on)
  {
    bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    local_ip = wifi_data_cur.addr.toString();
    if (conf_data.news_en & wifi_data_cur.cli) ns = newsClient -> getTitle(newsIndex);
    cli = wifi_data_cur.cli;
#endif
    mydsp.runing_string_start(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen); // перезапуск бегущей строки для двухстрочных дисплеев
  }
  mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen, text_size, conf_data.color_up, conf_data.color_dwn); // передача видеобуфера (screen) на физический уровень
}
