#ifndef SERVO_H
#define SERVO_H

/**
 * @file servo.h
 * @brief Servo motor control class for PiCar-X
 *
 * Provides angle-based control of servo motors using PWM signals.
 * Typically used for steering control.
 */

#include "robot_hat_pwm.h"

/**
 * @class Servo
 * @brief Servo motor controller with angle-based positioning
 *
 * Controls a servo motor using PWM signals. Converts angle values
 * to appropriate PWM duty cycles for precise positioning.
 */
class Servo
{
private:
    RobotHAT_PWM *pwm;   ///< Pointer to PWM controller
    int currentAngle;    ///< Current servo angle
    int minAngle;        ///< Minimum angle limit
    int maxAngle;        ///< Maximum angle limit
    float minPulseWidth; ///< Minimum pulse width (ms)
    float maxPulseWidth; ///< Maximum pulse width (ms)
    int angleOffset;     ///< Calibration offset for center position

    /**
     * @brief Convert angle to PWM percentage
     * @param angle Angle in degrees
     * @return PWM duty cycle percentage
     */
    float angleToPWM(int angle);

public:
    /**
     * @brief Construct a new Servo object
     * @param pwm_controller Pointer to initialized RobotHAT_PWM object
     * @param min_angle Minimum angle limit (default: -90)
     * @param max_angle Maximum angle limit (default: 90)
     *
     * Initializes the servo with default pulse widths for standard servos:
     * - Min pulse: 0.5ms (0 degrees)
     * - Max pulse: 2.5ms (180 degrees)
     */
    Servo(RobotHAT_PWM *pwm_controller, int min_angle = -90, int max_angle = 90);

    /**
     * @brief Set servo angle
     * @param angle Desired angle in degrees
     *
     * Sets the servo to the specified angle. Automatically clamps
     * to the min/max angle range set during construction.
     */
    void setAngle(int angle);

    /**
     * @brief Get current servo angle
     * @return int Current angle in degrees
     */
    int getAngle();

    /**
     * @brief Set servo to center position (0 degrees)
     */
    void center();

    /**
     * @brief Set custom pulse width range
     * @param min_pulse Minimum pulse width in milliseconds (default: 0.5)
     * @param max_pulse Maximum pulse width in milliseconds (default: 2.5)
     *
     * Allows calibration for different servo models with varying pulse width ranges.
     */
    void setPulseWidthRange(float min_pulse, float max_pulse);

    /**
     * @brief Set angle offset for calibration
     * @param offset Offset in degrees to add to all angle commands
     *
     * Use this to calibrate the servo's center position. For example,
     * if the servo drifts right at 0°, use a negative offset.
     */
    void setAngleOffset(int offset);
};

#endif // SERVO_H
