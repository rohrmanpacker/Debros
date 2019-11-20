#include "Enes100.h"

//Motor set-up
int enA = 9;
int in1 = 7;
int in2 = 8;
int enB = 10;
int in3 = 11;
int in4 = 12;

double theta_d;
float tolerance = 0.05;
float pi = 3.14;

int go = 255;
int turnvoltage = 240;

void setup() {
  // put your setup code here, to run once:
  Enes100.begin("Debros", DEBRIS , 19, 4, 5);
  delay(1000);

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
  // put your main code here, to run repeatedly:
  turntoAngle(pi, tolerance);
}

void turntoAngle(float theta_desired, float tol) {
  Enes100.updateLocation();
  
  if(mod2pi(2*pi+Enes100.location.theta-theta_desired)<mod2pi(2*pi-Enes100.location.theta+theta_desired)){
    while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) {//turn CCW
      turnclockwise();
      Enes100.updateLocation();
      Enes100.println(Enes100.location.theta);
    }
  }else{
    while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) { //turn CW
      turncounterclockwise();
      Enes100.updateLocation();
      Enes100.println(Enes100.location.theta);
    }
  }
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

float mod2pi(float dividend){
  while(dividend >= 2*pi){
    dividend -= 2*pi;
  }
  return dividend;
}

void turncounterclockwise() {
  analogWrite(enA, turnvoltage);
  analogWrite(enB, turnvoltage);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnclockwise() {
  analogWrite(enA, turnvoltage);
  analogWrite(enB, turnvoltage);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
