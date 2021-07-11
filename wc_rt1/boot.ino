//#include ".\headers\fonts.h"
#include "fonts.h"


static uint8_t cur_sym_pos[3] = {0, 0, 0};
static uint8_t  num_st = 0;


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
      firq1();
      break;

    case 1: // once per every 3 minute
      rtc_check();
      break;
    case 2: // conf_data.period * 1 minute
      snr_data = GetSnr(ram_data, conf_data);
      break;

    case 3: // 55 sec
      max_st = 4;
      if (end_run_st) runing_string_start(); //Запуск бегущей строки;
      break;

    case 4: // 5 sec
      disp_mode++;
      if (disp_mode > 12) disp_mode %= 12;
      break;

    case 5: // 0.5 sec
      firq6();
      break;

    case 6:
      firq7();
      break;

    case 7:
      if (m32_8time_act) firq8();
      break;

    case 8:
      if (disp_on) firq9();
      break;

    default: // no IRQ
      break;
  }
}

void runing_string_start() // ---------------------------- Запуск бегущей строки
{
  num_st++; //Перебор строк.
  if (num_st > max_st) num_st = 1;

  String local_ip = "192.168.0.0";
#if defined(__xtensa__)
  local_ip =  WiFi.localIP().toString();
#endif

  st1 = pr_str(num_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_data, local_ip, cur_br);
  /*
    DBG_OUT_PORT.print(F("num_st = "));
    DBG_OUT_PORT.println(num_st);
    DBG_OUT_PORT.print(F("st1 = "));
    DBG_OUT_PORT.println(st1);
  */
  f_dsp.utf8rus(st1);

  cur_sym_pos[0] = 0;
  cur_sym_pos[1] = 0;

  end_run_st = false;
  if (conf_data.type_disp == 20) f_dsp.CLS(screen);
}

void firq1() // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;
# if defined(__xtensa__)

  if (!web_cli && !web_ap && !conf_data.wifi_off) start_wifi();

  if (hour_cnt % 12 == 0) GetNtp();
  if (hour_cnt % 6 == 0 && conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));
  if (hour_cnt % 6 == 0 && conf_data.use_pp == 2)
  {
    wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);
  }
#endif
  hour_cnt++;
}

void firq6() // 0.5 sec main cycle
{
  //-------------Refresh current time in rtc_data------------------

  rtc_data.hour      = hour();         // Текущее время. Час.
  rtc_data.min       = minute();       // Текущее время. Минута.
  rtc_data.sec       = second();       // Текущее время. Секунда.
  rtc_data.day       = day();          // Текущее время. День.
  rtc_data.wday      = weekday();      // Текущее время. День недели.
  rtc_data.month     = month();        // Текущее время. Месяц.
  rtc_data.year      = year();         // Текущее время. Год.

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

  if (Alarmed()) rtc_data.wasAlarm = true;
  if (rtc_data.wasAlarm & !play_snd)
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

void firq7() // 0.180 sec Communications with server
{
#if defined(__xtensa__)
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
        end_run_st = f_dsp.scroll_String(8, 15, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
        if (end_run_st || (cur_sym_pos[0] < cur_sym_pos[2])) runing_string_start(); // перезапуск бегущей строки
        cur_sym_pos[2] = cur_sym_pos[0];
      }
      ht1633_ramFormer2(screen, 0, 8);
    }
    if (conf_data.type_disp == 31)
    {
      if  (!nm_is_on & !end_run_st)
      {
        end_run_st = f_dsp.scroll_String(20, 25, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
        if (end_run_st || (cur_sym_pos[0] < cur_sym_pos[2])) runing_string_start(); // перезапуск бегущей строки
        cur_sym_pos[2] = cur_sym_pos[0];
      }
      ht1633_ramFormer(screen, 0, 13);
    }
    ht1633->setBrightness(cur_br);
    ht1633->write();
  }
}

void firq8() // 0.060 sec
{
  uint8_t pos = 0;
  if (conf_data.type_disp > 20 && conf_data.type_disp < 29) pos = 32;

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

void firq9() //0.030 sec running string is out switch to time view
{
  if ((conf_data.type_disp > 19) & (conf_data.type_disp < 29) & !nm_is_on & !end_run_st)
  {
    end_run_st = f_dsp.scroll_String(0, 31, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5, 1, 1);
    if ((conf_data.type_disp != 20) & (end_run_st || (cur_sym_pos[0] < cur_sym_pos[2]))) runing_string_start(); // перезапуск бегущей строки
    cur_sym_pos[2] = cur_sym_pos[0];
  }

  switch (ram_data.type_vdrv)
  {
    case 2:
      if (conf_data.type_disp == 20)
      {
        m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
        m7219_ramFormer(screen);
        m7219 -> write();
      }
      break;

    case 3:
      if (conf_data.type_disp == 23)
      {
#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)
        m3216_ramFormer(screen);
        m3216 -> swapBuffers(true);
#endif
      }
      break;
    case 5:
      if (conf_data.type_disp == 22)
      {
        //ORANGE = 3 GREEN = 1
        ht1632_ramFormer(screen, 3, 1);
        m1632 -> pwm(cur_br);
        m1632 -> sendFrame();
      }
      break;
  }
}
