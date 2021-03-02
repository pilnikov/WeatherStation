
void ht1632_init()
{
  m1632 = new HT1632C(WRCLK_PIN, /*clk*/ CS_PIN /*cs*/);
  m1632 -> setFrequency(100000);
}

void ht1632_ramFormer(byte *in, const uint8_t color1, const uint8_t color2)
{
  for (uint8_t x = 0; x < 32; x++)
  {
    uint8_t dt = 0b1;
    for (uint8_t y = 0; y < 8; y++)
    {
      m1632 -> plot(x, y, (in[x] & dt << y) ? color1 : BLACK);
      m1632 -> plot(x, y + 8, (in[x + 32] & dt << y) ? color2 : BLACK);
    }
  }
}
