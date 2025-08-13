/*
Nano ESP32 MAC Address - Prints MAC address for ESP-NOW
Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/
*/

#include "WiFi.h"

void setup(){
  Serial.begin(115200);
  while(!Serial) {
    ; // Wait for the USB serial port to start up
  }
  WiFi.mode(WIFI_MODE_STA);
  
  Serial.println(WiFi.macAddress());
}

void loop(){

}