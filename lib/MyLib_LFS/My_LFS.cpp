#include "My_LFS.h"

void LFS::begin(void)
{
#if defined(ESP8266)
  if (!LittleFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LittleFS.format();
  }
  else
  {
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUT_PORT.printf(" FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
  }
#elif CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32C3
  if (!LittleFS.begin())
  {
    DBG_OUT_PORT.print("\n Failed to mount file system, try format it!\n");
    LittleFS.format();
  }
  else
  {
    File root = LittleFS.open("/");

    String output = "[";
    if (root.isDirectory()) {
      File file = root.openNextFile();
      while (file) {
        if (output != "[") output += ',';
        output += "{\"type\":\"";
        output += (file.isDirectory()) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(file.name());
        output += "\"}";
        file = root.openNextFile();
      }
      DBG_OUT_PORT.println(output);
    }
  }
# endif
}

String LFS::formatBytes(uint32_t bytes)
{
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024 / 1024) + "MB";
  } else {
    return String(bytes / 1024 / 1024 / 1024) + "GB";
  }
}

void LFS::writeFile(const char * path, const char * message) 
{
  DBG_OUT_PORT.printf("Writing file: %s\n", path);
  LittleFS.remove(path);

  File file = LittleFS.open(path, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    DBG_OUT_PORT.println("File written");
  } else {
    DBG_OUT_PORT.println("Write failed");
  }
  file.close();
}

String LFS::readFile(const char* path)
{
  DBG_OUT_PORT.printf("Reading file: %s\n", path);
  String msg = String();
  File file = LittleFS.open(path, "r");
  if (!file)
  {
	msg = "Failed to open file for reading";
    DBG_OUT_PORT.println(msg);
  }
  else
  {
    while (file.available())
    {
      msg += (char)file.read();
    }
    file.close();
  }
  return msg;
}
