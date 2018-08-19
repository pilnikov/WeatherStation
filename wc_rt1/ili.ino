
void ili_init()
{
  //tft = new Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
  tft = new Adafruit_ILI9341(5, 16, 23, 18, 17, 19);
  tft -> begin();
  tft -> setRotation(3);
}

const char* dstAbbrev = "RTZ+5";
const uint8_t SCREEN_WIDTH = 240;
const uint8_t SCREEN_HEIGHT = 320;

void ili_time(void) {
  char time_str[11];

  tft -> fillRect(0, 0, SCREEN_WIDTH, 55, ILI9341_BLACK);

  tft -> setTextAlignment(TEXT_ALIGN_CENTER);
  tft -> setTextSize(2);
  tft -> setTextColor(ILI9341_WHITE);
  char date[30];

  if (conf_data.rus_disp)
  {
    sprintf(date, "%s %2d %s %4d г.", f_dsp.dow_name_r(weekday()), day(), f_dsp.month_name_r(month()), year());
  }
  else
  {
    sprintf(date, "%s %s %2d %4d", dayStr(weekday()), monthStr(month()), day(), year());
  }
  tft -> setCursor(SCREEN_WIDTH / 2, 6);
  tft -> print(date);

  tft -> setTextSize(5);

  if (conf_data.use_pm)
  {
    uint8_t hr = (hour() + 11) % 12 + 1; // take care of noon and midnight
    sprintf(time_str, "%2d:%02d:%02d\n", hr, minute(), second());
    tft -> setCursor(SCREEN_WIDTH / 2, 20);
    tft -> print(time_str);
  }
  else
  {
    sprintf(time_str, "%02d:%02d:%02d\n", hour(), minute(), second());
    tft -> drawString(SCREEN_WIDTH / 2, 20, time_str);
  }

  tft -> setTextAlignment(TEXT_ALIGN_LEFT);
  tft -> setTextSize(2);
  tft -> setTextColor(ILI9341_BLUE);
  if (conf_data.use_pm)
  {
    sprintf(time_str, "%s\n%s", dstAbbrev, hour() >= 12 ? "PM" : "AM");
    tft -> setCursor(195, 27);
    tft -> print(time_str);
  }
  else
  {
    sprintf(time_str, "%s", dstAbbrev);
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
  tft -> setTextAlignment(TEXT_ALIGN_RIGHT);
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
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) return 0;
  else if (dbm >= -50) return 100;
  else return 2 * (dbm + 100);
}


