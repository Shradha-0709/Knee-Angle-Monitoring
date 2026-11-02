# KNEE ANGLE MONITORING

## Project Overview

The Knee Angle Monitoring project is a low-cost educational prototype developed using Arduino Uno and two MPU6050 sensors.

One sensor is attached to the thigh and the other to the lower leg. The sensors measure the tilt of each leg segment. The Arduino calculates the relative knee angle and displays it on a 16×2 LCD.

LED indicators provide a simple alert based on a demonstration threshold of 90°.

## Components Required

- Arduino Uno – 1
- MPU6050 Sensor – 2
- 16×2 LCD Display – 1
- 10kΩ Potentiometer – 1
- Green LED – 1
- Red LED – 1
- 220Ω–330Ω Resistors – 2
- Breadboard and jumper wires
- USB cable
- Mounting straps

## Connections

### MPU6050 Sensors

| Sensor Pin | Arduino Pin |
|---|---|
| SDA | A4 |
| SCL | A5 |
| VCC | Appropriate supply |
| GND | GND |
| First sensor AD0 | LOW (0x68) |
| Second sensor AD0 | HIGH (0x69) |

### LCD Connections

| LCD Pin | Arduino Pin |
|---|---|
| RS | 12 |
| E | 11 |
| D4 | 5 |
| D5 | 4 |
| D6 | 3 |
| D7 | 2 |
| RW | GND |

### LED Connections

- Green LED – Pin 8
- Red LED – Pin 9

Use current-limiting resistors for both LEDs.

## Working Principle

1. Two MPU6050 sensors collect accelerometer readings.
2. Arduino reads the sensor data through I2C communication.
3. Tilt angles are calculated using the atan2() function.
4. The relative angle between the thigh and lower leg is calculated.
5. The knee angle is displayed on the LCD.
6. If the angle exceeds 90°, the red LED glows.
7. Otherwise, the green LED glows.

## Software Requirements

- Arduino IDE
- Wire Library
- LiquidCrystal Library
- C/C++ programming

## Applications

- Educational biomechanics projects
- Human movement monitoring demonstrations
- Rehabilitation technology prototypes
- Embedded systems learning

## Future Scope

- Integration of gyroscope readings.
- Use of complementary or Kalman filters.
- Improved sensor calibration.
- Wireless monitoring using Bluetooth or Wi-Fi.
- Data logging and graphical analysis.
- Validation against a reference goniometer.

## Limitations

The current prototype uses accelerometer-based tilt estimation only. Sensor alignment, movement acceleration and mounting consistency can affect the readings. It is not a clinically validated medical device.

## Authors

- J Shradha – E0525027
- A S Thejashwini – E0525059

**Institution:** Sri Ramachandra Institute of Higher Education and Research

**Course:** Arduino for Internet of Things (IoT Projects)
