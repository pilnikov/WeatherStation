void a595_init()
{
  char tstr[255];

#if defined(ARDUINO_ARCH_ESP32)
  //G1  R1 |
  //GND B1 |
  //G2  R2 |
  //GND B2 |
  //B   A  |
  //D   C  |
  //LAT CLK|
  //GND OE |

  //                             (oe,clk,lat, r1, g1, b1, r2, g2, b2,  a,  b,  c, d)

#endif

#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)
  if (conf_data.type_disp == 23)
  {

    m3216 = new RGBmatrixPanel(A_PIN, B_PIN, C_PIN, CLK_PIN, LAT_PIN, OE_PIN, true);
    m3216 -> begin();
    m3216 -> cp437(true);
    m3216 -> setTextSize(1);
    m3216 -> setTextWrap(false); // Allow text to run off right edge

    st1 = "Hello";
    if (conf_data.rus_lng) st1 = "Салют";

    f_dsp.utf8rus(st1);

    strncpy(tstr, st1.c_str(), 6);

    print_(tstr, strlen(tstr), screen, 0, font5x7, 5, 1);
    m3216_ramFormer(screen);
    m3216 -> swapBuffers(true);
  }
#endif
}

#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)

void m3216_ramFormer(byte *in)
{
  for (uint8_t x = 0; x < 32; x++)
  {
    uint8_t dt = 0b1;
    for (uint8_t y = 0; y < 8; y++)
    {
      m3216 -> drawPixel(x, y, (in[x] & dt << y) ?  m3216 -> ColorHSV(700, 255, cur_br, true) : 0);
      m3216 -> drawPixel(x, y + 8, (in[x + 32] & dt << y) ?  m3216 -> ColorHSV(200, 255, cur_br, true) : 0);
    }
  }
}
#endif
