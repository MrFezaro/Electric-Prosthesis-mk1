#include <Arduino.h>
#include "IRsensor.hpp"
#include "Motors.hpp"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    IRsensorSetup();
}

void loop() {
    const int rawValue = getIRsensorVal();
    const int calibratedValue = readIRsensor(rawValue);
    motorLoop(motor1, calibratedValue, A2, 475, 535, state1);
    motorLoop(motor2, calibratedValue, A3, 475, 545, state2);
    motorLoop(motor3, calibratedValue, A6, 475, 545, state3);
}