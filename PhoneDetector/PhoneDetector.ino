const unsigned int ledPin = 3;
const unsigned int threshold = 250;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int reading = analogRead(A1);
  unsigned int ledval = max(0, reading - threshold);
  analogWrite(ledPin, ledval);
  if (reading > threshold) {
    Serial.println(reading);
  }
  delay(5);
}
