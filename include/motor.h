#ifndef MOTOR_H
#define MOTOR_H

/**
 * @file motor.h
 * @brief DC Motor control class for PiCar-X
 * 
 * Provides high-level control of DC motors using the Robot HAT PWM controller.
 * Supports forward/backward motion with speed control and calibration.
 */

#include "robot_hat_pwm.h"

/**
 * @class Motor
 * @brief DC Motor controller with direction and speed control
 * 
 * Controls a single DC motor using GPIO for direction and PWM for speed.
 * Supports speed range from -100 (full backward) to +100 (full forward).
 */
class Motor {
private:
    int dirPin;              ///< GPIO pin for direction control
    RobotHAT_PWM* pwm;       ///< Pointer to PWM controller for speed
    int calibration;         ///< Speed calibration offset value
    int currentSpeed;        ///< Current speed value (-100 to 100)
    bool reversed;           ///< If true, reverse the direction logic

public:
    /**
     * @brief Construct a new Motor object
     * @param direction_pin GPIO pin number for direction control
     * @param pwm_controller Pointer to initialized RobotHAT_PWM object
     * @param reverse_direction If true, reverses motor direction (default: false)
     * 
     * Initializes the motor with the given direction pin and PWM controller.
     * The direction pin is automatically configured as OUTPUT.
     * Use reverse_direction=true if motor is physically wired backwards.
     */
    Motor(int direction_pin, RobotHAT_PWM* pwm_controller, bool reverse_direction = false);
    
    /**
     * @brief Get the current motor speed
     * @return int Current speed value (-100 to 100)
     */
    int getSpeed();

    /**
     * @brief Set motor speed and direction
     * @param speed Speed value from -100 to 100
     *              - Negative values: backward motion
     *              - Positive values: forward motion
     *              - Zero: motor stopped
     * 
     * The speed is automatically clamped to valid range and calibration is applied.
     */
    void setSpeed(int speed);
    
    /**
     * @brief Move motor forward
     * @param speed Forward speed (0-100)
     * 
     * Convenience method for forward motion. Automatically clamps to 0-100 range.
     */
    void forward(int speed);
    
    /**
     * @brief Move motor backward
     * @param speed Backward speed (0-100)
     * 
     * Convenience method for backward motion. Automatically clamps to 0-100 range.
     */
    void backward(int speed);
    
    /**
     * @brief Stop the motor
     * 
     * Sets motor speed to zero, stopping all motion.
     */
    void stop();
    
    /**
     * @brief Set speed calibration offset
     * @param cal Calibration value to add to speed commands
     * 
     * Used to compensate for motor differences. Positive values increase speed,
     * negative values decrease speed. Applied automatically in setSpeed().
     */
    void setCalibration(int cal);
    
    /**
     * @brief Set motor direction reversal
     * @param reverse If true, reverses motor direction logic
     * 
     * Use this if the motor is physically wired backwards.
     * Can be changed at runtime if needed.
     */
    void setReversed(bool reverse);
    
    /**
     * @brief Check if motor direction is reversed
     * @return bool True if motor is reversed
     */
    bool isReversed();
};

#endif // MOTOR_H
