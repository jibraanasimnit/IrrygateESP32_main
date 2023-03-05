#ifndef DATA_BASE_H__
#define DATA_BASE_H__

#define FORMAT_SPIFFS_IF_FAILED   true

#include <FS.h>
#include "SPIFFS.h"
#include "data.h"

#ifndef ESP32
#define FILE_WRITE   "w"
#define FILE_READ    "r"
#define FILE_APPEND  "a"
#endif

namespace Database {
    fs::FS& fs = SPIFFS;
    bool begin() {
      return SPIFFS.begin(
        #ifdef ESP32
        FORMAT_SPIFFS_IF_FAILED
        #endif
      );
    }

    bool format() {
      return SPIFFS.format();
    }

    bool createFile(String name) {
      File file = fs.open(name, FILE_WRITE);
      if (file) {
        file.close();
        return true;
      } else {
        return false;
      }
    }

    bool writeFile(String name, String data) {
      File file = fs.open(name, FILE_WRITE);
      if (!file) {
        return false;
      }
      if (!file.print(data)) {
        file.close();
        return false;
      }
      file.close();
      return true;     
    }

    bool readFile(String name) {
      Data::payload = "";
      File file = fs.open(name, FILE_READ);
      if (!file) {
        return false;
      }
      int index = 0;
      while (index++ < file.size()) {
        Data::payload += (char)file.read();
      }
      return true;
    }

    bool hasFile(String name) {
      return fs.exists(name);
    }

    bool renameFile(String original, String newer) {
      if (Database::hasFile(original)) {
        fs.rename(original, newer);
        return true;
      }
      return false;
    }
    
    bool removeFile(String filename) {
      if (Database::hasFile(filename)) {
        fs.remove(filename);
        return true;
      }
      return false;
    }

    String& payload() {
      return Data::payload;
    }
};



#endif
