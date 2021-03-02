#include "fonts.h"

/*
   Function prototypes
*/
void ht1633_init(void);
void ht1633_ramFormer(byte*, uint8_t, uint8_t);
void ht1633_ramFormer2(byte*, uint8_t, uint8_t);


void ht1633_init()
{
  ht1633 = new HT16K33;
  ht1633->init(ram_data.ht_addr);
  ht1633->setBrightness(14);
  ht1633->clear();
  ht1633->write();
}


void ht1633_ramFormer(byte *in, uint8_t x1, uint8_t x2)
{
  /*
     seg  0 row 0 0x __xx 7seg upside down dp in top left  BIG byte in  0  1
     seg  1 row 1 0x __xx 7seg upside down dp in top left  BIG byte in  2  3
     seg  2 row 2 0x __xx 7seg upside down colum           BIG byte in  4  5
     seg  3 row 3 0x __xx 7seg upside down dp in top left  BIG byte in  6  7
     seg  4 row 4 0x __xx 7seg normal                          byte in  8  9
     seg  5 row 5 0x __xx 7seg normal dp in bot right          byte in 10 11
     seg  6 row 6 0x __xx 7seg normal                          byte in 12 13
     seg  7 row 7 0x __xx 7seg normal dp in bot right          byte in 14 15
     seg  8 row 1 0x xx__ 7seg normal                          byte in 16 17
     seg  9 row 0 0x xx__ 7seg normal dp in bot right          byte in 18 19
     seg 10 row 4 0x xx__ (segments a - g1) row 7 0x xx__ (segments g - m) 14 seg normal                                          byte in 20 21
     seg 11 row 5 0x xx__ (segments a - g1) row 3 0x xx__ (segments g2, k) 14 seg normal (segments h, i, j, l, m is not present)  byte in 22 23
     seg 12 row 6 0x xx__ (segments a - g1) row 2 0x xx__ (segments g - m) 14 seg normal (segments       j,    m is not present)  byte in 24 25

     table of correspondence between digitNum and row numbers
  */
  //uint8_t d_r [8] = {9, 8, 15, 14, 10, 11, 12, 13};

  if  (x1 < 0 || x2 > 13) return;

  uint16_t _row = 0;

  for (uint8_t i = x1; i < x2; i++)
  {
    if (i < 4) f_dsp.roll_seg(in[i]);

    if (i < 8) // Позиции с 0 по 7
    {
      _row = (in[i * 2 + 1] & 0xFF);
      ht1633->setRow(i, _row);
    }
    else
    {
      if (i < 10)  // Позиции 8, 9
      {
        _row = (in[i * 2 + 1] & 0xFF) << 8;
        ht1633->setRow(9 - i, _row);
      }
      else
      {
        if (i == 10) // Старший байт позиции 10
        {
          _row = (in[i * 2] & 0xFF) << 8;
          ht1633->setRow(7, _row);
        }
        else // Старший байт позиций 11,12
        {
          _row = (in[i * 2] & 0xFF) << 8;
          ht1633->setRow(14 - i, _row);
        }
        // Младший байт позиций 10 - 12
        _row = (in[i * 2 + 1] & 0xFF) << 8;
        ht1633->setRow(i - 6, _row);
      }
    }
  }
}

void ht1633_ramFormer2(byte *in, uint8_t x1, uint8_t x2)
{
  uint16_t _row = 0;

  for (uint8_t i = x1, y = x1 * 2; i < x2; i++, y++)
  {
    _row = 0;

    if (in[y] & 0x80) // запись точки
    {
      _row  = (in[y - 2] << 7) | 0x4000;
      _row |= (in[y - 1] & 0x7F);
      ht1633->setRow(i - 1, _row);
      y += 2;
    }

    _row = in[y] << 7;
    y++;
    if (in[y] & 0x80) _row |= 0x4000; // если во входящем буфере была точка - запишем ее
    _row |= (in[y] & 0x7F);
    ht1633->setRow(i, _row);
  }
}
