// Exts.h

#ifndef Exts_h
#define Exts_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>
#include <ArduinoJson.h>
  
#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

class ES
{
  public :
    snr_data_t get_ts(String);
    snr_data_t get_es(String);
    wf_data_t get_gm(String);
    String put_ts(char*, bool, bool, bool, bool, bool, bool, bool, snr_data_t);
};

extern ES extsrv;

#endif  // Exts_h
