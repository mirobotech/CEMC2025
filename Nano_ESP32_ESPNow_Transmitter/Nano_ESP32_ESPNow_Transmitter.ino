/*
Nano ESP32 ESPNow Transmitter - Transmits data to control LEDs
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

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xDC, 0xDA, 0x0C, 0x21, 0x63, 0x90};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int redVal;
  int greenVal;
  int blueVal;
  bool ledState;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Define hardware devices
const int POT1 = A0;
const int PB1 = D0;

// Define software variables
int potPosition;
int red = 0;
int green = 0;
int blue = 0;
bool led = LOW;
bool PB1State;

void setup() {
  pinMode(PB1, INPUT_PULLUP);
  pinMode(POT1, INPUT);
  // Init Serial Monitor
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  myData.redVal = 0;
  myData.greenVal = map(analogRead(POT1), 0, 4095, 255, 0);
  myData.blueVal = 0;
  myData.ledState = digitalRead(PB1);
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(500);
}