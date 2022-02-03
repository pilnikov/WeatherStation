#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

void web_setup();

void start_serv();
void  stop_serv();

void handleNTP();
void handlejTime();
void handleSetTime();
void handlejWiFi();
void handleSetWiFi();
void handleSetIp1();
void handleSetIp2();

void handlejPard();
void handlejPars();
void handlejParc();
void handlejAlarm();
void handleSetFont();
void handleSetAlarm();
void handlejUart();
void handlejTrm();
void handleSetPartrm();

void handlejActA();
void handlejActB();
void handlejActT();

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

File fsUploadFile; //Should be placed here as global
# endif
