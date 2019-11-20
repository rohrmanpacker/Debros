#include "Enes100.h"
const int InfaredSensorPin = 12; 
const int InfaredSensorPin2 = 13;

float destinationX;
float destinationY;
bool atMission = false;

void setup() {
  // put your setup code here, to run once:
    Enes100.begin("Debros", DEBRIS, 18, 4, 5);
    delay(1000);
    Enes100.print("Destination is at (");
    Enes100.print(Enes100.destination.x);
    Enes100.print(", ");
    Enes100.print(Enes100.destination.y);
    Enes100.println(")");
    destinationX = Enes100.destination.x;
    destinationY = Enes100.destination.y;

}

void loop() {
  // put your main code here, to run repeatedly:
  Enes100.updateLocation();
   /*Enes100.print("Destination is at (");
    Enes100.print(Enes100.destination.x);
    Enes100.print(", ");
    Enes100.print(Enes100.destination.y);
    Enes100.println(")");*/
  
  if (atMission == false) {
     if (calculateDistance() < 0.23) {
      Enes100.println("GOT TO MISSION");
      Enes100.print("OSV is at (");
      Enes100.print(Enes100.location.x);
      Enes100.print(", ");
      Enes100.print(Enes100.location.y);
      Enes100.print(", ");
      Enes100.print(Enes100.location.theta);
      Enes100.println(")");
      atMission = true;
     }
     else if (Enes100.updateLocation()) {
        Enes100.print("OSV is at (");
        Enes100.print(Enes100.location.x);
        Enes100.print(", ");
        Enes100.print(Enes100.location.y);
        Enes100.print(", ");
        Enes100.print(Enes100.location.theta);
        Enes100.print(", ");
        Enes100.print(calculateDistance());
        Enes100.print(", ");
        Enes100.print(destinationX);
        Enes100.print(", ");
        Enes100.print(destinationY);
        Enes100.println(")");
        
    } 
    else {
        // OSV's location was not found
        Enes100.println("404 Not Found");
    } //else statement
    
  }  //if not at mission
}  //loop

double calculateDistance() {
  Enes100.updateLocation();
  double sqx = (destinationX - Enes100.location.x)*(destinationX - Enes100.location.x);
  double sqy = (destinationY - Enes100.location.y)*(destinationY - Enes100.location.y);
  return sqrt(sqx+sqy);
}
