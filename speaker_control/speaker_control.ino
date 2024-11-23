#include <PCM.h>

const int chunkSize = 2;
String chunk;
char buffer[chunkSize];
int index = 0;

const unsigned char a[] PROGMEM = {}; //tts

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    buffer[index++] = Serial.read();
    if (index >= chunkSize) {
      buffer[index] = '\0';
      chunk = String(buffer);
      index = 0;
    }
  }
}
