#include "fonts.h"

// Global message buffers shared by Serial and Scrolling functions

void m7219_init()
{
  //  m7219 = new MAX72XX(MAX72XX(MAX72XX::DR0CR0RR1_HW, 16, 8));

  //  m7219 -> begin();
  SPI.pins(14, -1, 13, 16);
  SPI.begin();
  m7219_1 = new Max72(16, 4, 2);
  m7219_1 -> begin();

  f_dsp.CLS(screen, sizeof screen);

  char Tstr[25];
  memset (Tstr, 0, 25);
  strcpy(Tstr, "1234");

  f_dsp.print_(Tstr, 4, screen, 0, font5x7, 5, 1);

  m7219_ramFormer2(screen, 4, 2);
}

void m7219_ramFormer2(byte *ram_buff, uint8_t hdisp, uint8_t vdisp)
{
  uint8_t qmatrix = vdisp * hdisp;
  uint8_t mSize  = qmatrix << 3;

  uint8_t buff[mSize];

  for (uint8_t x = 0; x < mSize; x += 8) // шаг 8 - кол во строк в одном модуле; mSize - всего байт (колонок) в наборе = hdisp х 8 x vdisp;
  {
    uint8_t b[8];
    memset(b, 0, 8);

    for (uint8_t y = 0; y < 8; y++)
    {
      byte a = 0x1;
      for (uint8_t z = 0; z < 8; z++)
      {
        b[z] |= ram_buff[x + y] & a ? 0x1 << (7 - y) : 0x0; // поворот каждой матрицы на 90 градусов против часовой стрелки
        a <<= 1;
        buff[x + z] = b[z] >> 1;// переставляем биты из 7 6 5 4 3 2 1 0
        buff[x + z] |= b[z] & 0x1 ? 0x80 : 0x0; //    в 6 5 4 3 2 1 0 7
      }
    }
  }
  m7219_1 -> setRam(buff, mSize);
  m7219_1 -> write();

  /*
    m7219 -> control(0, mSize - 1, MAX72XX::UPDATE, MAX72XX::OFF);
    m7219 -> clear();
    m7219 -> transform(0, 7, MAX72XX::TFLR);

    //  m7219 -> setBuffer(31, 32, buff);
    //m7219 -> setBuffer(63, 32, buff + 32);
    uint8_t t_size = 1;
    for (uint8_t x = 0; x < 32; x++)
    {
      uint8_t dt = 0b1;
      for (uint8_t y = 0; y < 8; y++)
      {
        for (uint8_t xz = 0; xz < t_size; xz++)
        {
          uint8_t _x = (x * t_size) + xz;
          for (uint8_t yz = 0; yz < t_size; yz++)
          {
            uint8_t _y = (y * t_size) + yz;
            uint8_t _yy = _y + (8 * t_size);
    #if defined(__AVR_ATmega2560__)
            m7219 -> setPoint(_x, _y, (buff[x] & dt << y) ?  1 : 0);
            m7219 -> setPoint(_x, _yy, (buff[x + 32] & dt << y) ?  1 : 0);
    #elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
            m7219 -> setPoint(_x, _y, (buff[x] & dt << y) ?  1 : 0);
            m7219 -> setPoint(_x, _yy, (buff[x + 32] & dt << y) ?  1 : 0);
    #endif
          }
        }
      }
    }
    m7219 -> control(0, mSize - 1, MAX72XX::UPDATE, MAX72XX::ON);
  */
}
