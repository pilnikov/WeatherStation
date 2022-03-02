#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Udt.h>
#include <ArduinoJson.h>
#include <Snr.h>
#include <myrtc.h>

void web_setup();

void start_serv();
void  stop_serv();

void handleNTP();
void handleSetTime();
void handleSetPart();
void handleSetWiFi();
void handleSetIp1();
void handleSetIp2();
void handleEndSetWiFi();
void handleExit();
# ifdef MATRIX
void handleSetFont();
# endif //MATRIX
void handleSetPard();
void handleSetPars1();
void handleSetPars2();
void handleSetPars3();
void handleRcvSnr();
void handleSetParc();
void handleSetPartrm();
void handleSetNews();

void handlejActT();
void handlejActA();
void handlejTime();
void handlejWiFi();
void handlejParc();
void handlejActB();
void handlejPard();
void handlejPars();
void handlejTS();
void handlejSnr();
void handleUpdSnr();
void handlejUart();
void handlejTrm();
void handlejNews();
void handlejNewsT();

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();
