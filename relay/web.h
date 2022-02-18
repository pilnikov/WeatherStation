#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

void web_setup();

void start_serv();
void  stop_serv();

void handleSET_1();
void handleAuto_1();
void handleSET_2();
void handleAuto_2();
void handleSetWiFi();
void handleSetPar();
void handlejWiFi();
void handlejPar();
void handlejAct();
void handleExit();

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

File fsUploadFile;
# endif
