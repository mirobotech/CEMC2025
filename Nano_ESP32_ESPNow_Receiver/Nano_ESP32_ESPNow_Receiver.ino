/*
Nano ESP32 ESPNow Receiver - receives LED data
Modified from sample code in the Arduino Nano ESP32 User Manual:

  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int redVal;
  int greenVal;
  int blueVal;
  bool ledState;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  analogWrite(LED_GREEN, myData.greenVal);
  digitalWrite(LED_BUILTIN, myData.ledState);
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Red: ");
  Serial.println(myData.redVal);
  Serial.print("Green: ");
  Serial.println(myData.greenVal);
  Serial.print("Blue: ");
  Serial.println(myData.blueVal);
  Serial.print("LED: ");
  Serial.println(myData.ledState);
  Serial.println();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }
  Serial.print("Starting ESP-Now");
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  digitalWrite(LED_BUILTIN, myData.ledState);
}