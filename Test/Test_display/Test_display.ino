//------------------------------------------------------
//#include ".\headers\conf.h"
#include "constr.h"
#include "conf.h"
#include "global_var.h"


char tstr[25];

void setup() {
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

#if defined(ESP8266)
  gcf.gpio_sda = 4;
  gcf.gpio_scl = 5;
  gcf.gpio_dio = 13;
  gcf.gpio_clk = 14;
  gcf.gpio_dcs = 16;
  gcf.gpio_dwr = 2;
#endif

  mcf.vdrv_t = 2;  //MAX7219
  mcf.dsp_t = 21;  //M32x16MONO
  mcf.auto_br = false;
  mcf.nmd_br = 7;  // Man brigthness
  mcf.man_br = 7;

  //------------------------------------------------------  Начинаем инициализацию Hardware
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  Wire.setPins(gcf.gpio_sda, gcf.gpio_scl);
#endif

#if defined(ESP8266)
  Wire.begin(gcf.gpio_sda, gcf.gpio_scl);
#else
  Wire.begin();
#endif

  //------------------------------------------------------  Инициализируем выбранный чип драйвера дисплея
  if (mcf.dsp_t > 0) mydsp_hw._init(mcf.vdrv_t, mcf.dsp_t, gcf, hw_data.ht_addr, hw_data.lcd_addr, text_size);
  //------------------------------------------------------  Инициализируем датчики
  if (hw_data.bh1750_present) lightMeter.begin();


  //------------------------------------------------------  Инициализируем GPIO

  if (!hw_data.bh1750_present) pinMode(gcf.gpio_ana, INPUT);
  if ((mcf.thermo_t == 0) & (mcf.vdrv_t != 5)) pinMode(gcf.gpio_led, OUTPUT);                          // Initialize the LED_PIN pin as an output
  if ((mcf.thermo_t == 0) & (mcf.vdrv_t != 5)) digitalWrite(gcf.gpio_led, gcf.led_pola ? HIGH : LOW);  //Включаем светодиод

  DBG_OUT_PORT.println(F("GPIO inital"));

  //-------------------------------------------------------- Гасим светодиод
  if ((mcf.thermo_t == 0) & (mcf.vdrv_t != 5)) digitalWrite(gcf.gpio_led, gcf.led_pola ? LOW : HIGH);


  //-------------------------------------------------------- Регулируем яркость дисплея
  if (mcf.auto_br) {
    snr_data.f = f_dsp2.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
    cur_br = f_dsp2.auto_br(snr_data.f, mcf);
  } else {
    if (rtc_time.nm_is_on) cur_br = mcf.nmd_br;  // Man brigthness
    else cur_br = mcf.man_br;
    snr_data.f = cur_br;
  }
  DBG_OUT_PORT.print(F("brightness from sensor..."));
  DBG_OUT_PORT.println(snr_data.f);
}

void loop() {
  //------------------------------------------------------ Распределяем системные ресурсы
  main_loop();
}
