#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>
#include "VoiceRecognitionV3.h"
#include <SoftwareSerial.h>

const int PulseWire = 0;
const int LED13 = 13;
int Threshold = 550;

int value;
int tmp_sensor = A1;
float voltage;
float temperatureC;

bool yes_or_no;
bool danger = false;

char speaker[1];

uint8_t buf[64];

VR myVR(2,3);
SoftwareSerial SpeakerInstruct(4,5);

PulseSensorPlayground pulseSensor;
void setup() {
  Serial.begin(9600);
  myVR.begin(9600);

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold);   

  pinMode(tmp_sensor, INPUT);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  
  }
}

void loop() {
  if(Serial.available()>0){
    char reads = Serial.read();
    if(reads == '1'){
      danger == true;
    }
  }
  if(millis()%1000==0){
    value = analogRead(tmp_sensor); // 체온 체크
    voltage = value * 5.0 / 1023.0;
    temperatureC = voltage / 0.01;
    temperatureC -= 13;

    int myBPM = pulseSensor.getBeatsPerMinute();

    if (pulseSensor.sawStartOfBeat()) { // 심박 데이터체크
      Serial.println("A HeartBeat Happened");
      Serial.print("BPM: ");
      Serial.println(myBPM);         
    }
    
    Serial.print("temperature :");
    Serial.print(temperatureC);
    Serial.println("C");
    String send_temperatureC = (String(temperatureC).length()<5)? (String(temperatureC).length()<4) ? "00" + String(temperatureC): "0" + String(temperatureC): String(temperatureC);
    String send_BPM = (String(myBPM).length()<3)? (String(myBPM).length()<2) ? "00" + String(myBPM): "0" + String(myBPM): String(myBPM);
    String send_data = send_BPM + " " + send_temperatureC;
    sendInChunks(send_data);
  }
  int ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case (0)://도와줘
        speaker[0] = 'h';
        break;
      case (1)://살려줘
        speaker[0] = 'f';
        break;
      case (2)://구해줘
        speaker[0] = 'w';
        break;
      case (3)://어
        yes_or_no = true;
        speaker[0] = 'y';
        break;
      case (4)://아니
        yes_or_no = false;
        speaker[0] = 'n';
        break;
      case (5)://아파
        speaker[0] = 's';
        break;
    }
    SpeakerInstruct.write(speaker);
    Serial.println(speaker);
  }
  delay(1000);
}

void sendInChunks(String data) {
  int chunkSize = 10;
  int length = data.length();

  for (int i = 0; i < length; i += chunkSize) {
    String chunk = data.substring(i, i + chunkSize);
    
    Serial.write(chunk.c_str()); 
    Serial.print("Sent chunk: ");
    Serial.println(chunk);

    delay(100);
  }
}

