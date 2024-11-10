#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>
#include <PCM.h>

const unsigned char danger_Signal[] PROGMEM = {};

const int PulseWire = 0;
const int LED13 = 13;
int Threshold = 550;

int value;
int tmp_sensor = A1;
float voltage;
float temperatureC;

const int sent_time = 10; // 센서 데이터 전달 속도(초)

PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(9600); // 출력
  Serial1.begin(9600); // 블루투스
  Serial2.begin(9600); // stt

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold);   

  pinMode(tmp_sensor, INPUT);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  
  }
}

void loop() {
  if(millis()%(sent_time*1000)){
    value = analogRead(tmp_sensor); // 체온 체크
    voltage = value * 5.0 / 1023.0;
    temperatureC = voltage / 0.01;

    int myBPM = pulseSensor.getBeatsPerMinute();

    if (pulseSensor.sawStartOfBeat()) { // 심박 데이터체크
      Serial.println("A HeartBeat Happened");
      Serial.print("BPM: ");
      Serial.println(myBPM);         
    }
    
    Serial.print("temperature :");
    Serial.print(temperatureC);
    Serial.println("C");

    String send_data = String(myBPM) + " " + String(temperatureC); // 보낼 데이터
    sendInChunks(send_data);
  }
  //startPlayback(danger_Signal, sizeof(danger_Signal)); tts 구현 : 지유 목소리 녹음 ㄱㄱ
}

void sendInChunks(String data) {
  int chunkSize = 64; // 보넬 비트 수(최대 64bit)
  int length = data.length();

  for (int i = 0; i < length; i += chunkSize) {
    String chunk = data.substring(i, i + chunkSize);
    
    Serial1.print(chunk);
    Serial.print("Sent chunk: ");
    Serial.println(chunk);

    delay(100);
  }
}



