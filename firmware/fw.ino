#include <Scheduler.h>

#include "oled.h"
#include "pad.h"

void probe_state() {
  Serial1.println("AT+A2DPSTAT");
  delay(5000);
}

void setup() {
  Serial1.begin(115200);
  Serial.begin();

  oled.init();
  oled.display();

  Serial1.println("AT+REBOOT");
  
  pad.calibrate();

  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);

  delay(1000);
  
  oled.clear();
  
  Scheduler.startLoop(oled_update, 2048);
  Scheduler.startLoop(pad_update, 1024);
  Scheduler.startLoop(probe_state, 256);
}

void loop() {      
  if(Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    
    Serial.println(cmd);

    if(cmd.startsWith("+TRACKINFO")) {
      info.media = cmd.substring(11, cmd.lastIndexOf('\xff'));
      info.media.replace("\xff", " - ");
      info.media.concat(" - ");
    }

    if(cmd.startsWith("+TRACKSTAT")) {
      int comma = cmd.indexOf(',', 13);
      
      info.progress.t = cmd.substring(13, comma).toInt();
      info.progress.T = cmd.substring(comma + 1, cmd.length()).toInt();
    }
    
    if(cmd.startsWith("+PLAYSTAT")) {
      info.playstat = cmd.substring(10, cmd.length()).toInt();
    }

    if(cmd.startsWith("+A2DPSTAT")) {
      info.connstat = cmd.substring(10, cmd.length()).toInt();
    }

    if(cmd.startsWith("+HFPAUDIO")) {
      info.vrstat = cmd.substring(10, cmd.length()).toInt();
    }
    
  }
  
  delay(50);
}
