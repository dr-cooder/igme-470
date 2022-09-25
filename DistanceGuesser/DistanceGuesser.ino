// won't change
bool debug = true;
const int trigPin = 3;
const int echoPin = 2;
const int closerLedPin = 6;
const int fartherLedPin = 14;
const int buttonPin = 9;
// will change
bool lastButtonState = LOW;
bool currentButtonState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(closerLedPin, OUTPUT);
  pinMode(fartherLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    getSonar();
    blinkBlonk();
  }
  lastButtonState = currentButtonState;
}

unsigned long getSonar() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long duration = pulseIn(echoPin, HIGH);
  unsigned long distance = duration * 0.0343 / 2;
  if (debug) {
    Serial.print(distance);
    Serial.println(" cm");
  }
}

void blinkBlonk() {
  digitalWrite(closerLedPin, HIGH);
  delay(250);
  digitalWrite(closerLedPin, LOW);
  delay(250);
  digitalWrite(closerLedPin, HIGH);
  delay(250);
  digitalWrite(closerLedPin, LOW);
  delay(250);
  digitalWrite(closerLedPin, HIGH);
  delay(250);
  digitalWrite(closerLedPin, LOW);
}
