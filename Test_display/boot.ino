//#include ".\headers\fonts.h"
//#include "conf.h"
#include "fonts.h"

static uint8_t  num_st = 1;
static char     st1[254];

static uint8_t  max_st = 6;                             // макс кол-во прокручиваемых строк
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

  mydsp.pr_str(num_st, max_st, conf_data, snr_data, wf_data, wf_data_cur, rtc_time, rtc_alm, local_ip, cur_br, st1, cli, ns);

  DBG_OUT_PORT.print(F("num_st = "));
  DBG_OUT_PORT.println(num_st);
  DBG_OUT_PORT.print(F("st1 = "));
  DBG_OUT_PORT.println(st1);


  if (conf_data.rus_lng & (conf_data.type_vdrv == 12)) ff_dsp.lcd_rus(st1);
  if (conf_data.rus_lng & (conf_data.type_vdrv != 12)) ff_dsp.utf8rus(st1);

  end_run_st = false;
  if (conf_data.type_disp == 20) ff_dsp.CLS(screen, sizeof screen);

  newsIndex ++;
  if (newsIndex > 9) newsIndex = 0;
}

void firq0() // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;
  hour_cnt++;
}

void firq2()
{
}

void firq5() // 0.5 sec main cycle
{
  //-------------Forming string version of current time ------------------
  if (disp_on)
  {
    //-------------Brigthness------------------
    if (conf_data.auto_br)
    {
      snr_data.f = ff_dsp.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
      cur_br = ff_dsp.auto_br(snr_data.f, conf_data);
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
    bool m32_8time_act = false;
    if (!((conf_data.type_disp == 20) & !end_run_st))
    {
      m32_8time_act = mydsp.time_view(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, screen, conf_data, snr_data, cur_br); // break time view while string is running
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      cur_br = ddd;
      mydsp.write_dsp(true, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
    }
  }
  else cur_br = 0;

  blinkColon = !blinkColon;

}

void firq6() // 0.180 sec Communications with server
{
  static bool divider;

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
        end_run_st = ff_dsp.scroll_String(x1, x2, st1, screen, font14s, 2, 0, 2);
      }
      mydsp.ht1633_ramFormer2(screen, 0, 7);
    }
    if (conf_data.type_disp == 31)
    {
      if  (!end_run_st)
      {
        end_run_st = ff_dsp.scroll_String(20, 25, st1, screen, font14s, 2, 0, 2);
      }
      mydsp.ht1633_ramFormer(screen, 0, 13);
    }
    mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
  }
  if (conf_data.type_vdrv == 12)
  {
    if (conf_data.type_disp == 19)
    {
      if  (!end_run_st & divider)
      {
        uint8_t x1 = 0;
        if (conf_data.time_up) x1 = 1;
        end_run_st = ff_dsp.lcd_mov_str(16, st1);
        mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, x1, conf_data.time_up, screen);
      }
    }
  }
  divider = !divider;
}

void firq7() // 0.060 sec
{
  uint8_t pos = 0;
  if (conf_data.type_disp > 20 && conf_data.type_disp < 29 && !conf_data.time_up) pos = 32;

  mydsp.scroll_disp(pos, screen);
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
    end_run_st = ff_dsp.scroll_String(x1, x2, st1, screen, font5x7, 5, 1, 1);
  }
  if ((conf_data.type_disp != 20) & end_run_st & !rtc_time.nm_is_on) runing_string_start(); // перезапуск бегущей строки

  switch (conf_data.type_vdrv)
  {
    case 2:
      if (conf_data.type_disp == 20 || conf_data.type_disp == 21)
      {
        mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
        if (conf_data.type_disp == 20) mydsp.m7219_ramFormer(screen);
        if (conf_data.type_disp == 21) mydsp.m7219_ramFormer2(screen, 4, 2);
      }
      break;

    case 3:
      if (conf_data.type_disp == 23 || conf_data.type_disp == 24 || conf_data.type_disp == 25)
      {
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32  || CONFIG_IDF_TARGET_ESP32S2
        mydsp.m3216_ramFormer(screen, cur_br, text_size);
#endif
      }
      break;
    case 5:
      if (conf_data.type_disp == 22)
      {
        //ORANGE = 3 GREEN = 1
        mydsp.ht1632_ramFormer(screen, conf_data.color_up, conf_data.color_dwn);
        mydsp.write_dsp(false, conf_data.type_vdrv, conf_data.type_disp, cur_br, conf_data.time_up, screen);
      }
      break;
  }
}
