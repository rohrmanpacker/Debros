const int InfraredSensorPin = A5 ;//Connect the signal pin to the digital pin
const int InfraredSensorPin2 = A4 ;//Connect the signal pin to the digital pin

void setup() {
  Serial.begin(9600);
  Serial.println("Start!");
  pinMode(InfraredSensorPin, INPUT);
  pinMode(InfraredSensorPin2, INPUT);
}
void loop() {
  Serial.print("Infrared Switch Status 1: ");
  Serial.println(digitalRead(InfraredSensorPin));

  Serial.print("Infrared Switch Status 2: ");
  Serial.println(digitalRead(InfraredSensorPin2));
  delay(100);;
}
