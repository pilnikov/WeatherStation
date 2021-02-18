#include "fonts.h"


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
