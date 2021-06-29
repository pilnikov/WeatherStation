
void m7adopt(byte*, uint8_t, uint8_t);

const uint8_t lcd_row = 2;
const uint8_t lcd_col = 16;


void m7219_init()
{
  char tstr[255];

  if (conf_data.type_disp < 10) m7219 = new Max72(conf_data.gpio_dcs, 1, 1);
  else m7219 = new Max72(conf_data.gpio_dcs, 1, 4);

  m7219 -> begin();
  f_dsp.CLS(screen);

  if (conf_data.type_disp == 20)
  {
    st1 = "Hello";
    if (conf_data.rus_lng) st1 = "Салют";

    f_dsp.utf8rus(st1);

    strncpy(tstr, st1.c_str(), 6);

    f_dsp.print_(tstr, strlen(tstr), screen, 0, font5x7, 5, 1);
    m7219_ramFormer(screen);
  }

  if (conf_data.type_disp > 0 && conf_data.type_disp < 10)
  {
    st1 = "7219";
    strncpy(tstr, st1.c_str(), 6);
    f_dsp.print_(tstr, 5, screen, 0, font14s, 2, 0);
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
        b[z] |= buff[24 - x + y] & a ? 0x1 << (7 - y) : 0x0; // поворот каждой матрицы на 90 градусов против часовой стрелки
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
  f_dsp.compressor7(in, x1, x2);
  for (uint8_t i = x1; i < x2; i++)
  {
    f_dsp.mir_seg(in[i]); //position on the display
  }
  m7219 -> setRam(in, x2 - x1);
}


////////////////////////////////////////////m3264///////////////////////////////////////////////////////////////
void a595_init()
{

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
    char tstr[255];

    m3216 = new RGBmatrixPanel(A_PIN, B_PIN, C_PIN, CLK_PIN, LAT_PIN, OE_PIN, true);
    m3216 -> begin();
    m3216 -> cp437(true);
    m3216 -> setTextSize(1);
    m3216 -> setTextWrap(false); // Allow text to run off right edge

    st1 = "Hello";
    if (conf_data.rus_lng) st1 = "Салют";

    f_dsp.utf8rus(st1);

    strncpy(tstr, st1.c_str(), 6);

    f_dsp.print_(tstr, strlen(tstr), screen, 0, font5x7, 5, 1);
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



//////////////////////////////////////////lcd//////////////////////////////////////////////////////////////////////
void pcf8574_init()
{
#if defined(__xtensa__)
  lcd = new LiquidCrystal_I2C(ram_data.lcd_addr, lcd_col, lcd_row);
  lcd -> init();
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
  lcd = new LiquidCrystal_I2C(ram_data.lcd_addr);
  lcd -> begin(lcd_col, lcd_row);               // initialize the lcd
#endif

  lcd -> backlight(); //Включаем подсветку

  st1 = "Hello";
  if (conf_data.rus_lng) st1 = "Привет";
  lcd -> setCursor(5, 0);
  f_dsp.lcd_rus(st1);
  lcd -> print (st1);
  st1 = "World";
  if (conf_data.rus_lng) st1 = "Мир!!!";
  lcd -> setCursor(5, 1);
  f_dsp.lcd_rus(st1);
  lcd -> print (st1);
}

void lcd_time()
{
  // Displays the current date and time, and also an alarm indication
  //      22:59:10 16:30 A
  lcd -> setCursor(0, 1);
  bool _alarmed = rtc_data.a_hour < 24 && rtc_data.a_min < 59;

  byte _h = hour() % 100, _m = minute() % 100, _s = second() % 100, ah = rtc_data.a_hour % 100, am = rtc_data.a_min % 100;
  char msg[16];
  snprintf(msg, 16, " %2u:%02u:%02u  -:-  ", _h, _m, _s);
  if (_alarmed)
  {
    snprintf(msg, 16, " %2u:%02u:%02u %2u:%02u", _h, _m, _s, ah, am);
    if (conf_data.rus_lng) snprintf(msg, 16, " %2u:%02u:%02u %2u:%02u\355", _h, _m, _s, ah, am);
  }
  lcd -> print(msg);
}


///////////////////////////////////////////////////7seg////////////////////////////////////////////////////////////
void tm1637_init()
{
  tm1637 = new TM1637(conf_data.gpio_clk, conf_data.gpio_dio);
  tm1637->clear();
  tm1637->set_br(7);
}


void ht1621_init()
{
  ht21 = new HT1621(conf_data.gpio_dcs, conf_data.gpio_clk, conf_data.gpio_dio); // ss, rw, data
  ht21->begin();

  ht21->sendCommand(HT1621::RC256K);
  ht21->sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht21->sendCommand(HT1621::SYS_EN);
  ht21->sendCommand(HT1621::LCD_ON);

  uint8_t i = 0;

  ht21->clear_all();// clear memory
  f_dsp.CLS(screen);

  h_dsp.digit(1, 1, screen); /* 1 */
  h_dsp.digit(3, 2, screen); /* 2 */
  h_dsp.digit(5, 6, screen); /* 6 */
  h_dsp.digit(7, 1, screen); /* 1 */

  for (i = 1; i < 8; i++)
  {
    ht21->write(i, screen[i]);

    if (i < 7) h_dsp.bat(i - 1, screen);
    ht21->write(21, screen, 2);
    delay(250);
  }
}

////////////////////////////////////ht1632//////////////////////////////////////////////////////////////////

void ht1632_init()
{
  m1632 = new HT1632C(conf_data.gpio_dwr, /*clk*/ conf_data.gpio_dcs /*cs*/);
  m1632 -> setFrequency(100000);
}

void ht1632_ramFormer(byte *in, const uint8_t color1, const uint8_t color2)
{
  for (uint8_t x = 0; x < 32; x++)
  {
    uint8_t dt = 0b1;
    for (uint8_t y = 0; y < 8; y++)
    {
      m1632 -> plot(x, y, (in[x] & dt << y) ? color1 : 0);
      m1632 -> plot(x, y + 8, (in[x + 32] & dt << y) ? color2 : 0);
    }
  }
}


///////////////////////////////ht1633/////////////////////////////////////////////////////////////////////////////
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

  uint16_t _row[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  for (uint8_t i = x1; i < x2; i++)
  {
    byte seg_roll = in[i];
    if (i < 4) f_dsp.roll_seg(seg_roll);

    if (i < 8) _row[i] |= (seg_roll & 0xFF); // Позиции с 0 по 7
    else
    {
      if (i < 10)  // Позиции 8, 9
      {
        _row[9 - i] |= (seg_roll & 0xFF) << 8;
      }
      else
      {
        seg_roll = in[i * 2 + 1];

        if (i == 10) // Старший байт позиции 10
        {
          _row[7] |= (in[i * 2] & 0xFF) << 8;
        }
        else // Старший байт позиций 11,12
        {
          _row[14 - i] |= (in[i * 2] & 0xFF) << 8;
        }
        // Младший байт позиций 10 - 12
        _row[i - 6] |= (seg_roll & 0xFF) << 8;
      }
    }
  }
  for (uint8_t i = 0; i < 8; i++) ht1633->setRow(i, _row[i]);
}

void ht1633_ramFormer2(byte *in, uint8_t x1, uint8_t x2)
{
  uint16_t _row = 0;
  if  (x1 < 0 || x2 > 8) return;

  for (uint8_t i = x1, y = x1; i < x2; i++, y++)
  {
    _row = 0;

    if (in[y] & 0x80) // запись точки
    {
      _row  = (in[y - 2] << 8) | 0x8000;
      _row |= (in[y - 1] & 0xFF);
      ht1633->setRow(i - 1, _row);
      y += 2;
    }

    _row = in[y] << 8;
    y++;
    if (in[y] & 0x80) _row |= 0x8000; // если во входящем буфере была точка - запишем ее
    _row |= (in[y] & 0xFF);
    ht1633->setRow(i, _row);
  }
}

////////////////////////////////////////////ili9341/////////////////////////////////////////////////

void ili9341_init()
{
  //tft = new Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
  tft = new Adafruit_ILI9341(5, 16, 23, 18, 17, 19);
  tft -> begin();
  tft -> setRotation(3);
}

const char* dstAbbrev = "RTZ+5";
const uint16_t SCREEN_WIDTH = 240;
const uint16_t SCREEN_HEIGHT = 320;

void ili_time(void)
{
  char time_str[11];

  tft -> fillRect(0, 0, SCREEN_WIDTH, 55, ILI9341_BLACK);

  //  tft -> setTextAlignment(TEXT_ALIGN_CENTER);
  tft -> setTextSize(2);
  tft -> setTextColor(ILI9341_WHITE);
  char date[30];

  if (conf_data.rus_lng)
  {
    //    snprintf(date, 20, "%s %2d %s %4dг.", sdnr[weekday()], day(), smnr[month()], year());
  }
  else
  {
    //    snprintf(date, 20, "%s %s %2d %4d", dayStr(weekday()), monthStr(month()), day(), year());
  }
  tft -> setCursor(SCREEN_WIDTH / 2, 6);
  tft -> print(date);

  tft -> setTextSize(5);

  if (conf_data.use_pm)
  {
    //uint8_t h = (hour() + 11) % 12 + 1; // take care of noon and midnight
    //snprintf(time_str, "%2d:%02d:%02d\n", h, minute(), second());
    tft -> setCursor(SCREEN_WIDTH / 2, 20);
    tft -> print(time_str);
  }
  else
  {
    //    snprintf(time_str, "%02d:%02d:%02d\n", hour(), minute(), second());
    //    tft -> drawString(SCREEN_WIDTH / 2, 20, time_str);
  }

  //  tft -> setTextAlignment(TEXT_ALIGN_LEFT);
  tft -> setTextSize(2);
  tft -> setTextColor(ILI9341_BLUE);
  if (conf_data.use_pm)
  {
    //    snprintf(time_str, "%s\n%s", dstAbbrev, hour() >= 12 ? "PM" : "AM");
    tft -> setCursor(195, 27);
    tft -> print(time_str);
  }
  else
  {
    //    snprintf(time_str, "%s", dstAbbrev);
    tft -> setCursor(195, 27);
    tft -> print(time_str);
  }
  drawWifiQuality();
}

void drawWifiQuality()
{
  int8_t quality = getWifiQuality();
  tft -> setTextColor(ILI9341_WHITE);
  tft -> setTextSize(1);
  //  tft -> setTextAlignment(TEXT_ALIGN_RIGHT);
  tft -> setCursor(228, 9);
  tft -> print(String(quality) + "%");

  for (int8_t i = 0; i < 4; i++)
  {
    for (int8_t j = 0; j < 2 * (i + 1); j++) if (quality > i * 25 || j == 0) tft -> drawPixel(230 + 2 * i, 18 - j, ILI9341_WHITE);
  }
}

// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality()
{
  int32_t dbm = -200;

#if defined(__xtensa__)
  dbm = WiFi.RSSI();
#endif

  if (dbm <= -100) return 0;
  else if (dbm >= -50) return 100;
  else return 2 * (dbm + 100);
}
