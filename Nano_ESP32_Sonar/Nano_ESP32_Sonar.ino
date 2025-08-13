/*
Nano ESP32 SONAR - HC-SR04P Sonar distance sensing
Demonstrates:
  - Basic SONAR function with returned value
  - Range limiting
  - error checking
*/

// Define hardware devices
const int TRIG = 3;     // SONAR distance sensor TRIG(ger) output
const int ECHO = 5;     // SONAR distance sensor ECHO input

// Define software variables
int distance;           // Distance to closest object in cm

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }
  pinMode(ECHO,INPUT);
  pinMode(TRIG,OUTPUT);
  digitalWrite(TRIG, LOW);  // Ensure module is not triggered
}

// Sonar range function template (real code below - don't peek!)
int sonarRange(void) {
  // Make a TRIG pulse to start distance measurement
  
  // Wait for ECHO to go high and time the ECHO pulse 
  
  // Convert ECHO duration to distance, and return distance value
  
}

void loop() {
  distance = sonarRange();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  delay(100);
}

/*
SONAR ranging function with maximum range limit and error checking.
Returns the range to the closest target in cm. The 'max' parameter
(cm) limits the maximum search range. Returns either: -1 if a previous
ranging operation is still in progress; 0 if no target is found within
the max range; or the range to closest target in cm. Example use:
range = sonarRange(100);  // Find closest target within 100 cm
*/
/* Completed sonarRange function
int sonarRange(int max) {
  if(digitalRead(ECHO) == HIGH) {
    return -1;                // ECHO in progress. Return error code.
  }
  digitalWrite(TRIG, HIGH);   // All clear? Trigger a new SONAR ping.
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Time the ECHO pulse duration (includes TRIG setup and transmit
  // time suitable for most 5V HC-SR04 SONAR modules).
  // ** Use 1220 instead of 320, below, to obtain max range in Tinkercad. **
  // unsigned long duration = pulseIn(ECHO, HIGH, max * 58 + 320);

  // Note: some 3.3V-capable HC-SR04P modules may have much longer
  // TRIG setup times than 5V HC-SR04 modules. Comment out the line
  // above and use the line below when using a slower HC-SR04P module.

  // Time the ECHO pulse duration (for slower HC-SR04P SONAR modules)
  unsigned long duration = pulseIn(ECHO, HIGH, max * 58 + 2320);

  if(duration == 0) {
    return 0;                 // Return 0 if no target is within max range
  }
  return (int(duration / 58));	// Return distance to target in cm
}
*/
