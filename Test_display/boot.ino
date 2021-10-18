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
      firq1();
      break;

    case 1: // once per every 3 minute
      break;

    case 2: // conf_data.period * 1 minute
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
  String local_ip = "192.168.0.0";
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  //local_ip = myIP.toString();
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

void firq1() // 1 hour
{
}

void firq6() // 0.5 sec main cycle
{
  //-------------Forming string version of current time ------------------
  if (conf_data.boot_mode == 2)
  {
    memset (tstr, 0, 25);
    rtc_data_t rt = rtc_data;
    cur_time_str(rt, tstr);
  }
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
    if (!((conf_data.type_disp == 20) & !end_run_st & !nm_is_on)) time_view(conf_data.type_disp, ram_data.type_vdrv, end_run_st, nm_is_on, blinkColon); // break time view while string is running
  }
  else cur_br = 0;

  blinkColon = !blinkColon;

}

void firq7() // 0.180 sec Communications with server
{
  if (ram_data.type_vdrv == 11)
  {
    if (conf_data.type_disp == 11)
    {
      if  (!nm_is_on & !end_run_st)
      {
        end_run_st = f_dsp.scroll_String(8, 15, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font14s, 2, 0, 2);
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
        end_run_st = f_dsp.lcd_mov_str(16, cur_sym_pos[0], st1, st2);
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
#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)
        m3216_ramFormer(screen, cur_br, text_size);
#endif
      }
      break;
    case 5:
      if (conf_data.type_disp == 22)
      {
        //ORANGE = 3 GREEN = 1
        ht1632_ramFormer(screen, 1, 3);
        m1632 -> pwm(cur_br);
        m1632 -> sendFrame();
      }
      break;
  }
}