/*
Nano ESP32 Temperature - Demonstrates temperature sensing
Demonstrates:
  - analog input and temperature conversion
  - float calculation
  - int() conversion
*/

// Define hardware devices
const int TMP1 = A1;

// Define software variables
int rawTemp;
float tempV;              // Temperature sensor voltage in mV
float tempC;

void setup() {
  pinMode(TMP1, INPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }
}

void loop() {
  rawTemp = analogRead(TMP1);
  tempV = rawTemp * 3300 / 4095 + 70; // Convert to mV + correction
  tempC = (tempV - 500) / 10;   // Remove offset and convert to °C

  Serial.print("Raw temp: ");
  Serial.print(rawTemp);
  Serial.print("\tTemp (mV): ");
  Serial.print(tempV);
  Serial.print("\tTemp °C: ");
  Serial.println(tempC);

  delay(1000);
}
