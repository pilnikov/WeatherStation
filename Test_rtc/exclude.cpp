void alm_act()
{
/*
  dmsg.alarm_msg(rtc_cfg.n_cur_alm, rtc_cfg.type_disp, rtc_cfg.rus_lng);  // Сообщение на индикатор

  switch (rtc_cfg.alarms[rtc_cfg.n_cur_alm][4])     // Выполняем экшн
  {
    case 0:
      rtc_cfg.a_muz = rtc_cfg.alarms[rtc_cfg.n_cur_alm][3];
      play_snd = true;
      break;

    case 1:
      nm_is_on = true;                       // Включаем ночной режим
      break;
    case 2:
      nm_is_on = false;                      // Выключаем ночной режим
      break;
    case 3:
      disp_on = true;
      if (ram_data.type_vdrv == 12)
      {
        lcd->backlight();
        lcd->display();
      }
      if (ram_data.type_vdrv == 2)
      {
        m7219->shutdown(false);
        m7219->write();
      }
      break;
    case 4:
      disp_on = false;
      cur_br = 0;
      snr_data.f = 0;
      f_dsp.CLS(screen, sizeof screen);
      switch (ram_data.type_vdrv)
      {
        case 1:
          tm1637->set_br(0);
          tm1637->clear();
          break;
        case 2:
          m7219->shutdown(true);
          m7219->write();
          break;
        case 3:
          if (rtc_cfg.type_disp == 23 || rtc_cfg.type_disp == 24 || rtc_cfg.type_disp == 25)
          {
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32
            m3216_ramFormer(screen, cur_br, text_size);
#endif
          }
          break;
        case 12:
          lcd->noBacklight();
          lcd->noDisplay();
          break;
        case 11:
          ht1633->clear();
          ht1633->setBrightness(0);
          ht1633->write();
          break;
      }
      break;
    case 5:
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
      radio_snd("cli.start");
#endif
      break;
    case 6:
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
      radio_snd("cli.stop");
#endif
      break;
  }
*/
}
