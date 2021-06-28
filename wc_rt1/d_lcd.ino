
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

  byte _h = hour()%100, _m = minute()%100, _s = second()%100, ah = rtc_data.a_hour%100, am = rtc_data.a_min%100;
  char msg[16];
  snprintf(msg, 16, " %2u:%02u:%02u  -:-  ", _h, _m, _s);
  if (_alarmed)
  {
    snprintf(msg, 16, " %2u:%02u:%02u %2u:%02u",_h, _m, _s, ah, am);
    if (conf_data.rus_lng) snprintf(msg, 16, " %2u:%02u:%02u %2u:%02u\355", _h, _m, _s, ah, am);
  }
  lcd -> print(msg);
}
