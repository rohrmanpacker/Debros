//Motor set-up
int enA = 6;
int in1 = 7;
int in2 = 8;
int enB = 11;
int in3 = 10;
int in4 = 9;

int go = 200;
int turnvoltage = 240;


void setup() {
  // put your setup code here, to run once:
  
  //Motor setup
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void loop() {
  // put your main code here, to run repeatedly

  goforward(5000);
  gobackward(5000);
  turn(5000);
  
  
}

void goforward(int t) {
  analogWrite(enA, go);
  analogWrite(enB, go);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(t);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void turn(int t) {
  analogWrite(enA, turnvoltage);
  analogWrite(enB, turnvoltage);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(t);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void gobackward(int t) {
  analogWrite(enA, go);
  analogWrite(enB, go);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(t);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}  
