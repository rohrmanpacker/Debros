#include "HX711.h"
#include <Servo.h>

//servo 
Servo myservo1; 
Servo myservo2; 
int pos = 134;
int change = 1;

// load cell
const int LOADCELL_DOUT_PIN = 2; 
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

//conductivity
int volt;
String material;

void setup() {
  Serial.begin(9600);

  //setting up sensors
  //servo
  myservo1.attach(10);  // attaches the servo on pin 10 to the servo object
  myservo1.write(pos);
  myservo2.attach(11);  // attaches the servo on pin 11 to the servo object
  myservo2.write(pos);

  //load cell
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();          // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  
}


void loop() {
  //MOVING STUFF
  
  //put ur shit here jenny
  
  // MEASURING STUFF 
  
  //load cell
  long reading = scale.get_units();
  float weight = (2.19 * .001 * reading);  
  
  //conductivity
  volt = analogRead(1);
  material = "none";
  
  //if debris is collected (measuring stuff)
  if(weight >20){ 
    if(volt<512){
      material = "metal";
    }else {
      material = "plastic";
    }
  } 

  //if debris isn't collected (collecting mechanism)
  if(pos >= 15 || weight < 20){ 
    //servo
    myservo1.write(pos);
    myservo2.write(pos);
    
    if(pos <= 10){
      change *=-1;
      delay(1000);
    }
    
    if(pos >= 134){
      change *=-1;
      delay(1000);
    }
    
    pos+=change;
    
  }else{ //when debris is collected
    
  }
  
  //delay
  delay(5);
  
  //print shit
  Serial.print("Servo Pos:\t");Serial.print(pos);
  //Serial.print(" Reading:\t");Serial.print(reading);
  Serial.print(" Weight:\t");Serial.print(weight);
  //Serial.print(" Conductivity:\t");Serial.print(volt);
  Serial.print(" Material:\t");Serial.print(material);
  Serial.println();
  
  
}
