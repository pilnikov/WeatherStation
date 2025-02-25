/////////////////////////////////////////////////////////////////////////////////////////
//#include ".\headers\conf.h"
#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

# if defined(ESP8266)
  conf_data.gpio_sda = 4;
  conf_data.gpio_scl = 5;
  conf_data.gpio_dio = 13;
  conf_data.gpio_clk = 14;
  conf_data.gpio_dcs = 16;
  conf_data.gpio_dwr = 2;
#endif

  conf_data.type_vdrv = 2;  //MAX7219
  conf_data.type_disp = 21; //M32x16MONO
  conf_data.auto_br = false;
  conf_data.nmd_br = 7;  // Man brigthness
  conf_data.man_br = 7;

  m7219_init();
}

void loop()
{
}
