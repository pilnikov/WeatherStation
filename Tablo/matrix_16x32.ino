
void matrix_init()
{
  matrix = new RGBmatrixPanel(A, B, C, CLK, LAT, OE, true);
  matrix -> begin();
  matrix -> cp437(true);
  matrix -> setTextSize(tsize);
  matrix -> setTextWrap(false); // Allow text to run off right edge
}

void _matrix(uint8_t bright)
{
  uint8_t h = hour();
  matrix -> fillScreen(0);

  if (h < 7 || h >= 17) //гашение экрана в нерабочее время
  {
    bright = 50;
    matrix -> drawPixel(h, 0, matrix -> ColorHSV(500, 255, 100, true));
    matrix -> drawPixel(minute() / 2, 15, matrix -> ColorHSV(500, 255, 100, true));
    if (minute() % 10 == 0 && second() == 0) synchro();
  }
  else
  {
    //Отображаем бегущую строку
    mx_mov_str (st1, 0, 5);

#ifdef font5pt
    //Отображаем часы
    matrix -> setTextColor(matrix -> ColorHSV(400, 255, bright, true));
    render_number_in_place (h, 7, 8);

    //Отображаем двоеточие
    if (second() % 10 % 2 == 0)
    {
      matrix -> drawPixel(15, 11, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(16, 11, matrix -> ColorHSV(200, 255, bright, true));
      //matrix -> drawPixel(15, 13, matrix -> ColorHSV(200, 255, bright, true));
      //matrix -> drawPixel(16, 13, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(15, 12, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(16, 12, matrix -> ColorHSV(200, 255, bright, true));
    }
    else
    {
      matrix -> drawPixel(15, 9, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(16, 9, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(15, 10, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(16, 10, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(15, 13, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(16, 13, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(15, 14, matrix -> ColorHSV(200, 255, bright, true));
      matrix -> drawPixel(16, 14, matrix -> ColorHSV(200, 255, bright, true));
    }

    //Отображаем минуты

    matrix -> setTextColor(matrix -> ColorHSV(400, 255, bright, true));
    render_00number_in_place (minute(), 20, 8);

    //Отображаем секунды

    int a = second();
    if (second() > 29) a = 60 - second();
    matrix -> drawPixel(a, 15, matrix -> ColorHSV(800, 255, bright, true));

#endif

#ifdef font4pt
    //----------------------------------------------------------------------

    matrix -> setTextColor(matrix -> ColorHSV(400, 255, bright, true));

    for (int i = 0; i < num; i++)
    {
      if (!(i == 0 && h < 9))
      {
        matrix -> drawChar (digPos[i], 8, d[i], matrix -> ColorHSV(400, 255, bright, true), 0, 1);
      }
    }

    switch (millis() / 250 % 10)
    {
      case 1:
        matrix -> drawLine (  8, 11, 10, 11, matrix -> ColorHSV(200, 255, bright, true)); // -
        matrix -> drawLine ( 20, 11, 22, 11, matrix -> ColorHSV(200, 255, bright, true)); // -
        break;
      case 2:
        matrix -> drawLine (  8, 10, 10, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /

        matrix -> drawLine ( 20, 10, 22, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        break;
      case 3:
        matrix -> drawLine (  9, 10,  9, 12, matrix -> ColorHSV(200, 255, bright, true)); // |
        matrix -> drawLine ( 21, 10, 21, 12, matrix -> ColorHSV(200, 255, bright, true)); // |
        break;
      case 4:
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        break;
      case 5:
        matrix -> drawLine (  8, 11, 10, 11, matrix -> ColorHSV(200, 255, bright, true)); // -
        matrix -> drawLine ( 20, 11, 22, 11, matrix -> ColorHSV(200, 255, bright, true)); // -
        break;
      case 6:
        matrix -> drawLine (  8, 10, 10, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /

        matrix -> drawLine ( 20, 10, 22, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        break;
      case 7:
        matrix -> drawLine (  9, 10,  9, 12, matrix -> ColorHSV(200, 255, bright, true)); // |
        matrix -> drawLine (  8, 11, 10, 11, matrix -> ColorHSV(200, 255, bright, true)); // -

        matrix -> drawLine ( 21, 10, 21, 12, matrix -> ColorHSV(200, 255, bright, true)); // |
        matrix -> drawLine ( 20, 11, 22, 11, matrix -> ColorHSV(200, 255, bright, true)); // -
        break;
      case 8:
        matrix -> drawLine (  8, 10, 10, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /

        matrix -> drawLine ( 20, 10, 22, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        break;
      case 9:
        matrix -> drawLine (  9, 10,  9, 12, matrix -> ColorHSV(200, 255, bright, true)); // |
        matrix -> drawLine (  8, 11, 10, 11, matrix -> ColorHSV(200, 255, bright, true)); // -

        matrix -> drawLine ( 21, 10, 21, 12, matrix -> ColorHSV(200, 255, bright, true)); // |
        matrix -> drawLine ( 20, 11, 22, 11, matrix -> ColorHSV(200, 255, bright, true)); // -
        break;
      case 0:
        matrix -> drawLine (  8, 10, 10, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        matrix -> drawLine (  8, 12, 10, 10, matrix -> ColorHSV(200, 255, bright, true)); // /

        matrix -> drawLine ( 20, 10, 22, 12, matrix -> ColorHSV(200, 255, bright, true)); // \
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        matrix -> drawLine ( 20, 12, 22, 10, matrix -> ColorHSV(200, 255, bright, true)); // /
        break;
    }
#endif

#ifdef new_max
    //----------------------------------------------------------------------
    for (int i = 0; i < num; i++)
    {
      if (!(i == 0 && h < 9))
      {
        matrix -> drawPartChar(digPos[i],   8,        d[i], matrix -> ColorHSV(700, 255, cur_br, true), 0,   digtrans[i]); // набегает
        if (digtrans[i] != 7)
        {
          matrix -> drawPartChar(digPos[i], 8, digoldig[i], matrix -> ColorHSV(700, 255, cur_br, true), 0, - digtrans[i]); // убегает
          /*
            DBG_OUT_PORT.print("pos - ");
            DBG_OUT_PORT.println(digtrans[i]);
            DBG_OUT_PORT.print("sym - ");
            DBG_OUT_PORT.println(digoldig[i]);
          */
          digtrans[i]++; // опускает символы
        }
      }
    }
#endif

#ifdef _debug
    char msg[8];
    sprintf(msg, "%2u:%02u:%02u", hour(), minute(), second());
    DBG_OUT_PORT.println(msg);
#endif
  }
  matrix -> swapBuffers(true);
}


