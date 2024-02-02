#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define BASE_SPEED 200
#define TURN_SPEED 200

// LCD Display -------------------------------------------- //
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Bluetooth Communication -------------------------------- //
SoftwareSerial BT(0, 1);

// HC-SR04 Pins ------------------------------------------- //
const uint8_t frontTrig = A1;
const uint8_t frontEcho = A0;
const uint8_t rightTrig = 8;
const uint8_t rightEcho = 7;
const uint8_t leftTrig = 12;
const uint8_t leftEcho = 11;

// HC-SR04 Delays ------------------------------------------ //
int trigHighDelay = 10;
int trigLowDelay = 2;

// Buzzer -------------------------------------------------- //
const uint8_t buzzerPin = 13;
unsigned long prevTime = 0;
unsigned long onDuration = 800;
unsigned long offDuration = 400;
bool isBuzzerOn = false;

// Motor A Pins: RIGHT ------------------------------------- //
const uint8_t ENA = 10;
const uint8_t IN1 = 3;
const uint8_t IN2 = 4;

// Motor B Pins: LEFT -------------------------------------- //
const uint8_t ENB = 9;
const uint8_t IN3 = 6;
const uint8_t IN4 = 5;

const int defaultSteeringValue = 536;
const int defaultDrivingValue = 522;
int steeringValue, drivingValue;
int state;

int front, right, left;
String dataString, receivedString;

void goForward(int = BASE_SPEED);
void goBackward(int = BASE_SPEED);
void turnRight(int = TURN_SPEED);
void turnLeft(int = TURN_SPEED);

void setup() {
    BT.begin(9600);

    lcd.begin();
    lcd.backlight();
    
    pinMode(frontTrig, OUTPUT);
    pinMode(frontEcho, INPUT);
    pinMode(rightTrig, OUTPUT);
    pinMode(rightEcho, INPUT);
    pinMode(leftTrig, OUTPUT);
    pinMode(leftEcho, INPUT);

    pinMode(buzzerPin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    
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

    // lcd.clear();
    // truckTootSound(300, front);

    dataString = String(front) + "," + String(right) + "," + String(left);
    if (BT.available() > 0) {
        BT.println(dataString);
    }

    if (BT.available() > 0) {
        receivedString = BT.readStringUntil('\n');
    }
    
    state = sscanf(receivedString.c_str(), "%d,%d", &steeringValue, &drivingValue);
    if (state == 2) {
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

void truckTootSound(int frequency, int frontDistance) {
    if (frontDistance > 10)  {
        noTone(buzzerPin);
        return;
    }
    
    if (isBuzzerOn && (millis() - prevTime >= onDuration)) {
        noTone(buzzerPin);
        isBuzzerOn = false;
        prevTime = millis();
    } else if (!isBuzzerOn && (millis() - prevTime >= offDuration)) {
        tone(buzzerPin, frequency);
        isBuzzerOn = true;
        prevTime = millis();
    }
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
