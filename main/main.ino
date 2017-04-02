/*
 * Müllabfuhr
 */

//Import
#include <Wire.h>
#include <Zumo32U4Motors.h>
#include <Zumo32U4LCD.h>
#include <Zumo32U4.h>
//#include <Zumo32U4Encoders.h>
//#include <Zumo32U4LineSensors.h>

//Objekte
LSM303 compass;
Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4ButtonA a;
Zumo32U4ButtonB b;
Zumo32U4ButtonC c;

//Konstanten
const int SLOW = 1;
const int FAST = 2;
const int FORWARD = 1;
const int BACKWARD = -1;
const int baseSpeed = 100;
const int rCorrection = -5;
const int lCorrection = 0;
const int turnDelay = 600;

//Variablen
int force;

void setup() {
  //Initialisierung des Beschleunigungssensors
  Wire.begin();
  compass.init();
  compass.enableDefault();

  //Bildschirmausgabe, warten auf Startsignal
  lcd.print(readBatteryMillivolts());
  a.waitForButton();

  //Hauptprogramm
  lcd.clear();
  lcd.print("Fahren...");
  drive(FORWARD,FAST,2000);
  drive(BACKWARD,FAST,3000);
  drive(FORWARD,SLOW,400);
  turnLeft();
  drive(FORWARD,SLOW,400);
  turnRight();

  drive(FORWARD,FAST,3000);
  drive(BACKWARD,FAST,3200);
  drive(FORWARD,SLOW,400);
  turnLeft();
  drive(FORWARD,SLOW,600);
  turnRight();
}

void loop() {

}

/*
 * Lässt den Roboter fahren
 * 
 * @params direction  Richtung
 * @params sMod       Geschwindigkeitsmodifikator
 * @params duration   maximale Fahrzeit
 */
void drive(int direction, int sMod, int duration) {
  int endTime = millis() + duration;                            //Berechnet Zeitpunkt an dem die Methode spätestens beendet wird
  int lSpeed = direction * ((sMod * baseSpeed) + lCorrection);  //Berechnet Geschwindigkeit der linken Seite
  int rSpeed = direction * ((sMod * baseSpeed) + rCorrection);  //Berechnet Geschwindigkeit der rechten Seite
  motors.setSpeeds(lSpeed, rSpeed);
  
  if (duration > 900) {   //Wenn Fahrt über 900 ms -> Wanderkennung
    delay(500);           //Warten bis konstante Geschwindigkeit erreicht wurde
    do {
      compass.readAcc();  //Kompass auslesen
      //Werte aller Achsen addieren und 1 G subtrahieren
      force = (abs(compass.a.x) + abs(compass.a.y) + abs(compass.a.z)) - 16500;
      delay(10);
      lcd.clear();
      lcd.print(force);
    } while (force < 15000 && millis() < endTime); //Abbruch wenn Kraftänderung über 15000 (fast 1 G) oder wenn Endzeit erreicht
  } else {
    delay(duration);
  }
}

/*
 * Dreht den Roboter um 90 Grad nach links
 */
void turnRight() {
  motors.setSpeeds(baseSpeed,-baseSpeed);
  delay(turnDelay);
  motors.setSpeeds(0,0);
}

/*
 * Dreht den Roboter um 90 Grad nach rechts
 */
void turnLeft() {
  motors.setSpeeds(-baseSpeed,baseSpeed);
  delay(turnDelay);
  motors.setSpeeds(0,0);
}


