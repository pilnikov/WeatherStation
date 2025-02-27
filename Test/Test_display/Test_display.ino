//------------------------------------------------------
//#include ".\headers\conf.h"
#include "Scr_frm.h"

// ---------------------------------------------------- Constructors
// ---------------------------------------------------- LM
BH1750 lightMeter;

// ---------------------------------------------------- HW Level of displays
MyDspHW mydsp_hw;

//-------------------------------------------------------- Bright control
MyDspBCF mydsp_bcf;

//-------------------------------------------------------- MatrixScreenContentFormer
MSCF mscf;

//-------------------------------------------------------- SegmentScreenContentFormer
SSCF sscf;

// ---------------------------------------------------- Global Var
static uint16_t cur_br = 7;

//---------------------------------------------------------------------------------------------------
static byte screen[64];        // display buffer
static uint8_t text_size = 1;  // размер текста
static uint8_t newsIndex;
String local_ip = "192.168.0.0", ns = String();

//---------------------------------------------------------------------------------------------------

snr_data_t snr_data;
snr_cfg_t snr_cfg_data;
main_cfg_t mcf;
gpio_cfg_t gcf;
hw_data_t hw_data;
wf_data_t wf_data_cur;
wf_data_t wf_data;
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
wifi_cfg_data_t wifi_data;
wifi_cur_data_t wifi_data_cur;
#endif
rtc_hw_data_t rtc_hw;
rtc_cfg_data_t rtc_cfg;
rtc_time_data_t rtc_time;
rtc_alm_data_t rtc_alm;

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

  mcf.vdrv_t = 5;  //ht1632
  mcf.dsp_t = 22;  //M32x16BICOL
  mcf.auto_br = false;
  mcf.nmd_br = 7;  // Man brigthness
  mcf.man_br = 7;
  mcf.color_up = 0;
  mcf.color_dwn = 1;

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
    snr_data.f = mydsp_bcf.ft_read(hw_data.bh1750_present, lightMeter.readLightLevel(), gcf.gpio_ana);
    cur_br = mydsp_bcf.auto_br(snr_data.f, mcf);
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
