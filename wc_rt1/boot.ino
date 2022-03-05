//#include ".\headers\fonts.h"
//#include "conf.h"
#include "fonts.h"

FD f_dsp2; //For Display

static uint8_t  cur_sym_pos[3] = {0, 0, 0};
static uint8_t  num_st = 1;
static char     st1[254];
static char     st2[20];

const uint8_t   q_dig = 6;                              // количество цифр на дисплее
static uint8_t  max_st = 6;                             // макс кол-во прокручиваемых строк

static bool     d_notequal[q_dig];
const uint8_t   digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                   // убегающая цифра
static uint16_t buffud[64];
static uint8_t  disp_mode  = 0;

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
      if ((conf_data.type_disp == 20) & end_run_st & !rtc_time.nm_is_on) runing_string_start(); // запуск бегущей строки для однострочных дисплеев
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
            conf_data.type_thermo, conf_data.type_vdrv, conf_data.gpio_led, conf_data.led_pola, blinkColon, serv_ms, &conf_data);

  //------------------------------------------------------  Верифицируем ночной режим
  rtc_time.nm_is_on = myrtc.nm_act(rtc_time.ct, rtc_cfg.nm_start, rtc_cfg.nm_stop);
}

void runing_string_start() // ---------------------------- Запуск бегущей строки
{
  String local_ip = "192.168.0.0", ns = String();
  static uint8_t newsIndex;
  bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  local_ip = wifi_data_cur.addr.toString();
  if (conf_data.news_en & wifi_data_cur.cli) ns = newsClient -> getTitle(newsIndex);
  cli = wifi_data_cur.cli;
#endif
  memset(st1, 0, 254);
  memset(st2, 0, 20);

  pr_str(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, st1, cli, ns);

  DBG_OUT_PORT.print(F("num_st = "));
  DBG_OUT_PORT.println(num_st);
  DBG_OUT_PORT.print(F("st1 = "));
  DBG_OUT_PORT.println(st1);


  if (conf_data.rus_lng & (conf_data.type_vdrv == 12)) f_dsp2.lcd_rus(st1);
  if (conf_data.rus_lng & (conf_data.type_vdrv != 12)) f_dsp2.utf8rus(st1);

  cur_sym_pos[0] = 0;
  cur_sym_pos[1] = 0;

  end_run_st = false;
  if (conf_data.type_disp == 20) f_dsp2.CLS(screen, sizeof screen);

  newsIndex ++;
  if (newsIndex > 9) newsIndex = 0;
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
    uint16_t ddd = cur_br;
    //-----------------------------------------
    // run slowely time displays here
    m32_8time_act = false;
    if (!((conf_data.type_disp == 20) & !end_run_st))
    {
      m32_8time_act = time_view(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, screen, conf_data, snr_data,
                                oldDigit, digPos_x, d_notequal, buffud, q_dig, cur_br); // break time view while string is running
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      cur_br = ddd;
      write_dsp(true, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
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

  if (conf_data.type_vdrv == 11)
  {
    if (conf_data.type_disp == 11)
    {
      if  (!end_run_st)
      {
        uint8_t x1 = 8, x2 = 15;
        if (!conf_data.time_up)
        {
          x1 = 0;
          x2 = 7;
        }
        end_run_st = f_dsp2.scroll_String(x1, x2, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
      }
      ht1633_ramFormer2(screen, 0, 7);
    }
    if (conf_data.type_disp == 31)
    {
      if  (!end_run_st)
      {
        end_run_st = f_dsp2.scroll_String(20, 25, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
      }
      ht1633_ramFormer(screen, 0, 13);
    }
    write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
  }
  if (conf_data.type_vdrv == 12)
  {
    if (conf_data.type_disp == 19)
    {
      if  (!end_run_st & divider)
      {
        uint8_t x1 = 0;
        if (conf_data.time_up) x1 = 1;
        end_run_st = f_dsp2.lcd_mov_str(16, cur_sym_pos[0], st1, st2);
        write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, x1, conf_data.time_up, (byte*)st2);
      }
    }
  }
  divider = !divider;
}

void firq7() // 0.060 sec
{
  uint8_t pos = 0;
  if (conf_data.type_disp > 20 && conf_data.type_disp < 29 && !conf_data.time_up) pos = 32;

  uint8_t font_wdt = 5;
  byte nbuf[64];

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (i > 3) font_wdt = 3;

    if (d_notequal[i])
    {
      f_dsp2.shift_ud(true, false, nbuf + pos, screen + pos,  buffud + pos, digPos_x[i],  digPos_x[i] + font_wdt); // запуск вертушка для изменившихся позиций
    }
  }
}

void firq8() //0.030 sec running string is out switch to time view
{
  if (conf_data.type_disp > 19 && conf_data.type_disp < 29 && !end_run_st)
  {
    uint8_t x1 = 32, x2 = 63;
    if (!conf_data.time_up)
    {
      x1 = 0;
      x2 = 31;
    }
    end_run_st = f_dsp2.scroll_String(x1, x2, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5, 1, 1);
  }
  if ((conf_data.type_disp != 20) & end_run_st & !rtc_time.nm_is_on) runing_string_start(); // перезапуск бегущей строки

  switch (conf_data.type_vdrv)
  {
    case 2:
      if (conf_data.type_disp == 20 || conf_data.type_disp == 21)
      {
        write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
        if (conf_data.type_disp == 20) m7219_ramFormer(screen);
        if (conf_data.type_disp == 21) m7219_ramFormer2(screen, 4, 2);
      }
      break;

    case 3:
      if (conf_data.type_disp == 23 || conf_data.type_disp == 24 || conf_data.type_disp == 25)
      {
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32  || CONFIG_IDF_TARGET_ESP32S2
        m3216_ramFormer(screen, cur_br, text_size);
#endif
      }
      break;
    case 5:
      if (conf_data.type_disp == 22)
      {
        //ORANGE = 3 GREEN = 1
        ht1632_ramFormer(screen, conf_data.color_up, conf_data.color_dwn);
        write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
}
      break;
  }
}
