#include "HX711.h"
#include <Servo.h>

//servo 
Servo myservo1; 
Servo myservo2; 


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2; 
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

int volt;
String material;

int pos = 135;
int change = -2;

void setup() {
  Serial.begin(9600);
  myservo1.attach(10);  // attaches the servo on pin 10 to the servo object
  myservo1.write(pos);
  myservo2.attach(11);  // attaches the servo on pin 11 to the servo object
  myservo2.write(pos);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

}



void loop() {
  // put your main code here, to run repeatedly:

  //load cell
  long reading = scale.get_units();
  float weight = (2.19 * .001 * reading);  
  
  //conductivity
  volt = analogRead(1);
  material = "none";
  if(weight >20){ //if debris is collected
    if(volt<512){
      material = "metal";
    }else {
      material = "plastic";
    }
  }else{ //if debris isn't collected
    //servo
    myservo1.write(pos);
    myservo2.write(pos);
  
    pos+=change;
    if(pos <= 0){change *=-1;}
    if(pos >= 135){change *=-1;}
  
  }
  
  //delay
  delay(5);
  
  //print shit
  Serial.print("Position:\t");Serial.print(pos);
  Serial.print(" Reading:\t");Serial.print(reading);
  Serial.print(" Weight:\t");Serial.print(weight);
  Serial.print(" Conductivity:\t");Serial.print(volt);
  Serial.print(" Material:\t");Serial.print(material);
  Serial.println();
  
  
}
