/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-light-sensor
 * 
 * Smoothng: https://docs.arduino.cc/built-in-examples/analog/Smoothing
 */

const int inputPin = A2;
const int ledPin = 2;

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

const int brightnessThreshold = 20;

void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, HIGH);
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  Serial.begin(9600);
}

void loop() {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  Serial.print("Analog reading: ");
  if (average < 10) {
    Serial.print(" ");
  }
  Serial.print(average);

  // We'll have a few threshholds, qualitatively determined
  if (average < brightnessThreshold) {
    Serial.println(" - Night");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println(" - Day");
    digitalWrite(ledPin, LOW);
  }

  delay(10);
}
