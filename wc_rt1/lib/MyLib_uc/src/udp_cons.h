#ifndef udp_cons_h
#define udp_cons_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#endif

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
  Definitions
******************************************************************************/

//IPAddress      ConsoleAddressUDP  = IPAddress(192,168,100,99);

class udp_cons : public Print
{
  private:
    WiFiUDP        ConsoleUDP;
    IPAddress      Addr = IPAddress(192, 168, 111, 132); //адрес ПК на котором смотрим диагностику
    uint16_t       Port = 4023;
    char           buf_printf[256];
    uint8_t        i;
    
  public:
    void begin(uint16_t, IPAddress);
    virtual size_t write(uint8_t);
    using Print::write;
};

#endif

