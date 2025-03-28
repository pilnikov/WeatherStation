//#include ".\headers\fonts.h"
//#include "conf.h"
#include "fonts.h"

static uint8_t num_st = 1;
static char st1[254];

static uint8_t max_st = 6;  // макс кол-во прокручиваемых строк
static uint8_t disp_mode = 0;

static bool end_run_st = false,
            m32_8time_act = false,
            disp_on = true,
            blinkColon = false;

static uint8_t hour_cnt = 0;
static unsigned long alarm_time = millis();


void main_loop() {
  //------------------------------------------------------ interrupts

  bool cli = false;

  unsigned long t3 = mcf.period * 2000L;
  const uint8_t irq_q = 9;
  static uint8_t _st = 0;
  static unsigned long buff_ms, _sum = 60002L;
  const unsigned long timers[irq_q] = { 120000L, 60000L, t3, 1800L, 180, 16, 6, 2, 1 }, base_t = 30L, _offset = base_t / (irq_q + 1);  // значения * base_t -> время в мс

  uint8_t irq = irq_q + 1;

  if (millis() >= buff_ms) {
    _st++;
    if (_st >= irq_q) {
      _st = 0;
      _sum++;
      if (_sum > timers[0]) _sum = 1;
    }

    if (timers[_st] != 0) {
      if (_sum % timers[_st] == 0) irq = _st;
    }
    buff_ms = millis() + _offset;
  }

  switch (irq) {
    case 0:  // once per every hour
      firq0();
      break;

    case 1:  // once per every 3 minute
      break;

    case 2:  // mcf.period * 1 minute
      firq2();
      break;

    case 3:  // 55 sec
      max_st = 6;
      if ((mcf.dsp_t == 20) & end_run_st & !rtc_time.nm_is_on) {
        bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
        local_ip = wifi_data_cur.addr.toString();
        //        if (mcf.news_en & wifi_data_cur.cli) ns = newsClient -> getTitle(newsIndex);
        cli = wifi_data_cur.cli;
#endif
        mscf.h_scroll_restart(num_st, max_st, mcf, snr_data, wf_data, wf_data_cur,
                              rtc_time, rtc_alm, local_ip, cur_br, cli, ns,
                              newsIndex, end_run_st, st1, screen);  // перезапуск бегущей строки для однострочных дисплеев
      }
      break;

    case 4:  // 5 sec
      disp_mode++;
      if (disp_mode > 12) disp_mode %= 12;
      break;

    case 5:  // 0.5 sec
      firq5();
      break;

    case 6:
      firq6();
      break;

    case 7:
      if (m32_8time_act) {
        firq7();
      }
      break;

    case 8:
      if (disp_on) {
        firq8();
      }
      break;

    default:  // no IRQ
      break;
  }
}

void firq0()  // 1 hour
{
  if (hour_cnt > 23) hour_cnt = 0;
  hour_cnt++;
}

void firq2() {
}

void firq5()  // 0.5 sec main cycle
{
  if (disp_on) {
    //-------------Brigthness------------------
    if (mcf.auto_br) {
      snr_data.f = mydsp_bcf.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
      cur_br = mydsp_bcf.auto_br(snr_data.f, mcf);
    } else {
      if (rtc_time.nm_is_on) cur_br = mcf.nmd_br;  // Man brigthness
      else cur_br = mcf.man_br;
      snr_data.f = cur_br;
    }
    uint16_t ddd = cur_br;  // костыль!!!!!!!!!!!
    //-----------------------------------------
    // run slowely time displays here
    m32_8time_act = false;
    if (!((mcf.dsp_t == 20) & !end_run_st)) {
      if ((mcf.dsp_t > 0 && mcf.dsp_t < 14) || mcf.dsp_t == 30 || mcf.dsp_t == 31) {
        sscf.seg_scr_frm(rtc_cfg.use_pm, blinkColon, end_run_st, rtc_time, rtc_alm, screen, mcf, snr_data, ddd);
      } else {
        m32_8time_act = mscf.symbol_time_part_view(rtc_cfg.use_pm, end_run_st, rtc_time, rtc_alm, mcf, screen);
      }
      cur_br = ddd;

      DBG_OUT_PORT.println(F("11111111111111111111111111111111111111111111111111111111"));
      DBG_OUT_PORT.println();
      DBG_OUT_PORT.println(screen[16]);
      DBG_OUT_PORT.println(screen[20]);
      DBG_OUT_PORT.println(screen[24]);
      DBG_OUT_PORT.println(screen[31]);
      DBG_OUT_PORT.println();

      mydsp_hw._write(mcf.vdrv_t, mcf.dsp_t, cur_br, text_size, mcf.color_up, mcf.color_dwn, screen);
    }
  } else cur_br = 0;

  blinkColon = !blinkColon;
}

void firq6()  // 0.180 sec Communications with server
{
  static bool divider;
  mscf.h_scroll(true, divider, mcf.vdrv_t, mcf.dsp_t, mcf.time_up, end_run_st, st1, screen);

  DBG_OUT_PORT.println(F("2222222222222222222222222222222222222222222222222222222222"));
  DBG_OUT_PORT.println();
  DBG_OUT_PORT.println(screen[16]);
  DBG_OUT_PORT.println(screen[20]);
  DBG_OUT_PORT.println(screen[24]);
  DBG_OUT_PORT.println(screen[31]);
  DBG_OUT_PORT.println();

  if (mcf.vdrv_t != 12) mydsp_hw._write(mcf.vdrv_t, mcf.dsp_t, cur_br, text_size, mcf.color_up, mcf.color_dwn, screen);
  divider = !divider;
}

void firq7()  // 0.060 sec
{
  uint8_t pos = 0;
  if (mcf.dsp_t > 20 && mcf.dsp_t < 29 && !mcf.time_up) pos = 32;

  mscf.v_scroll_all(pos, screen);  // скроллинг вниз изменившихся символов
}

void firq8()  //0.030 sec running string is out switch to time view
{
  mscf.h_scroll(false, false, mcf.vdrv_t, mcf.dsp_t, mcf.time_up, end_run_st, st1, screen);
  if ((mcf.dsp_t != 20) & end_run_st & !rtc_time.nm_is_on) {
    bool cli = false;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    local_ip = wifi_data_cur.addr.toString();
    cli = wifi_data_cur.cli;
#endif
    mscf.h_scroll_restart(num_st, max_st, mcf, snr_data, wf_data, wf_data_cur, rtc_time,
                          rtc_alm, local_ip, cur_br, cli, ns, newsIndex, end_run_st, st1, screen);  // перезапуск бегущей строки для двухстрочных дисплеев
  }
  DBG_OUT_PORT.println(F("3333333333333333333333333333333333333333333333333333333333333333"));
  DBG_OUT_PORT.println();
  DBG_OUT_PORT.println(screen[16]);
  DBG_OUT_PORT.println(screen[20]);
  DBG_OUT_PORT.println(screen[24]);
  DBG_OUT_PORT.println(screen[31]);
  DBG_OUT_PORT.println();
  mydsp_hw._write(mcf.vdrv_t, mcf.dsp_t, cur_br, text_size, mcf.color_up, mcf.color_dwn, screen);  // передача видеобуфера (screen) на физический уровень
}
