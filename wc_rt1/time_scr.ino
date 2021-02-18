
void time_view()
{
  switch (ram_data.type_vdrv)
  {
    case 1:
      // TM1637
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          if (nm_is_on || disp_mode == 10 ) disp_mode = 13;
          seg7_mode(disp_mode, 0, 4);
          if (blinkColon) printDot(1);
          tm1637->set_br(cur_br);
          tm1637->display(0, screen[0]);
          tm1637->display(1, screen[1]);
          tm1637->display(2, screen[2]);
          tm1637->display(3, screen[3]);
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 20:
          // M32x8MONO
          break;
        case 21:
          // m32x16MONO
          break;
        case 22:
          // M32x16BICOL
          break;
        case 23:
          // M32x16COLOR
          break;
        case 24:
          // M64x32COLOR
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
    case 2:
      // MAX7219
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          if (nm_is_on || disp_mode == 10 ) disp_mode = 13;
          seg7_mode(disp_mode, 0, 4);
          if (blinkColon) printDot(1);

          m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness

          for (uint8_t i = 0; i < 4; i++)
          {
            byte a = 0x1;
            for (uint8_t y = 0; y < 8; y++)
            {
              m7219 -> drawPixel(i, y, (screen[i] & a));
              a <<= 1;
            }
          }
          m7219 -> write(); // Send bitmap to display          
          break;
        case 2:
          // 7SEGx6D
          if (nm_is_on || disp_mode == 10 ) disp_mode = 13;
          seg7_mode(disp_mode, 0, 6);
          if (blinkColon) printDot(1);

          m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness

          for (uint8_t i = 0; i < 6; i++)
          {
            byte a = 0x1;
            for (uint8_t y = 0; y < 8; y++)
            {
              m7219 -> drawPixel(i, y, (screen[i] & a));
              a <<= 1;
            }
          }
          m7219 -> write(); // Send bitmap to display          
          break;
        case 3:
          // 7SEGx8D
          seg7_mode(disp_mode, 0, 8);
          if (nm_is_on) disp_mode = 7;
          if (disp_mode > 5 ) disp_mode = 0;

          m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness

          for (uint8_t i = 0; i < 8; i++)
          {
            byte a = 0x1;
            for (uint8_t y = 0; y < 8; y++)
            {
              m7219 -> drawPixel(i, y, (screen[i] & a));
              a <<= 1;
            }
          }
          m7219 -> write(); // Send bitmap to display
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
          break;
        case 23:
          // M32x16COLOR
          break;
        case 24:
          // M64x32COLOR
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
    case 3:
      // 595
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 20:
          // M32x8MONO
          break;
        case 21:
          // m32x16MONO
          break;
        case 22:
          // M32x16BICOL
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
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
    case 4:
      // HT1621
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 20:
          // M32x8MONO
          break;
        case 21:
          // m32x16MONO
          break;
        case 22:
          // M32x16BICOL
          break;
        case 23:
          // M32x16COLOR
          break;
        case 24:
          // M64x32COLOR
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
    case 5:
      // HT1632
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 20:
          // M32x8MONO
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
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
    case 6:
      // ILI9341
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 20:
          // M32x8MONO
          break;
        case 21:
          // m32x16MONO
          break;
        case 22:
          // M32x16BICOL
          break;
        case 23:
          // M32x16COLOR
          break;
        case 24:
          // M64x32COLOR
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          if (disp_on) ili_time();
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
    case 11:
      // HT16K33
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 20:
          // M32x8MONO
          break;
        case 21:
          // m32x16MONO
          break;
        case 22:
          // M32x16BICOL
          break;
        case 23:
          // M32x16COLOR
          break;
        case 24:
          // M64x32COLOR
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          seg7_mode(0, 0, 4);
          if (nm_is_on) disp_mode = 9;
          if (disp_mode > 9 ) disp_mode = 1;
          seg7_mode(disp_mode, 4, 6);
          sprintf(tstr, "%02u", second());
          printDisplay_S(tstr, 10, 11);

          if (blinkColon) printDot(2);

          ht1633->setBrightness(cur_br);
          ht1633_ramFormer();
          ht1633->write();
          break;
      }
      break;
    case 12:
      // PCF8574
      switch (conf_data.type_disp)
      {
        case 1:
          // 7SEGx4D
          break;
        case 2:
          // 7SEGx6D
          break;
        case 3:
          // 7SEGx8D
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
          break;
        case 21:
          // m32x16MONO
          break;
        case 22:
          // M32x16BICOL
          break;
        case 23:
          // M32x16COLOR
          break;
        case 24:
          // M64x32COLOR
          break;
        case 25:
          // M64x64COLOR
          break;
        case 29:
          // 320x240COLOR
          break;
        case 30:
          // CUSTOM_1
          break;
        case 31:
          // CUSTOM_2
          break;
      }
      break;
  }

}
