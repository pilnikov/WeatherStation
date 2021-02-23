
void time_view()
{
  CLS();
  switch (conf_data.type_disp)
  {
    case 1:
      // 7SEGx4D
      if (nm_is_on || disp_mode == 10 ) disp_mode = 13;
      seg7_mode(disp_mode, 4);
      if (blinkColon) printDot(1);
      tm1637->set_br(cur_br);
      tm1637->display(0, screen[0]);
      tm1637->display(1, screen[1]);
      tm1637->display(2, screen[2]);
      tm1637->display(3, screen[3]);
      break;
    case 2:
      // 7SEGx6D
      if (nm_is_on || disp_mode == 10 ) disp_mode = 13;
      seg7_mode(disp_mode, 6);
      if (blinkColon) printDot(1);
      break;
    case 3:
      // 7SEGx8D
      seg7_mode(disp_mode, 8);
      if (nm_is_on) disp_mode = 7;
      if (disp_mode > 5 ) disp_mode = 0;
      break;
    case 10:
      // 14SEGx4D
      break;
    case 11:
      // 14SEGx8D
      break;
    case 12:
      // 16SEGx4D
      break;
    case 13:
      // 16SEGx8D
      break;
    case 19:
      // 2LINEx16D
      lcd_time();
      break;
    case 20:
      // M32x8MONO
      time_m32_8();
      break;
    case 21:
      // m32x16MONO
      break;
    case 22:
      // M32x16BICOL
      m1632_time();
      break;
    case 23:
      // M32x16COLOR
      break;
    case 24:
      // M64x32COLOR
      m3264_time();
      break;
    case 25:
      // M64x64COLOR
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
      size_tstr = snprintf(tstr, 2, "%02u", second());
      print_(tstr, strlen(tstr), screen, font14s, 2, 0);
      if (blinkColon) printDot(2);
      break;
    default:
      break;
  }
  ////////////////// SEND DATA TO PHYSICAL LEVEL ////////////////////

  switch (ram_data.type_vdrv)
  {
    case 0:
      //SERIAL
      break;
    case 1:
      //TM1637
      tm1637->set_br(cur_br);
      tm1637->display(0, screen[0]);
      tm1637->display(1, screen[2]);
      tm1637->display(2, screen[4]);
      tm1637->display(3, screen[6]);
      break;
    case 2:
      //MAX7219
      m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
      m7219 -> setRam(screen, sizeof(screen));
      m7219_ramFormer(screen);
      m7219 -> write(); // Send bitmap to display
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
      ht1633->setBrightness(cur_br);
      ht1633_ramFormer(screen);
      ht1633->write();
      break;
    case 12:
      //PCF8574
      pcf8574_init();
      break;
    default:
      break;
  }
}
