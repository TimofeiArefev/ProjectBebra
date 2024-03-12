#include <Adafruit_NeoPixel.h>

#define PIN                         11
#define NUMPIXELS                   4

#define MOT_A1                      4
#define MOT_A2                      5
#define MOT_B1                      6
#define MOT_B2                      10
#define MOT_R1                      2
#define MOT_R2                      3

#define trigPin                     12
#define echoPin                     13

#define CLOSE                       20
#define NORMAL                      30
#define FAR                         100

#define BRIGHTNES_LEVEL             20

#define TURN_90                     35

#define MOTOR_TURN_SPEED            400
#define CHECK_STRAIGT_LINE_MOVEMENT 6

#define DELAYVAL                    200

#define BLACK_LIMIT                 750

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sensor_A0, sensor_A1, sensor_A2, sensor_A3, sensor_A4, sensor_A5, sensor_A6, sensor_A7;

long duration;
int distance;

volatile int countL = 0;
volatile int countR = 0;

void ISR_L(){
  countL++;
}

void ISR_R(){
  countR++;
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
 
  pinMode(MOT_A1, OUTPUT);
  pinMode(MOT_A2, OUTPUT);
  pinMode(MOT_B1, OUTPUT);
  pinMode(MOT_B2, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pinMode(MOT_R1, INPUT_PULLUP);
  pinMode(MOT_R2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MOT_R1), ISR_R, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOT_R2), ISR_L, CHANGE);
}


void loop() {
  read();

  if (isRightSensors()){
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    turnRight(TURN_90);    
    delay(DELAYVAL);
  
  }
  else if (isLeftSensors() ){    
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    read();

    if(!isCenterSensors()){      
      turnLeft(TURN_90);
      delay(DELAYVAL);
    }  
  }
  else if (isNoSensors()){
    stop();
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    turnRightUltra();
    delay(DELAYVAL);
  }
  else if (sensor_A2  >= BLACK_LIMIT){
    smallTurnRight();
  }

  else if (sensor_A5  >= BLACK_LIMIT){
    smallTurnLeft();
  }
  else{
    goStraight();
  }
}

void read(){
  sensor_A0 = analogRead(A0);
  sensor_A1 = analogRead(A1);
  sensor_A2 = analogRead(A2);
  sensor_A3 = analogRead(A3);
  sensor_A4 = analogRead(A4);
  sensor_A5 = analogRead(A5);
  sensor_A6 = analogRead(A6);
  sensor_A7 = analogRead(A7);
}


bool isRightSensors(){
    return(isOverBlackLimit(sensor_A0) && isOverBlackLimit(sensor_A1) && isOverBlackLimit(sensor_A2)) || (isOverBlackLimit(sensor_A0) && isOverBlackLimit(sensor_A1)) || isOverBlackLimit(sensor_A0);
}

bool isLeftSensors(){
    return (isOverBlackLimit(sensor_A5) && isOverBlackLimit(sensor_A6)  && isOverBlackLimit(sensor_A7)) ||
  (isOverBlackLimit(sensor_A6) && isOverBlackLimit(sensor_A7));
}


bool isNoSensors(){
  return isBelowBlackLimit(sensor_A0) && isBelowBlackLimit(sensor_A1) && isBelowBlackLimit(sensor_A2) && isBelowBlackLimit(sensor_A3) &&  
  isBelowBlackLimit(sensor_A4) && isBelowBlackLimit(sensor_A5) && isBelowBlackLimit(sensor_A6)  && isBelowBlackLimit(sensor_A7);
}

bool isCenterSensors(){
  return isOverBlackLimit(sensor_A3) || isOverBlackLimit(sensor_A4);
}

bool isOverBlackLimit(int sensor){
  return sensor >= BLACK_LIMIT;
}

bool isBelowBlackLimit(int sensor){
  return sensor <= BLACK_LIMIT;
}

int culculateDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration*0.034/2;
}

void goStraight(){
  analogWrite(MOT_A1, 0);
  analogWrite(MOT_A2, MOTOR_TURN_SPEED);
  analogWrite(MOT_B1, 0);
  analogWrite(MOT_B2, 474);
}

void smallTurnLeft(){
  analogWrite(MOT_A2, MOTOR_TURN_SPEED);
  analogWrite(MOT_B2, 474);

}
void smallTurnRight(){
  analogWrite(MOT_B2, MOTOR_TURN_SPEED);
  analogWrite(MOT_A2, 480);
}


void turnLeft(int d){
  countL=0;
  countR=0;

  while(countL < d){
    digitalWrite(MOT_B2, HIGH);
    digitalWrite(MOT_A1, LOW);
    digitalWrite(MOT_A2, LOW);
    digitalWrite(MOT_B1, LOW);
  }
  stop();
}


void turnRight(int d){
  countL=0;
  countR=0;

  while(countR < d){
    digitalWrite(MOT_A2, HIGH);
    digitalWrite(MOT_A1, LOW);
    digitalWrite(MOT_B1, LOW);
    digitalWrite(MOT_B2, LOW);
  }
  stop();
}


void goStraight(int d){
  countL=0;
  countR=0;

  while(countR < d && countL < d){
    digitalWrite(MOT_A1, LOW);
    digitalWrite(MOT_A2, HIGH);
    digitalWrite(MOT_B1, LOW);
    digitalWrite(MOT_B2, HIGH);
  }
  stop();
}

void turnRightUltra(){
  fullTurnRight(200);
  while(true){
    read();
    if(isCenterSensors()){
      stop();
      break;
    }
  }
  stop();
  
}


void fullTurnRight(){
  digitalWrite(MOT_A2, HIGH);
  digitalWrite(MOT_B1, HIGH);
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B2, LOW);
}

// void fullTurnRight(int speed){
//   analogWrite(MOT_A2, speed);
//   analogWrite(MOT_B1, speed);
//   digitalWrite(MOT_A1, LOW);
//   digitalWrite(MOT_B2, LOW);
// }


void fullTurnLeft(){
  digitalWrite(MOT_B2, HIGH);
  digitalWrite(MOT_A1, HIGH);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_A2, LOW);
}
void stop(){
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B2, LOW);
}


void setPixlsRed(){
  Serial.println("Pixel Red");
  pixels.setPixelColor(0, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(1, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(2, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(3, pixels.Color(0, BRIGHTNES_LEVEL, 0));
}

void setPixlsGreen(){
  Serial.println("Pixel Green");
  pixels.setPixelColor(0, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
}

void setPixlsYellow(){
  Serial.println("Pixel Yellow");
  pixels.setPixelColor(0, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
}

