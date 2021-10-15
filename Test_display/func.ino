
//-------------------------------------------------------------- cur_time_str
void cur_time_str(rtc_data_t rt, char in[])
{
  const char* sdnr_1 = PSTR("ВС");
  const char* sdnr_2 = PSTR("ПН");
  const char* sdnr_3 = PSTR("ВТ");
  const char* sdnr_4 = PSTR("СР");
  const char* sdnr_5 = PSTR("ЧТ");
  const char* sdnr_6 = PSTR("ПТ");
  const char* sdnr_7 = PSTR("СБ");

  const char* const sdnr[] = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

  const char* sdne_1 = PSTR("Sun");
  const char* sdne_2 = PSTR("Mon");
  const char* sdne_3 = PSTR("Tue");
  const char* sdne_4 = PSTR("Wed");
  const char* sdne_5 = PSTR("Thu");
  const char* sdne_6 = PSTR("Fri");
  const char* sdne_7 = PSTR("Sat");

  const char* const sdne[] = {sdne_1, sdne_2, sdne_3, sdne_4, sdne_5, sdne_6, sdne_7};

  if (conf_data.rus_lng) sprintf_P(in, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdnr[rt.wday - 1], rt.day, rt.month, rt.year, rt.hour, rt.min, rt.sec);
  else sprintf_P(in, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdne[rt.wday - 1], rt.day, rt.month, rt.year, rt.hour, rt.min, rt.sec);
}
