#include "Header.h"

void setup() {
  DBG_OUT_PORT.begin(115200);
  DBG_OUT_PORT.println();
  //------------------------------------------------------  Запускаем I2C и проверяем наличие клиентов
  //conf_data = loadConfig();
  conf_data = defaultConfig();
  //saveConfig(conf_data);
# if defined(__xtensa__)
  Wire.begin(conf_data.gpio_sda, conf_data.gpio_scl);
# elif defined(__AVR_ATmega2560__)
  Wire.begin();
# endif

  ram_data = fsys.i2c_scan(conf_data);

  //------------------------------------------------------  Инициализируем RTC
  if (ram_data.type_rtc > 0) rtc_init();
  DBG_OUT_PORT.print(F("Type of rtc = "));
  DBG_OUT_PORT.println(ram_data.type_rtc);

  //------------------------------------------------------  Записывваем текущее время в rtc_data
  if (ram_data.type_rtc > 0) _now = DS3231.GetDateTime();
  else _now = RtcDateTime(__DATE__, __TIME__);

  rtc_data.hour = _now.Hour();
  rtc_data.min = _now.Minute();
  rtc_data.sec = _now.Second();
  rtc_data.wday = _now.DayOfWeek() + 1;
  rtc_data.month = _now.Month();
  rtc_data.day = _now.Day();
  rtc_data.year = _now.Year(); //костыль

  DBG_OUT_PORT.print(F("Current time is "));
  DBG_OUT_PORT.println(cur_time_str(rtc_data));

  //------------------------------------------------------  Инициализируем датчики
  if (ram_data.bh1750_present) lightMeter.begin();


  if (ram_data.type_snr1 > 0 || ram_data.type_snr2 > 0 || ram_data.type_snr3 > 0)
  {
    sens.dht_preset(conf_data.gpio_dht, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)

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
  if (!ram_data.bh1750_present) pinMode(conf_data.gpio_ana, INPUT);

  DBG_OUT_PORT.print(F("GPIO for LED..."));
  DBG_OUT_PORT.println(conf_data.gpio_led);
  DBG_OUT_PORT.print(F("LED Pola..."));
  DBG_OUT_PORT.println(conf_data.led_pola);
  DBG_OUT_PORT.print(F("Thermo..."));
  DBG_OUT_PORT.println(conf_data.type_thermo);

  pinMode(conf_data.gpio_led, OUTPUT);     // Initialize the LED_PIN pin as an output
  if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5))  digitalWrite(conf_data.gpio_led, conf_data.led_pola ? HIGH : LOW); //Включаем светодиод

  pinMode(conf_data.gpio_snd, OUTPUT);

  pinMode( conf_data.gpio_uar, OUTPUT );
  digitalWrite(conf_data.gpio_uar, HIGH);

  DBG_OUT_PORT.println(F("GPIO inital"));

  //------------------------------------------------------  Инициализируем дисплей
  conf_data.rus_lng = true;
  conf_data.use_pp = 0;
  conf_data.use_pm = false;
  m3216_init();

  //-------------------------------------------------------  Опрашиваем датчики
  GetSnr();

  if (snr_data.t1 == 99) snr_data.t1 = 11;
  if (snr_data.t2 == 99) snr_data.t2 = 22;
  if (snr_data.t3 == 99) snr_data.t3 = -33;
  if (snr_data.h1 == 0) snr_data.h1 = 41;
  if (snr_data.h2 == 0) snr_data.h2 = 42;
  if (snr_data.h3 == 0) snr_data.h3 = 43;
  if (snr_data.p == 700) snr_data.p = 728;

  //-------------------------------------------------------- Устанавливаем будильники
  set_alarm();

  rtc_data.a_hour = 20;
  rtc_data.a_min = 15;
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

  //------------------------------------------------------ Принимаем данные через UART

  //synchro();

  //-------------------------------------------------------- Гасим светодиод
  DBG_OUT_PORT.print(F("GPIO for LED..."));
  DBG_OUT_PORT.println(conf_data.gpio_led);
  DBG_OUT_PORT.print(F("LED Pola..."));
  DBG_OUT_PORT.println(conf_data.led_pola);
  DBG_OUT_PORT.print(F("Thermo..."));
  DBG_OUT_PORT.println(conf_data.type_thermo);

  if ((conf_data.type_thermo == 0) & (ram_data.type_vdrv != 5))   digitalWrite(conf_data.gpio_led, conf_data.led_pola ? LOW : HIGH);

  //------------------------------------------------------ Радостно пищим по окончаниии подготовки к запуску
  rtc_data.a_muz = 15;
  play_snd = true;

  DBG_OUT_PORT.println(F("End of setup"));
  sprintf_P(st1, PSTR("Starting...."));
}

void loop()
{
  irq_set();
}
