#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS 4

#define CLOSE 10
#define NORMAL 30
#define FAR 100

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

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
}


// the loop function runs over and over again forever
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance from the object = ");
  Serial.print(distance);
  Serial.println(" cm");
  // delay(1000);
  if(distance <= CLOSE){
    pixels.clear();
    setPixlsRed();
    pixels.show();

  }
  else if(distance <= NORMAL){
    pixels.clear();
    setPixlsYellow();
    pixels.show();
  }
  else{
    pixels.clear();
    setPixlsGreen();
    pixels.show();
  }
  // delay(1000);

  // 
  
  // delay(1000);
  
  // 

  // delay(1000);


}



void setPixlsRed(){
  Serial.println("Pixel Red");
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.setPixelColor(2, pixels.Color(0, 150, 0));
  pixels.setPixelColor(3, pixels.Color(0, 150, 0));
}

void setPixlsGreen(){
  Serial.println("Pixel Green");
  pixels.setPixelColor(0, pixels.Color(150, 0, 0));
  pixels.setPixelColor(1, pixels.Color(150, 0, 0));
  pixels.setPixelColor(2, pixels.Color(150, 0, 0));
  pixels.setPixelColor(3, pixels.Color(150, 0, 0));
}

void setPixlsYellow(){
  Serial.println("Pixel Yellow");
  pixels.setPixelColor(0, pixels.Color(150, 150, 0));
  pixels.setPixelColor(1, pixels.Color(150, 150, 0));
  pixels.setPixelColor(2, pixels.Color(150, 150, 0));
  pixels.setPixelColor(3, pixels.Color(150, 150, 0));
}

