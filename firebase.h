
#ifndef FIREBASE_H__
#define FIREBASE_H__
#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://irrygatedb-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "hx9xxTzpfpOAMPVMnC1SbBYK7ywKQIYWNzhA6bBA"

namespace FIREBASE {
  FirebaseData firebaseData;
  bool begin() {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    Firebase.setwriteSizeLimit(firebaseData, "tiny");
    return true;
  }
 
  void updateData(float temperature, float humidity, int lux) {
    FirebaseJson data;
    FirebaseData fbd;
    data.add("temperature", String(temperature));
    data.add("humidity", String(humidity));
    data.add("lux", String(lux));
    Firebase.push(fbd, String("/data"), &data);
  }
}
#endif