#ifndef ntp_h
#define ntp_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <WiFiUdp.h>

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#define SECS_PER_HOUR (3600UL)


class NTPTime 
{
  public:
    time_t getTime (IPAddress, int8_t);
    void   sendNTPpacket(IPAddress);
 
  private:
  
  protected:
};
extern NTPTime  getNTP;

#endif  // ntp_h
