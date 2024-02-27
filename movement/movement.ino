#include <Adafruit_NeoPixel.h>

#define PIN 11
#define NUMPIXELS 4

#define BRIGHTNES_LEVEL 20
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sensorsValues[] = {0, 0, 0, 0, 0, 0, 0, 0};

#define BLACK_LIMIT 700

const int MOT_A1 = 3;
const int MOT_A2 = 5;
const int MOT_B1 = 6;
const int MOT_B2 = 10;

const int MOT_R1 = 4;
const int MOT_R2 = 2;

const int trigPin = 12;
const int echoPin = 13;

int countL = 0;
int countR = 0;

int duration;
int distance;

bool grabed = true;
bool calibrated = false;
bool solved = false;
bool won = false;

void rotationL(){
  countL++;
}

void rotationR(){
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
  pinMode(MOT_R1, INPUT);
  pinMode(MOT_R2, INPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(MOT_R1), rotationL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOT_R2), rotationR, CHANGE);
}


void loop() {
  read_sensors();

  if(!solved){
    solve();
  }
}

void read_sensors(){
  for (int i = 0; i < 8; i++){
    sensorsValues[i] = analogRead(sensorPins[i]) > BLACK_LIMIT ? 1 : 0;
  }
}

void solve(){
  if (sensorsValues[4] == 1 && sensorsValues[5] == 1 && sensorsValues[6] == 1 && sensorsValues[7] == 1){
    forward();
    delay(100);
    stop();

    while(true){
      read_sensors();

      if (sensorsValues[0] == 1 && sensorsValues[1] == 1 && sensorsValues[2] == 1 && sensorsValues[3] == 1 && sensorsValues[4] == 1 && sensorsValues[5] == 1 && sensorsValues[6] == 1 && sensorsValues[7] == 1){
        stop();
        delay(250);
        solved = true;
        break;
      } else {
        fixed_right_1(45);
      }
    }
  } else if (sensorsValues[3] == 1 && sensorsValues[4] == 1){
    forward();
  } else if (sensorsValues[5] == 1 || sensorsValues[6] == 1 || sensorsValues[7] == 1){
    right();
  } else if (sensorsValues[0] == 1 || sensorsValues[1] == 1 || sensorsValues[2] == 1){
    left();
  } else if (sensorsValues[0] == 0 && sensorsValues[1] == 0 && sensorsValues[2] == 0 && sensorsValues[3] == 0 && sensorsValues[4] == 0 && sensorsValues[5] == 0 && sensorsValues[6] == 0 && sensorsValues[7] == 0){
    forward();
    delay(100);
    fixed_left_2(25);

    while(true){
      read_sensors();

      if (sensorsValues[0] == 1 || sensorsValues[1] == 1 || sensorsValues[2] == 1 || sensorsValues[3] == 1 || sensorsValues[4] == 1 || sensorsValues[5] == 1 || sensorsValues[6] == 1 || sensorsValues[7] == 1) {
        break;
      }

      if (sensorsValues[0] == 0 && sensorsValues[1] == 0 && sensorsValues[2] == 0 && sensorsValues[3] == 0 && sensorsValues[4] == 0 && sensorsValues[5] == 0 && sensorsValues[6] == 0 && sensorsValues[7] == 0) {
        left_spin();
      }
    }
  }
}

void forward(){
  digitalWrite(MOT_A1, HIGH);
  digitalWrite(MOT_B1, HIGH);
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B2, LOW);
}
void backward(){
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_A2, HIGH);
  digitalWrite(MOT_B2, HIGH);
}
void right(){
  analogWrite(MOT_A1, 250);
  analogWrite(MOT_B1, 130);
  analogWrite(MOT_A2, 0);
  analogWrite(MOT_B2, 0);
}
void fixed_right_1(int cyc){
  bool rotate = true;
  int cycles = cyc;

  countL = 0;
    
  while (rotate){
    if (countL < cycles){
      analogWrite(MOT_A1, 200);
      analogWrite(MOT_B1, 0);
      analogWrite(MOT_A2, 0);
      analogWrite(MOT_B2, 0);
    } else if(countL > cycles) {
      rotate = false;
    }
  }
}
void fixed_right_2(int cyc){
  bool rotate = true;
  int cycles = cyc;

  countL = 0;
    
  while (rotate){
    if (countL < cycles){
      analogWrite(MOT_A1, 150);
      analogWrite(MOT_B1, 0);
      analogWrite(MOT_A2, 0);
      analogWrite(MOT_B2, 150);
    } else if(countL > cycles) {
      rotate = false;
    }
  }
}
void left(){
  analogWrite(MOT_A1, 130);
  analogWrite(MOT_B1, 250);
  analogWrite(MOT_A2, 0);
  analogWrite(MOT_B2, 0);
}
void fixed_left_1(int cyc){
  bool rotate = true;
  int cycles = cyc;

  countR = 0;
    
  while (rotate){
    if(countR < cycles){
      analogWrite(MOT_A1, 0);
      analogWrite(MOT_B1, 200);
      analogWrite(MOT_A2, 0);
      analogWrite(MOT_B2, 0);
    }else if(countR > cycles){
      rotate = false;
    }
  }
}
void fixed_left_2(int cyc){
  bool rotate = true;
  int cycles = cyc;

  countR = 0;
    
  while (rotate)
  {
    if(countR < cycles)
    {
      analogWrite(MOT_A1, 0);
      analogWrite(MOT_B1, 150);
      analogWrite(MOT_A2, 150);
      analogWrite(MOT_B2, 0);
    }
    else if(countR > cycles)
    {
      rotate = false;
    }
  }
}
void left_spin()
{
  analogWrite(MOT_A1, 0);
  analogWrite(MOT_B1, 130);
  analogWrite(MOT_A2, 130);
  analogWrite(MOT_B2, 0);
}
void stop(){
  digitalWrite(MOT_A1, LOW);
  digitalWrite(MOT_B1, LOW);
  digitalWrite(MOT_A2, LOW);
  digitalWrite(MOT_B2, LOW);
}