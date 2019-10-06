#include "Enes100.h"
#include "Tank.h"
#include "math.h"

double theta_d;
float destinationX;
float destinationY;
bool atMission = false;
float tolerance = 0.05;

const float pi = 3.14;

void setup() {
	Enes100.begin("Team Name Here", BLACK_BOX, 3, 8, 9);
	Tank.begin();
	destinationX = Enes100.destination.x;
	destinationY = Enes100.destination.y;
	Enes100.updateLocation();
	Tank.setLeftMotorPWM(0);
	Tank.setRightMotorPWM(0);
}

void loop() {
	if (atMission == false) {
		//Begin by turning to the front
		turntoAngle(0, tolerance);

		//While not past obstacles
		while (Enes100.location.x < 3) {
			Enes100.updateLocation();
			
			//Unless obstacle detected in front, go forward
			while ((Tank.readDistanceSensor(1) >= 0.12) && (Enes100.location.x < 3)) {
				Enes100.updateLocation();
				Tank.setLeftMotorPWM(225);
				Tank.setRightMotorPWM(225);
			}
			//If obstacle detected
			if (Tank.readDistanceSensor(1) < 0.12) {
				
				//If in top third, turn right, go straight, and then turn back forward
				if (Enes100.location.y > 1.33) {
					turntoAngle(-pi/2, tolerance);
					while (Enes100.location.y > 1.3) {
						Enes100.updateLocation();
						Tank.setLeftMotorPWM(225);
						Tank.setRightMotorPWM(225);
					}
					Enes100.updateLocation();
					turntoAngle(0, tolerance);
				}
				
				//If in bottom third, turn left, go straight, then turn back forward
				else if (Enes100.location.y < 0.66) {
					turntoAngle(pi/2, tolerance);
					while (Enes100.location.y < 1) {
						Enes100.updateLocation();
						Tank.setLeftMotorPWM(225);
						Tank.setRightMotorPWM(225);
					}
					Enes100.updateLocation();
					turntoAngle(0, tolerance);
				}
				
				//If in middle, turn left, go straight, then turn back forward
				else {
					turntoAngle(pi/2, tolerance);
					while (Enes100.location.y < 1.4) {			
						Enes100.updateLocation();
						Tank.setLeftMotorPWM(225);
						Tank.setRightMotorPWM(225);
					}
					Enes100.updateLocation();
					turntoAngle(0, tolerance);
				}
			  }
			}
			
			//Once past obstacles, head directly to mission
			getToMission();
			atMission = true;
		}
		Tank.setLeftMotorPWM(0);
		Tank.setRightMotorPWM(0);
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
			Tank.setLeftMotorPWM(255);
			Tank.setRightMotorPWM(-255);
			Enes100.updateLocation();
		}
	}else{
		while ((Enes100.location.theta <= (theta_desired - tol)) || (Enes100.location.theta >= (theta_desired + tol))) { //turn CW
			Tank.setLeftMotorPWM(-255);
			Tank.setRightMotorPWM(255);
			Enes100.updateLocation();
		}
	}
	Tank.setLeftMotorPWM(0);
	Tank.setRightMotorPWM(0);
}

//Method to get to the mission site after the obstacles
void getToMission() {
	Enes100.updateLocation();
	theta_d = atan((destinationY - Enes100.location.y)/(destinationX - Enes100.location.x));
	turntoAngle(theta_d, tolerance);
	while (calculateDistance() > 0.23) {
		Tank.setLeftMotorPWM(255);
		Tank.setRightMotorPWM(255);
		Enes100.updateLocation();
	}
	Tank.setLeftMotorPWM(0);
	Tank.setRightMotorPWM(0);
}

//Method to calculate distance between OSV and the mission site
double calculateDistance() {
	Enes100.updateLocation();
	double sqx = (destinationX - Enes100.location.x)*(destinationX - Enes100.location.x);
	double sqy = (destinationY - Enes100.location.y)*(destinationY - Enes100.location.y);
	return sqrt(sqx+sqy);
}
