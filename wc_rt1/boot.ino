//#include ".\headers\fonts.h"
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
  unsigned long timers [10] = {0, 3600000L, 1800000L, conf_data.period * 60000L, 55000L, 5000L, 500, 180, 63, 30};

  uint8_t irq = 10;
  for (uint8_t i = 1; i < 10;  i++) if (millis() - irq_end[i] >  timers[i]) irq = i;
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
      if (disp_mode > 12) disp_mode -= 12;
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
    default:
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

      if (conf_data.type_disp != 20 && !nm_is_on)
      {
        cur_sym_pos[0] = 0;
        cur_sym_pos[1] = 0;
        end_run_st = false; // перезапуск бегущей строки;
      }
    }
  }
}

void firq1() // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;

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
  if (!nm_is_on && conf_data.type_disp == 20 && end_run_st)
  {
    cur_sym_pos[0] = 0;
    cur_sym_pos[1] = 0;
    end_run_st = false; // запуск бегущей строки
  }
}

void firq6() // 0.5 sec main cycle
{
  if (disp_on)
  {
    //-------------Brigthness------------------
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

    if (cur_br != cur_br_buf)
    {
      cur_br_buf = cur_br;
    }
    //-----------------------------------------

    // run slowely time displays here
    m32_8time_act = false;
    if (!(conf_data.type_disp == 20 && !end_run_st)) time_view(conf_data.type_disp, ram_data.type_vdrv); //break time view while scroll a string
  }
  Alarmed();
  Thermo();
  blinkColon = !blinkColon;
 
  //------------------------------------------------------ Отправляем данные через UART
  if (conf_data.type_disp == 50 && !digitalRead(uart_pin)) send_uart();
}

void firq7() // 0.2 sec Communications with server
{
  if (web_cli || web_ap)
  {
#if defined(ESP8266) || defined(ESP32)
    server.handleClient();
    if (debug_level == 2) DBG_OUT_PORT.printf("Serv sec %u\n", (millis() - serv_ms) / 1000);

    ArduinoOTA.handle();
    if ((millis() - serv_ms) > 300000L && conf_data.wifi_off) stop_serv(); // Истек таймер неактивности - останавливаем вебморду
# endif
  }
  if (ram_data.type_vdrv == 11)
  {
    if (conf_data.type_disp == 11)
    {
      if (!nm_is_on) end_run_st = scroll_String(8, 15, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
      ht1633_ramFormer2(screen, 0, 8);
      ht1633->setBrightness(cur_br);
      ht1633->write();
    }
    if (conf_data.type_disp == 31)
    {
      if (!nm_is_on) end_run_st = scroll_String(20, 25, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
      ht1633_ramFormer(screen, 0, 13);
      ht1633->setBrightness(cur_br);
      ht1633->write();
    }
  }
}

void firq8() // 0.125 sec
{
  uint8_t pos = 0;
  if (conf_data.type_disp > 20 && conf_data.type_disp < 29) pos = 32;

  if (m32_8time_act)
  {
    uint8_t font_wdt = 5;
    byte nbuf[64];

    for (uint8_t i = 0; i < q_dig; i++)
    {
      if (i > 3) font_wdt = 3;

      if (d_notequal[i])
      {
        shift_ud(true, false, nbuf + pos, screen + pos,  buffud + pos, digPos_x[i],  digPos_x[i] + font_wdt); // запуск вертушка для изменившихся позиций
      }
    }
  }
}

void firq9() //0.04 sec running string is out switch to time view
{
  if (conf_data.type_disp > 19 && conf_data.type_disp < 29  && disp_on)
  {
    if (!end_run_st) end_run_st = scroll_String(0, 31, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5, 1, 1);
  }

  if (ram_data.type_vdrv == 5 && conf_data.type_disp == 22 && disp_on)
  {
    ht1632_ramFormer(screen, ORANGE, GREEN);
    m1632 -> pwm(cur_br);
    m1632 -> sendFrame();
  }


  if (ram_data.type_vdrv == 2 && conf_data.type_disp == 20 && disp_on)
  {
    m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
    m7219_ramFormer(screen);
    m7219 -> write();
  }
}
