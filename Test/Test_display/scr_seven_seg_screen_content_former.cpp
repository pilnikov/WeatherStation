/* Метод формирует содержимое, отображаемое на семисегментных дисплеях */

#include "Scr_frm.h"

FFF scr77;  //For frame former funcions

void SSCF::seg_scr_frm(bool use_pm, bool blinkColon, bool end_run_st, rtc_time_data_t rtd, rtc_alm_data_t rta, byte *screen, main_cfg_t mcf, snr_data_t snr, uint16_t br) {

  uint8_t h = use_pm && rtd.hour != 12 ? rtd.hour % 12 : rtd.hour;
  h = h % 100;

  bool _alarmed = rta.num < 7, m32_8time_act = false;


  uint8_t mod = 13, x1 = 0;
  if (!mcf.time_up) x1 = 32;
  if (end_run_st || rtd.nm_is_on) scr77.CLS(screen, sizeof screen);
  switch (mcf.dsp_t) {
    case 1:
      // 7SEGx4D
      if (rtd.nm_is_on || mod == 10) mod = 13;
      SSCF::seg_add_data(mod, 4, screen, 0, snr, rtd, rta, use_pm, br);
      if (blinkColon) scr77.printDot(3, screen);
      break;
    case 2:
      // 7SEGx6D
      if (rtd.nm_is_on || mod == 9) mod = 11;
      SSCF::seg_add_data(mod, 6, screen, 0, snr, rtd, rta, use_pm, br);
      break;
    case 3:
      // 7SEGx8D
      if (rtd.nm_is_on || mod == 6) mod = 7;
      SSCF::seg_add_data(mod, 8, screen, 0, snr, rtd, rta, use_pm, br);
      break;
    case 10:
      // 14SEGx4D
      if (rtd.nm_is_on || mod == 10) mod = 13;
      SSCF::seg_add_data(mod, 4, screen, 0, snr, rtd, rta, use_pm, br);
      if (blinkColon) scr77.printDot(3, screen);
      break;
    case 11:
      // 14SEGx8D
      SSCF::seg_add_data(mod, 4, screen, x1 / 4, snr, rtd, rta, use_pm, br);
      if (blinkColon) scr77.printDot(x1 / 4 + 2, screen);
      break;
    case 12:
      // 16SEGx4D
      break;
    case 13:
      // 16SEGx8D
      SSCF::seg_add_data(mod, 4, screen, x1 / 4, snr, rtd, rta, use_pm, br);
      if (mod == 10) mod = 1;
      if (!rtd.nm_is_on) SSCF::seg_add_data(mod, 4, screen, 8, snr, rtd, rta, use_pm, br);
      if (blinkColon) scr77.printDot(x1 / 4 + 2, screen);
      break;
    case 30:
      // CUSTOM_1
      break;
    case 31:
      // CUSTOM_2
      SSCF::seg_add_data(mod, 4, screen, 0, snr, rtd, rta, use_pm, br);
      if (blinkColon) scr77.printDot(5, screen);
      if (mod == 9) mod = 1;
      if (!rtd.nm_is_on) SSCF::seg_add_data(mod, 6, screen, 8, snr, rtd, rta, use_pm, br);
      scr77.compressor7(screen, 0, 10);
      break;
    default:
      break;
  }
}
