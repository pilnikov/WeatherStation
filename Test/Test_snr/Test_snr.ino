#include <Snr.h>
#include <Wire.h>

#define _debug

snr_cfg_t snr_cfg_data;

snr_data_t td;
snr_data_t ed1;
snr_data_t ed2;
snr_data_t sd;
snr_data_t wd;

uint8_t temp_rtc = 99;

SNR sens;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
    uint8_t gpio_sda = 21, gpio_scl = 22;
# elif defined(ESP8266)
    uint8_t gpio_sda = 4, gpio_scl = 5;
# endif



#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
    Wire.setPins(gpio_sda, gpio_scl);
# elif defined(ESP8266)
    Wire.begin(gpio_sda, gpio_scl);
# else
    Wire.begin();
# endif

  snr_cfg_data.type_snr1 = 10;
  snr_cfg_data.type_snr2 = 0;
  snr_cfg_data.type_snr3 = 0;
  snr_cfg_data.type_snrp = 10;

  sd.t1 = 99;
  sd.t2 = 99;
  sd.t3 = 99;
  sd.h1 = 0;
  sd.h2 = 0;
  sd.h3 = 0;
  sd.p = 700;

  snr_cfg_data.gpio_dht = 2;

  if (snr_cfg_data.type_snr1 > 0 || snr_cfg_data.type_snr2 > 0 || snr_cfg_data.type_snr3 > 0)
  {
    if (snr_cfg_data.type_snr1 == 4 || snr_cfg_data.type_snr2 == 4 || snr_cfg_data.type_snr3 == 4)
    {
      sens.dht_preset(snr_cfg_data.gpio_dht, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)
    }

    sens.init(&snr_cfg_data);

    DBG_OUT_PORT.print(F("Snr type on channel 1 = "));
    DBG_OUT_PORT.println(snr_cfg_data.type_snr1);
    DBG_OUT_PORT.print(F("Snr type on channel 2 = "));
    DBG_OUT_PORT.println(snr_cfg_data.type_snr2);
    DBG_OUT_PORT.print(F("Snr type on channel 3 = "));
    DBG_OUT_PORT.println(snr_cfg_data.type_snr3);
    DBG_OUT_PORT.print("Snr type on pressure = ");
    DBG_OUT_PORT.println(snr_cfg_data.type_snrp);
    DBG_OUT_PORT.println(F("sensor inital"));
  }
}


void loop() {

  sd = sens.read_snr(snr_cfg_data, temp_rtc, td, ed1, ed2, wd); // Заполняем матрицу данных с датчиков
  Serial.println(sd.h1); //Внутренняя влажность
  Serial.println(sd.t1); //Внутренняя температура
  Serial.println(sd.h2); //Внешнняя влажность
  Serial.println(sd.t2); //Внешнняя температура
  Serial.println(sd.p);  //Давление

}
