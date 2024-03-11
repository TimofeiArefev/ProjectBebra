#include <Adafruit_NeoPixel.h>

#define PIN 11
#define NUMPIXELS 4

#define CLOSE 20
#define NORMAL 30
#define FAR 100

#define BRIGHTNES_LEVEL 20

#define  TURN_90 34

#define MOTOR_TURN_SPEED 400
#define CHECK_STRAIGT_LINE_MOVEMENT 6



//I have no idea what is that
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};

#define BLACK_LIMIT 750

const int MOT_A1 = 4;
const int MOT_A2 = 5;
const int MOT_B1 = 6;
const int MOT_B2 = 10;
const int MOT_R1 = 2;
const int MOT_R2 = 3;


int sensor_A0, sensor_A1, sensor_A2, sensor_A3, sensor_A4, sensor_A5, sensor_A6, sensor_A7;

const int trigPin = 12;
const int echoPin = 13;
// defines variables
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
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 
  pinMode(MOT_A1, OUTPUT); // Sets the echoPin as an Input
  pinMode(MOT_A2, OUTPUT); // Sets the echoPin as an Input
  pinMode(MOT_B1, OUTPUT); // Sets the echoPin as an Input
  pinMode(MOT_B2, OUTPUT); // Sets the echoPin as an Input

  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pinMode(MOT_R1, INPUT_PULLUP);
  pinMode(MOT_R2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MOT_R1), ISR_R, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOT_R2), ISR_L, CHANGE);


}


// the loop function runs over and over again forever
void loop() {
  // Clears the trigPin

  read();

  
  const int stopDeley = 1000;
  if (isRightSensors()){
    // stop();
    //Serial.println("1 turn right");

    // delay(stopDeley);
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    // delay(200);


    turnRight(TURN_90);
    // Serial.println("turn right");
    // delay(600);
    // stop();   // 90 d turn
    // turnRightUltra();
    
    delay(600);
  
  }
  else if (isLeftSensors() ){
    // stop();
    Serial.println("turn Left");
    // delay(stopDeley);
    
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    // delay(200);
    // stop();

    // delay(1000);
    
    read();

    if(!isCenterSensors()){
      
      turnLeft(TURN_90);
      // Serial.println("OH NO GO STRAIGHT");
      // delay(600);
      // stop(); 

      delay(600);
    }

  
  }
  else if (isNoSensors()){
    stop();
    Serial.println("Full stop");
    delay(stopDeley);    
    
    // fullTurnRight();

    turnRightUltra();


    delay(stopDeley);

  
  
  }
  else if (sensor_A2  >= BLACK_LIMIT){
    smallTurnRight();
    Serial.println("small turn Right");
  }

  else if (sensor_A5  >= BLACK_LIMIT){
    smallTurnLeft();
    Serial.println("small turn Left");
  }
  else{
    goStraight();
  }

  // if(sensor_A5  >= BLACK_LIMIT && sensor_A6 >= BLACK_LIMIT && sensor_A7  <= BLACK_LIMIT){
  //   smallTurnLeft();
  //   Serial.println("3 small turn LEFT");
  // }
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
  return isOverBlackLimit(sensor_A3) || isOverBlackLimit(sensor_A4) ; //||  isOverBlackLimit(sensor_A6)  isOverBlackLimit(sensor_A3) || 
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
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return duration*0.034/2;
}

void goStraight(){
  analogWrite(MOT_A1, 0);
  analogWrite(MOT_A2, 480 );
  analogWrite(MOT_B1, 0);
  analogWrite(MOT_B2, 474 );
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

    Serial.println(countL);
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

    // Serial.println(countR);
  }
  stop();
}


void goStraight(int d){
  countL=0;
  countR=0;

  while(countR < d){
    digitalWrite(MOT_A1, LOW);
    digitalWrite(MOT_A2, HIGH);
    digitalWrite(MOT_B1, LOW);
    digitalWrite(MOT_B2, HIGH);

    // Serial.println(countR);
  }
  stop();
}

void turnRightUltra(){
  fullTurnRight(210);
  while(true){
    read();
    if(isCenterSensors()){
      stop();
      break;
    }
  }
  fullTurnLeft();
  delay(100);
  stop();
  
}


void fullTurnRight(){
  digitalWrite(MOT_A2, HIGH);
  digitalWrite(MOT_B1, HIGH);
  

  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B2, LOW);
}

void fullTurnRight(int speed){
  analogWrite(MOT_A2, speed);
  analogWrite(MOT_B1, speed);

  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B2, LOW);
}



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

