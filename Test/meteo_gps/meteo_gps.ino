#include "conf.h"

void setup()
{
  Serial.begin(115200);

  tm_setup();
  i2c_setup();
  i2c_loop();
  rtc_setup();

  snr_ch_t sens2 = sens;
  sens = snr_init(sens2);
  read_snr(sens);
}

void loop()
{
  if (millis() - irq_end[1] > 6000)
  {
    set_time();
    irq_end[1] = millis();
  }

  if (millis() - t_start > 60000)
  {
    read_snr(sens);
    // Every 5 seconds we print an update
    t_start = millis();
  }

  if (millis() - t_dsec > 500)
  {
    colon = !colon;
    t_dsec = millis();
  }

  tm_loop();
}
