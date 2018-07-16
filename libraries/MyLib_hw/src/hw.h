#ifndef hw_h
#define hw_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#define MAX_MEMORY 81920
#define MAX_FREE_MEM 46224
#define PERC_FREE_MEM (MAX_FREE_MEM / 100)
#define MAX_PROGRAM 1044464
#define MIN_CKETCH 110744


class ESP8266HWInfo 
{
  public:
    void info();
    void printFreeMemP(); 
     int freeMem(); 
    void calcCyclosP();
    void cyclosInSecWork();
    byte cyclosDelayP();
    void cyclosDelayWork();
 
  private:
    void initStart_(String s);
    void initDone_();
  
  protected:
};
extern ESP8266HWInfo HWInfo;

#endif  // _hw_H_
