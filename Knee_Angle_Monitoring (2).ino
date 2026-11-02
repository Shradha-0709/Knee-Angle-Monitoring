#include <Wire.h>
#include <LiquidCrystal.h>
#include <math.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// MPU6050 I2C addresses
const byte MPU1 = 0x68; // Thigh sensor
const byte MPU2 = 0x69; // Lower-leg sensor

// LED pins
const int GREEN_LED = 8;
const int RED_LED = 9;

// Example alert threshold
const float ALERT_ANGLE = 90.0;

// MPU6050 registers
const byte PWR_MGMT_1 = 0x6B;
const byte ACCEL_XOUT_H = 0x3B;

bool sensor1OK = false;
bool sensor2OK = false;

// Store acceleration readings
struct AccelData {
  int16_t ax;
  int16_t ay;
  int16_t az;
};

// Check whether an I2C device responds
bool sensorExists(byte address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

// Initialize MPU6050
bool initializeMPU(byte address) {
  Wire.beginTransmission(address);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00); // Wake up sensor
  return Wire.endTransmission() == 0;
}

// Read accelerometer data
bool readAccel(byte address, AccelData &data) {
  Wire.beginTransmission(address);
  Wire.write(ACCEL_XOUT_H);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  byte received = Wire.requestFrom(address, (byte)6, (byte)true);
  if (received != 6) {
    return false;
  }

  data.ax = (int16_t)((Wire.read() << 8) | Wire.read());
  data.ay = (int16_t)((Wire.read() << 8) | Wire.read());
  data.az = (int16_t)((Wire.read() << 8) | Wire.read());

  return true;
}

// Estimate tilt angle from accelerometer readings.
// This measures tilt in one plane; sensor mounting matters.
float calculateTilt(AccelData data) {
  float angle = atan2((float)data.ax, (float)data.az) * 180.0 / PI;
  return angle;
}

// Calculate the smallest angular difference
float calculateRelativeAngle(float angle1, float angle2) {
  float difference = angle1 - angle2;

  while (difference > 180.0) {
    difference -= 360.0;
  }

  while (difference < -180.0) {
    difference += 360.0;
  }

  return fabs(difference);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.begin(16, 2);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Knee Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);

  sensor1OK = sensorExists(MPU1);
  sensor2OK = sensorExists(MPU2);

  if (sensor1OK) {
    sensor1OK = initializeMPU(MPU1);
  }

  if (sensor2OK) {
    sensor2OK = initializeMPU(MPU2);
  }

  lcd.clear();

  if (!sensor1OK || !sensor2OK) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");

    lcd.setCursor(0, 1);
    if (!sensor1OK && !sensor2OK) {
      lcd.print("Check both");
    } else if (!sensor1OK) {
      lcd.print("Check MPU 0x68");
    } else {
      lcd.print("Check MPU 0x69");
    }

    Serial.println("Sensor initialization failed.");
    Serial.println("Check wiring and I2C addresses.");

    while (true) {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      delay(500);
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("Sensors Ready!");
  delay(1000);
  lcd.clear();

  Serial.println("Both MPU6050 sensors detected.");
}

void loop() {
  AccelData thigh;
  AccelData lowerLeg;

  if (!readAccel(MPU1, thigh) || !readAccel(MPU2, lowerLeg)) {
    lcd.setCursor(0, 0);
    lcd.print("Reading Error ");
    lcd.setCursor(0, 1);
    lcd.print("Check sensors ");

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    delay(500);
    return;
  }

  // Estimate orientation of each leg segment
  float thighAngle = calculateTilt(thigh);
  float lowerLegAngle = calculateTilt(lowerLeg);

  // Estimate relative knee angle
  float kneeAngle = calculateRelativeAngle(thighAngle, lowerLegAngle);

  // Display angle
  lcd.setCursor(0, 0);
  lcd.print("Knee Angle:     ");
  lcd.setCursor(0, 1);
  lcd.print(kneeAngle, 1);
  lcd.print((char)223);
  lcd.print(" ");

  // Example threshold for project demonstration
  if (kneeAngle > ALERT_ANGLE) {
    lcd.print("ALERT!");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else {
    lcd.print("Normal");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  // Print readings to Serial Monitor
  Serial.print("Thigh tilt: ");
  Serial.print(thighAngle, 1);
  Serial.print(" | Lower-leg tilt: ");
  Serial.print(lowerLegAngle, 1);
  Serial.print(" | Estimated knee angle: ");
  Serial.println(kneeAngle, 1);

  delay(200);
}
