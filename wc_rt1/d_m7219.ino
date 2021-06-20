
void m7adopt(byte*, uint8_t, uint8_t);


void m7219_init()
{
  char tstr[255];

  if (conf_data.type_disp < 10) m7219 = new Max72(conf_data.gpio_dcs, 1, 1);
  else m7219 = new Max72(conf_data.gpio_dcs, 1, 4);

  m7219 -> begin();
  CLS(screen);

  if (conf_data.type_disp == 20)
  {
    st1 = "Hello";
    if (conf_data.rus_lng) st1 = "Салют";

    f_dsp.utf8rus(st1);

    strncpy(tstr, st1.c_str(), 6);

    print_(tstr, strlen(tstr), screen, 0, font5x7, 5, 1);
    m7219_ramFormer(screen);
  }

  if (conf_data.type_disp > 0 && conf_data.type_disp < 10)
  {
    st1 = "7219";
    strncpy(tstr, st1.c_str(), 6);
    print_(tstr, 5, screen, 0, font14s, 2, 0);
    m7adopt(screen, 0, 4);
  }

  m7219 -> write();
}


void m7219_ramFormer(byte *ram_buff)
{
  byte buff[32];

  for (uint8_t x = 0; x < 32; x += 8) // шаг 8 - кол во строк в одном модуле; 32 - всего байт (колонок) в наборе = 4х8 24 = 32 - 8
  {
    byte b[8];
    for (uint8_t z = 0; z < 8; z++)
    {
      b[z] = 0;
    }
    for (uint8_t y = 0; y < 8; y++)
    {
      buff[24 - x + y] = ram_buff[x + y]; // меняет последовательность модулей на зеркальную 0 1 2 3 -> 3 2 1 0   24 = 32 - 8 адреса в последнем модуле
      byte a = 1;
      for (uint8_t z = 0; z < 8; z++)
      {
        b[z] |= buff[24 - x + y] & a ? 0x1 << 7 - y : 0x0; // поворот каждой матрицы на 90 градусов против часовой стрелки
        a <<= 1;
      }
    }
    for (uint8_t z = 0; z < 8; z++)
    {
      buff[24 - x + z] = b[z];
    }
  }
  m7219 -> setRam(buff, 32);
}

void m7adopt(byte *in, uint8_t x1, uint8_t x2)
{
  compressor7(in, x1, x2);
  for (uint8_t i = x1; i < x2; i++)
  {
    f_dsp.mir_seg(in[i]); //position on the display
  }
  m7219 -> setRam(in, x2 - x1);
}
