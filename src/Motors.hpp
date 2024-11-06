// src/Motors.hpp

#ifndef MOTORS_HPP
#define MOTORS_HPP

#include <Arduino.h>
#include <SparkFun_TB6612.h>

constexpr int AIN1 = 3;
constexpr int AIN2 = 4;
constexpr int BIN1 = 7;
constexpr int BIN2 = 8;
constexpr int CIN1 = A1;
constexpr int CIN2 = A0;
constexpr int PWMA = 5;
constexpr int PWMB = 6;
constexpr int PWMC = 11;
constexpr int STBY = 9;

constexpr int OPEN = 1;
constexpr int CLOSED = 2;

int state1 = OPEN;
int state2 = OPEN;
int state3 = OPEN;

Motor motor1 = Motor(AIN1, AIN2, PWMA, 1, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, 1, STBY);
Motor motor3 = Motor(CIN1, CIN2, PWMC, 1, STBY);

inline void closeFinger(Motor &motor, const int pwm, const int currentSensPin, const int closeCurrent, int &state) {
    const int sensorValue = analogRead(currentSensPin);
    if (sensorValue > closeCurrent) {
        // Lower = Higher current
        motor.drive(pwm);
    } else {
        motor.brake();
        state = CLOSED;
    }
}

inline void openFinger(Motor &motor, const int pwm, const int currentSensPin, const int openCurrent, int &state) {
    const int sensorValue = analogRead(currentSensPin);
    if (pwm > 10 || pwm < -10) {
        motor.brake();
    } else if (sensorValue < openCurrent) {
        // Higher = Higher current
        motor.drive(-100);
    } else {
        motor.drive(100, 125);
        motor.brake();
        state = OPEN;
    }
}

inline void motorLoop(Motor &motor, const int pwm, const int currentSensPin, const int closeCurrent,
                      const int openCurrent, int &state) {
    switch (state) {
        case OPEN:
            closeFinger(motor, pwm, currentSensPin, closeCurrent, state);
            break;
        case CLOSED:
            openFinger(motor, pwm, currentSensPin, openCurrent, state);
            break;
        default:
            motor.brake();
            break;
    }
}

#endif // MOTORS_HPP
