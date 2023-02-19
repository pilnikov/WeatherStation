// Sysf.h

#ifndef Netwf_rt_h
#define Netwf_rt_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#include <OTA.h>
#include <HttpClient.h>
#include <WiFiClient.h>
#include <WiFi.h>

const char kPath[] = "/";
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;


#define DBG_OUT_PORT Serial

class NF 
{
   public:
    String    http_client(String);
    void      ssdp_init();
    void      OTA_init(char*, char*);
 
  private:
/*    String formatBytes(size_t bytes); 
    bool handleFileRead(String path);
    void handleFileUpload();
    void handleFileDelete();
    void handleFileCreate();
    void handleFileList();
*/  protected:
};

extern NF Nfunc;

#endif  // Netwf_rt_h


