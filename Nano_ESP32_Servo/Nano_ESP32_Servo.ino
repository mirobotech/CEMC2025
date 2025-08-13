/*
Nano ESP32 Servo - Servo control by potentiometer
Demonstrates:
  - Servo pulse creation using a simple function
  - Periodic code executing by counting millis()
*/

// Define hardware devices
const int POT1 = A0;
const int SERVO1 = 1;

// Define software variables
int potPosition;            // Raw pot input
int servo1Position;         // Servo 1 position in degrees

// Servo pulse function creates a servo pulse on the specified servo
// pin and should be called every 15-20ms. Map pulse duration to degrees.
// Example use: servoPulse(H5OUT, 45);
// -> creates a 1500us servo pulse on BEAPER Nano header pin H5 (using 90 deg servos).
void servoPulse(int servoPin, int deg) {
  int pulse = map(deg, 0, 90, 1000, 2000);  // For 90 degree (standard) servos
  // int pulse = map(deg, 0, 180, 500, 2500);  // For 180 degree servos
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(servoPin, LOW);
}

// Define and set timer variables
unsigned long servoTimer = millis();
unsigned long updateTimer = millis();

void setup() {
  pinMode(POT1, INPUT);
  pinMode(SERVO1, OUTPUT);
  digitalWrite(SERVO1, LOW);
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }
}

void loop() {
  potPosition = analogRead(POT1);
  servo1Position = map(potPosition, 0, 4095, 0, 90);

  if(millis() - servoTimer > 15) {
    servoTimer = millis();
    // Update servo from potentiometer position
    servoPulse(SERVO1, servo1Position);
  }

  if(millis() - updateTimer > 500) {
    updateTimer = millis();
    Serial.print("Pot: ");
    Serial.print(potPosition);
    Serial.print("  \tServo: ");
    Serial.println(servo1Position);
  }

  delay(1);
}
