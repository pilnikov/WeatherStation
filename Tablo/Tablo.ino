#include "Header.h"

void setup() {
  DBG_OUT_PORT.begin(115200);

  //------------------------------------------------------  Настраиваем конфигурацию

  strncpy(conf_data.sta_ssid,   " ", 17);
  strncpy(conf_data.sta_pass,   " ", 17);
  strncpy(conf_data.ap_ssid,    " ", 17);
  strncpy(conf_data.ap_pass ,   " ", 17);
  strncpy(conf_data.AKey_r,     " ", 17);
  strncpy(conf_data.AKey_w,     " ", 17);
  strncpy(conf_data.esrv1_addr, " ", 17);
  strncpy(conf_data.esrv2_addr, " ", 17);
  strncpy(conf_data.radio_addr, " ", 17);
  strncpy(conf_data.owm_key,    " ", 33);
  strncpy(conf_data.ch1_name, "Внутри",  17);
  strncpy(conf_data.ch2_name, "Снаружи", 17);
  strncpy(conf_data.ch3_name, "В бане",  17);


  conf_data.auto_corr        = false;
  conf_data.use_pm           = false;
  conf_data.every_hour_beep  = true;
  conf_data.snd_pola         = false;
  conf_data.led_pola         = true;
  conf_data.rus_lng          = true;
  conf_data.use_tst1         = false;
  conf_data.use_tst2         = false;
  conf_data.use_tst3         = false;
  conf_data.use_tsh1         = false;
  conf_data.use_tsh2         = false;
  conf_data.use_tsh3         = false;
  conf_data.use_tsp          = false;
  conf_data.wifi_off         = false;
  conf_data.use_pp           = 0;
  conf_data.time_zone        = 5;
  conf_data.type_vdrv        = 3;
  conf_data.type_disp        = 23;

  //---------------------------- Датчики
  conf_data.type_snr1        = 10;
  conf_data.type_snr2        = 4;
  conf_data.type_snr3        = 0;
  conf_data.type_snrp        = 10;

  conf_data.type_rtc         = 1;
  conf_data.type_thermo      = 0;
  conf_data.src_thermo       = 0;
  conf_data.lb_thermo        = 0;
  conf_data.hb_thermo        = 0;
  conf_data.nm_start         = 17;
  conf_data.nm_stop          = 7;
  conf_data.ts_ch_id         = 0;
  conf_data.pp_city_id       = 0;
  conf_data.period           = 10;
  conf_data.man_br           = 14;
  conf_data.auto_br          = true;

  conf_data.gpio_sda         = 20;
  conf_data.gpio_scl         = 21;
  conf_data.gpio_dio         = A3;
  conf_data.gpio_clk         = A2;
  conf_data.gpio_dcs         = A1;
  conf_data.gpio_dwr         = A0;
  conf_data.gpio_trm         = A6;
  conf_data.gpio_sqw         = 19;
  conf_data.gpio_snd         = A7;
  conf_data.gpio_led         = A6;
  conf_data.gpio_btn         = A5;

  //--------------------------------Уровни для автояркости
  conf_data.br_level[0]      = 200;
  conf_data.br_level[1]      = 1;
  conf_data.br_level[2]      = 1;
  conf_data.br_level[3]      = 230;

  conf_data.type_font        = 0;

  //--------------------------------Будильники

  for (uint8_t i = 0; i <= 6; i++)
  {
    for (uint8_t j = 0; j <= 4; j++)
    {
      conf_data.alarms[i][j] = 0;
    }
  }
  conf_data.alarms[0][0] = 2;
  conf_data.alarms[0][1] = 16;
  conf_data.alarms[0][2] = 30;
  conf_data.alarms[0][3] = 13;
  conf_data.alarms[0][4] = 0;

  conf_data.alarms[1][0] = 2;
  conf_data.alarms[1][1] = 16;
  conf_data.alarms[1][2] = 38;
  conf_data.alarms[1][3] = 7;
  conf_data.alarms[1][4] = 0;

  conf_data.alarms[2][0] = 2;
  conf_data.alarms[2][1] = 11;
  conf_data.alarms[2][2] = 59;
  conf_data.alarms[2][3] = 8;
  conf_data.alarms[2][4] = 0;
  //------------------------------------------------------  Запускаем I2C и проверяем наличие клиентов

  Wire.begin();

  ram_data = fsys.i2c_scan(conf_data);

  //------------------------------------------------------  Инициализируем RTC
  if (ram_data.type_rtc > 0) rtc_init();
  DBG_OUT_PORT.print(F("Type of rtc = "));
  DBG_OUT_PORT.println(ram_data.type_rtc);

  //------------------------------------------------------  Инициализируем датчики

  if (ram_data.bh1750_present) lightMeter.begin();


  if (ram_data.type_snr1 > 0 || ram_data.type_snr2 > 0 || ram_data.type_snr3 > 0)
  {
    sens.dht_preset(DHT_PIN, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)

    ram_data_t sens_data = ram_data;

    sens_data = sens.init(ram_data);
    ram_data = sens_data;

    DBG_OUT_PORT.print(F("Snr type on channel 1 = "));
    DBG_OUT_PORT.println(ram_data.type_snr1);
    DBG_OUT_PORT.print(F("Snr type on channel 2 = "));
    DBG_OUT_PORT.println(ram_data.type_snr2);
    DBG_OUT_PORT.print(F("Snr type on channel 3 = "));
    DBG_OUT_PORT.println(ram_data.type_snr3);
    DBG_OUT_PORT.print(F("Snr type on pressure = "));
    DBG_OUT_PORT.println(ram_data.type_snrp);
    DBG_OUT_PORT.println(F("sensor inital"));
  }

  //------------------------------------------------------  Инициализируем GPIO
  pinMode(conf_data.gpio_btn, INPUT_PULLUP);
  if (!ram_data.bh1750_present) pinMode(ANA_SNR, INPUT);
  pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
  if (conf_data.type_thermo == 0  && ram_data.type_vdrv != 5) digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW);  //Включаем светодиод

  pinMode(conf_data.gpio_snd, OUTPUT);

  pinMode( uart_pin, OUTPUT );
  digitalWrite(uart_pin, HIGH);

  DBG_OUT_PORT.println(F("GPIO inital"));

  //------------------------------------------------------  Инициализируем дисплей
  conf_data.rus_lng = true;
  conf_data.use_pp = 0;
  conf_data.use_pm = false;
  m3216_init();

  //-------------------------------------------------------  Опрашиваем датчики
  GetSnr();

  //-------------------------------------------------------- Гасим светодиод
  if (conf_data.type_thermo == 0 && ram_data.type_vdrv != 5)   digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);

  //-------------------------------------------------------- Устанавливаем будильники
  set_alarm();

  //-------------------------------------------------------- Регулируем яркость дисплея
  if (conf_data.auto_br)
  {
    snr_data.f = ft_read(ram_data.bh1750_present);
    cur_br = auto_br(snr_data.f);
  }
  else
  {
    cur_br = conf_data.man_br;  // Man brigthness
    snr_data.f = cur_br;
  }
  DBG_OUT_PORT.print(F("brightness from sensor..."));
  DBG_OUT_PORT.println(snr_data.f);

  //------------------------------------------------------ Принимаем данные через UART

  //synchro();


  //------------------------------------------------------ Радостно пищим по окончаниии подготовки к запуску
  rtc_data.a_muz = 15;
  play_snd = true;

  DBG_OUT_PORT.println(F("End of setup"));
}

void loop()
{
  irq_set();
}
