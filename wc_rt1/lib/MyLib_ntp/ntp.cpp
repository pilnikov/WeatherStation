#include "ntp.h"

// ----------------------------------- UDP
WiFiUDP UDP;

unsigned int  localPort    = 8888;
const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming & outgoing packets

//-------------------------------------------------------------- getNtpTime
time_t NTPTime::getTime(IPAddress IP_Addr, int8_t timeZone)
{
  unsigned long ntp_ms = millis();
  unsigned long ntp_time = 0;
  bool suc = false;

# ifdef _debug
  DBG_OUT_PORT.println("Starting UDP");
# endif

  UDP.begin(localPort);

  while (UDP.parsePacket() > 0) ; // discard any previously received packets
# ifdef _debug
  DBG_OUT_PORT.print( "Transmit NTP Request to ip: "); DBG_OUT_PORT.println(IP_Addr);
# endif
  sendNTPpacket(IP_Addr);

  while (millis() - ntp_ms < 1500 && !suc)
  {
    int size = UDP.parsePacket();
    if (size >= NTP_PACKET_SIZE)
    {
# ifdef _debug
      DBG_OUT_PORT.println("Receive NTP Response");
# endif
      UDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900  = (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      ntp_time = secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
      suc = true; //Пакет успешно получен
# ifdef _debug
      DBG_OUT_PORT.println("NTP Packet sucsessfuly recieved :-)");
# endif
    }
  }
  UDP.stop();
  if (suc) return ntp_time;
  else {
# ifdef _debug
    DBG_OUT_PORT.println("No NTP Response :-(");
# endif
    return 0; // return 0 if unable to get the time
  }
}

//-------------------------------------------------------------- sendNTPpacket
void NTPTime::sendNTPpacket(IPAddress address)
{
// set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  UDP.beginPacket(address, 123); //NTP requests are to port 123
  UDP.write(packetBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

