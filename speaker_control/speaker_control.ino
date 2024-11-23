#include <PCM.h>

const unsigned char a[] PROGMEM = {}; //tts

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    String incomingData = mySerial.readString();
    if(incomingData == "a"){
      startPlayback(a, sizeof(a));
    }
  }
}
