#include <Udt.h>

#include <Snr.h>

#define _debug

ram_data_t rd;
snr_data_t sr_data;
snr_data_t ts_data;
snr_data_t es_data1;
snr_data_t es_data2;
wf_data_t wf_data_cur;

SNR sens;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  rd.type_snr1 = 4;
  rd.type_snr2 = 0;
  rd.type_snr3 = 0;
  rd.type_snrp = 0;

  ram_data_t snr_data = rd;
  pinMode(36, INPUT);
  sens.dht_preset(36, 22); //Тут устанавливается GPIO для DHT и его тип (11, 21, 22)

  snr_data = sens.init(rd);

  rd = snr_data;
  Serial.println("start2");
}


void loop() {

  sr_data = sens.read_snr(rd.type_snr1, rd.type_snr2, rd.type_snr3, rd.type_snrp, rd.temp_rtc, ts_data, es_data1, es_data2, wf_data_cur); // Заполняем матрицу данных с датчиков
  Serial.println(sr_data.h1); //Внутренняя влажность
  Serial.println(sr_data.t1); //Внутренняя температура
  Serial.println(sr_data.h2); //Внешнняя влажность
  Serial.println(sr_data.t2); //Внешнняя температура
  Serial.println(sr_data.p); //Давление

}
