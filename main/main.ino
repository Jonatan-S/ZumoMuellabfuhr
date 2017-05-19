/*
   Müllabfuhr
*/

//Import
#include <Wire.h>
#include <Zumo32U4Encoders.h>
#include <Zumo32U4Motors.h>
#include <Zumo32U4LCD.h>
#include <Zumo32U4.h>

//Objekte
LSM303 compass;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
Zumo32U4LCD lcd;
Zumo32U4ButtonA a;
Zumo32U4ButtonB b;
Zumo32U4ButtonC c;

//Konstanten
//const int SLOW = 1;
//const int FAST = 2;
const int FORWARD = 1;
const int BACKWARD = -1;
int RIGHT = 1;
int LEFT = -1;
const int baseSpeed = 180;
const int turnDelay = 500;

//Variablen
int temp;
int force;
boolean invert;
int16_t countsLeft;
int16_t countsRight;

void setup() {
  //Initialisierung des Beschleunigungssensors
  Wire.begin();
  compass.init();
  compass.enableDefault();

  motors.flipLeftMotor(true);

  //Bildschirmausgabe, warten auf Startsignal
  lcd.print(readBatteryMillivolts());
  a.waitForButton();

  //Hauptprogramm
  lcd.clear();
  lcd.print("Fahren...");
  drive(FORWARD, 600);
  turn(LEFT);
}

void loop() {
  drive(FORWARD, 3000);
  drive(BACKWARD, 200);
  turn(RIGHT);
  invert = drive(FORWARD, 600);
  if (invert) drive(BACKWARD, 400);
  else drive(BACKWARD, 600);
  turn(LEFT);
  if (invert) {
    LEFT = -LEFT;
    RIGHT = -RIGHT;
    invert = false;
  }
  drive(FORWARD, 150);
  drive(BACKWARD, 3000);
  drive(FORWARD, 400);
  turn(RIGHT);
  drive(FORWARD, 400);
  turn(LEFT);
}

/*
   Lässt den Roboter fahren

   @params direction  Richtung
   @params sMod       Geschwindigkeitsmodifikator
   @params duration   maximale Fahrzeit
*/
boolean drive(int direction, int duration) {
  int endTime = millis() + duration;                   //Berechnet Zeitpunkt an dem die Methode spätestens beendet wird
  int lSpeed = direction * baseSpeed;  //Berechnet Geschwindigkeit der linken Seite
  int rSpeed = direction * baseSpeed;  //Berechnet Geschwindigkeit der rechten Seite
  resetEncoders();
  motors.setSpeeds(lSpeed, rSpeed);

  if (duration > 300) {   //Wenn Fahrt über 900 ms -> Wanderkennung
    delay(200);           //Warten bis konstante Geschwindigkeit erreicht wurde
    do {
      refreshCounts();
      if (abs(countsLeft) > abs(countsRight)) {
        temp = (countsLeft - countsRight) / 100;
        lSpeed -= direction * temp;
        rSpeed += direction * temp;
      } else if (abs(countsRight) > abs(countsLeft)) {
        temp = (countsRight - countsLeft) / 100;
        rSpeed -= direction * temp;
        lSpeed += direction * temp;
      }
      lSpeed = constrain(lSpeed, 190, 210);
      rSpeed = constrain(rSpeed, 190, 210);

      motors.setSpeeds(lSpeed, rSpeed);


      compass.readAcc();  //Kompass auslesen
      //Werte aller Achsen addieren und 1 G subtrahieren
      force = (abs(compass.a.x) + abs(compass.a.y) + abs(compass.a.z)) - 16500;
      delay(10);
      lcd.gotoXY(0, 0);
      lcd.print(force);
    } while (force < 15000 && millis() < endTime); //Abbruch wenn Kraftänderung über 15000 (fast 1 G) oder wenn Endzeit erreicht
    motors.setSpeeds(0, 0);
    return force > 15000;
  } else {
    delay(duration);
    motors.setSpeeds(0, 0);
  }
}

/*
   Dreht den Roboter um 90 Grad in die übergebene Richtung

   @params direction  Richtung
*/
void turn(int direction) {
  resetEncoders();
  motors.setSpeeds(direction * baseSpeed, -direction * baseSpeed);
  do {
    refreshCounts();
    lcd.gotoXY(0, 0);
    lcd.print(countsLeft);
    lcd.gotoXY(0, 1);
    lcd.print(countsRight);
  } while (abs(countsLeft) + abs(countsRight) < 500);
  motors.setSpeeds(0, 0);
}

void resetEncoders() {
  countsLeft = encoders.getCountsAndResetLeft();
  countsRight = encoders.getCountsAndResetRight();
}

void refreshCounts() {
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();
}

