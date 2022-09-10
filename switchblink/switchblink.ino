/*
  Based on "Button" example
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

// won't change
const int colorSwitchPin = 2;
const int blinkButtonPin = 4;
const int greenLedPin =  5;
const int redLedPin =  7;
const int blinkInterval = 125;
// will change
int colorSwitchState = 0;
int blinkButtonState = 0;
int blinkState = true;
int lastBlinkTime = -blinkInterval;

void setup() {
  // initialize the LED pins as an output:
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  // initialize the switch pins as an input:
  pinMode(colorSwitchPin, INPUT);
  pinMode(blinkButtonPin, INPUT);
}

void loop() {
  // read the state of the switch values:
  blinkButtonState = digitalRead(blinkButtonPin);
  colorSwitchState = digitalRead(colorSwitchPin);

  // if blink button is not held, light should be solid
  if (blinkButtonState == LOW) {
    blinkState = true;
    lastBlinkTime = -blinkInterval;
    // this way as soon as blink button is held, blinkState will switch from on to off
  } else {
    // if it's been the appropriate amount of time since the last blink...
    if ( (millis() - lastBlinkTime) >= blinkInterval) {
      // blink
      blinkState = !blinkState;
      // update the last blink time to now
      lastBlinkTime = millis();
    }
  }
  
  // check state of color switch. If it is HIGH, the green light is on (or blinking if applicable), otherwise the red one is:
  if (colorSwitchState == HIGH) {
    // turn green LED on and red LED off:
    digitalWrite(greenLedPin, blinkState);
    digitalWrite(redLedPin, LOW);
  } else {
    // turn green LED off and red LED on:
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, blinkState);
  }
}
