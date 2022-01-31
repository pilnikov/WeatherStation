void web_setup();

void start_serv();
void  stop_serv();

void handlejWiFi();
void handleSetWiFi();

bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();


String getContentType(String);
void printFile(const char*);

void fs_setup();

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void hard_restart(); 
# endif

String formatBytes(uint32_t);
