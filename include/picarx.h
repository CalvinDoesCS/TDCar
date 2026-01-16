#ifndef PICARX_H
#define PICARX_H

/**
 * @file picarx.h
 * @brief PiCar-X robot control class
 *
 * High-level robot control that coordinates motors and steering.
 * Provides simple methods for forward, backward, turning, and stopping.
 */

#include "motor.h"
#include "servo.h"
#include "robot_hat_pwm.h"

/**
 * @class PiCarX
 * @brief High-level robot control for PiCar-X
 *
 * Manages two DC motors and a steering servo for complete robot control.
 * Abstracts the complexity of controlling individual motors and steering.
 */
class PiCarX
{
private:
    Motor *leftMotor;  ///< Left motor controller
    Motor *rightMotor; ///< Right motor controller
    Servo *steering;   ///< Steering servo controller

public:
    /**
     * @brief Construct a new PiCarX object
     * @param left Pointer to left motor object
     * @param right Pointer to right motor object
     * @param steer Pointer to steering servo object
     */
    PiCarX(Motor *left, Motor *right, Servo *steer);

    /**
     * @brief Set steering angle
     * @param angle Steering angle in degrees (-90 to 90, 0 is center)
     *
     * Positive angles turn right, negative angles turn left.
     */
    void setSteeringAngle(int angle);

    /**
     * @brief Center the steering
     */
    void centerSteering();

    /**
     * @brief Get current steering angle
     * @return int Current steering angle
     */
    int getSteeringAngle();

    /**
     * @brief Move forward at specified speed
     * @param speed Speed value (0-100)
     *
     * Both motors move forward at the same speed.
     */
    void forward(int speed);

    /**
     * @brief Move backward at specified speed
     * @param speed Speed value (0-100)
     *
     * Both motors move backward at the same speed.
     */
    void backward(int speed);

    /**
     * @brief Turn left while moving forward
     * @param speed Forward speed (0-100)
     * @param turn_rate Turn sharpness (0-100, higher = sharper turn)
     *
     * Slows down left motor relative to right motor to turn left.
     */
    void turnLeft(int speed, int turn_rate = 50);

    /**
     * @brief Turn right while moving forward
     * @param speed Forward speed (0-100)
     * @param turn_rate Turn sharpness (0-100, higher = sharper turn)
     *
     * Slows down right motor relative to left motor to turn right.
     */
    void turnRight(int speed, int turn_rate = 50);

    /**
     * @brief Rotate in place to the left
     * @param speed Rotation speed (0-100)
     *
     * Left motor backward, right motor forward for zero-radius turn.
     */
    void rotateLeft(int speed);

    /**
     * @brief Rotate in place to the right
     * @param speed Rotation speed (0-100)
     *
     * Left motor forward, right motor backward for zero-radius turn.
     */
    void rotateRight(int speed);

    /**
     * @brief Set individual motor speeds
     * @param left_speed Left motor speed (-100 to 100)
     * @param right_speed Right motor speed (-100 to 100)
     *
     * Provides direct control over each motor independently.
     */
    void setSpeed(int left_speed, int right_speed);

    /**
     * @brief Stop both motors
     *
     * Brings the robot to a complete stop.
     */
    void stop();

    /**
     * @brief Get current left motor speed
     * @return int Left motor speed (-100 to 100)
     */
    int getLeftSpeed();

    /**
     * @brief Get current right motor speed
     * @return int Right motor speed (-100 to 100)
     */
    int getRightSpeed();
};

#endif // PICARX_H
