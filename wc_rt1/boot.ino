#include "fonts.h"

void irq_set()
{
  //------------------------------------------------------ interrupts
  /*  irq1.attach(3600, firq1);
    irq2.attach(1800, rtc_check);
    irq3.attach(conf_data.period * 60, GetSnr);
    irq4.attach(55, firq4);
    irq6.attach(0.5, firq6);
    //irq7.attach(0.2, firq7);
    irq8.attach(0.062, firq8);
    irq9.attach(0.04, firq9);
  */
  unsigned long timers [10] = {0, 3600000L, 1800000L, conf_data.period * 60000L, 55000L, 5000L, 500, 200, 63, 30};

  uint8_t irq = 10;
  for (uint8_t i = 1; i < 10;  i++) if (millis() - irq_end[i] >  timers[i]) irq = i;
  //&& millis() - irq_end[i] <  timers[i] + 100

  switch (irq)
  {
    case 1:
      firq1();
      irq_end[1] = millis();
      break;

    case 2:
      rtc_check();
      irq_end[2] = millis();
      break;

    case 3:
      GetSnr();
      irq_end[3] = millis();
      break;

    case 4:
      firq4();
      irq_end[4] = millis();
      break;

    case 5:
      disp_mode++;
      disp_mode2++;
      if (disp_mode2 > 12)
      {
        disp_mode2 = 0;
        disp_mode = 0;
      }
      irq_end[5] = millis();
      break;

    case 6:
      firq6();
      irq_end[6] = millis();
      break;

    case 7:
      firq7();
      irq_end[7] = millis();
      break;

    case 8:
      firq8();
      irq_end[8] = millis();
      break;

    case 9:
      firq9();
      irq_end[9] = millis();
      break;
  }

  if (end_run_st != end_run_st_buf)
  {
    end_run_st_buf = end_run_st;

    if (end_run_st)
    {
      num_st++; //Перебор строк.
      if (num_st > max_st) num_st = 1;

      st1 = pr_str(num_st);

      f_dsp.utf8rus(st1);

      if (conf_data.type_disp == 19 || conf_data.type_disp == 22 || conf_data.type_disp == 24) end_run_st = false; // перезапуск бегущей строки;
    }
  }
}

void firq1() // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;

  nm_veri();
  if (!web_cli && !web_ap && !conf_data.wifi_off) start_wifi();

  if (hour_cnt % 12 == 0) GetNtp();
  if (hour_cnt % 6 == 0 && conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));
  if (hour_cnt % 6 == 0 && conf_data.use_pp == 2) {
    wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);
  }

  hour_cnt++;
}

void firq4() // 55sec
{
  if (!nm_is_on) end_run_st = false; // запуск бегущей строки
}

void firq6() // 0.5 sec main cycle
{
  if (disp_on)
  {
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

    // run slowely time displays here
    if (end_run_st) time_view();
  }
  Alarmed();
  Thermo();
  blinkColon = !blinkColon;
  if (colon < 4) colon++;
  else colon = 1;
}

void firq7() // 0.2 sec Communications with server
{
  if (web_cli || web_ap)
  {
#if defined(ESP8266) || defined(ESP32)
    server.handleClient();
    if (debug_level == 2) DBG_OUT_PORT.printf("Serv sec %u\n", (millis() - serv_ms) / 1000);

    ArduinoOTA.handle();
    yield();
    if ((millis() - serv_ms) > 300000L && conf_data.wifi_off) stop_serv(); // Истек таймер неактивности - останавливаем вебморду
# endif
  }
}

void firq8() // 0.125 sec
{
  if (conf_data.type_disp == 20 && disp_on && end_run_st)
  {
    uint8_t font_wdt = 5;

    for (uint8_t i = 0; i < q_dig; i++)
    {
      if (i > 3) font_wdt = 3;

      if (d_notequal[i])
      {
        shift_ud(true, false, buff1, screen,  digPos_x[i],  digPos_x[i] + font_wdt); // запуск вертушка для изменившихся позиций
      }
    }
  }
}


void firq9() //0.04 sec running string is out switch to time view
{
# ifdef new_max
  if (conf_data.type_disp == 20 && disp_on)
  {
    if (cur_br != cur_br_buf)
    {
      m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
      cur_br_buf = cur_br;
    }

    if (!end_run_st)
    {
      end_run_st = scroll_String(0, 31, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5);
    }

    m7219_ramFormer(screen);
    m7219 -> write();
  }
  if (!end_run_st)
  {
    // if (conf_data.type_disp == 19 && disp_on) end_run_st = mov_str(conf_data.type_disp,          lcd_col, st1, 0, cur_sym_pos[0]);
    //    if (conf_data.type_disp == 20)            end_run_st = mov_str(conf_data.type_disp, m7219 -> width(), st1, 0, cur_sym_pos[0]);
    //if (conf_data.type_disp == 22 && disp_on) end_run_st = mov_str(conf_data.type_disp, m1632 -> width(), st1, 0, cur_sym_pos[0]);
#if defined(ESP32)
    //if (conf_data.type_disp == 24 && disp_on) end_run_st = mov_str(conf_data.type_disp, m3264 -> width(), st1, 0, cur_sym_pos[0]);
#endif
  }
  if (conf_data.type_disp == 22 && disp_on)     m1632_time();
#if defined(ESP32)
  if (conf_data.type_disp == 24 && disp_on)     m3264_time();
#endif
#endif
}
