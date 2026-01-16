#include "picarx.h"
#include <algorithm>

PiCarX::PiCarX(Motor *left, Motor *right, Servo *steer)
{
    leftMotor = left;
    rightMotor = right;
    steering = steer;

    // Initialize steering to center position
    centerSteering();
}

void PiCarX::setSteeringAngle(int angle)
{
    steering->setAngle(angle);
}

void PiCarX::centerSteering()
{
    steering->center();
}

int PiCarX::getSteeringAngle()
{
    return steering->getAngle();
}

void PiCarX::forward(int speed)
{
    speed = std::max(0, std::min(100, speed));
    leftMotor->forward(speed);
    rightMotor->forward(speed);
}

void PiCarX::backward(int speed)
{
    speed = std::max(0, std::min(100, speed));
    leftMotor->backward(speed);
    rightMotor->backward(speed);
}

void PiCarX::turnLeft(int speed, int turn_rate)
{
    speed = std::max(0, std::min(100, speed));
    turn_rate = std::max(0, std::min(100, turn_rate));

    // Reduce left motor speed based on turn rate
    int left_speed = speed * (100 - turn_rate) / 100;

    leftMotor->forward(left_speed);
    rightMotor->forward(speed);
}

void PiCarX::turnRight(int speed, int turn_rate)
{
    speed = std::max(0, std::min(100, speed));
    turn_rate = std::max(0, std::min(100, turn_rate));

    // Reduce right motor speed based on turn rate
    int right_speed = speed * (100 - turn_rate) / 100;

    leftMotor->forward(speed);
    rightMotor->forward(right_speed);
}

void PiCarX::rotateLeft(int speed)
{
    speed = std::max(0, std::min(100, speed));
    leftMotor->backward(speed);
    rightMotor->forward(speed);
}

void PiCarX::rotateRight(int speed)
{
    speed = std::max(0, std::min(100, speed));
    leftMotor->forward(speed);
    rightMotor->backward(speed);
}

void PiCarX::setSpeed(int left_speed, int right_speed)
{
    leftMotor->setSpeed(left_speed);
    rightMotor->setSpeed(right_speed);
}

void PiCarX::stop()
{
    leftMotor->stop();
    rightMotor->stop();
}

int PiCarX::getLeftSpeed()
{
    return leftMotor->getSpeed();
}

int PiCarX::getRightSpeed()
{
    return rightMotor->getSpeed();
}
