#include "Header.h"

void setup() {
  DBG_OUT_PORT.begin(9600);

  //------------------------------------------------------  Инициализируем uart pin
  pinMode( uart_pin, OUTPUT );
  digitalWrite(uart_pin, HIGH);

  //------------------------------------------------------  Инициализируем и опрашиваем I2C
  conf_data.type_snr1 = 2;
  conf_data.type_snr2 = 2;
  conf_data.type_snrp = 2;

  Wire.begin(); //Запускаем I2C и проверяем наличие клиентов
  ram_data = fsys.i2c_scan(conf_data);


  //------------------------------------------------------  Инициализируем и опрашиваем датчики

  ram_data_t snr_data = ram_data;

  snr_data = sens.init(ram_data);

  ram_data = snr_data;

  //------------------------------------------------------  Инициализируем и опрашиваем COM порт
  setTime(1530687234);
  synchro();

  //------------------------------------------------------  Инициализируем дисплей
  conf_data.rus_lng = true;
  conf_data.use_pp = 0;
  conf_data.use_pm = false;
  matrix_init();
}

void loop()
{
  _matrix(cur_br);
  grad ++;   //Переливаемся
  if (grad >= 1536) grad -= 1536;
  if (millis() - refresh_time > 500)
  {
    matrix_time();
    refresh_time = millis();
  }
}
