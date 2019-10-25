#include "HX711.h"
#include <Servo.h>

//servo 
Servo myservo; 

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

int volt;
String material;

void setup() {
  Serial.begin(9600);
  myservo.attach(5);  // attaches the servo on pin 5 to the servo object
  myservo.write(00);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

}

int pos = 0;
int change = 1;

void loop() {
  // put your main code here, to run repeatedly:

  //servo
  myservo.write(pos);
  pos+=change;
  if(pos <=   0){change =  1;}
  if(pos >= 180){change = -1;}
  

  //load cell
  long reading = scale.get_units();
  float weight = (2.19 * .001 * reading);

  //conductivity
  volt = analogRead(1);
  if(volt<512){
    material = "metal";
  }else {
    material = "plastic";
  }

  //delay
  delay(5);
  
  //print shit
  //Serial.print(" Position:\t");Serial.print(pos);
  Serial.print("reading:\t");Serial.print(reading);Serial.print(" weight:\t");Serial.print(weight);Serial.print(" Conductivity:\t");Serial.print(volt);Serial.print(" Material:\t");Serial.print(material);
  Serial.println();
  
  
}
