//#include ".\headers\fonts.h"
#include "fonts.h"

void irq_set()
{
  //------------------------------------------------------ interrupts
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
    case 0: // one per hour
      rtc_check();
      break;

    case 1: // one per 10 min
      GetSnr();
      break;

    case 2: // one per 0.5 sec
      firq2();
      break;

    case 3:
      firq3();  // one per 0.125 sec
      break;

    case 4: // one per 0.04 sec
      if (disp_on) firq4();
      break;
    default:
      break;
  }

  uint8_t h = rtc_data.hour;
  disp_on = true;
  /* if (h < 7 || h >= 17) //гашение экрана в нерабочее время
    {
     m3216 -> drawPixel(h, 0, m3216 -> ColorHSV(500, 255, 100, true));
     m3216 -> drawPixel(rtc_data.min / 2, 15, m3216 -> ColorHSV(500, 255, 100, true));
     if (rtc_data.min % 10 == 0 && rtc_data.sec == 0) synchro();
     disp_on = false;
    }
  */
  // ----------------------------------------------------- Проигрываем звуки
#if defined(__xtensa__)
  //inital sound card
  Buzz.play(pgm_read_ptr(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);
#elif defined (__AVR__)
  Buzz.play(pgm_read_word(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);   //inital sound card
#endif
  play_snd = false;
  //------------------------------------------------------  Верифицируем ночной режим
  if (conf_data.nm_start <  conf_data.nm_stop) nm_is_on = ((rtc_data.hour >= conf_data.nm_start) & (rtc_data.hour < conf_data.nm_stop));
  else nm_is_on = (rtc_data.hour >= conf_data.nm_start || rtc_data.hour < conf_data.nm_stop);
}

void runing_string_start()
{
  String local_ip = "192.168.0.0";

  st1 = " ";
  pr_str(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_data, local_ip, cur_br, st1);

  DBG_OUT_PORT.print(F("num_st = "));
  DBG_OUT_PORT.println(num_st);
  DBG_OUT_PORT.print(F("st1 = "));
  DBG_OUT_PORT.println(st1);

  f_dsp.utf8rus(st1);

  cur_sym_pos[0] = 0;
  cur_sym_pos[1] = 0;

  end_run_st = false;
}

void firq2() // 0.5 sec main cycle
{
  //_now = DS3231.GetDateTime();
  _now = RtcDateTime(__DATE__, __TIME__);
  rtc_data.hour = _now.Hour();
  rtc_data.min = _now.Minute();
  rtc_data.sec = _now.Second();
  rtc_data.wday = _now.DayOfWeek() + 1;
  rtc_data.month = _now.Month();
  rtc_data.day = _now.Day();
  rtc_data.year = _now.Year(); //костыль

  if (disp_on)
  {
    //-------------------------------------------------------- Регулируем яркость дисплея
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
    f_dsp.time_m32_8(screen, 32, oldDigit, digPos_x, d_notequal, buffud,  font5x7, conf_data.use_pm, q_dig, rtc_data); // display Time
  }

  if (Alarmed()) rtc_data.wasAlarm = true;
  if (rtc_data.wasAlarm & !play_snd)
  {
    set_alarm();
    rtc_data.wasAlarm = false;
  }
}

void firq3() // 0.125 sec
{
  const uint8_t pos = 32;

  uint8_t font_wdt = 5;
  static byte nbuf[64];

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (i > 3) font_wdt = 3;

    if (d_notequal[i])
    {
      f_dsp.shift_ud(true, false, nbuf + pos, screen + pos,  buffud + pos, digPos_x[i],  digPos_x[i] + font_wdt); // запуск вертушка для изменившихся позиций
    }
  }
}

void firq4() //0.04 sec running string is out switch to time view
{
  //  if (!nm_is_on & !end_run_st)
  if (!end_run_st)
  {
    end_run_st = f_dsp.scroll_String(0, 31, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5, 1, 1);
    if (end_run_st)
    {
      runing_string_start(); // перезапуск бегущей строки
    }
    /*  DBG_OUT_PORT.print(F("cur_sym_pos[0] is "));
        DBG_OUT_PORT.println(cur_sym_pos[0]);
        DBG_OUT_PORT.print(F("cur_sym_pos[1] is "));
        DBG_OUT_PORT.println(cur_sym_pos[1]);
    */
  }

  m3216_ramFormer(screen, cur_br, 2);
  m3216 -> swapBuffers(true);
}
