//#include ".\headers\fonts.h"
#include "fonts.h"


static uint8_t cur_sym_pos[3] = {0, 0, 0};
static uint8_t  num_st = 1;


void irq_set()
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
      if (end_run_st) runing_string_start(); //Запуск бегущей строки;
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
}

void runing_string_start() // ---------------------------- Запуск бегущей строки
{
  String local_ip = "192.168.0.0";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  local_ip = myIP.toString();
#endif
  memset(st1, 0, 254);
  memset(st2, 0, 20);

  pr_str(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_data, local_ip, cur_br, st1);

  DBG_OUT_PORT.print(F("num_st = "));
  DBG_OUT_PORT.println(num_st);
  DBG_OUT_PORT.print(F("st1 = "));
  DBG_OUT_PORT.println(st1);


  if (conf_data.rus_lng & (ram_data.type_vdrv == 12)) f_dsp.lcd_rus(st1);
  if (conf_data.rus_lng & (ram_data.type_vdrv != 12)) f_dsp.utf8rus(st1);

  cur_sym_pos[0] = 0;
  cur_sym_pos[1] = 0;

  end_run_st = false;
  if (conf_data.type_disp == 20) f_dsp.CLS(screen, sizeof screen);
}

void firq0() // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;
# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

  if (!conf_data.wifi_off)
  {
    //stop_wifi();
    //myIP = start_wifi(conf_data.sta_ssid, conf_data.sta_pass, conf_data.ap_ssid, conf_data.ap_pass);
  }

  if (web_cli)
  {
    if ((hour_cnt % 12 == 0) & conf_data.auto_corr) GetNtp();

    if (hour_cnt % 6 == 0 && conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));
    if (hour_cnt % 6 == 0 && conf_data.use_pp == 2)
    {
      wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);
    }
    if (conf_data.news_en)
    {
      newsClient.updateNewsClient(conf_data.news_api_key, conf_data.news_source);
      newsClient.updateNews();
    }
  }
#endif
  hour_cnt++;
}

void firq2()
{
  snr_data_t sb = snr_data;
  snr_data = GetSnr(ram_data, conf_data);
  if (ram_data.type_snr1 == 12)
  {
    snr_data.t1 = sb.t1;
    snr_data.h1 = sb.h1;
  }
  if (ram_data.type_snr2 == 12)
  {
    snr_data.t2 = sb.t2;
    snr_data.h2 = sb.h2;
  }
  if (ram_data.type_snr3 == 12)
  {
    snr_data.t3 = sb.t3;
    snr_data.h3 = sb.h3;
  }
  if (ram_data.type_snrp == 12)
  {
    snr_data.p = sb.p;
  }
}

void firq5() // 0.5 sec main cycle
{
  //-------------Refresh current time in rtc_data------------------
  static unsigned long alarm_time;

  GetTime();

  //-------------Forming string version of current time ------------------
  memset (tstr, 0, 25);
  rtc_data_t rt = rtc_data;
  cur_time_str(rt, tstr);
  if (disp_on)
  {
    //-------------Brigthness------------------
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
    //-----------------------------------------
    // run slowely time displays here
    m32_8time_act = false;
    if (!((conf_data.type_disp == 20) & !end_run_st & !nm_is_on)) time_view(conf_data.type_disp, ram_data.type_vdrv); // break time view while string is running
  }
  else cur_br = 0;
  if (!rtc_data.wasAlarm) //Проверка будильников
  {
    if (Alarmed())
    {
      rtc_data.wasAlarm = true;
      alarm_time = millis() + 2000;
    }
  }

  if (rtc_data.wasAlarm & (millis() > alarm_time)) //Перезапуск будильников
  {
    set_alarm();
    rtc_data.wasAlarm = false;
  }
  Thermo(snr_data, conf_data);
  blinkColon = !blinkColon;

  //------------------------------------------------------ Отправляем данные через UART
#if defined(ESP8266)
  if (conf_data.type_disp == 50 && !digitalRead(conf_data.gpio_uar)) send_uart();
#endif
}


void firq6() // 0.180 sec Communications with server
{
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  if (web_cli || web_ap)
  {
    server.handleClient();
    if (debug_level == 2)
    {
      uint16_t a = (millis() - serv_ms) / 1000;
      DBG_OUT_PORT.printf("Serv sec %u\n", a);
    }
    ArduinoOTA.handle();
    if (((millis() - serv_ms) > 300000L) & conf_data.wifi_off) stop_serv(); // Истек таймер неактивности - останавливаем вебморду
  }
# endif

  if (ram_data.type_vdrv == 11)
  {
    if (conf_data.type_disp == 11)
    {
      if  (!nm_is_on & !end_run_st)
      {
        uint8_t x1 = 8, x2 = 15;
        if (!conf_data.time_up)
        {
          x1 = 0;
          x2 = 7;
        }
        end_run_st = f_dsp.scroll_String(x1, x2, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
        if (end_run_st) runing_string_start(); // перезапуск бегущей строки
      }
      ht1633_ramFormer2(screen, 0, 8);
    }
    if (conf_data.type_disp == 31)
    {
      if  (!nm_is_on & !end_run_st)
      {
        end_run_st = f_dsp.scroll_String(20, 25, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
        if (end_run_st) runing_string_start(); // перезапуск бегущей строки
      }
      ht1633_ramFormer(screen, 0, 13);
    }
    ht1633->setBrightness(cur_br);
    ht1633->write();
  }

  if (ram_data.type_vdrv == 12)
  {
    if (conf_data.type_disp == 19)
    {
      if  (!nm_is_on & !end_run_st)
      {
        uint8_t x1 = 1;
        if (!conf_data.time_up) x1 = 0;
        end_run_st = f_dsp.lcd_mov_str(x1, cur_sym_pos[0], st1, st2);
        if (end_run_st) runing_string_start(); // перезапуск бегущей строки
        else
        {
          lcd -> setCursor(0, 0);
          lcd -> print(st2);
        }
      }
    }
  }
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
      f_dsp.shift_ud(true, false, nbuf + pos, screen + pos,  buffud + pos, digPos_x[i],  digPos_x[i] + font_wdt); // запуск вертушка для изменившихся позиций
    }
  }
}

void firq8() //0.030 sec running string is out switch to time view
{
  if (conf_data.type_disp > 19 && conf_data.type_disp < 29 && !nm_is_on && !end_run_st)
  {
    uint8_t x1 = 32, x2 = 63;
    if (!conf_data.time_up)
    {
      x1 = 0;
      x2 = 31;
    }
    end_run_st = f_dsp.scroll_String(x1, x2, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5, 1, 1);
    if ((conf_data.type_disp != 20) & end_run_st) runing_string_start(); // перезапуск бегущей строки
  }

  switch (ram_data.type_vdrv)
  {
    case 2:
      if (conf_data.type_disp == 20 || conf_data.type_disp == 21)
      {
        m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
        if (conf_data.type_disp == 20) m7219_ramFormer(screen);
        if (conf_data.type_disp == 21) m7219_ramFormer2(screen, 4, 2);
      }
      break;

    case 3:
      if (conf_data.type_disp == 23 || conf_data.type_disp == 24 || conf_data.type_disp == 25)
      {
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32
        m3216_ramFormer(screen, cur_br, text_size);
#endif
      }
      break;
    case 5:
      if (conf_data.type_disp == 22)
      {
        //ORANGE = 3 GREEN = 1
        ht1632_ramFormer(screen, conf_data.color_up, conf_data.color_dwn);
        m1632 -> pwm(cur_br);
        m1632 -> sendFrame();
      }
      break;
  }
}
