#include <Adafruit_NeoPixel.h>

#define PIN 11
#define NUMPIXELS 4

#define CLOSE 20
#define NORMAL 30
#define FAR 100

#define BRIGHTNES_LEVEL 20

#define MOTOR_TURN_SPEED 50




//I have no idea what is that
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

unsigned long start = millis();

const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sensorsValues[] = {0, 0, 0, 0, 0, 0, 0, 0};

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

  getSensors();

  if(
    sensorsValues[6] == 1 &&
    sensorsValues[7] == 1
  ){
    do {
      left();
      getSensors();
    } while(!(
        (sensorsValues[5] == 1 || sensorsValues[2] == 1) ||
        (sensorsValues[4] == 1 && sensorsValues[3] == 1)
      ));
  } else if (
    (sensorsValues[5] == 1 || sensorsValues[2] == 1) ||
    (sensorsValues[4] == 1 && sensorsValues[3] == 1)
  ){
    forward();
  } else {
    do{
      right();
      getSensors();
      break;
    }while(!(
      (sensorsValues[5] == 1 || sensorsValues[2] == 1) || 
      (sensorsValues[4] == 1 && sensorsValues[3] == 1)
    ));
  }
}

void getSensors(){
  for (int i = 0; i < sizeof(sensorPins) / sizeof(sensorPins[0]); i++) {
    int sensorState = analogRead(sensorPins[i]);
    sensorsValues[i] = sensorState >= BLACK_LIMIT ? 1 : 0;
    
    Serial.print(sensorsValues[i]);
    Serial.print(" ");

  }
  Serial.println("");
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

void forward(){
  analogWrite(MOT_A1, 0);
  analogWrite(MOT_A2, 480);
  analogWrite(MOT_B1, 0);
  analogWrite(MOT_B2, 474);
}

void backward(){
  analogWrite(MOT_A1, 480);
  analogWrite(MOT_A2, 0);
  analogWrite(MOT_B1, 474);
  analogWrite(MOT_B2, 0);
}

void stop(){
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B2, LOW);
}

void left(){
  digitalWrite(MOT_A1, HIGH);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B2, HIGH);
}

void right(){
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B1, HIGH);
  digitalWrite(MOT_A2, HIGH);
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

