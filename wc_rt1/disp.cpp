#include "disp.h"
#include "fonts.h"

// ---------------------------------------------------- Display drivers

//---------------------------------------------------------------------------TM1637
TM1637 *tm1637;

//---------------------------------------------------------------------------HT1633
HT16K33 *ht1633;

//---------------------------------------------------------------------------LCD1602
LiquidCrystal_I2C *lcd;

//---------------------------------------------------------------------------MAX7219 4 x 8 x 8 Matrix Display
Max72 *m7219;

//---------------------------------------------------------------------------HT1621
HT1621 *ht21;

//---------------------------------------------------------------------------HT1632
HT1632C *m1632;

//---------------------------------------------------------------------------Matrix
#if defined(__AVR_ATmega2560__)
RGBmatrixPanel *m3216;
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
Adafruit_Protomatter *m3216;
#endif

//---------------------------------------------------------------------------ILI9341
Adafruit_ILI9341 *tft;

FD f_dsp; //For Display
HT h_dsp; //For Display

void m7219_init(byte type_disp, byte gpio_dcs, byte *screen)
{
  if (type_disp < 10) m7219 = new Max72(gpio_dcs, 1, 1);
  else
  {
    if (type_disp == 20) m7219 = new Max72(gpio_dcs, 4, 1);
    if (type_disp == 21) m7219 = new Max72(gpio_dcs, 4, 2);
  }

  m7219 -> begin();
  f_dsp.CLS(screen, sizeof screen);

  if (type_disp > 0 && type_disp < 10)
  {
    char Tstr[25];
    memset (Tstr, 0, 25);
    strcpy(Tstr, "7219");
    f_dsp.print_(Tstr, 5, screen, 0, font14s, 2, 0);
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
  m7219 -> write();
}

void m7219_ramFormer2(byte *ram_buff, uint8_t hdisp, uint8_t vdisp)
{
  uint8_t qmatrix = vdisp * hdisp;
  uint8_t mSize  = qmatrix << 3;

  byte buff[mSize];

  for (uint8_t x = 0; x < mSize; x += 8) // шаг 8 - кол во строк в одном модуле; mSize - всего байт (колонок) в наборе = hdisp х 8 x vdisp;
  {
    byte b[8];
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
  m7219 -> setRam(buff, mSize);
  m7219 -> write();
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
void a595_init(byte type_disp)
{
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2

  if (type_disp == 23 || type_disp == 24 || type_disp == 25)
  {
#if defined(__AVR_ATmega2560__)
    //G1  R1 | 12 04
    //GND B1 |  g 13
    //G2  R2 | 15 14
    //E   B2 | 25 21
    //B   A  | 05 26
    //D   C  | 19 18
    //LAT CLK| 32 27
    //GND OE |  g 33

    uint8_t A_PIN =  54, //A0 Пин A
            B_PIN =  55, //A1 Пин B
            C_PIN =  56, //A2 Пин C
            //            D_PIN =  57, //A3 Пин D

            CLK_PIN =  11,  // Пин CLK MUST be on PORTB! (Use pin 11 on Mega)
            LAT_PIN =  10,  // Пин LAT
            OE_PIN =   9;   // Пин OE
    m3216 = new RGBmatrixPanel(A_PIN, B_PIN, C_PIN, CLK_PIN, LAT_PIN, OE_PIN, true);

#elif CONFIG_IDF_TARGET_ESP32
    uint8_t rgbPins[] = {4, 12, 13, 14, 15, 21},
                        addrPins[] = {26, 5, 18, 19, 25},
                                     clockPin   = 27, // Must be on same port as rgbPins
                                     latchPin   = 32,
                                     oePin      = 33,
                                     naddr_pin  = 3,
                                     wide       = 32;
    if (conf_data.type_disp != 23) wide = 64;
    if (conf_data.type_disp == 24)
    {
      naddr_pin = 4;
      text_size = 2;
    }
    if (conf_data.type_disp == 25)
    {
      naddr_pin = 5;
      text_size = 4;
    }

    m3216 = new Adafruit_Protomatter(
      wide,        // Matrix width in pixels
      6,           // Bit depth -- 6 here provides maximum color options
      1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
      naddr_pin, addrPins, // # of address pins (height is inferred), array of pins
      clockPin, latchPin, oePin, // Other matrix control pins
      true);       // HERE IS THE MAGIG FOR DOUBLE-BUFFERING!

#elif CONFIG_IDF_TARGET_ESP32S2
    uint8_t rgbPins[] = {16, 15, 14, 13, 11, 12},
                        addrPins[] = {34, 33, 36, 35, 16},
                                     clockPin   = 10, // Must be on same port as rgbPins
                                     latchPin   = 9,
                                     oePin      = 21,
                                     naddr_pin  = 3,
                                     wide       = 32;
    if (conf_data.type_disp != 23) wide = 64;
    if (conf_data.type_disp == 24)
    {
      naddr_pin = 4;
      text_size = 2;
    }
    if (conf_data.type_disp == 25)
    {
      naddr_pin = 5;
      text_size = 4;
    }


    m3216 = new Adafruit_Protomatter(
      wide,        // Matrix width in pixels
      6,           // Bit depth -- 6 here provides maximum color options
      1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
      naddr_pin, addrPins, // # of address pins (height is inferred), array of pins
      clockPin, latchPin, oePin, // Other matrix control pins
      true);       // HERE IS THE MAGIG FOR DOUBLE-BUFFERING!#endif

#endif

#if defined(__AVR_ATmega2560__)
    m3216 -> begin();

#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2

    ProtomatterStatus status = m3216 -> begin();

    if (status != PROTOMATTER_OK)
    {
      conf_data.type_vdrv = 0;
      DBG_OUT_PORT.print(F("Failed initialize display - set it to 0..."));
    }
#endif
  }
#endif
}

void m3216_ramFormer(byte *in, uint8_t c_br, uint8_t t_size)
{
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
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
          m3216 -> drawPixel(_x, _y, (in[x] & dt << y) ?  m3216 -> ColorHSV(700, 255, c_br, true) : 0);
          m3216 -> drawPixel(_x, _yy, (in[x + 32] & dt << y) ?  m3216 -> ColorHSV(400, 255, c_br, true) : 0);
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
          m3216 -> drawPixel(_x, _y, (in[x] & dt << y) ?  m3216 -> color565(c_br, 0 , 0) : 0);
          m3216 -> drawPixel(_x, _yy, (in[x + 32] & dt << y) ?  m3216 -> color565(0, c_br, 0) : 0);
#endif
        }
      }
    }
  }
#if defined(__AVR_ATmega2560__)
  m3216 -> swapBuffers(true);
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
  m3216 -> show();
#endif
#endif
}

//////////////////////////////////////////lcd//////////////////////////////////////////////////////////////////////
void pcf8574_init(byte addr, uint8_t lcd_col, uint8_t lcd_row, bool rus_lng)
{
  static char st1[16];

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  lcd = new LiquidCrystal_I2C(addr, lcd_col, lcd_row);
  lcd -> init();
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
  lcd = new LiquidCrystal_I2C(addr);
  lcd -> begin(lcd_col, lcd_row);               // initialize the lcd
#endif

  lcd -> backlight(); //Включаем подсветку

  strcpy(st1, "Hello");
  if (rus_lng) strcpy(st1, "Привет");
  lcd -> setCursor(5, 0);
  f_dsp.lcd_rus(st1);
  lcd -> print (st1);
  strcpy(st1, "World");
  if (rus_lng) strcpy(st1, "Мир!!!");
  lcd -> setCursor(5, 1);
  f_dsp.lcd_rus(st1);
  lcd -> print (st1);
}

void lcd_time(byte *buf, bool t_up)
{
  // Displays the current date and time, and also an alarm indication
  //      22:59:10 16:30 A
  if (t_up) lcd -> setCursor(0, 0);
  else lcd -> setCursor(0, 1);
  lcd -> print((char*)buf);
}



///////////////////////////////////////////////////7seg////////////////////////////////////////////////////////////
void tm1637_init(byte gpio_clk, byte gpio_dio)
{
  tm1637 = new TM1637(gpio_clk, gpio_dio);
  tm1637->clear();
  tm1637->set_br(7);
}

void ht1621_init(byte gpio_dcs, byte gpio_clk, byte gpio_dio, byte* screen)
{
  ht21 = new HT1621(gpio_dcs, gpio_clk, gpio_dio); // ss, rw, data
  ht21->begin();

  ht21->sendCommand(HT1621::RC256K);
  ht21->sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht21->sendCommand(HT1621::SYS_EN);
  ht21->sendCommand(HT1621::LCD_ON);

  uint8_t i = 0;

  ht21->clear_all();// clear memory
  f_dsp.CLS(screen, sizeof screen);

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

void ht1632_init(byte gpio_dwr, byte gpio_dcs)
{
  m1632 = new HT1632C(gpio_dwr, /*clk*/ gpio_dcs /*cs*/);
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


void ht1633_init(byte addr)
{
  ht1633 = new HT16K33;
  ht1633->init(addr);
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
  if  (x1 < 0 || x2 > 7) return;

  for (uint8_t i = x1, y = x1; i <= x2; i++, y++)
  {
    _row = in[y] << 8;
    y++;
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

void ili_time(bool rus_lng, bool use_pm)
{
  char time_str[11];

  tft -> fillRect(0, 0, SCREEN_WIDTH, 55, ILI9341_BLACK);

  //  tft -> setTextAlignment(TEXT_ALIGN_CENTER);
  tft -> setTextSize(2);
  tft -> setTextColor(ILI9341_WHITE);
  char date[30];

  if (rus_lng)
  {
    //    snprintf(date, 20, "%s %2d %s %4dг.", sdnr[rtc_data.wday], rtc_data.day, smnr[rtc_data.month], rtc_data.year);
  }
  else
  {
    //    snprintf(date, 20, "%s %s %2d %4d", dayStr(rtc_data.wday), monthStr(rtc_data.month), rtc_data.day, rtc_data.year);
  }
  tft -> setCursor(SCREEN_WIDTH / 2, 6);
  tft -> print(date);

  tft -> setTextSize(5);

  if (use_pm)
  {
    //uint8_t h = (rtc_data.hour + 11) % 12 + 1; // take care of noon and midnight
    //snprintf(time_str, "%2d:%02d:%02d\n", h, rtc_data.min, rtc_data.sec);
    tft -> setCursor(SCREEN_WIDTH / 2, 20);
    tft -> print(time_str);
  }
  else
  {
    //    snprintf(time_str, "%02d:%02d:%02d\n", rtc_data.hour, rtc_data.min, rtc_data.sec);
    //    tft -> drawString(SCREEN_WIDTH / 2, 20, time_str);
  }

  //  tft -> setTextAlignment(TEXT_ALIGN_LEFT);
  tft -> setTextSize(2);
  tft -> setTextColor(ILI9341_BLUE);
  if (use_pm)
  {
    //    snprintf(time_str, "%s\n%s", dstAbbrev, rtc_data.hour >= 12 ? "PM" : "AM");
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

#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
  //  dbm = WiFi.RSSI();
#endif

  if (dbm <= -100) return 0;
  else if (dbm >= -50) return 100;
  else return 2 * (dbm + 100);
}


void write_dsp(bool from_time, uint8_t type_vdrv, uint8_t type_disp, uint8_t br, bool time_up, byte* screen)
{
  switch (type_vdrv)
  {
    case 0:
      //SERIAL
      break;
    case 1:
      //TM1637
      tm1637->set_br(br);
      for (uint8_t a = 0; a < 4; a++) tm1637->display(a, screen[a]);
      break;
    case 2:
      //MAX7219
      if (from_time)
      {
        if (type_disp < 10)
        {
          m7adopt(screen, 0, 8);
          m7219 -> setIntensity(br); // Use a value between 0 and 15 for brightness
          m7219 -> write();
        }
      }
      else m7219 -> setIntensity(br); // Use a value between 0 and 15 for brightness
      break;
    case 3:
      //595
      break;
    case 4:
      //HT1621
      break;
    case 5:
      //HT1632
      m1632 -> pwm(br);
      m1632 -> sendFrame();
      break;
    case 6:
      //ILI9341
      break;
    case 11:
      //HT16K33
      if (from_time)
      {
        if (type_disp != 31 && type_disp != 11)
        {
          if (type_disp == 13) ht1633_ramFormer2(screen, 0, 7);
          else ht1633_ramFormer2(screen, 0, 3);
        }
      }
      ht1633->setBrightness(br);
      ht1633->write();
      break;
    case 12:
      //PCF8574
      if (from_time) lcd_time(screen, time_up);
      else
      {
        lcd -> setCursor(0, br/*as x1*/);
        lcd -> print((char*)screen/*as st2*/);
      }
      break;
    default:
      break;
  }
}
void display_off(byte type_vdrv, byte type_disp, byte br, byte* screen, uint8_t text_size)
{
  f_dsp.CLS(screen, sizeof screen);
  switch (type_vdrv)
  {
    case 1:
      tm1637->set_br(0);
      tm1637->clear();
      break;
    case 2:
      m7219->shutdown(true);
      m7219->write();
      break;
    case 3:
      if (type_disp == 23 || type_disp == 24 || type_disp == 25)
      {
#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32
        m3216_ramFormer(screen, br, text_size);
#endif
      }
      break;
    case 12:
      lcd->noBacklight();
      lcd->noDisplay();
      break;
    case 11:
      ht1633->clear();
      ht1633->setBrightness(0);
      ht1633->write();
      break;
    default:
      break;
  }
}

void display_on(byte type_vdrv)
{
  switch (type_vdrv)
  {
    case 12:
      lcd->backlight();
      lcd->display();
      break;
    case 2:
      m7219->shutdown(false);
      m7219->write();
      break;
    default:
      break;
  }
}
