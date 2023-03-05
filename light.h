#ifndef LIGHT_H__
#define LIGHT_H__


#define LIGHT_SENSOR_PIN 33 

namespace Light {
  int getVal(){
    float reading = 0;
    int total = 50;
    for (int i=0; i<total; i++) {
      reading += analogRead(LIGHT_SENSOR_PIN);
      delay(1);
    }
    return reading / total;
  }
}

#endif