#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Servo.h>

// Stick pins.
const int X = A0;
const int Y = A1;

const int OPEN = 255;
const int CLOSE = 0;
const int STICK_DEADZONE = 50;
const int STICK_ZERO = 512;

const int REFRESH = 5;
int lastUp = 0;
int lastDown = 0;
int lastLeft = 0;
int lastRight = 0;

Adafruit_MotorShield AFMS_TOP(0x61);
Adafruit_MotorShield AMFS_BOT(0x60);

void setup() {
  AFMS_TOP.begin(); 
  AFMS_BOT.begin();
  Adafruit_DCMotor *upCell      = AFMS_BOT.getMotor(1);
  Adafruit_DCMotor *rightCell   = AFMS_BOT.getMotor(2); 
  Adafruit_DCMotor *downCell    = AFMS_BOT.getMotor(3);
  Adafruit_DCMotor *leftCell    = AFMS_BOT.getMotor(4);
  Adafruit_DCMotor *actuator    = AFMS_TOP.getMotor(1);
  Adafruit_DCMotor *jamVac      = AFMS_TOP.getMotor(3);
  Adafruit_DCMotor *actuatorVac = AFMS_TOP.getMotor(4);
  jamVac      -> run(FORWARD);
  actuatorVac -> run(FORWARD);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
}

void loop() {
  int x = stickPos(X);
  int y = stickPos(Y);
  main(x, y);
  delay(REFRESH);
}

int stickPos(int axis) {
  int reading = analogRead(axis);
  if (reading > STICK_ZERO + STICK_DEADZONE) {
    return (1);
  }
  else if (reading < STICK_ZERO - STICK_DEADZONE) {
    return (-1);
  }
  else {
    return (0);
  }
}

void main(int x, int y) {
  if (x == 0 && y == 1) {
    controlJamming(0, 1, 1, 1);
  }
  if (x == 1 && y == 1) {
    controlJamming(0, 0, 1, 1);
  }
  if (x == 1 && y == 0) {
    controlJamming(1, 0, 1, 1);
  }
  if (x == 1 && y == -1) {
    controlJamming(1, 0, 0, 1);
  }
  if (x == 0 && y == -1) {
    controlJamming(1, 1, 0, 1);
  }
  if (x == -1 && y == -1) {
    controlJamming(1, 1, 0, 0);
  }
  if (x == -1 && y == 0) {
    controlJamming(1, 1, 1, 0);
  }
  if (x == -1 && y == 1) {
    controlJamming(0, 1, 1, 0);
  }
}

void controlJamming(int up, int right, int down, int left) {
    jam(up, upCell);
    jam(right, rightCell);
    jam(down, downCell);
    jam(left, leftCell);
}
 
void jam(int state, Adafruit_DCMotor motor) {
  if (state == 1) {
    motor -> run(FORWARD);
    motor -> setSpeed(OPEN);
  } else {
    motor -> setSpeed(CLOSE);
    motor -> run(BACKWARD);
  }
}
