// Exts.h

#ifndef Exts_h
#define Exts_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <ArduinoJson.h>
  
#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#include <Udt.h>
#include <Snr.h>

class ES
{
  public :
    snr_data_t 
	get_ts(String),
    get_es(String);
    wf_data_t get_gm(String);
    String put_ts(char*, uint8_t, snr_data_t);
};


#pragma once
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined(ARDUINO_ARCH_ESP32) || CONFIG_IDF_TARGET_ESP32C3
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#endif
#include <JsonListener.h>
#include <JsonStreamingParser.h> // --> https://github.com/squix78/json-streaming-parser

class NewsApiClient: public JsonListener {

  private:
    String 
	mySource = "",
    myApiKey = "",
    
	currentKey = "";
    
	int counterTitle = 0;

    typedef struct {
      String 
	  title,
      description,
      url;
    } newsfeed;

    newsfeed news[10];
    
    const char* servername = "newsapi.org";  // remote server we will connect to
  
  public:
    NewsApiClient(char* ApiKey, char* NewsSource);
    void 
	updateNewsClient(char* ApiKey, char* NewsSource),
    updateNews(),
    updateNewsSource(char* source);
    
    String 
	getTitle(int index),
    getDescription(int index),
    getUrl(int index),
    cleanText(String text);
    
    virtual void 
	whitespace(char c),
    startDocument(),
    key(String key),
    value(String value),

    endArray(),
    endObject(),
    endDocument(),
    startArray(),
    startObject();
};

#endif  // Exts_h
