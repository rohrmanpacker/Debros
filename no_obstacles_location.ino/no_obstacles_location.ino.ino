



#include "Enes100.h"
const int InfaredSensorPin = 12; 
const int InfaredSensorPin2 = 13;

//Motor set-up
int enA = 9;
int in1 = 7;
int in2 = 8;
int enB = 10;
int in3 = 11;
int in4 = 12;

double theta_d;
float destinationX;
float destinationY;
float tolerance = 0.05;
float pi = 3.14;
bool atMission = false;


void setup() {
  // put your setup code here, to run once:
  Enes100.begin("Debros", DEBRIS , 19, 4, 5);
delay(1000);

    Enes100.print("Destination is at (");
    Enes100.print(Enes100.destination.x);
    Enes100.print(", ");
    Enes100.print(Enes100.destination.y);
    Enes100.println(")");

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
    // Update the OSV's current location
    if (Enes100.updateLocation()) {
        Enes100.print("OSV is at (");
        Enes100.print(Enes100.location.x);
        Enes100.print(", ");
        Enes100.print(Enes100.location.y);
        Enes100.print(", ");
        Enes100.print(Enes100.location.theta);
        Enes100.println(")");
    } else {
        // OSV's location was not found
        Enes100.println("404 Not Found");
    }

    getToMission2();
}

void getToMission2() {
  Enes100.updateLocation();
  theta_d = atan((destinationY - Enes100.location.y)/(destinationX - Enes100.location.x));
  turntoAngle(theta_d, tolerance);
  while (calculateDistance() > 0.23) {
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    Enes100.updateLocation();
  }



  if (atMission == false) {
    getToMission(); 
    atMission = true;
  }
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  Enes100.println("Debros R At Mission Site :) ");
}


//Method to turn to an angle

void turntoAngle(float theta_desired, float tol) {
  Enes100.updateLocation();
  
  if(mod2pi(2*pi+Enes100.location.theta-theta_desired)<mod2pi(2*pi-Enes100.location.theta+theta_desired)){
    while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) {//turn CCW
      analogWrite(enA, 255);
      analogWrite(enB, 255);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      Enes100.updateLocation();
    }
  }else{
    while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) { //turn CW
      analogWrite(enA, 255);
      analogWrite(enB, 255);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      Enes100.updateLocation();
    }
  }
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}


//Method to get to the mission site after the obstacles
void getToMission() {
  Enes100.updateLocation();
  theta_d = atan((destinationY - Enes100.location.y)/(destinationX - Enes100.location.x));
  turntoAngle(theta_d, tolerance);
  while (calculateDistance() > 0.23) {
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    Enes100.updateLocation();
  }
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

//Method to calculate distance between OSV and the mission site
double calculateDistance() {
  Enes100.updateLocation();
  double sqx = (destinationX - Enes100.location.x)*(destinationX - Enes100.location.x);
  double sqy = (destinationY - Enes100.location.y)*(destinationY - Enes100.location.y);
  return sqrt(sqx+sqy);
}


float mod2pi(float dividend){
  while(dividend >= 2*pi){
    dividend -= 2*pi;
  }
  return dividend;
}
