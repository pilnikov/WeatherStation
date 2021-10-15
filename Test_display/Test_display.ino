/////////////////////////////////////////////////////////////////////////////////////////
//#include ".\headers\conf.h"
#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

# if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
# elif defined(__AVR_ATmega2560__)
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
