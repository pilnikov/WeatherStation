#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

void web_setup();

void start_serv();
void  stop_serv();

void handleRoot();
void handleXML();
void handleNTP();
void handlejTime();
void handleSetTime();
void handlejWiFi();
void handleSetWiFi();
void handlejPard();
void handlejPars();
void handlejParc();
void handlejAlarm();
void handleSetFont();
void handleSetAlarm();
void handlejUart();
void handlejTrm();
void handleSetPartrm();

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

void cur_time_str(rtc_data_t, char);
# endif
