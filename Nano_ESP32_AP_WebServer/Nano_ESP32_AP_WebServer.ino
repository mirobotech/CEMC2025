/*
  Arduino Nano ESP32 WiFi Access point and web server

  Creates a wireless access point and hosts a simple webserver
  that allows web control of the RGB LED.
 */

#include "WiFi.h"
#include "arduino_secrets.h" 

// Enter your secret SSID and password data in the arduino_secrets.h file/tab
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

// The default local IP address is 192.168.4.1
// Enter custom AP configuration details below:
IPAddress local_ip(192,168,4,2);
IPAddress gateway(192,168,4,2);
IPAddress subnet(255,255,255,0);

// Define variables
int redState = HIGH;              // Starting LED pin states
int greenState = HIGH;
int blueState = LOW;

const char index_html[] PROGMEM = R"rawHTMLpage(
<!DOCTYPE HTML><html>
<head>
  <title>Nano ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Verdana,Helvetica,sans-serif; display: inline-block; text-align: center;}
    h1 {font-size: 2rem;}
    h2 {font-size: 2rem;}
    p {font-size: 1.6rem;}
    body {max-width: 600px; margin: 0px auto; padding-bottom: 25px;}
    .button {
      background-color: white;
      border: 2px solid #124881;
      border-radius: 12px;
      color: #124881;
      font-size: 1.2rem;
      padding: 0;
      width: 64px;
      height: 64px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      margin: 0px auto;
      transition-duration: 0.2s;
      cursor: pointer;
    }
    .button:active {
      color: white;
      background-color:#2496F2;
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 64px;
      height: 34px;
    }

    /* Hide default HTML checkbox */
    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    /* Create the slider */
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #124881;
      -webkit-transition: .2s;
      transition: .2s;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .2s;
      transition: .2s;
    }

    input:checked + .slider {
      background-color: #2496F2;
    }

    input:focus + .slider {
      box-shadow: 0 0 1px #2496F2;
    }

    input:checked + .slider:before {
      -webkit-transform: translateX(30px);
      -ms-transform: translateX(30px);
      transform: translateX(30px);
    }

    /* Rounded sliders */
    .slider.round {
      border-radius: 34px;
    }

    .slider.round:before {
      border-radius: 50%;
    }
  </style>
  <script>
  function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?pd="+element.id+"&val=1", true); }
  else { xhr.open("GET", "/update?pd="+element.id+"&val=0", true); }
  xhr.send();
  }

  function buttonPress(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?pd="+element.id+"&val=1", true);
  xhr.send();
  }

  function buttonRelease(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?pd="+element.id+"&val=0", true);
  xhr.send();
  }
  </script>
</head>
<body>
  <h2>ESP32 RGB Control</h2>
  <p>Boop the buttons!</p>
  <p>
    <button class="button" onmousedown="buttonPress(this)" onmouseup="buttonRelease(this)" id="12">R</button>
    <button class="button" onmousedown="buttonPress(this)" onmouseup="buttonRelease(this)" id="13">G</button> 
    <button class="button" onmousedown="buttonPress(this)" onmouseup="buttonRelease(this)" id="14">B</button>
  </p>
  <p>Toggle the switches!</p>
  <p style="text-align: left; margin-left: 30%;">
  <label class="switch">
    <input type="checkbox" onchange="toggleCheckbox(this)" id="2" >
    <span class="slider round"></span>
  </label>
  Red
  </p>
  <p style="text-align: left; margin-left: 30%;">
  <label class="switch">
    <input type="checkbox" onchange="toggleCheckbox(this)" id="3" >
    <span class="slider round"></span>
  </label>
  Green
  </p>
  <p style="text-align: left; margin-left: 30%;">
  <label class="switch">
    <input type="checkbox" onchange="toggleCheckbox(this)" id="4" checked>
    <span class="slider round"></span>
  </label>
  Blue
  </p>
</body>
</html>
)rawHTMLpage";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Active HIGH
  pinMode(LED_BLUE, OUTPUT);    // Active LOW
  pinMode(LED_GREEN, OUTPUT);   // Active LOW
  pinMode(LED_RED, OUTPUT);     // Active LOW
  digitalWrite(LED_BLUE, LOW);  // Turn blue LED on

  // Initialize the serial port and wait for it to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }

  // Start the AP (access point)
  startAP();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // When a client connects,
    Serial.println("Client connected");     // print a message to the serial monitor
    String currentLine = "";                // Create a String to hold incoming client data
    while (client.connected()) {            // Loop while the client is connected
      if (client.available()) {             // If client has bytes to read,
        char c = client.read();             // read a byte and
        Serial.write(c);                    // copy it to the serial monitor.
        if (c == '\n') {                    // If the byte is a newline character...
          // If the current line is blank, two newline characters were received in a row
          // ending the client HTTP request, so answer with a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Send the content of the HTTP response following the header:
            client.print(index_html);       // Send web page

            // End the HTTP response with another blank line:
            client.println();

            // Connection ended, so break out of the while loop:
            break;
          }
          else {      // The first newline character clears currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // If character is not a carriage return,
          currentLine += c;      // add it to the end of the currentLine
        }
        else if (c == '\r') {    // If character is a carriage return
          // check client header for 'GET /update?' XML HTTP requests

          // Data is sent in the format 'GET /update?pd=1&val=1', where
          // 'pd' = peripheral device and 'val' = value.
          if (currentLine.indexOf("pd=2") > 0 || currentLine.indexOf("pd=12") > 0) {
            if (currentLine.indexOf("val=1") > 0) {
              redState = LOW;
            }
            if (currentLine.indexOf("val=0") > 0) {
              redState = HIGH;
            }
            digitalWrite(LED_RED, redState);
          }

          if (currentLine.indexOf("pd=3") > 0 || currentLine.indexOf("pd=13") > 0) {
            if (currentLine.indexOf("val=1") > 0) {
              greenState = LOW;
            }
            if (currentLine.indexOf("val=0") > 0) {
              greenState = HIGH;
            }
            digitalWrite(LED_GREEN, greenState);
          }

          if (currentLine.indexOf("pd=4") > 0 || currentLine.indexOf("pd=14") > 0) {
            if (currentLine.indexOf("val=1") > 0) {
              blueState = LOW;
            }
            if (currentLine.indexOf("val=0") > 0) {
              blueState = HIGH;
            }
            digitalWrite(LED_BLUE, blueState);
          }
        }
      }
    }

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
    Serial.println();
  }
}

void startAP() {
  Serial.println(F("Starting Arduino Nano ESP32 Web Server"));
  Serial.println(F("Creating access point"));
  // Create access point
  if (!WiFi.softAP(ssid, pass)) {
    Serial.println("Access point creation failed - system halted.");
    while(true);
  }

  // Use custom IP address (optional)
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  // Print network details:
  Serial.print("Join network '");
  Serial.print(WiFi.softAPSSID());
  IPAddress IP = WiFi.softAPIP();
  Serial.print("' and browse to: http://");
  Serial.println(IP);
  
  // Start http server
  server.begin();
  Serial.println("HTTP server started");
}
