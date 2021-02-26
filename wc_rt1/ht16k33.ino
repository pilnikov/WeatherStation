#include "fonts.h"

/*
  table of correspondence between digitNum and row numbers
*/
uint8_t d_r [8] {9, 8, 15, 14, 10, 11, 12, 13};

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
  uint16_t _row[8];

  for (uint8_t i = x1, y = x1 * 2; i < x2; i++, y++)
  {
    if (in[y] & 0x80)
    {
      _row[i - 1] |= 0x8080;
      ht1633->setRow(i - 1, _row[i - 1]);
      y += 2;
    }

    if (i < 4) f_dsp.roll_seg(in[y]);

    _row[i] = in[d_r[y]];
    _row[i] <<= 8;
    _row[i] |= in[y];
    ht1633->setRow(i, _row[y]);
  }
}

void ht1633_ramFormer2(byte *in, uint8_t x1, uint8_t x2)
{
  uint16_t _row[8];

  for (uint8_t i = x1, y = x1 * 2; i < x2; i++, y++)
  {
    if (in[y] & 0x80)
    {
      _row[i - 1] |= 0x4000;
      ht1633->setRow(i - 1, _row[i - 1]);
      y += 2;
    }

    _row[i] = in[y];
    _row[i] <<= 7;
    y++;
    if (in[y] & 0x80) _row[i] |= 0x4000;
    _row[i] |= (in[y] & 0x7F);
    ht1633->setRow(i, _row[i]);
  }
}
