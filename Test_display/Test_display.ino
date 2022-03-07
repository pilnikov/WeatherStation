//------------------------------------------------------
//#include ".\headers\conf.h"
#include "constr.h"
#include "conf.h"
#include "global_var.h"


char            tstr[25];

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

  //------------------------------------------------------  Начинаем инициализацию Hardware
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  Wire.setPins(conf_data.gpio_sda, conf_data.gpio_scl);
# endif

# if defined(ESP8266)
  Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
# else
  Wire.begin();
# endif

  //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
  mydsp.disp_init(conf_data.type_vdrv, conf_data.type_disp, conf_data.gpio_uar, conf_data.gpio_dio, conf_data.gpio_clk, conf_data.gpio_dcs, conf_data.gpio_dwr, hw_data.ht_addr, hw_data.lcd_addr, screen, text_size, conf_data.rus_lng);
  //------------------------------------------------------  Инициализируем датчики
  if (hw_data.bh1750_present) lightMeter.begin();


  //------------------------------------------------------  Инициализируем GPIO

  if (!hw_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);
  if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
  if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

  DBG_OUT_PORT.println(F("GPIO inital"));

  //-------------------------------------------------------- Гасим светодиод
  if ((conf_data.type_thermo == 0) & (conf_data.type_vdrv != 5)) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);


  //-------------------------------------------------------- Регулируем яркость дисплея
  if (conf_data.auto_br)
  {
    snr_data.f = ff_dsp.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), conf_data.gpio_ana);
    cur_br = ff_dsp.auto_br(snr_data.f, conf_data);
  }
  else
  {
    if (rtc_time.nm_is_on) cur_br = conf_data.nmd_br;  // Man brigthness
    else cur_br = conf_data.man_br;
    snr_data.f = cur_br;
  }
  DBG_OUT_PORT.print(F("brightness from sensor..."));
  DBG_OUT_PORT.println(snr_data.f);
}

void loop()
{
  //------------------------------------------------------ Распределяем системные ресурсы
  main_loop();
}
