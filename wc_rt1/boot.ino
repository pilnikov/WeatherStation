
void irq_set()
{
  //------------------------------------------------------ interrupts
  /*  irq1.attach(3600, firq1);
    irq2.attach(1800, rtc_check);
    irq3.attach(conf_data.period * 60, GetSnr);
    irq4.attach(55, firq4);
    irq6.attach(0.5, firq6);
    //irq7.attach(0.2, firq7);
    irq8.attach(0.19, firq8);
    irq9.attach(0.04, firq9);
  */


  uint8_t irq = 10;
  if (millis() - irq_end[1] > 3600000L)  irq = 1;
  else if (millis() - irq_end[2] > 1800000L)  irq = 2;
  else if (millis() - irq_end[3] > conf_data.period * 60000L)  irq = 3;
  else if (millis() - irq_end[4] >   55000L)  irq = 4;
  else if (millis() - irq_end[6] >      500)  irq = 6;
  else if (millis() - irq_end[7] >      200)  irq = 7;
  else if (millis() - irq_end[8] >      190)  irq = 8;
  else if (millis() - irq_end[9] >       40)  irq = 9;

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

    case 6:
      firq6();
      irq_end[6] = millis();
      break;

    case 7:
      firq7();
      irq_end[7] = millis();
      break;

    case 8:
      //      firq8();
      irq_end[8] = millis();
      break;

    case 9:
      firq9();
      irq_end[9] = millis();
      break;
  }

  long dutty = 190;
  if (ram_data.type_disp == 4 || ram_data.type_disp == 8 || ram_data.type_disp == 9) dutty = 20;

  if (millis() - scroll_time[0] > dutty)
  {
    scroll_time[0] = millis();
    if (!end_run_st) cur_sym_pos[0]++; //перемещение по строке вправо на один пиксель
  }

  if (!end_run_st)
  {
    if (ram_data.type_disp == 1 && disp_on) end_run_st = mov_str(ram_data.type_disp,          lcd_col, st1, 0, cur_sym_pos[0]);
    if (ram_data.type_disp == 4)            end_run_st = mov_str(ram_data.type_disp, m7219 -> width(), st1, 0, cur_sym_pos[0]);
    if (ram_data.type_disp == 8 && disp_on) end_run_st = mov_str(ram_data.type_disp, m1632 -> width(), st1, 0, cur_sym_pos[0]);
#if defined(ESP32)
    if (ram_data.type_disp == 9 && disp_on) end_run_st = mov_str(ram_data.type_disp, m3264 -> width(), st1, 0, cur_sym_pos[0]);
#endif
  }

  if (end_run_st != end_run_st_buf)
  {
    end_run_st_buf = end_run_st;

    if (end_run_st)
    {
      cur_sym_pos[0] = 0;

      num_st++; //Перебор строк.
      if (num_st > max_st) num_st = 1;
      st1 = pr_str(num_st);

      if (ram_data.type_disp == 1 || ram_data.type_disp == 8 || ram_data.type_disp == 9) end_run_st = false; // перезапуск бегущей строки;
    }
  }
  if (ram_data.type_disp == 10 && disp_on)      ili_time();
}

void firq1()
{
  if (hour_cnt > 23) hour_cnt = 0;

  nm_veri();
  if (!web_cli && !web_ap && conf_data.use_es) start_wifi();

  if (hour_cnt % 12 == 0 ) GetNtp();
  if (hour_cnt %  6 == 0 && conf_data.use_pp == 1) wf_data = e_srv.get_gm(gs_rcv(conf_data.pp_city_id));
  if (hour_cnt %  6 == 0 && conf_data.use_pp == 2) {
    wf_data = getOWM_forecast(conf_data.pp_city_id, conf_data.owm_key);
    wf_data.press_min = round((wf_data.press_max - wf_data_cur.press_max) / 1.3332239);
  }

  hour_cnt++;
}

void firq4()
{
  if (!nm_is_on) end_run_st = false; // запуск бегущей строки
}

void firq6()
{
  if (disp_on)
  {
    if (conf_data.auto_br)
    {
      snr_data.f = sens.ft_read(ram_data.bh1750_present);
      cur_br = auto_br(snr_data.f, conf_data.br_level);
    }
    else
    {
      cur_br = conf_data.man_br;  // Man brigthness
      snr_data.f = cur_br;
    }

    if (ram_data.type_disp == 1) lcd_time();
    if (ram_data.type_disp == 2 || ram_data.type_disp == 3 || ram_data.type_disp == 5 || ram_data.type_disp == 6 || ram_data.type_disp == 7) sevenseg();
    if (ram_data.type_disp == 4 && end_run_st)
    {
      matrix_refresh();
#ifndef new_max
      m7219_time();
      cur_sym_pos[0] = 0;
#endif
    }
    if (ram_data.type_disp == 8 && disp_on)
    {
      matrix_refresh();
#ifndef new_max
      m1632_time();
#endif
    }
    if (ram_data.type_disp == 9 && disp_on)
    {
      matrix_refresh();
#ifndef new_max
      m3264_time();
#endif
    }
  }
  Alarmed();
  Thermo();
  blinkColon = !blinkColon;
  if (colon < 4) colon ++;
  else colon = 1;
}

void firq7()
{
  if (web_cli || web_ap)
  {
#if defined(ESP8266) || defined(ESP32)
    server.handleClient();
    if (debug_level == 2) DBG_OUT_PORT.printf ("Serv sec %u\n", (millis() - serv_ms) / 1000);

    ArduinoOTA.handle();
    yield();
    if ((millis() - serv_ms) > 300000L && (!conf_data.use_es || (conf_data.use_es && web_ap))) stop_serv(); // Истек таймер неактивности - останавливаем вебморду
# endif
  }
}


void firq9()
{
# ifdef new_max
  if (ram_data.type_disp == 4 && end_run_st)  m7219_time();
  if (ram_data.type_disp == 8 && disp_on)     m1632_time();
#if defined(ESP32)
  if (ram_data.type_disp == 9 && disp_on)     m3264_time();
#endif
#endif
}
