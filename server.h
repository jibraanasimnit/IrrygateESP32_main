#ifndef SERVER_H__
#define SERVER_H__
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "database.h"
#include "JSON.h"
#include "WiFi.h"
namespace MyWebServer {
  bool updatesNeeded = true;
  AsyncWebServer server(80);

  void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    File f;
    if (!index) {
      Serial.println("Handle File Upload Name: " + filename);
      f = SPIFFS.open("/" + filename, "w");
    }
    if (!f) {
      f = SPIFFS.open("/" + filename, "a");
    }
    if (!f) {
      Serial.println("File Open Failed");
      return;
    }
    if (len) {
      f.write(data, len);
    }
    if (final) {
      f.close();
    }
  }

  bool initializeDNS() {
    for (int i=0; i<10; i++) {
      String domain = String("freq_gen") + i;
      if (MDNS.begin(domain.c_str())) {
        Serial.print("Domain assigned: ");
        Serial.println(domain);
        MDNS.addService("http", "tcp", 80);
        return true;
      }
    }
    return false;
  }

  void begin() {
    // server.on("/file", HTTP_POST, [](AsyncWebServerRequest *request){
    //   AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"status\":\"okay\"}");
    //   response->addHeader("Access-Control-Allow-Origin", "*");
    //   request->send(response);
    // }, handleFileUpload);
    
    // server.on("/remove", HTTP_GET, [](AsyncWebServerRequest *request){
    //   String path = request->arg("path");
    //   Serial.println(String("Removing ") + path);
    //   Database::removeFile(path);
    //   AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"status\":\"okay\"}");
    //   response->addHeader("Access-Control-Allow-Origin", "*");
    //   request->send(response);
    // });
    
    // server.on("/rename", HTTP_GET, [](AsyncWebServerRequest *request){
    //   String path = request->arg("path");
    //   String newname = request->arg("name");
    //   Serial.println(String("Renaming ") + path + " to " + newname);
    //   Database::renameFile(path, newname);
    //   AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"status\":\"okay\"}");
    //   response->addHeader("Access-Control-Allow-Origin", "*");
    //   request->send(response);
    // });
    
    // server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request){
    //   String path = request->arg("path");
    //   AsyncWebServerResponse *response;
    //   if (Database::hasFile(path)) {
    //     Serial.println(path);
    //     response = request->beginResponse(Database::fs, path);
    //   } else {
    //     String data = JSON::JSON();
    //     JSON::add(data, "status", "file not found");
    //     response = request->beginResponse(400, "application/json", data);
    //   }
    //   response->addHeader("Access-Control-Allow-Origin", "*");
    //   request->send(response);
    // });

    server.on("/credential", HTTP_GET, [](AsyncWebServerRequest *request) {
      String ssid = request->arg("ssid");
      String pass = request->arg("password");
      Serial.println(ssid);
      Serial.println(pass);
      AsyncWebServerResponse *response;
      if (!ssid.length()) {
        String data = JSON::JSON();
        JSON::add(data, "error", "invalid ssid");
        response = request->beginResponse(400, "application/json", data);
      } else {
        String data = JSON::JSON();
        JSON::add(data, "status", "done");
        response = request->beginResponse(200, "application/json", data);
      }

      WIFI::updateCredentials(ssid, pass);
      response->addHeader("Access-Control-Allow-Origin", "*");
      request->send(response);
    });
    
    server.begin();
  }
}

#endif