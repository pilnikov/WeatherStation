//#include "conf.h"
#include "udp_cons.h"

void udp_cons::begin(uint16_t port, IPAddress addr)
{
    ConsoleUDP.begin(port);  
    Port = port;
    Addr = addr;
    //Serial.begin(speed);
}

inline size_t udp_cons::write(uint8_t value)
{
  //Serial.print(value);
  //send(value, HIGH);
  buf_printf[i] = value;
  i++;

  if (value == '\n') 
  {
    ConsoleUDP.beginPacket(Addr, Port);
    ConsoleUDP.write(buf_printf, i);
    ConsoleUDP.endPacket();
 
//    Serial.print(buf_printf);
    delay(1);
    i = 0;
  }
    return 1; // assume sucess
}
