#include <Udt.h>

#include <Snr.h>

#define _debug

ram_data_t ram_data;
sr_data_t sr_data;
sr_data_t ts_data;
sr_data_t es_data;

SNR sens;

void setup() {
  Serial.begin(9600);
  Serial.println("start");

  ram_data.type_int_snr = 3;
  ram_data.type_ext_snr = 0;
  ram_data.type_prs_snr = 0;

  ram_data_t snr_data = ram_data;

  sens.dht_preset(0, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)
  snr_data = sens.init(ram_data);

  ram_data = snr_data;
  Serial.println("start2");
}


void loop() {

  sr_data = sens.read_snr(ram_data.type_int_snr, ram_data.type_ext_snr, ram_data.type_prs_snr, ram_data.temp_rtc, ts_data, es_data); // Опрашиваем датчики
  Serial.println(sr_data.h1); //Внутренняя влажность
  Serial.println(sr_data.t1); //Внутренняя температура
  Serial.println(sr_data.h2); //Внешнняя влажность
  Serial.println(sr_data.t2); //Внешнняя температура
  Serial.println(sr_data.p); //Давление

}
