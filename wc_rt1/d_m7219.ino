#include "fonts.h"


void m7219_init()
{
  digHt = 8;  // Высота матрицы в пикселях

  if (conf_data.type_disp >= 1 && conf_data.type_disp <= 3) m7219 = new Max72xxPanel(CS_PIN, 1, 1);
  else m7219 = new Max72xxPanel(CS_PIN, 1, 4);

  m7219 -> begin();
/*
  if (conf_data.type_disp >= 1 && conf_data.type_disp <= 3) m7219 -> setRotation(0, 0);
  else
  { m7219 -> setRotation(m7219 -> getRotation() + 3); //1 - 90  2 - 180   3 - 270
    m7219 -> cp437(true);
    m7219 -> setTextSize(1);
    m7219 -> setTextWrap(false); // Allow text to run off right edge
  }
  m7219 -> fillScreen(LOW);
  m7219 -> setCursor(1, 0);
*/
  st1 = "Hello";
  if (conf_data.rus_lng) st1 = "Салют";

  f_dsp.utf8rus(st1);

  //m7219 -> print(st1);
 
  strcpy((char*)tstr, st1.c_str());
 
  print_m32_8(tstr, st1.length(), screen);
  m7219_ramFormer(screen);
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
  m7219 -> setRam(buff, sizeof(buff));
}
