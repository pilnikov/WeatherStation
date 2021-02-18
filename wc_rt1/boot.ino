
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
  if (millis() - irq_end[9] > 40)
  {
    irq = 9;
  }
  else
  {
    if (millis() - irq_end[8] > 125)
    {
      irq = 8;
    }
    else
    {
      if (millis() - irq_end[7] > 200)
      {
        irq = 7;
      }
      else
      {
        if (millis() - irq_end[6] > 500)
        {
          irq = 6;
        }
        else
        {
          if (millis() - irq_end[5] > 5000L)
          {
            irq = 5;
          }
          else
          {
            if (millis() - irq_end[4] > 55000L)
            {
              irq = 4;
            }
            else
            {
              if (millis() - irq_end[3] > conf_data.period * 60000L)
              {
                irq = 3;
              }
              else
              {
                if (millis() - irq_end[2] > 1800000L)
                {
                  irq = 2;
                }
                else
                {
                  if (millis() - irq_end[1] > 3600000L)
                  {
                    irq = 1;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

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

  long dutty = 190;
  if (conf_data.type_disp > 19) dutty = 20;

  if (millis() - scroll_time[0] > dutty)
  {
    scroll_time[0] = millis();
    if (!end_run_st) cur_sym_pos[0]++; //перемещение по строке вправо на один пиксель
  }

  if (!end_run_st)
  {
    if (conf_data.type_disp == 19 && disp_on) end_run_st = mov_str(conf_data.type_disp,          lcd_col, st1, 0, cur_sym_pos[0]);
    //    if (conf_data.type_disp == 20)            end_run_st = mov_str(conf_data.type_disp, m7219 -> width(), st1, 0, cur_sym_pos[0]);
    if (conf_data.type_disp == 22 && disp_on) end_run_st = mov_str(conf_data.type_disp, m1632 -> width(), st1, 0, cur_sym_pos[0]);
#if defined(ESP32)
    if (conf_data.type_disp == 24 && disp_on) end_run_st = mov_str(conf_data.type_disp, m3264 -> width(), st1, 0, cur_sym_pos[0]);
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

      if (conf_data.type_disp == 19 || conf_data.type_disp == 22 || conf_data.type_disp == 24) end_run_st = false; // перезапуск бегущей строки;
    }
  }


  if (conf_data.type_disp == 29 && disp_on) ili_time();
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
    wf_data.press_min = round((wf_data.press_max - wf_data_cur.press_max) / 1.3332239);
  }

  hour_cnt++;
}

void firq4() // 55sec
{
  if (!nm_is_on)
  {
    end_run_st = false; // запуск бегущей строки
    //DBG_OUT_PORT.print("run string..");
    //DBG_OUT_PORT.println(end_run_st);
  }
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
  if (conf_data.type_disp == 20 && disp_on)
  {
    if (cur_br != cur_br_buf)
    {
      m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
      cur_br_buf = cur_br;
    }

    if (conf_data.type_disp == 20 && !end_run_st)
    {
      strcpy((char*)tstr, st1.c_str());
      end_run_st = scroll_String(0, 31, tstr,  sizeof(tstr), cur_sym_pos[2], cur_sym_pos[3], screen, 2);
    }

    m7219_ramFormer(screen);
    m7219 -> write();
  }
}

void firq9() //0.04 sec running string is out switch to time view
{
# ifdef new_max
  if (conf_data.type_disp == 22 && disp_on)     m1632_time();
#if defined(ESP32)
  if (conf_data.type_disp == 24 && disp_on)     m3264_time();
#endif
#endif
}
