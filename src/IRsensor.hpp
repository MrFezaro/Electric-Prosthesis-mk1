//
// Created by Fezaro on 05.09.2024.
//
#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <Arduino.h>
#include <MAX30105.h>

const int BUTTON1_PIN = 10;
const int BUTTON2_PIN = 12;
const int BUTTON3_PIN = 2;
const float ALPHA = 0.5; // Smoothing factor (0 < alpha <= 1)

bool calibrated = false;
bool button1Pressed = false;
bool button2Pressed = false;
int minValue = 0;
int maxValue = 0;
float filteredValue = 0;


// IR Sensor
MAX30105 particleSensor;

void IRsensorSetup() {
    pinMode(BUTTON1_PIN, INPUT);
    pinMode(BUTTON2_PIN, INPUT);
    pinMode(BUTTON3_PIN, INPUT);


    Serial.println("MAX30105 Basic Readings Example");

    // Initialize sensor
    if (!particleSensor.begin()) {
        Serial.println("MAX30105 was not found. Please check wiring/power.");
        while (1);
    }

    particleSensor.setup(); // Configure sensor. Use 6.4mA for LED drive
}

int getIRsensorVal() {
    int value = particleSensor.getRed();

    // Apply EMA filter
    filteredValue = ALPHA * value + (1 - ALPHA) * filteredValue;

    return filteredValue;
}

void checkButtonStates() {
    int sensorValue = getIRsensorVal();

    if (digitalRead(BUTTON1_PIN) == HIGH) {
        if (!calibrated) {
            minValue = sensorValue;
            Serial.print("Min value is set to: ");
            Serial.println(minValue);
            delay(250);
        }
        button1Pressed = true;
    } else {
        button1Pressed = false;
    }
    if (digitalRead(BUTTON2_PIN) == HIGH) {
        if (!calibrated) {
            maxValue = sensorValue;
            Serial.print("Max value is set to: ");
            Serial.println(maxValue);
            delay(250);
        }
        button2Pressed = true;
    } else {
        button2Pressed = false;
    }
    if (digitalRead(BUTTON3_PIN) == HIGH) {
        calibrated = !calibrated; // Toggle the calibrated state
        Serial.print("Calibration state toggled to: ");
        Serial.println(calibrated ? "OFF" : "ON");
        calibrated ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
    }
}

int readIRsensor(int value) {
    checkButtonStates();

    if (calibrated && button1Pressed) {
        return -100;
    }
    if (calibrated && button2Pressed) {
        return 100;
    }

    int mappedValue = map(value, minValue, maxValue, 0, 255);
    mappedValue = constrain(mappedValue, 0, 255);

    return calibrated ? mappedValue : 0;
}

#endif // FUNCTIONS_HPP
