#include <Adafruit_NeoPixel.h>

#define PIN 11
#define NUMPIXELS 4

#define CLOSE 20
#define NORMAL 30
#define FAR 100

#define BRIGHTNES_LEVEL 20

#define MOTOR_TURN_SPEED 400




//I have no idea what is that
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};

#define BLACK_LIMIT 800

const int MOT_A1 = 3;
const int MOT_A2 = 5;
const int MOT_B1 = 6;
const int MOT_B2 = 10;


const int trigPin = 12;
const int echoPin = 13;
// defines variables
long duration;
int distance;


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
}


// the loop function runs over and over again forever
void loop() {
  // Clears the trigPin
  distance = culculateDistance();

  for (int i = 0; i < 8; i++) {
    int sensorState = analogRead(sensorPins[i]);
    Serial.print(sensorState);
    Serial.print(" ");
  }
  Serial.println(" ");
  delay(100);

  int sensor_A0 = analogRead(A0);
  int sensor_A1 = analogRead(A1);
  int sensor_A2 = analogRead(A2);
  int sensor_A3 = analogRead(A3);
  int sensor_A4 = analogRead(A4);
  int sensor_A5 = analogRead(A5);
  int sensor_A6 = analogRead(A6);
  int sensor_A7 = analogRead(A7);


  if (sensor_A0 >= BLACK_LIMIT && sensor_A1 >= BLACK_LIMIT && sensor_A2  >= BLACK_LIMIT){
    stop();
    Serial.println("1 turn right");
    delay(3000);
    goStraight();
    delay(200);
    turnRight();
    Serial.println("turn right");

    delay(600);
    stop();
    delay(3000);
  
  }
  else if (sensor_A5 >= BLACK_LIMIT && sensor_A6 >= BLACK_LIMIT && sensor_A7  >= BLACK_LIMIT){
    stop();
    Serial.println("turn Left");
    delay(3000);
    goStraight();
    delay(200);
    turnLeft();
    Serial.println("turn left");

    delay(600);
    stop();
    delay(3000);
  
  }
  else if (sensor_A0 <= BLACK_LIMIT && sensor_A1 <= BLACK_LIMIT && sensor_A2  <= BLACK_LIMIT && sensor_A3 <= BLACK_LIMIT && 
  sensor_A4  <= BLACK_LIMIT && sensor_A5 <= BLACK_LIMIT && sensor_A6 <= BLACK_LIMIT && sensor_A7  <= BLACK_LIMIT){
    stop();
    Serial.println("Full stop");
    delay(3000);    
    fullTurnRight();
    Serial.println("turn right");

    delay(740);
    stop();
    delay(3000);

  
  
  }
  else if (sensor_A2  >= BLACK_LIMIT){
    smallTurnRight();
    Serial.println("small turn Right");
  }

  else if (sensor_A5  >= BLACK_LIMIT){
    smallTurnLeft();
    Serial.println("small turn Left");
  }
  // else if (sensor_A1 >= BLACK_LIMIT && sensor_A2  >= BLACK_LIMIT){
  //   turnRight();
  //   Serial.println("2 turn Right");
  // }
  // else if (sensor_A2  >= BLACK_LIMIT){
  //   smallTurnRight();
  //   Serial.println("3 small turn Right");
  // }
  else{
    goStraight();
  }

  // if(sensor_A5 >= BLACK_LIMIT && sensor_A6 >= BLACK_LIMIT && sensor_A7  <= BLACK_LIMIT){
  //   smallTurnLeft();
  //   Serial.println("3 small turn LEFT");
  // }
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
  analogWrite(MOT_A2, 480);
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


void turnLeft(){
  digitalWrite(MOT_B2, HIGH);

  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B1, LOW);
}


void turnRight(){
  digitalWrite(MOT_A2, HIGH);
  
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_B2, LOW);
}


void fullTurnRight(){
  digitalWrite(MOT_A2, HIGH);
  digitalWrite(MOT_B1, HIGH);
  

  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B2, LOW);
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

