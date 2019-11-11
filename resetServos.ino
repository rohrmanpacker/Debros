#include <Servo.h>

//servo 
Servo myservo1; 
Servo myservo2; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo1.attach(10);  // attaches the servo on pin 10 to the servo object
  myservo1.write(0);
  myservo2.attach(11);  // attaches the servo on pin 11 to the servo object
  myservo2.write(0);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("reset");
}
