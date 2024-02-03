#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8);
const byte address[6] = "01100";

typedef struct {
    int xL;
    int yL;
    int xR;
    int yR;
} DATA;
DATA data;

int xLeft  = A1;
int yLeft  = A0;
int xRight = A3;
int yRight = A2;

void setup() {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

void loop() {
    int xLeftValue  = analogRead(xLeft);
    xLeftValue = map(xLeftValue, 0, 1023, 1023, 0);
    
    int yLeftValue  = analogRead(yLeft);
    yLeftValue = map(yLeftValue, 0, 1023, 1023, 0);
    
    int xRightValue = analogRead(xRight);
    xRightValue = map(xRightValue, 0, 1023, 1023, 0);
    
    int yRightValue = analogRead(yRight);
    yRightValue = map(yRightValue, 0, 1023, 1023, 0);

    data.xL = xLeftValue;
    data.yL = yLeftValue;
    data.xR = xRightValue;
    data.yR = yRightValue;

    radio.write(&data, sizeof(DATA));
}
