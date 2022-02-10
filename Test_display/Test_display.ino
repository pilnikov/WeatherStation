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

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  Wire.setPins(conf_data.gpio_sda, conf_data.gpio_scl);
# endif

# if defined(ESP8266)
  Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
# else
  Wire.begin();
# endif

  ram_data = fsys.i2c_scan(conf_data);
  if (ram_data.bh1750_present) lightMeter.begin();

  //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
  memset(st1, 0, 254);
  memset(screen, 0, 64);
  //    ram_data.type_vdrv = 0;

  switch (ram_data.type_vdrv)
  {
    case 0:
#if defined(ESP8266)
      pinMode(conf_data.gpio_uar, INPUT_PULLUP);
#endif
      break;
    case 1:
      tm1637_init();
      break;
    case 2:
      m7219_init();
      break;
    case 3:
      a595_init();
      break;
    case 4:
      ht1621_init();
      break;
    case 5:
      ht1632_init();
      break;
    case 6:
      ili9341_init();
      break;
    case 11:
      ht1633_init();
      break;
    case 12:
      pcf8574_init();
      break;
  }
  DBG_OUT_PORT.print(F("Type chip driver of display = "));
  DBG_OUT_PORT.println(ram_data.type_vdrv);

  //-------------------------------------------------------- Регулируем яркость дисплея
  if (conf_data.auto_br)
  {
    snr_data.f = f_dsp.ft_read(ram_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
    cur_br = f_dsp.auto_br(snr_data.f, conf_data);
  }
  else
  {
    if (nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
    else cur_br = conf_data.man_br;
    snr_data.f = cur_br;
  }
  DBG_OUT_PORT.print(F("brightness from sensor..."));
  DBG_OUT_PORT.println(snr_data.f);
}

void loop()
{
  //------------------------------------------------------ Распределяем системные ресурсы
  irq_set();
}
