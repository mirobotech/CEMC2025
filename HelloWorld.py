# Import Pin and time functions
from machine import Pin
import time

# Configure Arduino Nano ESP32 built-in LEDs
LED_BUILTIN = Pin(48, Pin.OUT, value=0)   # Active high
LED_BLUE = Pin(45, Pin.OUT, value=1)      # Active low
LED_GREEN = Pin(0, Pin.OUT, value=1)      # Active low
LED_RED = Pin(46, Pin.OUT, value=0)       # Active low
PB1 = Pin(44, Pin.PULL_UP)

while True:
  LED_BLUE.value(0)
  time.sleep(0.5)
  LED_BLUE.value(1)
  time.sleep(0.5)