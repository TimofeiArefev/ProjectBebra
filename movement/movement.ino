#include <Adafruit_NeoPixel.h>

#define TESTING_MODE false
//NeoPixel pin
#define PIN 5

//Pins
//TODO: change motor pins
#define MOT_A1 11
#define MOT_A2 10
#define MOT_B1 9
#define MOT_B2 6
#define MOT_R1 3
#define MOT_R2 2

#define trigPin 12
#define echoPin 13

#define GRIP 4

#define CLOSE 20
#define NORMAL 30
#define FAR 100

//NeoPixel settings
#define NUMPIXELS 4
#define BRIGHTNES_LEVEL 20


#define TURN_90 38
//All turns

//Movement
#define MOTOR_TURN_SPEED 180
#define CHECK_STRAIGT_LINE_MOVEMENT 6

#define MOTOR_A_SPEED 235
#define MOTOR_B_SPEED 255

#define DELAYVAL 200

//Black limit
int BLACK_LIMIT = 775;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int sensorPins[] = { A0, A1, A2, A3, A4, A5, A6, A7 };
int sensor_A0, sensor_A1, sensor_A2, sensor_A3, sensor_A4, sensor_A5, sensor_A6, sensor_A7;

long duration;
int distance;

bool started = false;
bool solved = false;
bool ended = false;

volatile int countL = 0;
volatile int countR = 0;

void ISR_L() {
  countL++;
}

void ISR_R() {
  countR++;
}

void setup() {
  Serial.begin(9600);
  pixels.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(MOT_A1, OUTPUT);
  pinMode(MOT_A2, OUTPUT);
  pinMode(MOT_B1, OUTPUT);
  pinMode(MOT_B2, OUTPUT);

  pinMode(GRIP, OUTPUT);
  digitalWrite(GRIP, LOW);

  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pinMode(MOT_R1, INPUT_PULLUP);
  pinMode(MOT_R2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MOT_R1), ISR_R, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOT_R2), ISR_L, CHANGE);
}


void loop() {
  if (TESTING_MODE) {
    started = true;
  }

  if (!started) {
    start();
  } else if (!solved) {
    maze();
  } else if (!ended) {
    end();
  }
}

void start() {

  int distance = culculateDistance();
  while( distance > 30){
    distance = culculateDistance();
    Serial.println(distance);
  }
  activationWait();
  
  int blackLimit[3];
  int currentIndex = 0;
  int color;

  for (int i = 0; i < 6; i++) {
    stop();
    delay(100);
    int curentColor = getAverageLightLevel();
    delay(100);
    goStraightSlow();

    color = curentColor;
    while (color > curentColor - 300 && color < curentColor + 300) {
      color = getAverageLightLevel();
    }
    if (i % 2 == 1) {
      Serial.println(curentColor);
      blackLimit[currentIndex] = curentColor;
      currentIndex++;
    }
  }

  BLACK_LIMIT = getAverageBlackLimit(blackLimit) - 100;
  Serial.print("res = ");
  Serial.println(BLACK_LIMIT);
  stop();
  delay(1000);

  goStraight(10);

  intercept();
  stop();

  goStraight(12);
  turnLeft(TURN_90);
  delay(100);

  started = true;
}

void maze() {
  read();
  if (isLeftSensors()) {
    stop();
    delay(10);
    read();
  }
  if (isRightSensors()) {
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    read();
    if (isAllSensors()) {
      solved = true;
    } else {
      turnRight(TURN_90);
      delay(DELAYVAL);
    }

  } else if (isLeftSensors()) {
    goStraight(CHECK_STRAIGT_LINE_MOVEMENT);
    read();
    if (!isCenterSensors()) {
      turnLeft(TURN_90);
      delay(DELAYVAL);
    }
  } else if (isNoSensors()) {
    goStraight(8);
    turnRightUltra();
    delay(DELAYVAL);
  } else if (sensor_A2 >= BLACK_LIMIT) {
    smallTurnRight();
  } else if (sensor_A5 >= BLACK_LIMIT) {
    smallTurnLeft();
  } else {
    goStraight();
  }

  // solved = true;
}

void end() {
  delay(1000);
  goBack(5);
  ungrab();
  goBack(30);
  while (true) {
    setPixlsRed();
    delay(200);
    setPixlsYellow();
    delay(200);
    setPixlsGreen();
    delay(200);
  }
  ended = true;
}

void read() {
  sensor_A0 = analogRead(A0);
  sensor_A1 = analogRead(A1);
  sensor_A2 = analogRead(A2);
  sensor_A3 = analogRead(A3);
  sensor_A4 = analogRead(A4);
  sensor_A5 = analogRead(A5);
  sensor_A6 = analogRead(A6);
  sensor_A7 = analogRead(A7);
}

int getAverageLightLevel() {
  read();
  return (sensor_A0 + sensor_A1 + sensor_A2 + sensor_A3 + sensor_A4 + sensor_A5 + sensor_A6 + sensor_A7) / 8;
}

int getAverageBlackLimit(int* array) {
  int res = 0;
  for (int i = 0; i < 3; i++) {
    res += array[i];
  }
  return res / 3;
}

bool isAllSensors() {
  return (isOverBlackLimit(sensor_A0) && isOverBlackLimit(sensor_A1) && isOverBlackLimit(sensor_A2) && isOverBlackLimit(sensor_A3) && isOverBlackLimit(sensor_A4) && isOverBlackLimit(sensor_A5) && isOverBlackLimit(sensor_A6) && isOverBlackLimit(sensor_A7));
}

bool isRightSensors() {
  return (isOverBlackLimit(sensor_A0) && isOverBlackLimit(sensor_A1) && isOverBlackLimit(sensor_A2)) || (isOverBlackLimit(sensor_A0) && isOverBlackLimit(sensor_A1)) || isOverBlackLimit(sensor_A0);
}

bool isLeftSensors() {
  return (isOverBlackLimit(sensor_A5) && isOverBlackLimit(sensor_A6) && isOverBlackLimit(sensor_A7)) || (isOverBlackLimit(sensor_A6) && isOverBlackLimit(sensor_A7));
}


bool isNoSensors() {
  return isBelowBlackLimit(sensor_A0) && isBelowBlackLimit(sensor_A1) && isBelowBlackLimit(sensor_A2) && isBelowBlackLimit(sensor_A3) && isBelowBlackLimit(sensor_A4) && isBelowBlackLimit(sensor_A5) && isBelowBlackLimit(sensor_A6) && isBelowBlackLimit(sensor_A7);
}

bool isCenterSensors() {
  return isOverBlackLimit(sensor_A3) || isOverBlackLimit(sensor_A4);
}

bool isOverBlackLimit(int sensor) {
  return sensor >= BLACK_LIMIT;
}

bool isBelowBlackLimit(int sensor) {
  return sensor <= BLACK_LIMIT;
}

int culculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void intercept() {
  grab();
}

void grab() {
  Serial.println("grab");
  for (int i = 0; i < 15; i++) {
    digitalWrite(GRIP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(GRIP, LOW);
    delayMicroseconds(19000);
  }
}

void ungrab() {
  Serial.println("ungrab");
  for (int i = 0; i < 15; i++) {
    digitalWrite(GRIP, HIGH);
    delayMicroseconds(1500);
    digitalWrite(GRIP, LOW);
    delayMicroseconds(18500);
  }
}

void goStraight() {
  setPixlsGreen();
  analogWrite(MOT_A2, MOTOR_A_SPEED);
  analogWrite(MOT_B2, MOTOR_B_SPEED);
  analogWrite(MOT_A1, LOW);
  analogWrite(MOT_B1, LOW);
}

void goStraightSlow() {
  setPixlsGreen();
  analogWrite(MOT_A2, 200);
  analogWrite(MOT_B2, 200);
  analogWrite(MOT_A1, LOW);
  analogWrite(MOT_B1, LOW);
}

void smallTurnLeft() {
  analogWrite(MOT_A2, MOTOR_TURN_SPEED);
  analogWrite(MOT_B2, MOTOR_B_SPEED);
}
void smallTurnRight() {
  analogWrite(MOT_B2, MOTOR_TURN_SPEED);
  analogWrite(MOT_A2, MOTOR_A_SPEED);
}


void turnLeft(int d) {
  setPixlsYellow();
  countL = 0;
  countR = 0;

  while (countL < d) {
    analogWrite(MOT_B2, MOTOR_B_SPEED);
    analogWrite(MOT_A1, LOW);
    analogWrite(MOT_A2, LOW);
    analogWrite(MOT_B1, LOW);
  }
  stop();
}


void turnRight(int d) {
  setPixlsYellow();
  countL = 0;
  countR = 0;

  while (countR < d) {
    analogWrite(MOT_A2, MOTOR_A_SPEED);
    analogWrite(MOT_A1, LOW);
    analogWrite(MOT_B1, LOW);
    analogWrite(MOT_B2, LOW);
  }
  stop();
}




void goStraight(int d) {
  setPixlsGreen();
  countL = 0;
  countR = 0;

  while (countR < d) {
    analogWrite(MOT_A2, MOTOR_A_SPEED);
    analogWrite(MOT_B2, MOTOR_B_SPEED);
    analogWrite(MOT_A1, LOW);
    analogWrite(MOT_B1, LOW);
  }
  stop();
}

void goBack(int d) {
  setPixlsGreen();
  countL = 0;
  countR = 0;

  while (countR < d) {
    analogWrite(MOT_A1, MOTOR_A_SPEED);
    analogWrite(MOT_B1, MOTOR_B_SPEED);
    analogWrite(MOT_A2, LOW);
    analogWrite(MOT_B2, LOW);
  }
  stop();
}

void turnRightUltra() {
  setPixlsRed();
  fullTurnRight(210);
  while (true) {
    read();
    if (isCenterSensors()) {
      stop();
      break;
    }
  }
  stop();
  fullTurnLeft();
  delay(110);
  stop();
}

void fullTurnRight() {
  analogWrite(MOT_A2, MOTOR_A_SPEED);
  analogWrite(MOT_B1, MOTOR_B_SPEED);
  analogWrite(MOT_A1, LOW);
  analogWrite(MOT_B2, LOW);
}


void fullTurnRight(int speed) {
  analogWrite(MOT_A2, speed);
  analogWrite(MOT_B1, speed + 6);
  analogWrite(MOT_A1, LOW);
  analogWrite(MOT_B2, LOW);
}



void fullTurnLeft() {
  analogWrite(MOT_B2, MOTOR_B_SPEED);
  analogWrite(MOT_A1, MOTOR_A_SPEED);
  analogWrite(MOT_B1, LOW);
  analogWrite(MOT_A2, LOW);
}

void stop() {
  analogWrite(MOT_A1, LOW);
  analogWrite(MOT_B1, LOW);
  analogWrite(MOT_A2, LOW);
  analogWrite(MOT_B2, LOW);
}

void activationWait(){
  pixels.setPixelColor(0, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  delay(2000);
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  delay(2000);
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  delay(2000);
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  delay(2000);
  setPixlsGreen();
}

void setPixlsRed() {
  // Serial.println("Pixel Red");
  pixels.setPixelColor(0, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(1, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(2, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(3, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.show();
}

void setPixlsGreen() {
  // Serial.println("Pixel Green");
  pixels.setPixelColor(0, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.show();
}

void setPixlsYellow() {
  // Serial.println("Pixel Yellow");
  pixels.setPixelColor(0, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.show();
}

void setPixlsBlue() {
  // Serial.println("Pixel Blue");
  pixels.setPixelColor(0, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.setPixelColor(1, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.setPixelColor(2, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.setPixelColor(3, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.show();
}