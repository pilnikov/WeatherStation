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

String cur_time_str();
