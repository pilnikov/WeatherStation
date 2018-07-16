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

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

const String html_begin_head();
const String html_style();
const String html_table();
const String html_begin_body();
const String html_end();
const String opt_val();
const String sel_d();
String html_script();
String num2_to_str(int);
String cur_time_str();
