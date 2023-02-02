#include <touch.h>

void touch_callback(touch::length_t);

touch pad = touch(PA0, PA13, 15 , touch_callback);

void touch_callback(touch::length_t len) {
  static int led = 0;
  
  Serial.println(len);
  
  switch(len) {
    case(touch::SHORT_TAP):
        Serial1.println("AT+PLAYPAUSE"); 
        break;
    case(touch::LONG_TAP):
        Serial1.println("AT+HFPVR=1");
        break;
    case(touch::LONG_HOLD):
        digitalWrite(PC13, led ^= true);
        break;
//    case(touch::DOUBLE_TAP):
//        Serial1.println("AT+FORWARD");
//        break;
  }
};

void pad_update() {
  pad.update();
  yield();
}
