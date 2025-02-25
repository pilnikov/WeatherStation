#include "Scr.h"
#include "fonts.h"

FD dsp; //For Display funcions
CT rtc; //For RTC Common

bool MSG::time_view(bool use_pm, bool blinkColon, bool end_run_st, rtc_time_data_t rt, rtc_alm_data_t rta, byte *screen, main_cfg_t mcf, snr_data_t snr, uint16_t br)
{

  uint8_t h = use_pm && rt.hour != 12 ? rt.hour % 12 : rt.hour;
  h = h % 100;

  bool _alarmed = rta.num < 7, m32_8time_act = false;


  uint8_t mod = 13, x1 = 0;
  if (!mcf.time_up) x1 = 32;
  if (end_run_st || rt.nm_is_on) dsp.CLS(screen, sizeof screen);
  switch (mcf.dsp_t)
  {
    case 1:
      // 7SEGx4D
      if (rt.nm_is_on || mod == 10) mod = 13;
      MSG::seg7_mode(mod, 4, screen, 0, snr, rt, rta, use_pm, br);
      if (blinkColon) dsp.printDot(3, screen);
      break;
    case 2:
      // 7SEGx6D
      if (rt.nm_is_on || mod == 9) mod = 11;
      MSG::seg7_mode(mod, 6, screen, 0, snr, rt, rta, use_pm, br);
      break;
    case 3:
      // 7SEGx8D
      if (rt.nm_is_on || mod == 6) mod = 7;
      MSG::seg7_mode(mod, 8, screen, 0, snr, rt, rta, use_pm, br);
      break;
    case 10:
      // 14SEGx4D
      if (rt.nm_is_on || mod == 10) mod = 13;
      MSG::seg7_mode(mod, 4, screen, 0, snr, rt, rta, use_pm, br);
      if (blinkColon) dsp.printDot(3, screen);
      break;
    case 11:
      // 14SEGx8D
      MSG::seg7_mode(mod, 4, screen, x1/4, snr, rt, rta, use_pm, br);
      if (blinkColon) dsp.printDot(x1/4 + 2, screen);
      break;
    case 12:
      // 16SEGx4D
      break;
    case 13:
      // 16SEGx8D
      MSG::seg7_mode(mod, 4, screen, x1/4, snr, rt, rta, use_pm, br);
      if (mod == 10) mod = 1;
      if (!rt.nm_is_on) MSG::seg7_mode(mod, 4, screen, 8, snr, rt, rta, use_pm, br);
      if (blinkColon) dsp.printDot(x1/4 + 2, screen);
      break;
    case 19:
      // 2LINEx16D
      if (_alarmed)
      {
        rtc_hms_t alt = rtc.unix_to_hms(rta.time);
        if (mcf.rus_lng) sprintf_P((char*)screen, PSTR("%3d:%02d:%02d %2d:%02d\355"), h, rt.min, rt.sec, alt.h, alt.m);
        else sprintf_P((char*)screen, PSTR("%3d:%02d:%02d %2d:%02d"), h, rt.min, rt.sec, alt.h, alt.m);
      }
      else  sprintf_P((char*)screen, PSTR("%3d:%02d:%02d --:-- "), h, rt.min, rt.sec);
      break;
    case 20:
      // M32x8MONO
      m32_8time_act = dsp.time_m32_8(screen, 0, change, dposx, old, buffud, font5x7, use_pm, q_dig, rt);
      break;
    case 21:
      // m32x16MONO
      m32_8time_act = dsp.time_m32_8(screen, x1, change, dposx, old, buffud, font5x7, use_pm, q_dig, rt);
      break;
    case 22:
      // M32x16BICOL
      m32_8time_act = dsp.time_m32_8(screen, x1, change, dposx, old, buffud, font5x7, use_pm, q_dig, rt);
      break;
    case 23:
      // M32x16COLOR
      m32_8time_act = dsp.time_m32_8(screen, x1, change, dposx, old, buffud, font5x7, use_pm, q_dig, rt);
      break;
    case 24:
      // M64x32COLOR
      m32_8time_act = dsp.time_m32_8(screen, x1, change, dposx, old, buffud, font5x7, use_pm, q_dig, rt);
      break;
    case 25:
      // M64x64COLOR
      m32_8time_act = dsp.time_m32_8(screen, x1, change, dposx, old, buffud, font5x7, use_pm, q_dig, rt);
      break;
    case 29:
      // 320x240COLOR
      //ili_time(mcf.rus_lng, use_pm);
      break;
    case 30:
      // CUSTOM_1
      break;
    case 31:
      // CUSTOM_2
      MSG::seg7_mode(mod, 4, screen, 0, snr, rt, rta, use_pm, br);
      if (blinkColon) dsp.printDot(5, screen);
      if (mod == 9) mod = 1;
      if (!rt.nm_is_on) MSG::seg7_mode(mod, 6, screen, 8, snr, rt, rta, use_pm, br);
      dsp.compressor7(screen, 0, 10);
      break;
    default:
      break;
  }
 return m32_8time_act;
}
