/*
  Arduino LSM6DS3 - Using both Accelerometer & Gyroscope
  WM Harris 2/26/22
  adapted from Arduino LSM6DS3 examples by Riccardo Rizzo
  d12  - accel or gyro switch
  LED pairs: one for pos values, one for neg values
  10 - xPos
  d9 - xNeg
  d6 - zNeg
  d5 - zPos
  d3 - yNeg
  d2 - yPos
*/

#include <Arduino_LSM6DS3.h>
//input switch to gyro or accel
const int switchPin = 12;

//LEDs to show readings
const int xNeg = 9;
const int xPos = 10;
const int yNeg = 3;
const int yPos = 2;
const int zNeg = 6;
const int zPos = 5;

//max readings for LED calibration
float maxA = .01;
float maxG = .01;

void setup() {
  pinMode(switchPin, INPUT);
  //Set all LED pins for output
  //including skipped pins for easy looping
  for (int apin = 2; apin <= 10; apin++) {
    pinMode(apin, OUTPUT);
  }
  // Turn on LEDs til serial connects & LSM starts
  for (int apin = 2; apin <= 10; apin++) {
    digitalWrite(apin, HIGH);
  }
  
  Serial.begin(9600);
  while (!Serial);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  //test gyro
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  //test accel
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  //Turn off all LEDs
  for (int apin = 2; apin <= 10; apin++) {
    digitalWrite(apin, LOW);
  }
}

void loop() {
  float x, y, z;

  if (digitalRead(switchPin) == HIGH) { //gyro
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
      
      Serial.print(0); //for gyro
      Serial.print("\t");
      Serial.print(x);
      Serial.print("\t");
      Serial.print(y);
      Serial.print("\t");
      Serial.print(z);
      Serial.print("\t");
      Serial.println(maxG);

      int gpin; // target LED pin
      
      if (x > 0) { //pos or neg? turn off other LED
        digitalWrite(xPos, LOW);
        gpin = xNeg;
      } else {
        digitalWrite(xNeg, LOW);
        gpin = xPos;
      }
      x = abs(x);
      maxG = max(maxG, x); //update max gyro value
      
      x = map(x, 0, maxG, 0, 255); //scale to 8bit
      analogWrite(gpin, x);

      if (y > 0) {//pos or neg? turn off other LED
        digitalWrite(yPos, LOW);
        gpin = yPos;
      } else {
        digitalWrite(yNeg, LOW);
        gpin = yNeg;
      }
      y = abs(y);
      maxG = max(maxG, y); //update max gyro value
      
      y = map(y, 0, maxG, 0, 255); //scale to 8bit
      analogWrite(gpin, y);

      if (z > 0) {//pos or neg? turn off other LED
        gpin = zPos;
        digitalWrite(zNeg, LOW);
      } else {
        gpin = zNeg;
        digitalWrite(zPos, LOW);        
      }
      z = abs(z);
      maxG = max(maxG, z); //update max gyro value
      
      z = map(z, 0, maxG, 0, 255); //scale to 8bit
      analogWrite(gpin, z);
    }
  } else { //accel
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      
      Serial.print(1); //for accel
      Serial.print("\t");
      Serial.print(x);
      Serial.print("\t");
      Serial.print(y);
      Serial.print("\t");
      Serial.print(z);
      Serial.print("\t");
      Serial.println(maxA);

      int apin = xPos;
      if (x > 0) { //pos or neg? turn off other LED
        digitalWrite(xPos, LOW);
        apin = xNeg;
      } else {
        digitalWrite(xNeg, LOW);
        apin = xPos;
      }
      x = abs(x);
      maxA = max(maxA, x); //update max accel value
      
      x = map(x, 0, maxA, 0, 255); //scale to 8bit
      analogWrite(apin, x);

      if (y > 0) { //pos or neg? turn off other LED
        digitalWrite(yPos, LOW);
        apin = yPos;
      } else {
        digitalWrite(yNeg, LOW);
        apin = yNeg;
      }
      y = abs(y);
      maxA = max(maxA, y); //update max accel value
      
      y = map(y, 0, maxA, 0, 255); //scale to 8bit
      analogWrite(apin, y);

      if (z > 0) { //pos or neg? turn off other LED
        apin = zPos;
        digitalWrite(zNeg, LOW);
      } else {
        apin = zNeg;
        digitalWrite(zPos, LOW);        
      }
      z = abs(z);
      maxA = max(maxA, z); //update max accel value
      
      z = map(z, 0, maxA, 0, 255); //scale to 8bit
      analogWrite(apin, z);
    }
  }
}
