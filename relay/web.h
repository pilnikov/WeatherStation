#if defined(__xtensa__) || CONFIG_IDF_TARGET_ESP32C3

void web_setup();

void start_serv();
void  stop_serv();

void handleSET_1();
void handleRESET_1();
void handleSET_1();
void handleRESET_1();
void handlejWiFi();
void handleSetWiFi();


String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

# endif
