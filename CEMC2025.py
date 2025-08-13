# Import Pin and time functions
from machine import Pin, PWM, ADC, Timer, time_pulse_us
import time

# Configure Arduino Nano ESP32 built-in LEDs
LED_BUILTIN = Pin(48, Pin.OUT, value=0)   # Active high
LED_BLUE = Pin(45, Pin.OUT, value=1)      # Active low
LED_GREEN = Pin(0, Pin.OUT, value=1)      # Active low
LED_RED = Pin(46, Pin.OUT, value=0)       # Active low

# Configure Breadboard parts from New Arduino, New Tricks session
PB1 = Pin(44, Pin.IN, Pin.PULL_UP)
Q1 = ADC(14, atten = ADC.ATTN_11DB)
POT1 = ADC(1, atten = ADC.ATTN_11DB)
TMP1 = ADC(2, atten = ADC.ATTN_11DB)
TRIG = Pin(6, Pin.OUT)
ECHO = Pin(8, Pin.IN)

# Create PWM objects for servos
# At freq=50, duty_u16 values from 3276-6554 correspond to 1-2ms pulses for
# regular 90 degree hobby servos, and values from 1638-8192 correspond to
# 500us-2.5ms pulses for 180 degree servos. 4915 is the midpoint.
SERVO1 = PWM(Pin(43), freq=50)
SERVO1.duty_u16(4915)

# Set SERVO1 position to rotation degree value
# Un-comment appropriate duty_u16 line to map input value in degrees to pulse length
def servo1_position(deg):
  SERVO1.duty_u16(map(deg, 0, 90, 3277, 6554))    # 1000us-2000us pulses for 90 deg. servo
  # SERVO1.duty_u16(map(deg, 0, 180, 1782, 8192))   # 544us-2500us pulses for 180 deg. servo
  # SERVO1.duty_u16(map(deg, 0, 180, 1638, 8192))   # 500us-2500us pulses for 180 deg. servo

# Declare servo position variables and set starting position
servo1_angle = 45  # Servo position for 0-90 degree servos

# Set servos to starting positions
servo1_position(servo1_angle)

# Map function. Map value in input range to output range.
def map(value, in_min, in_max, out_min, out_max):
  return int((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

# Return SONAR range to closest target in cm. Limit search range to max distance, in cm.
# Example use: object_distance = sonar_range(100)
# -> returns distance to closest target within 100 cm, or returns 0 if no target
# is found or if the target is beyond the specified range. Returns -1 or -2 for ECHO
# time-out errors or if SONAR module is not ready.
def sonar_range(max):
  # Check if previous ECHO has finished
  if ECHO.value() == 1:
    return -2   # ECHO in progress. Exit and return error code.
    # Alternatively, un-coment the lines below to wait for ECHO to
    # finish, and add suggested setup time before starting new TRIG ping.
    # pulse_duration = time_pulse_us(ECHO, 1, 40000)
    # if pulse_duration < 0:
    #     return pulse_duration   # Error, ECHO is stuck high
    # time.sleep_ms(10)           # Recommended SONAR module reset/setup time
        
  # Create TRIG pulse
  TRIG.value(1)
  time.sleep_us(10)
  TRIG.value(0)
  # HC-SR04P (3.3V-capable module, also labelled as RCWL-9610A 2022) delays for 
  # ~2320us after TRIG pulse and before ECHO starts, so max range values shorter
  # that ~40cm will time-out before the ECHO pulse starts. Add an extra check to
  # explicitly wait for ECHO to start when max < 40cm.
  if max < 40:
    pulse_duration = time_pulse_us(ECHO, 0, 2500)
    if pulse_duration < 0:
      return pulse_duration           # ECHO setup took too long - returns -1
    
  # Wait for ECHO to start and time ECHO pulse. Set time-out to max range.
  pulse_duration = time_pulse_us(ECHO, 1, (max + 1) * 58)
  if pulse_duration < 0:
    return 0                        # Out of range - return 0
  else:
    return pulse_duration / 58.2    # Return range in cm 

print()
print("Press PB1 to start!")

while PB1.value() == 1:
  time.sleep_ms(20)
  
while True:
  PB1_state = PB1.value()
  print("PB1: {:d}".format(PB1_state))
  
  Q1_value = Q1.read_u16()
  print("Q1 value: {:d}".format(Q1_value))

  Q1_volts = Q1.read_uv() / 1000000
  print("Q1 voltage: {:.2f} V".format(Q1_volts))

  # add code to read the temperature sensor and convert its value to degrees C
  
  POT1_value = POT1.read_u16()
  servo1_angle = map(POT1_value, 0, 65535, 0, 90)
  servo1_position(servo1_angle)

  # Check SONAR for objects within 100cm
  range = sonar_range(100)
  if range > 0:
    print("Range: {:.1f} cm".format(range))
  elif range == 0:
    print("Over range.")
  elif range == -1:
    print("ECHO end error.")
  elif range == -2:
    print("ECHO in progress.")

  print()
  time.sleep_ms(50)

