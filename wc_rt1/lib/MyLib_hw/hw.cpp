/*
  Modul Hardware for D1 mini
  part of Arduino Mega Server project
*/

#include "hw.h"


void ESP8266HWInfo::info()
{
  initStart_("Hardware");
  DBG_OUT_PORT.print(F("SDK version:     ")); DBG_OUT_PORT.println(ESP.getSdkVersion());
  DBG_OUT_PORT.print(F("Boot version:    ")); DBG_OUT_PORT.println(ESP.getBootVersion());
  DBG_OUT_PORT.print(F("Boot mode:       ")); DBG_OUT_PORT.println(ESP.getBootMode());
  DBG_OUT_PORT.print(F("Reset info:      ")); DBG_OUT_PORT.println(ESP.getResetInfo());
  DBG_OUT_PORT.print(F("CPU freq:        ")); DBG_OUT_PORT.print(ESP.getCpuFreqMHz()); DBG_OUT_PORT.println(" MHz");
  DBG_OUT_PORT.print(F("Free memory:     "));
  DBG_OUT_PORT.print(ESP.getFreeHeap());
  DBG_OUT_PORT.print(F(" ("));
  printFreeMemP();
  DBG_OUT_PORT.println(F("%)"));

  DBG_OUT_PORT.printf("Chip ID:         %08X\n", ESP.getChipId());
  DBG_OUT_PORT.printf("Flash ID:        %08X\n", ESP.getFlashChipId());

  DBG_OUT_PORT.print(F("Flash speed:     ")); DBG_OUT_PORT.print(ESP.getFlashChipSpeed() / 1000000); DBG_OUT_PORT.println(" MHz");
  DBG_OUT_PORT.print(F("Flash mode:      ")); DBG_OUT_PORT.println(ESP.getFlashChipMode());
  DBG_OUT_PORT.print(F("Flash size:      ")); DBG_OUT_PORT.println(ESP.getFlashChipSize());
  if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize())
  {
    DBG_OUT_PORT.print(F("Flash real size: ")); DBG_OUT_PORT.println(ESP.getFlashChipRealSize());
  }
  DBG_OUT_PORT.print(F("SPIFFS size:     ")); DBG_OUT_PORT.println(ESP.getFlashChipRealSize() - (ESP.getSketchSize() + ESP.getFreeSketchSpace()));
  DBG_OUT_PORT.print(F("Program size:    ")); DBG_OUT_PORT.println(ESP.getSketchSize() + ESP.getFreeSketchSpace());
  DBG_OUT_PORT.print(F("Sketch size:     ")); DBG_OUT_PORT.println(ESP.getSketchSize());
  DBG_OUT_PORT.print(F("Free for sketch: ")); DBG_OUT_PORT.println(ESP.getFreeSketchSpace());
  DBG_OUT_PORT.print(F("Analog input     ")); DBG_OUT_PORT.println(analogRead(A0));
  DBG_OUT_PORT.print(F("GPIO             ")); DBG_OUT_PORT.println((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
  initDone_();
}


void ESP8266HWInfo::printFreeMemP() 
{
  DBG_OUT_PORT.print(ESP.getFreeHeap() / PERC_FREE_MEM);
}

int ESP8266HWInfo::freeMem() 
{
  return ESP.getFreeHeap();
}


// CPU load
// ESP-12F, 80 MHz, QIO 40 MHz

// cyclos in sec
long cyclosInSec  = 0;
long cyclosInSecP = 0;

void ESP8266HWInfo::calcCyclosP()
{
  cyclosInSecP = 100 - (cyclosInSec / 10);
  if (cyclosInSecP <   0)
  {
    cyclosInSecP =   0;
  }
  if (cyclosInSecP > 100)
  {
    cyclosInSecP = 100;
  }
  cyclosInSec = 0;
}

void ESP8266HWInfo::cyclosInSecWork()
{
  cyclosInSec++;
  if (true)
  {
    calcCyclosP();
  }
}

// cyclos delay
#define MAX_CPU_BUFFER 10
unsigned long bufferCpuLoad[MAX_CPU_BUFFER];
unsigned long oldCycle = 0;

byte ESP8266HWInfo::cyclosDelayP()
{
  unsigned long summ = 0;
  for (byte i = 0; i < MAX_CPU_BUFFER; i++)
  {
    summ += bufferCpuLoad[i];
  }
  int cyclosDelay = summ / MAX_CPU_BUFFER; // delay >= 100 ms (100% load)
  cyclosDelay /= 2; //  k=10 => delay >= 1 s (100% load)
  if (cyclosDelay <   0) {
    cyclosDelay =   0;
  }
  if (cyclosDelay > 100) {
    cyclosDelay = 100;
  }
  return cyclosDelay;
}

void ESP8266HWInfo::cyclosDelayWork()
{
  unsigned long now2 = millis();
  for (byte i = MAX_CPU_BUFFER - 1; i > 0; i--)
  {
    bufferCpuLoad[i] = bufferCpuLoad[i - 1];
  }
  bufferCpuLoad[0] = now2 - oldCycle;
  oldCycle = now2;
}

void ESP8266HWInfo::initStart_(String s)
{
  DBG_OUT_PORT.print("Modul ");
  DBG_OUT_PORT.print(s);
  DBG_OUT_PORT.println("...");
}

void ESP8266HWInfo::initDone_()
{
  DBG_OUT_PORT.println(F("started"));
}
