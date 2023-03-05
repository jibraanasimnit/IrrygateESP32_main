#include "WiFi.h"
#include "database.h"
#include "firebase.h"
#include "light.h"
#include "server.h"
#include "DHT_11.h"


void setup() {
  Serial.begin(115200);
  Database::begin();
  WIFI::begin();
  DHT_11::begin();
  MyWebServer::begin();
}


void loop() {
  WIFI::ensureConnection();
  DHT_11::read();
  
  if (WIFI::connected()) {
    if (WIFI::justConnected) {
      WIFI::justConnected = false;
      Serial.println(WiFi.localIP());
      if (FIREBASE::begin()) {
        Serial.println("Firebase initialized");
      }
    }
  }

  static uint32_t start = millis();
  if (millis() - start > 5000) {
    start = millis();
    float t = DHT_11::getTempInC();
    float h = DHT_11::getHumidity();
    float l = Light::getVal();
    
    Serial.println(String("t: ") + t);
    Serial.println(String("h: ") + h);
    Serial.println(String("l: ") + l);
    if (WIFI::connected()) {
      FIREBASE::updateData(t, h, l);
      Serial.println("data updated");
    } else {
      Serial.println("wifi not connected");
    }
  }

}
