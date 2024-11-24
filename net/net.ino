#include <WiFiS3.h>

char ssid[] = " ";
char pass[] = " ";
int keyIndex = 0; 

int status = WL_IDLE_STATUS;
char server[] = "192.168.171.2";

WiFiClient wifiClient;

const int chunkSize = 10;
char buffer[chunkSize];
int index = 0;

String chunk = "000 000.00";

uint8_t receive = 0;

void setup() {
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  if (wifiClient.connect("server", 8030)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    // wifiClient.println("GET /tables/list/ HTTP/1.1");
    wifiClient.println("GET / HTTP/1.1");
    wifiClient.println("Host: " + String(server));
    wifiClient.println("Connection: close");
    wifiClient.println();
  }
}

void loop() {
  read_response();

  while (Serial.available() > 0) {
    buffer[index++] = Serial.read();
    if (index >= chunkSize) {
      buffer[index] = '\0';
      chunk = String(buffer);
      index = 0;                   
    }
  }
  if (wifiClient.connected()) {
    wifiClient.println(chunk);
  }


  if (!wifiClient.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    wifiClient.stop();
    while (true);
  }
  delay(100);
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void read_response() {
  uint32_t received_data_num = 0;
  while (wifiClient.available()) {
    char c = wifiClient.read();
    receive = static_cast<int>(c);
  }
  if(receive == 1){
    Serial.write("E");
    receive = 0;
  }
}