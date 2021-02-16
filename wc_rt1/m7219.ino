#include "font5x7.h"


void m7219_init()
{
  digHt = 8;  // Высота матрицы в пикселях

  if (conf_data.type_disp >= 1 && conf_data.type_disp <= 3) m7219 = new Max72xxPanel(CS_PIN, 1, 1);
  else m7219 = new Max72xxPanel(CS_PIN, 1, 4);

  m7219 -> begin();

  if (conf_data.type_disp >= 1 && conf_data.type_disp <= 3) m7219 -> setRotation(0, 0);
  else
  { m7219 -> setRotation(m7219 -> getRotation() + 3); //1 - 90  2 - 180   3 - 270
    m7219 -> cp437(true);
    m7219 -> setTextSize(1);
    m7219 -> setTextWrap(false); // Allow text to run off right edge
  }
  m7219 -> fillScreen(LOW);
  m7219 -> setCursor(1, 0);

  st1 = "Hello";
  if (conf_data.rus_lng) st1 = "Салют";

  f_dsp.utf8rus(st1);
  m7219 -> print(st1);
  m7219 -> write();
}


void m7219_time()
{
  if (cur_br != cur_br_buf)
  {
    m7219 -> setIntensity(cur_br); // Use a value between 0 and 15 for brightness
    cur_br_buf = cur_br;
  }
  m7219 -> fillScreen(LOW);

  uint8_t h = hour();
  // Do 24 hour to 12 hour format conversion when required.
  if (conf_data.use_pm && hour() > 12) h = hour() - 12;

#ifdef font5pt
  //Отображаем часы
  render_number_in_place (h, 9, 0);

  //Отображаем двоеточие
  if (second() % 10 % 2 == 0)
  {
    m7219 -> drawPixel(15, 3, 1);
    m7219 -> drawPixel(16, 3, 1);
    m7219 -> drawPixel(15, 4, 1);
    m7219 -> drawPixel(16, 4, 1);
  }
  else
  {
    m7219 -> drawPixel(15, 1, 1);
    m7219 -> drawPixel(16, 1, 1);
    m7219 -> drawPixel(15, 2, 1);
    m7219 -> drawPixel(16, 2, 1);
    m7219 -> drawPixel(15, 5, 1);
    m7219 -> drawPixel(16, 5, 1);
    m7219 -> drawPixel(15, 6, 1);
    m7219 -> drawPixel(16, 6, 1);
  }
  //Отображаем минуты

  render_00number_in_place (minute(), 18, 0);

  //Отображаем секунды

  int a = second();
  if (second() > 29) a = 60 - second();
  m7219 -> drawPixel(a, 7, 1);
#endif

#ifdef font4pt
  //----------------------------------------------------------------------

  for (int i = 0; i < num; i++)
  {
    if (!(i == 0 && h < 9))
    {
      m7219 -> drawChar(digPos_x_[i], 0, screen[i], HIGH, LOW, 1);
    }
  }

  point++;
  if (point > 9) point = 0;
  switch (point)
  {
    case 1:
      m7219 -> drawLine (  8, 4, 10, 4, 1); // -
      m7219 -> drawLine ( 20, 4, 22, 4, 1); // -
      break;
    case 2:
      m7219 -> drawLine (  8, 3, 10, 5, 1); // \
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /

      m7219 -> drawLine ( 20, 3, 22, 5, 1); // \
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      break;
    case 3:
      m7219 -> drawLine (  9, 3,  9, 5, 1); // |
      m7219 -> drawLine ( 21, 3, 21, 5, 1); // |
      break;
    case 4:
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      break;
    case 5:
      m7219 -> drawLine (  8, 4, 10, 4, 1); // -
      m7219 -> drawLine ( 20, 4, 22, 4, 1); // -
      break;
    case 6:
      m7219 -> drawLine (  8, 3, 10, 5, 1); // \
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /

      m7219 -> drawLine ( 20, 3, 22, 5, 1); // \
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      break;
    case 7:
      m7219 -> drawLine (  9, 3,  9, 5, 1); // |
      m7219 -> drawLine (  8, 4, 10, 4, 1); // -

      m7219 -> drawLine ( 21, 3, 21, 5, 1); // |
      m7219 -> drawLine ( 20, 4, 22, 4, 1); // -
      break;
    case 8:
      m7219 -> drawLine (  8, 3, 10, 5, 1); // \
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /

      m7219 -> drawLine ( 20, 3, 22, 5, 1); // \
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      break;
    case 9:
      m7219 -> drawLine (  9, 3,  9, 5, 1); // |
      m7219 -> drawLine (  8, 4, 10, 4, 1); // -

      m7219 -> drawLine ( 21, 3, 21, 5, 1); // |
      m7219 -> drawLine ( 20, 4, 22, 4, 1); // -
      break;
    case 0:
      m7219 -> drawLine (  8, 3, 10, 5, 1); // \
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /
      m7219 -> drawLine (  8, 5, 10, 3, 1); // /

      m7219 -> drawLine ( 20, 3, 22, 5, 1); // \
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      m7219 -> drawLine ( 20, 5, 22, 3, 1); // /
      break;
  }
# endif

# ifdef new_max

  //----------------------------------------------------------------------

  for (int i = 0; i < num; i++)
  {
    if (!(i == 0 && h < 9))
    {
      m7219 -> drawPartChar(digPos_x_[i],   0,   screen[i], HIGH, LOW,   digPos_y_[i]); // набегает
      if (digPos_y_[i] != 7)
      {
        m7219 -> drawPartChar(digPos_x_[i], 0, digoldig[i], HIGH, LOW, - digPos_y_[i]); // убегает
        /*
          DBG_OUT_PORT.print("pos - ");
          DBG_OUT_PORT.println(digPos_y_[i]);
          DBG_OUT_PORT.print("sym - ");
          DBG_OUT_PORT.println(digoldig[i]);
        */
        digPos_y_[i]++; // опускает символы
      }
    }
  }

#endif
  m7219 -> write(); // Send bitmap to display
}
