#ifndef WIFI_H__
#define WIFI_H__
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "JSON.h"
#include "database.h"



#define AP_SSID    "ESP32"      // replace your deviceID here
#define AP_PASS    "12345678"

#define WIFI_CREDENTIALS_UNIQUE_KEY   88

#define WIFI_TIME_OUT    15000

namespace WIFI {
  bool justConnected = true;
  int disconnectionCount = 0;
  bool justDisconnected = true;
  bool credentialsUpdated = false;

  String defaultSSID = "Y";
  String defaultPASS = "oxygenkills";

  String ssid;
  String password;
  void updateCredentials(String, String); 
  bool timeout() {
    static uint32_t start = millis();
    if (millis() - start > WIFI_TIME_OUT) {
      start = millis();
      return true;
    }
    return false;
  }

  void ensureConnection() {
    if (WiFi.status() == WL_CONNECTED) {
      disconnectionCount = 0;
      if (justConnected) {
        justDisconnected = true;
        Serial.println(String("Device Connected to ") + WIFI::ssid);
      }
    } else {
      if (justDisconnected) {
        justConnected = true;
        justDisconnected = false;
        Serial.println(String("Device disconnected from ") + WIFI::ssid);
      }
      if (timeout()) {
        disconnectionCount++;
        WiFi.begin(WIFI::ssid.c_str(), WIFI::password.c_str());
        Serial.println(String("Trying to reconnect to ") + WIFI::ssid);
      }
    }
  }
  void loadCredentials() {
    Database::readFile("/credentials.json");
    if (JSON::read("wcuk").toInt() != WIFI_CREDENTIALS_UNIQUE_KEY) {
      updateCredentials(WIFI::defaultSSID, WIFI::defaultPASS);
      Database::readFile("/credentials.json");
    }
    Serial.println(Database::payload());
    WIFI::ssid = JSON::read("ssid");
    WIFI::password = JSON::read("password");

  }

  void updateCredentials(String wifi, String password) {
    String credentials = JSON::JSON();
    JSON::add(credentials, "ssid", wifi);
    JSON::add(credentials, "password", password);
    JSON::add(credentials, "wcuk", WIFI_CREDENTIALS_UNIQUE_KEY, true);
    Database::writeFile("/credentials.json", credentials);
  }
  void begin() {
    WIFI::loadCredentials();
    WiFi.begin(WIFI::ssid.c_str(), WIFI::password.c_str());   // read wifi credentials from EEPROM and start in wifi mode
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASS);
  }

  bool connected() {
    return WiFi.status() == WL_CONNECTED;
  }

};
#endif