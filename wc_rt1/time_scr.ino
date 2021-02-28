void time_view(uint8_t, uint8_t);


void time_view(uint8_t type_disp, uint8_t type_vdrv)
{
  if (end_run_st)CLS();
  switch (type_disp)
  {
    case 1:
      // 7SEGx4D
      if (nm_is_on || disp_mode == 10) disp_mode = 13;
      seg7_mode(disp_mode, 4);
      if (blinkColon) printDot(3);
      break;
    case 2:
      // 7SEGx6D
      if (nm_is_on || disp_mode == 9) disp_mode = 11;
      seg7_mode(disp_mode, 6);
      break;
    case 3:
      // 7SEGx8D
      if (nm_is_on || disp_mode == 6) disp_mode = 7;
      seg7_mode(disp_mode, 8);
      break;
    case 10:
      // 14SEGx4D
      if (nm_is_on || disp_mode == 10) disp_mode = 13;
      seg7_mode(disp_mode, 4);
      if (blinkColon) printDot(3);
      break;
    case 11:
      // 14SEGx8D
      seg7_mode(13, 4);
      if (blinkColon) printDot(3);
      break;
    case 12:
      // 16SEGx4D
      break;
    case 13:
      // 16SEGx8D
      if (nm_is_on || disp_mode == 10) disp_mode = 1;
      seg7_mode(disp_mode, 4);
      memmove (screen + 32, // цель
               screen,      // источник
               8);          // объем
      seg7_mode(13, 4);
      memmove (screen + 8,  // цель
               screen + 32, // источник
               8);          // объем
      if (blinkColon) printDot(3);
      break;
    case 19:
      // 2LINEx16D
      lcd_time();
      break;
    case 20:
      // M32x8MONO
      m32_8time_act = time_m32_8(screen, 0, buff1, oldDigit, digPos_x, d_notequal);
      break;
    case 21:
      // m32x16MONO
      m32_8time_act = time_m32_8(screen, 32, buff1, oldDigit, digPos_x, d_notequal);
      break;
    case 22:
      // M32x16BICOL
      m32_8time_act = time_m32_8(screen, 32, buff1, oldDigit, digPos_x, d_notequal);
      break;
    case 23:
      // M32x16COLOR
      m32_8time_act = time_m32_8(screen, 32, buff1, oldDigit, digPos_x, d_notequal);
      break;
    case 24:
      // M64x32COLOR
      m32_8time_act = time_m32_8(screen, 32, buff1, oldDigit, digPos_x, d_notequal);
      break;
    case 25:
      // M64x64COLOR
      m32_8time_act = time_m32_8(screen, 32, buff1, oldDigit, digPos_x, d_notequal);
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
      seg7_mode(0, 4);
      if (nm_is_on) disp_mode = 9;
      if (disp_mode > 9 ) disp_mode = 1;
      seg7_mode(disp_mode, 6);
      compressor7(screen, 14);
      size_tstr = snprintf(tstr, 2, "%02u", second());
      print_(tstr, strlen(tstr), screen, font14s, 2, 0);
      if (blinkColon) printDot(2);
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
      compressor7(screen, 14);
      tm1637->set_br(cur_br);
      for (uint8_t i = 0; i < 6; i++)
      {
        tm1637->display(i, screen[i]);
      }
      break;
    case 2:
      //MAX7219
      if (conf_data.type_disp < 10 && disp_on)
      {
        m7adopt(screen, 18);
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
      switch (type_disp)
      {
        case 1:
          // 7SEGx4D
          ht1633_ramFormer2(screen, 0, 4);
          break;
        case 2:
          // 7SEGx6D
          ht1633_ramFormer2(screen, 0, 6);
          break;
        case 3:
          // 7SEGx8D
          ht1633_ramFormer2(screen, 0, 8);
          break;
        case 10:
          // 14SEGx4D
          ht1633_ramFormer2(screen, 0, 4);
          break;
        case 11:
          // 14SEGx8D
          ht1633_ramFormer2(screen, 0, 4);
          break;
        case 12:
          // 16SEGx4D
          break;
        case 13:
          // 16SEGx8D
          ht1633_ramFormer2(screen, 0, 8);
          break;
        case 31:
          // CUSTOM_2
          ht1633_ramFormer(screen, 0, 8);
          break;
        default:
          ht1633_ramFormer2(screen, 0, 8);
          break;
      }
      ht1633->setBrightness(cur_br);
      ht1633->write();
      break;
    case 12:
      //PCF8574
      break;
    default:
      break;
  }
}