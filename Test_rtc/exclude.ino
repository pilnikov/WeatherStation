//#include "conf.h"

void alarm1_action()
{
  play_snd = true;
  if (rtc_cfg.alarms[rtc_alm.num].type == 4)
  {
    rtc_cfg.alarms[rtc_alm.num].type = 0;
    from_client = myrtccfg.to_json(rtc_cfg);
#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3
    conf_f = "/conf_rtc.json";
    lfs.writeFile(conf_f, from_client.c_str());
#endif
    rtc_alm = myrtc.set_alarm(rtc_cfg, rtc_time.ct, rtc_hw.a_type == 1);
  }
}
