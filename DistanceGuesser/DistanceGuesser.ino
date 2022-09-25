// Felix Warner
// 9/25/22
// Distance Guessing Game

// won't change
const bool debug = true;
const unsigned int sonarReadingCount = 80;
const unsigned int pulseTimeout = 3500; // don't run the sensor for any longer than would be necessary to reach the highest possible distance answer
const unsigned int minDistance = 20;
const unsigned int maxDistance = 50;
const int trigPin = 3;
const int echoPin = 2;
const int closerLedPin = 6;
const int fartherLedPin = 14;
const int buttonPin = 9;
// will change
unsigned long answer = 0; // this will be updated at initialization
bool lastButtonState = LOW;
bool currentButtonState = LOW;

void setup() {
  answer = random(minDistance, maxDistance);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(closerLedPin, OUTPUT);
  pinMode(fartherLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  if (debug) {
    Serial.print("Answer: "); // TO-DO: this does not seem to print at initialization
    Serial.println(answer);
  }
}

void loop() {
  currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == HIGH && lastButtonState == LOW) { // moment button is pressed
    unsigned long guess = getSonar();
    if (guess > answer + 2) { // too far
      blinkBlonk(true, false);
    } else if (guess < answer - 2) { // too close
      blinkBlonk(false, true);
    } else { // within range
      if (debug) {
        Serial.println("DING DING DING!!!");
      }
      blinkBlonk(true, true);
      answer = random(minDistance, maxDistance);
      if (debug) {
        Serial.print("New answer: ");
        Serial.println(answer);
      }
    }
  }
  lastButtonState = currentButtonState;
}

unsigned long getSonar() {
  unsigned long distanceSum = 0; // start building an average
  Serial.print("Readings: ");
  for (int i = 0; i <= sonarReadingCount; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    unsigned long duration = pulseIn(echoPin, HIGH, pulseTimeout);
    if (duration == 0) duration = pulseTimeout; // if pulseIn reaches the timeout it will return exactly 0; more accurately the output can be interpreted as maximum distance away
    unsigned long distance = duration  * 0.0343 / 2; // convert to centimeters
    if (debug) {
      Serial.print(distance);
      Serial.print(" ");
    }
    distanceSum += distance;
  }
  distanceSum /= sonarReadingCount;
  if (debug) {
    Serial.println();
    Serial.print("Average: ");
    Serial.println(distanceSum);
  }
  return distanceSum;
}

void blinkBlonk(const bool blinkCloser, const bool blinkFarther) {
  digitalWrite(closerLedPin, blinkCloser);
  digitalWrite(fartherLedPin, blinkFarther);
  delay(250);
  digitalWrite(closerLedPin, LOW);
  digitalWrite(fartherLedPin, LOW);
  delay(250);
  digitalWrite(closerLedPin, blinkCloser);
  digitalWrite(fartherLedPin, blinkFarther);
  delay(250);
  digitalWrite(closerLedPin, LOW);
  digitalWrite(fartherLedPin, LOW);
  delay(250);
  digitalWrite(closerLedPin, blinkCloser);
  digitalWrite(fartherLedPin, blinkFarther);
  delay(250);
  digitalWrite(closerLedPin, LOW);
  digitalWrite(fartherLedPin, LOW);
}
