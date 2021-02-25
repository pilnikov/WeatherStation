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

  for (uint8_t i = 0, y = 0; i < 8; i++, y++)
  {
    if (in[y] == 0x80)
    {
      _row[i - 1] |= 0x80;
      y++;
    }

    if (i < 4) f_dsp.roll_seg(in[y]);

    _row[i] = in[d_r[y]];
    _row[i] <<= 8;
    _row[i] |= in[y];
    ht1633->setRow(i, _row[y]);
  }
}

void ht1633_ramFormer2(byte *in)
{
  uint16_t _row[8];

  for (uint8_t i = 0, y = 0; i < 8; i++, y++)
  {
    if (in[y] == 0x80)
    {
      _row[i - 1] |= 0x80;
      y++;
    }

    _row[i] = in[y];
    _row[i] <<= 8;
    y++;
    _row[i] |= in[y];
    uint16_t aa = (_row[i] & 0xFF00) >> 1;
    _row[i] & 0x7F;  
    _row[i] |= aa;  
    ht1633->setRow(i, _row[i]);
  }
}
