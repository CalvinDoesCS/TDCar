#include "motor.h"
#include <wiringPi.h>
#include <algorithm>

Motor::Motor(int direction_pin, RobotHAT_PWM *pwm_controller, bool reverse_direction)
{
    dirPin = direction_pin;
    pwm = pwm_controller;
    calibration = 0;
    currentSpeed = 0;
    reversed = reverse_direction;

    // Setup direction pin as output
    pinMode(dirPin, OUTPUT);
}

void Motor::setSpeed(int speed)
{
    // Clamp speed to -100 to 100
    speed = std::max(-100, std::min(100, speed));

    // Store current speed
    currentSpeed = speed;

    // Apply calibration
    int adjusted_speed = speed + calibration;
    adjusted_speed = std::max(-100, std::min(100, adjusted_speed));

    // Apply reversal if needed
    if (reversed)
    {
        adjusted_speed = -adjusted_speed;
    }

    // Determine direction
    if (adjusted_speed >= 0)
    {
        // Forward direction
        digitalWrite(dirPin, LOW);
        pwm->pulse_width_percent(adjusted_speed);
    }
    else
    {
        // Backward direction
        digitalWrite(dirPin, HIGH);
        pwm->pulse_width_percent(-adjusted_speed);
    }
}

void Motor::forward(int speed)
{
    // Clamp to 0-100
    speed = std::max(0, std::min(100, speed));
    setSpeed(speed);
}

void Motor::backward(int speed)
{
    // Clamp to 0-100
    speed = std::max(0, std::min(100, speed));
    setSpeed(-speed);
}

void Motor::stop()
{
    setSpeed(0);
}

void Motor::setCalibration(int cal)
{
    calibration = cal;
}

int Motor::getSpeed()
{
    return currentSpeed;
}

void Motor::setReversed(bool reverse)
{
    reversed = reverse;
}

bool Motor::isReversed()
{
    return reversed;
}
