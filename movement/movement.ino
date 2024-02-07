#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 4

#define CLOSE 20
#define NORMAL 30
#define FAR 100

#define BRIGHTNES_LEVEL 20


//I have no idea what is that
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500


const int MOT_A1 = 2;
const int MOT_A2 = 3;
const int MOT_B1 = 4;
const int MOT_B2 = 5;


const int trigPin = 12;
const int echoPin = 11;
// defines variables
long duration;
int distance;


void setup() {
  Serial.begin(9600);
  pixels.begin();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 
  pinMode(MOT_A1, OUTPUT); // Sets the echoPin as an Input
  pinMode(MOT_A2, OUTPUT); // Sets the echoPin as an Input
  pinMode(MOT_B1, OUTPUT); // Sets the echoPin as an Input
  pinMode(MOT_B2, OUTPUT); // Sets the echoPin as an Input


}


// the loop function runs over and over again forever
void loop() {
  // Clears the trigPin
  distance = culculateDistance();
  // Prints the distance on the Serial Monitor
  Serial.print("Distance from the object = ");
  Serial.print(distance);
  Serial.println(" cm");

  // stop();
  // delay(1000);
  if(distance <= CLOSE){
    pixels.clear();
    setPixlsRed();
    pixels.show();
    stopGoStraight();
    turnLeft();
  }
  else if(distance <= NORMAL){
    stopTurnLeft();
    pixels.clear();
    setPixlsYellow();
    pixels.show();
    goStraight();
    
  }
  else{
    stopTurnLeft();
    pixels.clear();
    setPixlsGreen();
    pixels.show();
    goStraight();

  }


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
  digitalWrite(MOT_A2, HIGH);
  digitalWrite(MOT_B2, HIGH);
}

void stopGoStraight(){
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B2, LOW);
}

void turnLeft(){
  digitalWrite(MOT_A1, HIGH);
  digitalWrite(MOT_B2, HIGH);
}
void stopTurnLeft(){
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

