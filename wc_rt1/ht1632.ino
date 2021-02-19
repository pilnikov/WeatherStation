
void ht1632_init()
{
#if defined(ESP8266) || defined(ESP32)

  digHt = 16; // Высота матрици в пикселях

  m1632 = new HT1632Class(CLK_PIN, CS_PIN, WRCLK_PIN, DIO_PIN);
  m1632 -> setBrightness(12); // Use a value between 0 and 15 for brightness
  m1632 -> cp437(true);
  m1632 -> setTextSize(1);
  m1632 -> setTextWrap(false); // Allow text to run off right edge

  m1632 -> clear();

  st1 = "Hello";
  if (conf_data.rus_lng) st1 = "Салют";

  m1632 -> setCursor(2, 0);
  m1632 -> setTextColor(1, 3);
  f_dsp.utf8rus(st1);
  m1632 -> print(st1);
  m1632 -> render();

  st1 = "World!";
  if (conf_data.rus_lng) st1 = " Мир! ";

  m1632 -> setCursor(0, 8);
  m1632 -> setTextColor(0, 3);
  f_dsp.utf8rus(st1);
  m1632 -> print(st1);
  m1632 -> render();

  delay(300);
#endif
}


void m1632_time()
{
  if (cur_br != cur_br_buf)
  {
    m1632 -> setBrightness(cur_br);
    cur_br_buf = cur_br;
  }
  m1632 -> clear();
  //*----------------------------------------------------------------------
  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (conf_data.use_pm && hour() > 12) h = hour() - 12;
#ifdef font4pt

  if (h > 9)  d[0] = h / 10 + '0';
  d[1] = h % 10 + '0';
  d[2] = minute() / 10 + '0';
  d[3] = minute() % 10 + '0';
  d[4] = second() / 10 + '0';
  d[5] = second() % 10 + '0';

  if (h > 9) m1632 -> drawChar(0, 8, d[0], 0, 3, 1);

  m1632 -> drawChar(4, 8, d[1], 0, 3, 1);

  m1632 -> drawChar(11, 8, d[2], 0, 3, 1);
  m1632 -> drawChar(16, 8, d[3], 0, 3, 1);

  m1632 -> drawChar(23, 8, d[4], 0, 3, 1);
  m1632 -> drawChar(28, 8, d[5], 0, 3, 1);

  switch (colon)
  {
    case 1:
      m1632 -> drawPixel ( 9, 10, 2); /* - */
      m1632 -> drawPixel ( 9, 11, 2); /* - */

      m1632 -> drawPixel (21, 10, 2); /* - */
      m1632 -> drawPixel (21, 11, 2); /* - */
      break;
    case 2:
      m1632 -> drawPixel ( 9, 11, 2); /* \ */
      m1632 -> drawPixel (10, 11, 2); /* / */

      m1632 -> drawPixel (21, 11, 2); /* \ */
      m1632 -> drawPixel (22, 11, 2); /* / */
      break;
    case 3:
      m1632 -> drawPixel (10, 11, 3); /* / */
      m1632 -> drawPixel ( 9, 11, 2); /* | */
      m1632 -> drawPixel ( 9, 12, 2); /* | */

      m1632 -> drawPixel (22, 11, 3); /* / */
      m1632 -> drawPixel (21, 11, 2); /* | */
      m1632 -> drawPixel (21, 12, 2); /* | */
      break;
    case 4:
      m1632 -> drawPixel ( 8, 11, 2); /* / */
      m1632 -> drawPixel ( 9, 11, 2); /* / */

      m1632 -> drawPixel (20, 11, 2); /* / */
      m1632 -> drawPixel (21, 11, 2); /* / */
      break;
  }
#endif

#ifdef new_max
  //----------------------------------------------------------------------

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (!(i == 0 && h < 9))
    {
      m1632 -> drawPartChar(digPos_x[i],   8,   screen[i], 0, 3,   digPos_y[i]); // набегает
      if (digPos_y[i] != 7)
      {
        m1632 -> drawPartChar(digPos_x[i], 8, oldDigit[i], 0, 3, - digPos_y[i]); // убегает
        /*
          DBG_OUT_PORT.print("pos - ");
          DBG_OUT_PORT.println(digPos_y[i]);
          DBG_OUT_PORT.print("sym - ");
          DBG_OUT_PORT.println(oldDigit[i]);
        */
        digPos_y[i]++; // опускает символы
      }
    }
  }
#endif
  m1632 -> render(); // Send bitmap to display
}
