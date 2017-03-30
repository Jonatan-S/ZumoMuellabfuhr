/*
 * Müllabfuhr
 */

//#include <Zumo32U4Encoders.h>
#include <Zumo32U4Motors.h>
#include <Zumo32U4LCD.h>
#include <Zumo32U4.h>
//#include <Zumo32U4LineSensors.h>

Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4ButtonA a;
Zumo32U4ButtonB b;
Zumo32U4ButtonC c;

int dSpeed = 200; // Driving Speed
int tSpeed = 100; // Turning Speed

void setup() {
  // put your setup code here, to run once:
  a.waitForButton();
  lcd.print("Fahren...");
  forward(2000);
  back(3000);
  forward(400,0);
  turnLeft();
  forward(400,0);
  turnRight();

  forward(3000);
  back(3200);
  forward(400,0);
  turnLeft();
  forward(600,0);
  turnRight();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void forward(int t) {
  motors.setSpeeds(dSpeed,dSpeed-5);
  delay(t);
}

void forward(int t, int p) {
  motors.setSpeeds(tSpeed,tSpeed);
  delay(t);
}

void back(int t) {
  motors.setSpeeds(-dSpeed,-dSpeed);
  delay(t);
}

void turnRight() {
  motors.setSpeeds(100,-100);
  delay(600);
  motors.setSpeeds(0,0);
}

void turnLeft() {
  motors.setSpeeds(-100,100);
  delay(600);
  motors.setSpeeds(0,0);
}


