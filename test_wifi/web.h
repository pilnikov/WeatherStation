void web_setup();

void start_serv();
void  stop_serv();

void handlejWiFi();
void handleSetWiFi();
void handleSetIp1();
void handleSetIp2();

bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();


String getContentType(String);

void fs_setup();

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
void hard_restart();
# endif

String formatBytes(uint32_t);

File fsUploadFile;
