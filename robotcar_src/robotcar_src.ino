int mainSwitch = 4;

//Fotosensorik
int sensor_RR = A0;             // Pin
int sensor_R  = A1;             // Pin
int sensor_M  = A2;             // Pin
int sensor_L  = A3;             // Pin
int sensor_LL = A4;             // Pin
int sensor_TriggerLimit = 500;  // Schaltschwelle ab wann Sensor schaltet
int direction = 0;              // 0 = Stop, 1 = Left-Left, 2 = Left, 3 = Straight, 4 = Right, 5 = Right-Right
long Zeitnehmer1 = 0;
long ZeitDelay1 = 2000;         // Zeit nach Linienverlust an den äußeren Sensoren in ms

//Ultraschallsensorik
int trigger=8;                  // Pin
int echo=7;                     // Pin
long dauer=0;           
long entfernung=0;
long minEntfernung=100;         // Distanz Mindestabstand in cm (Bei unterschreiten Stillstand)
long maxEntfernung=200;         // Distanz Maximalabstand in cm (Bei überschreiten Fullspeed)

//Motoren
int speed_Left = 0;
int speed_Right = 0;
int fullspeed = 100;            // Geschwindigkeit für Fullspeed in %
int speed_1 = 70;               // Erste Verzögerungsgeschwindigkeit in %
int speed_2 = 30;               // Zweite Verzögerungsgeschwindigkeit in %
int pwmMotorL1 = 3;             // Pin
int pwmMotorL2 = 5;             // Pin
int pwmMotorR1 = 6;             // Pin
int pwmMotorR2 = 9;             // Pin

void setup() {
  //Motoren
  pinMode(pwmMotorL1, OUTPUT);
  pinMode(mainSwitch, INPUT);
  pinMode(pwmMotorL2, OUTPUT);
  pinMode(pwmMotorR1, OUTPUT);
  pinMode(pwmMotorR2, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  // ** Sensor detektiert letzte Richtung
  if (analogRead(sensor_M) > sensor_TriggerLimit) {direction = 3;} 
  else if (analogRead(sensor_R) > sensor_TriggerLimit) {direction = 4;} 
  else if (analogRead(sensor_L) > sensor_TriggerLimit) {direction = 2;} 
  else if (analogRead(sensor_RR) > sensor_TriggerLimit) {direction = 5;} 
  else if (analogRead(sensor_LL) > sensor_TriggerLimit) {direction = 1;}

  // ** Setze Achsgeschwindigkeiten
  if (direction == 3) {
    speed_Left = fullspeed;
    speed_Right = fullspeed;
  } else if (direction == 4) {
    speed_Left = fullspeed;
    speed_Right = speed_1;
  } else if (direction == 2) {
    speed_Left = speed_1;
    speed_Right = fullspeed;
  } else if (direction == 5) {
    speed_Left = fullspeed;
    speed_Right = speed_2;
    //Wenn länger als X kein Sensor mehr getriggert wird dann STOP.
    Zeitnehmer1 = millis();    
    if (millis() > ZeitDelay1 + Zeitnehmer1 ) {
      direction = 0;
    }
  } else if (direction == 1) {
    speed_Left = speed_2;
    speed_Right = fullspeed;
    //Wenn länger als X kein Sensor mehr getriggert wird dann STOP.
    Zeitnehmer1 = millis();    
    if (millis() > ZeitDelay1 + Zeitnehmer1 ) {
      direction = 0;
    }
  }

  
  // ** Ultraschallsensor Abstand
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);
  entfernung = (dauer/2) * 0.03432;
  
  // ** Differenzrechnung bis Stillstand
  if (entfernung <= maxEntfernung && entfernung >= minEntfernung)
  {
    speed_Right = speed_Right - (1.0 / (maxEntfernung - minEntfernung) * (entfernung - minEntfernung));
  	speed_Left = speed_Left - (1.0 / (maxEntfernung - minEntfernung) * (entfernung - minEntfernung));
  } else if (entfernung > maxEntfernung) {
    speed_Right = speed_Right;
    speed_Left = speed_Left;
  } else {
    speed_Right = 0;
    speed_Left = 0;
  }
  
  // ** Speedfaktor
  speed_Right = speed_Right * 2,55;
  speed_Left = speed_Left * 2,55;

  // ** Fullstop
  if (direction == 0) {
    speed_Right = 0;
    speed_Left = 0;
  }
  
  // ** Analogwerte schreiben
  analogWrite(pwmMotorL1, speed_Right);
  analogWrite(pwmMotorL2, speed_Right);
  analogWrite(pwmMotorR1, speed_Left);
  analogWrite(pwmMotorR2, speed_Left);
    
  delay(200); //Wegen der Simulation
}
