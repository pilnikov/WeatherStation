//------------------------------------------------------  Получаем данные с датчиков
void GetSnr()
{
  snr_data_t ts_data;

  snr_data.t1 = 99;
  snr_data.t2 = 99;
  snr_data.t3 = 99;
  snr_data.h1 = 0;
  snr_data.h2 = 0;
  snr_data.h3 = 0;
  snr_data.p = 700;

  ram_data.temp_rtc = 99;

  RtcTemperature t1 = DS3231.GetTemperature();
  ram_data.temp_rtc = round(t1.AsFloatDegC());

  snr_data = sens.read_snr(ram_data.type_snr1, ram_data.type_snr2, ram_data.type_snr3, ram_data.type_snrp, ram_data.temp_rtc, ts_data, es_data, wf_data_cur); // Заполняем матрицу данных с датчиков
}
