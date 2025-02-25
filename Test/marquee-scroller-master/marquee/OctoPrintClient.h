/** The MIT License (MIT)

Copyright (c) 2018 David Payne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <ESP8266WiFi.h>
#include "libs/ArduinoJson/ArduinoJson.h"
#include <base64.h>

class OctoPrintClient {

private:
  char myServer[100];
  int myPort = 80;
  String myApiKey = "";
  String encodedAuth = "";

  void resetPrintData();
  boolean validate();
  
  String result;

  typedef struct {
    String averagePrintTime;
    String estimatedPrintTime;
    String fileName;
    String fileSize;
    String lastPrintTime;
    String progressCompletion;
    String progressFilepos;
    String progressPrintTime;
    String progressPrintTimeLeft;
    String state;
    String error;
  } PrinterStruct;

  PrinterStruct printerData;

  
public:
  OctoPrintClient(String ApiKey, String server, int port, String user, String pass);
  void getPrinterJobResults();
  void updateOctoPrintClient(String ApiKey, String server, int port, String user, String pass);

  String getAveragePrintTime();
  String getEstimatedPrintTime();
  String getFileName();
  String getFileSize();
  String getLastPrintTime();
  String getProgressCompletion();
  String getProgressFilepos();
  String getProgressPrintTime();
  String getProgressPrintTimeLeft();
  String getState();
  boolean isPrinting();
  boolean isOperational();
  String getError();
};
