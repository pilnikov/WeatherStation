
#if defined(ESP32)

/* create a hardware timer */
byte br = 127;
auto white = ESP32RGBmatrixPanel::AdafruitColor(br, br, br);
auto red   = ESP32RGBmatrixPanel::AdafruitColor(br, 0, 0);
auto cyan  = ESP32RGBmatrixPanel::AdafruitColor(0, br, br);
auto blue  = ESP32RGBmatrixPanel::AdafruitColor(0, 0, br);
auto green = ESP32RGBmatrixPanel::AdafruitColor(0, br, 0);

uint8_t mtxt_size = 2, my_coord = 16;

#endif

void a595_init()
{
#if defined(ESP32)
  //G1  R1 |
  //GND B1 |
  //G2  R2 |
  //GND B2 |
  //B   A  |
  //D   C  |
  //LAT CLK|
  //GND OE |

  //                             (oe,clk,lat, r1, g1, b1, r2, g2, b2,  a,  b,  c, d)
  if (conf_data.type_disp == 23)
  {
    mtxt_size = 1;
    my_coord = 8;
  }
  m3264 = new ESP32RGBmatrixPanel(23, 14, 27, 26, 25, 04, 13, 02, 33, 15, 19, 18, 5); //Flexible connection
  m3264 -> setBrightness(4); // Use a value between 0 and 15 for brightness
  m3264 -> cp437(true);
  m3264 -> setTextSize(mtxt_size);
  m3264 -> setTextWrap(false); // Allow text to run off right edge

  m3264 -> black();

  st1 = "Hello";
  if (conf_data.rus_lng) st1 = "Салют";

  m3264 -> setCursor(2, 0);
  m3264 -> setTextColor(white);
  f_dsp.utf8rus(st1);
  m3264 -> print(st1);

  st1 = "World!";
  if (conf_data.rus_lng) st1 = " Мир! ";

  m3264 -> setCursor(2, my_coord);
  m3264 -> setTextColor(red);
  f_dsp.utf8rus(st1);
  m3264 -> print(st1);

  m3264 -> update();

#endif
}

void getRGB(int hue, int sat, int val, int colors[3]) 
{
  int r;
  int g;
  int b;
  int base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0] = val;
    colors[1] = val;
    colors[2] = val;
  }
  else {

    base = ((255 - sat) * val) >> 8;

    switch (hue / 60) {
      case 0:
        r = val;
        g = (((val - base) * hue) / 60) + base;
        b = base;
        break;

      case 1:
        r = (((val - base) * (60 - (hue % 60))) / 60) + base;
        g = val;
        b = base;
        break;

      case 2:
        r = base;
        g = val;
        b = (((val - base) * (hue % 60)) / 60) + base;
        break;

      case 3:
        r = base;
        g = (((val - base) * (60 - (hue % 60))) / 60) + base;
        b = val;
        break;

      case 4:
        r = (((val - base) * (hue % 60)) / 60) + base;
        g = base;
        b = val;
        break;

      case 5:
        r = val;
        g = base;
        b = (((val - base) * (60 - (hue % 60))) / 60) + base;
        break;
    }

    colors[0] = r;
    colors[1] = g;
    colors[2] = b;
  }
}
