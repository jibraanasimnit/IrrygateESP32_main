#ifndef MAC_H__
#define MAC_H__
namespace MAC {
  String mac;
  void load() {
    char __address[20];
    WiFi.macAddress().toCharArray(__address, 18);
    mac = String(__address);
    mac.replace(":", "");
  }

  String getMac() {
    return mac;
  }
};
#endif