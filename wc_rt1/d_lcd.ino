
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_init()
{
#if defined(ESP8266) || defined(ESP32)
  lcd = new LiquidCrystal_I2C(ram_data.lcd_addr, lcd_col, lcd_row);
  lcd -> init();
#endif

#if defined(BOARD_RTL8710) || defined(BOARD_RTL8195A) || defined(BOARD_RTL8711AM)
  lcd = new LiquidCrystal_I2C(ram_data.lcd_addr);
  lcd -> begin(lcd_col, lcd_row);               // initialize the lcd
#endif

  lcd -> backlight(); //Включаем подсветку

  lcd -> setCursor(5, 0);
  if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Привет"));
  else lcd -> print("Hello");
  lcd -> setCursor(5, 1);
  if (conf_data.rus_disp) lcd -> print(fsys.lcd_rus("Мир!!!"));
  else lcd -> print("World");
}

void Time_LCD()
{
  // Displays the current date and time, and also an alarm indication
  //      22:59:10 16:30 A
  lcd -> setCursor(0, 1);
  bool alarmed = rtc_data.a_hour < 24 && rtc_data.a_min < 59;
  lcd -> print(f_dsp.form_time_string(hour(), minute(), second(), rtc_data.a_hour, rtc_data.a_min, alarmed, conf_data.rus_disp));
}
