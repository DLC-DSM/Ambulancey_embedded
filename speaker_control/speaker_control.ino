#include <PCM.h>

const unsigned char help[] PROGMEM = {}; //tts
const unsigned char fuck_help[] PROGMEM = {};
const unsigned char wtf_help[] PROGMEM = {};
const unsigned char danger[] PROGMEM = {};//대답
const unsigned char Q[] PROGMEM = {};//질문
const unsigned char sick[] PROGMEM = {};


void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    String read_data = Serial.read();
    switch (read_data) {
    case 'h':
      startPlayback(help, sizeof(help));
      break;
    case 'f':
      startPlayback(fuck_help, sizeof(fuck_help));
      break;
    case 'w':
      startPlayback(wtf_help, sizeof(wtf_help));
      break;
    case 't':
      startPlayback(Q, sizeof(Q));
      break;
    case 'D':
      startPlayback(danger, sizeof(danger));
      break;
    case 's':
      startPlayback(sick, sizeof(sick));
      break;
    }
  }
}
