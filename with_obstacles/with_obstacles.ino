#include "Enes100.h"
#include "math.h"

#include "HX711.h"
#include <Servo.h>

//servo 
Servo myservo1; 
//Servo myservo2; 
int pos = 134;
int change = 1;

// load cell
const int LOADCELL_DOUT_PIN = 2; 
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

//conductivity
int volt;
String material;

double theta_d;
float destinationX;
float destinationY;
bool atMission = false;
float tolerance = 0.05;
const int InfraredSensorPin = A5;
const int InfraredSensorPin2 = A4;

//Motor set-up
int enA = 6;
int in1 = 7;
int in2 = 8;
int enB = 5;
int in3 = 13;
int in4 = 4;

const float pi = 3.14;

int go = 255;
int turnvoltage = 240;

void setup() {
  delay(3000);
  Enes100.begin("Debros", DEBRIS, 9, 12, 10);
  delay(1000);

  destinationX = 3;
  destinationY = 3;

  Enes100.print("Destination: ");
  Enes100.print(destinationX);
  Enes100.print(" ");
  Enes100.println(destinationY);
  
  Enes100.updateLocation();

  Enes100.print("Current Location: ");
  Enes100.print(Enes100.location.x);
  Enes100.print(" ");
  Enes100.print(Enes100.location.y);
  Enes100.print(" ");
  Enes100.println(Enes100.location.theta);
  
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  myservo1.attach(11);  // attaches the servo on pin 10 to the servo object
  myservo1.write(pos);
  //myservo2.attach(11);  // attaches the servo on pin 11 to the servo object
  //myservo2.write(pos);

  //load cell
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();          // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
  
}

void loop() {
  up();
  
  if (atMission == false) {
    //Begin by turning to the front
    turntoAngle(0, tolerance);
    Enes100.println("Turned to 0");
    //While not past obstacles
    while (Enes100.location.x < 3) {
      Enes100.updateLocation();
      
      //Unless obstacle detected in front, go forward
      while ((digitalRead(InfraredSensorPin) && (digitalRead(InfraredSensorPin2))) && (Enes100.location.x < 3)) {
        Enes100.println("NO OBSTACLE DETECTED");
        Enes100.updateLocation();
        goforward();
      }
      //If obstacle detected
      if ((!digitalRead(InfraredSensorPin) || (!digitalRead(InfraredSensorPin2)))) {
        Enes100.println("OBSTACLE DETECTED");
        
        //If in top third, turn right, go straight, and then turn back forward
        if (Enes100.location.y > 1.33) {
          turntoAngle(-pi/2, tolerance);
          while (Enes100.location.y > 1.3) {
            Enes100.updateLocation();
            goforward();
          }
          Enes100.updateLocation();
          turntoAngle(0, tolerance);
        }
        
        //If in bottom third, turn left, go straight, then turn back forward
        else if (Enes100.location.y < 0.66) {
          turntoAngle(pi/2, tolerance);
          while (Enes100.location.y < 1) {
            Enes100.updateLocation();
            goforward();
          }
          Enes100.updateLocation();
          turntoAngle(0, tolerance);
        }
        
        //If in middle, turn left, go straight, then turn back forward
        else {
          turntoAngle(pi/2, tolerance);
          while (Enes100.location.y < 1.4) {      
            Enes100.updateLocation();
            goforward();
          }
          Enes100.updateLocation();
          turntoAngle(0, tolerance);
        }
        }
      }
      Enes100.println("PAST OBSTACLES");
      //Once past obstacles, head directly to mission
      getToMission();
      atMission = true;
    }
    analogWrite(enA, 0);
    analogWrite(enB, 0);

    measure();
}

float mod2pi(float dividend){
  while(dividend >= 2*pi){
    dividend -= 2*pi;
  }
  return dividend;
}

//Method to turn to an angle
void turntoAngle(float theta_desired, float tol) {
  Enes100.updateLocation();
  
  if(mod2pi(2*pi+Enes100.location.theta-theta_desired)<mod2pi(2*pi-Enes100.location.theta+theta_desired)){
    while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) {//turn CCW
      turnclockwise();
      Enes100.print(theta_desired);
      Enes100.print(" ");
      Enes100.print(Enes100.location.theta);
      Enes100.updateLocation();
    }
  }else{
    while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) { //turn CW
      turncounterclockwise();
      Enes100.print(theta_desired);
      Enes100.print(" ");
      Enes100.println(Enes100.location.theta);
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
  while (calculateDistance() > 0.13) {
    goforward();
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

void goforward() {
  analogWrite(enA, turnvoltage);
  analogWrite(enB, turnvoltage);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void gobackward() {
  analogWrite(enA, go);
  analogWrite(enB, go);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turncounterclockwise() {
  analogWrite(enA, turnvoltage);
  analogWrite(enB, turnvoltage);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}  

void turnclockwise() {
  analogWrite(enA, turnvoltage);
  analogWrite(enB, turnvoltage);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void up(){
  myservo1.write(10);
  //myservo2.write(10);
}

void down(){
  myservo1.write(134);
  //myservo2.write(134);
}

void pickup(){
    long reading = scale.get_units();
  float weight = (2.19 * .001 * reading);  
  
  //if debris isn't collected (collecting mechanism)
  if(pos >= 15 || weight < 20){ 
    //servo
    myservo1.write(pos);
    //myservo2.write(pos);
    
    if(pos <= 10){
      change *=-1;
      delay(1000);
    }
    
    if(pos >= 134){
      change *=-1;
      delay(1000);
    }
    pos+=change;
  }
  
}

void measure(){
  //load cell
  long reading = scale.get_units();
  float weight = (2.19 * .001 * reading);  
  
  //conductivity
  volt = analogRead(1);
  material = "none";
  
  //if debris is collected (measuring stuff)
  if(weight >20){ 
    if(volt<512){
      Enes100.mission(COPPER);
      Enes100.println("COPPER");
    }else {
      //Enes100.mission(PLASTIC);
      Enes100.println("PLASTIC");
    }
  } 

  
  //delay
  delay(5);
  
  Enes100.mission(weight);

}
