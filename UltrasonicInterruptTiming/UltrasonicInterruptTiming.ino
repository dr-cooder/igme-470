/*
  WM Harris 2022 - RIT * IGME-470.5-2 Timing experiment
  
  Switch (pin 9) controlling Ultrasonic input (Trig:3, Echo:2)
  Switch on: update LED (pin 11) blink duration based on sonar
  Switch off: blink duration based on last reading  
*/

boolean debug = true;

//switch to trigger sonar reading
const int switchPin = 9;

//sonar pin numbers
const int trigPin = 3; //starts sonar
const int echoPin = 2; //returns echo time

//flag whether to read sonar
volatile boolean sonarOn = false;

//blink LED state
byte ledState = LOW;

//LED output
const int ledPin = 11;
const int debugLed = 13; //used if debugging

//LED blink timing
unsigned long prevBlink = 0; //time of last blink
unsigned long blinkTime = 1000; //1sec default period

//Sonar echo value last time
unsigned long oldDist = 0;

void setup() {
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(debugLed, OUTPUT);

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  //breathe
  delay(200);
  //if switch changes value, start/stop sonar readings
  attachInterrupt(digitalPinToInterrupt(switchPin), updateSonarOn, CHANGE);
}

void loop() {
  if (sonarOn) { //Read sonar!
    unsigned long duration = getSonar();
    if (oldDist != duration) { //update blinking if changed
      oldDist = duration;
      duration = duration / 100;
      blinkTime = min(duration, 1000);

      if (debug) {
        Serial.print(duration);
        Serial.print(" ");
        Serial.println(blinkTime);
      }
    }
  }

  //Flip blink LED if blinkTime has elapsed since prevBlink
  unsigned long currentMillis = millis(); // get and save current time
  // Is it time to blink?
  if (currentMillis - prevBlink >= blinkTime) {
    prevBlink = currentMillis; // store this blink time
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  
  //breathe
  delay(10);
}

/** updateSonarOn: called only if switch state changes
 * via interrupt. Update sonarOn based on switch value
*/
void updateSonarOn() {
  sonarOn = digitalRead(switchPin) == HIGH;
}

/** getSonar: read sonar sensor and return echo duration
 * @return {unsigned long} duration of sonar pulse
 */
unsigned long getSonar() {
  if (debug) {
    digitalWrite(debugLed, HIGH);
  }
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //Read sonar value without getting interrupted
  noInterrupts();
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  unsigned long duration = pulseIn(echoPin, HIGH);
  
  interrupts(); //okay to interrupt now

  // Calculating the distance in cm
  if (debug) {
    digitalWrite(debugLed, LOW);
  }
  return duration;
}
