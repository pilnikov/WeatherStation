#include "Netwf_rt.h"

//--------------------------------------------------------- HTTP Client

String NF::http_client( String incoming )
{
  int err =0;
  String payload = String();

  DBG_OUT_PORT.print("True Connected to server...");
  DBG_OUT_PORT.println(incoming);
  
  WiFiClient c;
  HttpClient http(c);
  
  //err = http.get(incoming);
  if (err == 0)
  {
    DBG_OUT_PORT.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      DBG_OUT_PORT.print("Got status code: ");
      DBG_OUT_PORT.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        DBG_OUT_PORT.print("Content length is: ");
        DBG_OUT_PORT.println(bodyLen);
        DBG_OUT_PORT.println();
        DBG_OUT_PORT.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                DBG_OUT_PORT.print(c);
                payload += c;
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        DBG_OUT_PORT.print("Failed to skip response headers: ");
        DBG_OUT_PORT.println(err);
      }
    }
    else
    {    
      DBG_OUT_PORT.print("Getting response failed: ");
      DBG_OUT_PORT.println(err);
    }
  }
  else
  {
    DBG_OUT_PORT.print("Connect failed: ");
    DBG_OUT_PORT.println(err);
  }
  http.stop();
  return payload;

}

void NF::ssdp_init()
{
}

void NF::OTA_init(char* ssid, char* pass )
{
}