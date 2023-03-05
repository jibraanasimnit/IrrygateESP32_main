#ifndef DHT_11_H__
#define DHT_11_H__
#include <DHT.h>

//// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define DHTPIN  4
namespace DHT_11 {
  int currentReading;
  float h;
  float t;
  float f;
  
  DHT dht(DHTPIN, DHTTYPE);
  void read() {
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);
  }
  
  int getTempInC() {
    return t;
  }
  
  int getTempInF() {
    return f;
  }
  
  int getHumidity() {
    return h;
  }

  void begin() {
    dht.begin();
  }
};
#endif
