
void time_view(uint8_t, uint8_t);

char buf[16];

void time_view(uint8_t type_disp, uint8_t type_vdrv)
{

  uint8_t h = rtc_cfg.use_pm && rtc_time.hour != 12 ? rtc_time.hour % 12 : rtc_time.hour;
  h = h % 100;

  memset (buf, 0, 16);

  uint16_t minute_ala = (uint16_t) rtc_alm.hour  * 60 + rtc_alm.min;
  uint16_t minute_cur = (uint16_t) rtc_time.hour * 60 + rtc_time.min;
  bool _alarmed = ((minute_ala > minute_cur) & (rtc_alm.hour < 24));



  uint8_t mod = 13, x1 = 0;
  if (!conf_data.time_up) x1 = 32;
  if (end_run_st || rtc_time.nm_is_on) f_dsp.CLS(screen, sizeof screen);
  switch (type_disp)
  {
    case 1:
      // 7SEGx4D
      if (rtc_time.nm_is_on || disp_mode == 10) disp_mode = 13;
      seg7_mode(disp_mode, 4, screen, 0, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      if (blinkColon) f_dsp.printDot(3, screen);
      break;
    case 2:
      // 7SEGx6D
      if (rtc_time.nm_is_on || disp_mode == 9) disp_mode = 11;
      seg7_mode(disp_mode, 6, screen, 0, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      break;
    case 3:
      // 7SEGx8D
      if (rtc_time.nm_is_on || disp_mode == 6) disp_mode = 7;
      seg7_mode(disp_mode, 8, screen, 0, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      break;
    case 10:
      // 14SEGx4D
      if (rtc_time.nm_is_on || disp_mode == 10) disp_mode = 13;
      seg7_mode(disp_mode, 4, screen, 0, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      if (blinkColon) f_dsp.printDot(3, screen);
      break;
    case 11:
      // 14SEGx8D
      seg7_mode(mod, 4, screen, x1 / 4, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      if (blinkColon) f_dsp.printDot(x1 / 4 + 2, screen);
      break;
    case 12:
      // 16SEGx4D
      break;
    case 13:
      // 16SEGx8D
      seg7_mode(mod, 4, screen, x1 / 4, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      if (disp_mode == 10) disp_mode = 1;
      if (!rtc_time.nm_is_on) seg7_mode(disp_mode, 4, screen, 8, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      if (blinkColon) f_dsp.printDot(x1 / 4 + 2, screen);
      break;
    case 19:
      // 2LINEx16D
      if (_alarmed)
      {
        if (conf_data.rus_lng) sprintf_P(buf, PSTR("%3d:%02d:%02d %2d:%02d\355"), h, rtc_time.min, rtc_time.sec, rtc_alm.hour, rtc_alm.min);
        else sprintf_P(buf, PSTR("%3d:%02d:%02d %2d:%02d"), h, rtc_time.min, rtc_time.sec, rtc_alm.hour, rtc_alm.min);
      }
      else  sprintf_P(buf, PSTR("%3d:%02d:%02d --:-- "), h, rtc_time.min, rtc_time.sec);
      break;
    case 20:
      // M32x8MONO
      m32_8time_act = f_dsp.time_m32_8(screen, 0, oldDigit, digPos_x, d_notequal, buffud, font5x7, rtc_cfg.use_pm, q_dig, rtc_time);
      break;
    case 21:
      // m32x16MONO
      m32_8time_act = f_dsp.time_m32_8(screen, x1, oldDigit, digPos_x, d_notequal, buffud, font5x7, rtc_cfg.use_pm, q_dig, rtc_time);
      break;
    case 22:
      // M32x16BICOL
      m32_8time_act = f_dsp.time_m32_8(screen, x1, oldDigit, digPos_x, d_notequal, buffud, font5x7, rtc_cfg.use_pm, q_dig, rtc_time);
      break;
    case 23:
      // M32x16COLOR
      m32_8time_act = f_dsp.time_m32_8(screen, x1, oldDigit, digPos_x, d_notequal, buffud, font5x7, rtc_cfg.use_pm, q_dig, rtc_time);
      break;
    case 24:
      // M64x32COLOR
      m32_8time_act = f_dsp.time_m32_8(screen, x1, oldDigit, digPos_x, d_notequal, buffud, font5x7, rtc_cfg.use_pm, q_dig, rtc_time);
      break;
    case 25:
      // M64x64COLOR
      m32_8time_act = f_dsp.time_m32_8(screen, x1, oldDigit, digPos_x, d_notequal, buffud, font5x7, rtc_cfg.use_pm, q_dig, rtc_time);
      break;
    case 29:
      // 320x240COLOR
      ili_time();
      break;
    case 30:
      // CUSTOM_1
      break;
    case 31:
      // CUSTOM_2
      seg7_mode(mod, 4, screen, 0, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      if (blinkColon) f_dsp.printDot(5, screen);
      if (disp_mode == 9) disp_mode = 1;
      if (!rtc_time.nm_is_on) seg7_mode(disp_mode, 6, screen, 8, conf_data, snr_data, rtc_time, rtc_alm, cur_br, rtc_cfg.use_pm);
      f_dsp.compressor7(screen, 0, 10);
      break;
    default:
      break;
  }
  ////////////////// SEND DATA TO PHYSICAL LEVEL ////////////////////

  switch (type_vdrv)
  {
    case 0:
      //SERIAL
      break;
    case 1:
      //TM1637
      f_dsp.compressor7(screen, 0, 4);
      tm1637->set_br(cur_br);
      for (uint8_t a = 0; a < 4; a++) tm1637->display(a, screen[a]);
      break;
    case 2:
      //MAX7219
      if (conf_data.type_disp < 10)
      {
        m7adopt(screen, 0, 8);
        m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
        m7219 -> write();
      }
      break;
    case 3:
      //595
      break;
    case 4:
      //HT1621
      break;
    case 5:
      //HT1632
      break;
    case 6:
      //ILI9341
      break;
    case 11:
      //HT16K33
      if (conf_data.type_disp != 31 && conf_data.type_disp != 11)
      {
        if (conf_data.type_disp == 13) ht1633_ramFormer2(screen, 0, 7);
        else ht1633_ramFormer2(screen, 0, 3);
        ht1633->setBrightness(cur_br);
        ht1633->write();
      }
      break;
    case 12:
      //PCF8574
      lcd_time(buf, conf_data.time_up);
      break;
    default:
      break;
  }
}
