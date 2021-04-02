
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


#ifdef new_max
    //----------------------------------------------------------------------
    for (int i = 0; i < num; i++)
    {
      if (!(i == 0 && h < 9))
      {
        //        matrix -> drawPartChar(digPos[i],   8,        d[i], matrix -> ColorHSV(700, 255, cur_br, true), 0,   digtrans[i]); // набегает
        if (digtrans[i] != 7)
        {
          //          matrix -> drawPartChar(digPos[i], 8, digoldig[i], matrix -> ColorHSV(700, 255, cur_br, true), 0, - digtrans[i]); // убегает
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
