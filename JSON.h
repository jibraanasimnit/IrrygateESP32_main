#ifndef JSON_H__
#define JSON_H__
#define EMPTY      0
#define qKey(x)    (String('"') + x + "\":")

#include "data.h"
namespace JSON {
  String read(String&, String);
  String JSON();
  bool hasKey(String&, String);


  bool hasKey(String key) {
    return hasKey(Data::payload, key);
  }
  bool hasKey(String& json, String key) {
    if (json.indexOf(qKey(key)) > -1) {
      return true;
    }
    return false;
  }
  template <typename T>
  void add(String& json, String key, T __value, bool nonString = false) {
    if (JSON::hasKey(json, key)) {
      String val = JSON::read(json, key);
      json.replace(qKey(key) + '"' + val + '"', qKey(key) + '"' + String(__value) + '"');
      json.replace(qKey(key) + val, qKey(key) + String(__value));
    } else {
      json += "~";
      if (nonString) {
        json.replace("}~", String(',') + qKey(key) + String(__value) + '}');
      } else {
        json.replace("}~", String(',') + qKey(key) + '"' + String(__value) + "\"}");
      }
      json.replace("{,", "{");
    }
  }

  



  String JSON() {
    return "{}";
  }
  String read(String key) {
    return read(Data::payload, key);
  }
  String read(String& json, String key) {
    if (JSON::hasKey(json, key)) {
      int index = json.indexOf(qKey(key));
      index += qKey(key).length();
      if (isDigit(json[index])) {
        float response;
        sscanf(json.substring(index).c_str(), "%f", &response);
        return String(response);
      }
      if (json[index] == '"') {
        index++;
        json = json.substring(index);
        return json.substring(0, json.indexOf('"'));
      }
      char startCharacter = json[index];
      char endCharacter;
      if (startCharacter == '{') endCharacter = '}';
      if (startCharacter == '[') endCharacter = ']';
      int stack = 0;
      int startIndex = index;
      for (int i=index; i<json.length(); i++) {
        if (json[i] == startCharacter) {
          stack++;
        } else if (json[i] == endCharacter) {
          stack--;
        }
        if (stack == EMPTY) {
          return json.substring(startIndex, i+1);
        }
      }      
      return json.substring(index);
    }
    return "";
  }
};
#endif
