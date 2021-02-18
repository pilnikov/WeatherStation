#include "fonts.h"

/*
  table of correspondence between digitNum and row numbers
*/
uint8_t d_r [8] {9, 8, 15, 14, 10, 11, 12, 13};

/*
   Function prototypes
*/
void ht1633_init(void);
void ht1633_ramFormer(byte *);


void ht1633_init()
{
  ht1633 = new HT16K33;
  ht1633->init(ram_data.ht_addr);
  ht1633->setBrightness(14);
  ht1633->clear();
  ht1633->write();
}


void ht1633_ramFormer(byte *in)
{
  uint16_t _row[8];

  for (uint8_t i = 0; i < 8; i++) //seg
  {
    if (i < 4) f_dsp.roll_seg(in[i]);

    _row[i] = in[d_r[i]];
    _row[i] <<= 8;
    _row[i] |= in[i];
    ht1633->setRow(i, _row[i]);
  }
}
