#include <SoftwareSerial.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define BASE_SPEED 200
#define TURN_SPEED 200

// Bluetooth Communication -------------------------------- //
SoftwareSerial BT(0, 1);

// Radio Communication ------------------------------------ //
RF24 radio(7, 8);
const byte address[6] = "01100";

// HC-SR04 Pins ------------------------------------------- //
const uint8_t frontTrig = A0;
const uint8_t frontEcho = A1;
const uint8_t rightTrig = A2;
const uint8_t rightEcho = A3;
const uint8_t leftTrig = A4;
const uint8_t leftEcho = A5;

// HC-SR04 Delays ------------------------------------------ //
int trigHighDelay = 10;
int trigLowDelay = 2;

// Motor A Pins: RIGHT ------------------------------------- //
const uint8_t ENA = 10;
const uint8_t IN1 = 3;
const uint8_t IN2 = 4;

// Motor B Pins: LEFT -------------------------------------- //
const uint8_t ENB = 9;
const uint8_t IN3 = 6;
const uint8_t IN4 = 5;

// Data Variables ------------------------------------------ //
typedef struct {
  int xL;
  int yL;
  int xR;
  int yR;
} DATA;
DATA data;

const int defaultSteeringValue = 536;
const int defaultDrivingValue = 522;
int steeringValue;
int drivingValue;
int isRecording;

unsigned long prevSendingTime = 0;
unsigned long currentTime = 0;
unsigned long elapsedTime = 0;
const int sendingDuration = 1000;
bool isSendingOn = true;

int front, right, left;
String dataString;

void goForward(int = BASE_SPEED);
void goBackward(int = BASE_SPEED);
void turnRight(int = TURN_SPEED);
void turnLeft(int = TURN_SPEED);

void setup() {
    BT.begin(9600);
    
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    
    pinMode(frontTrig, OUTPUT);
    pinMode(frontEcho, INPUT);
    pinMode(rightTrig, OUTPUT);
    pinMode(rightEcho, INPUT);
    pinMode(leftTrig, OUTPUT);
    pinMode(leftEcho, INPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
    front = obstacleDistance(frontTrig, frontEcho);
    right = obstacleDistance(rightTrig, rightEcho);
    left = obstacleDistance(leftTrig, leftEcho);

    if (radio.available()) {       
        radio.read(&data, sizeof(DATA));
        steeringValue = data.xL;
        drivingValue = data.yR;
        isRecording = data.yL;

        if (BT.available()) {
            currentTime = millis();
            elapsedTime = currentTime - prevSendingTime;
            
            if (isSendingOn) {
                dataString = 
                    String(isRecording) + "," +
                    String(front) + "," +
                    String(right) + "," +
                    String(left) + "," +
                    String(steeringValue) + "," +
                    String(drivingValue);
    
                BT.println(dataString);
                if (elapsedTime > sendingDuration) {
                    isSendingOn = false;
                    prevSendingTime = currentTime;
                }          
            } else {
                BT.println("WAITING");
                if (elapsedTime > sendingDuration) {
                    isSendingOn = true;
                    prevSendingTime = currentTime;
                }
            }

            if (drivingValue >= defaultDrivingValue + 100) {
                goForward();
                return;
            } else if (drivingValue <= defaultDrivingValue - 100) {
                goBackward();
                return;
            } else {
                stopMotors();
            }

            if (steeringValue >= defaultSteeringValue + 100) {
                turnRight();
                return;
            } else if (steeringValue <= defaultSteeringValue - 100) {
                turnLeft();
                return;
            } else {
                stopMotors();
            }
        }
    }
}

int obstacleDistance(int trig, int echo) {
    unsigned long prevTrigLowTime = 0;
    unsigned long prevTrigHighTime = 0;
    
    int distance, duration;
    
    if (micros() - prevTrigLowTime < trigLowDelay) {
        digitalWrite(trig, LOW);
        prevTrigLowTime = micros();
    }

    if (micros() - prevTrigHighTime > trigHighDelay) {
        digitalWrite(trig, HIGH);
        prevTrigHighTime = micros();
    }
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);
    distance = duration * 0.034 / 2;

    return distance;
}

void goForward(int spd) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, spd);
    analogWrite(ENB, spd);
}
void goBackward(int spd) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, spd);
    analogWrite(ENB, spd);
}
void turnRight(int spd) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, spd);
    analogWrite(ENB, spd);
}
void turnLeft(int spd) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, spd);
    analogWrite(ENB, spd);
}
void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}
