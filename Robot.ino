#include <util/delay.h>
#include "ultrasonic.h"
#include "motors.h"

const double originalSpeed = 0.25;
const double distanceThreshold = 8;
double percentSpeed = originalSpeed;
double percentSpeed_left = originalSpeed;

int idealDirection = CENTER;
unsigned long t = 0;
bool firstWall = false;
bool secondWall = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Setup");
  setupUltrasonic();
  setupMotors();
  setupServo();
  setupLed();
  Serial.println("Finished Setup");
  // proximityRead(RIGHT);  // Initial read to adjust sensor state if needed
  t = millis();
}

void old_maneuver_right() {
  //forward
  setMotorSpeed(RIGHT, percentSpeed , FORWARD);
  setMotorSpeed(LEFT, percentSpeed , FORWARD);
  delay(300);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // turn Right
  setMotorSpeed(LEFT, percentSpeed , FORWARD);
  setMotorSpeed(RIGHT, percentSpeed , BACKWARD);
  delay(300);
  setMotorSpeed(LEFT, 0, FORWARD);
  setMotorSpeed(RIGHT, 0, FORWARD);
  delay(500);
  //forward
  setMotorSpeed(RIGHT, percentSpeed , FORWARD);
  setMotorSpeed(LEFT, percentSpeed , FORWARD);
  delay(800);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
}

void maneuver_small_distance(){
  setMotorSpeed(LEFT, percentSpeed, BACKWARD);  // rotate to left when theres a wall corrected
  setMotorSpeed(RIGHT, percentSpeed, BACKWARD);
  delay(450);  // angle turn corrected
  setMotorSpeed(LEFT, 0, FORWARD);
  setMotorSpeed(RIGHT, 0, FORWARD);
  delay(50);
  setMotorSpeed(LEFT, percentSpeed, FORWARD);
  setMotorSpeed(RIGHT, percentSpeed, BACKWARD);
  delay(500);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(200);

  setMotorSpeed(LEFT, percentSpeed, BACKWARD);  // rotate to left when theres a wall corrected
  setMotorSpeed(RIGHT, percentSpeed, BACKWARD);
  delay(450);  // angle turn corrected
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(200);
  setMotorSpeed(LEFT, percentSpeed + 0.4, BACKWARD);  // rotate to left when theres a wall corrected
  setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
  delay(300);  // angle turn corrected
  setMotorSpeed(LEFT, 0, FORWARD);
  setMotorSpeed(RIGHT, 0, FORWARD);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(200);
}

void maneuver_left() {
  setMotorSpeed(LEFT, 0, FORWARD);
  setMotorSpeed(RIGHT, 0, FORWARD);
  delay(50);
  setMotorSpeed(LEFT, percentSpeed_left + 0.4, BACKWARD);  // rotate to left when theres a wall corrected
  setMotorSpeed(RIGHT, percentSpeed_left + 0.4, FORWARD);
  delay(300);  // angle turn corrected
  setMotorSpeed(LEFT, 0, FORWARD);
  setMotorSpeed(RIGHT, 0, FORWARD);
}

void loop() {
  double d = proximityRead(RIGHT);
  if (millis() - t < 1000) {  // the servo looks to right for 1 second
    followWall(percentSpeed, distanceThreshold, RIGHT);
    if (d > 20) {  //change started here
      old_maneuver_right();
      // Serial.println(d);
      //old_maneuver_right();
    }  //change ended here
    if (d < 5){
      maneuver_small_distance();
    }
    
  } else {
    // SLLA();
    setMotorSpeed(LEFT, 0, FORWARD);
    setMotorSpeed(RIGHT, 0, FORWARD);
    if (proximityRead(CENTER) < distanceThreshold) {  // executes this if there is a wall within the distance threshhold
      percentSpeed_left = originalSpeed;
      maneuver_left();
      proximityRead(RIGHT);
    }
    moveServo(RIGHT);  //this is executed if there are no walls
    t = millis();
  }
  Serial.println(d);
}
