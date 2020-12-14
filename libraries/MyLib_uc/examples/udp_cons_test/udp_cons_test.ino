/*
You must install & run on your local
computer udp terminal, like as 
UdpTerm_Std.exe
*/


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "*******";   // your network SSID (name)
char pass[] = "*******";   // your network password

#define DEBUG_UDP

// ----------------------------------- ConsoleUDP
#ifdef DEBUG_UDP
#include <udp_cons.h>
udp_cons print_console_udp;
#define DBG_OUT_PORT print_console_udp
#else
#define DBG_OUT_PORT Serial
#endif

void setup()
{
 Serial.begin(9600);

# if defined(ESP8266)
  Serial.setDebugOutput(true);
# endif

# ifdef DEBUG_UDP
  DBG_OUT_PORT.begin(4023, IPAddress(192, 168, 1, 2)); //Address & port your local computer
# endif

  conn();

  DBG_OUT_PORT.println("Hello");
}

void loop() {
  // put your main code here, to run repeatedly:

  DBG_OUT_PORT.println("Hello");
  delay(1000);
  uint8_t a = 123;
  DBG_OUT_PORT.println(a);
  delay(1000);
}

void conn ()
{
  // We start by connecting to a WiFi network
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

