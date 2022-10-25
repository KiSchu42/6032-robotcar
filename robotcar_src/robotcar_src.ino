int mainSwitch = 4;

//Fotosensorik
int sensor_RR = A0;
int sensor_R  = A1;
int sensor_M  = A2;
int sensor_L  = A3;
int sensor_LL = A4;
int sensor_TriggerLimit = 500;

//Ultraschallsensorik
int trigger=8;
int echo=7;
long dauer=0;
long entfernung=0;
long minEntfernung=100;
long maxEntfernung=200;

//Motoren
int speed_Left = 0;
int speed_Right = 0;
int fullspeed = 100;
int speed_1 = 70;
int speed_2 = 30;
int pwmMotorL1 = 3;
int pwmMotorL2 = 5;
int pwmMotorR1 = 6;
int pwmMotorR2 = 9;

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
  if (analogRead(sensor_M) > sensor_TriggerLimit) {
    speed_Left = fullspeed;
    speed_Right = fullspeed;
  } else if (analogRead(sensor_R) > sensor_TriggerLimit) {
    speed_Left = fullspeed;
    speed_Right = speed_1;
  } else if (analogRead(sensor_L) > sensor_TriggerLimit) {
    speed_Left = speed_1;
    speed_Right = fullspeed;
  } else if (analogRead(sensor_RR) > sensor_TriggerLimit) {
    speed_Left = fullspeed;
    speed_Right = speed_2;
  } else if (analogRead(sensor_LL) > sensor_TriggerLimit) {
    speed_Left = speed_2;
    speed_Right = fullspeed;
  }
  
  //Ultraschallsensor Abstand
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);
  entfernung = (dauer/2) * 0.03432;
  
  //Differenzrechnung bis Stillstan
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
  
  // Speedfaktor
  speed_Right = speed_Right * 2,55;
  speed_Left = speed_Left * 2,55;
  
  // Analogwerte schreiben
  analogWrite(pwmMotorL1, speed_Right);
  analogWrite(pwmMotorL2, speed_Right);
  analogWrite(pwmMotorR1, speed_Left);
  analogWrite(pwmMotorR2, speed_Left);
    
  delay(200);
}