#include <Enes100.h>

int enA = 6;
int in1 = 7;
int in2 = 8;
int enB = 11;
int in3 = 10;
int in4 = 9;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}

void loop() {
  delay(100);
  analogWrite(enA, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enB, 255);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(3000); 
  
  analogWrite(enA, 0);
  digitalWrite(enB, 0);
  delay(1000);
  
  analogWrite(enA, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  analogWrite(enB, 255);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(3000);

  analogWrite(enA, 0);
  digitalWrite(enB, 0);
  delay(1000);
}
