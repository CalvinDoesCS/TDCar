#ifndef PID_H
#define PID_H

/**
 * @file pid.h
 * @brief PID Controller implementation
 *
 * Provides a generic PID (Proportional-Integral-Derivative) controller
 * for smooth motor control and precise positioning.
 */

/**
 * @class PID
 * @brief PID Controller for precise control systems
 *
 * Implements a standard PID control algorithm with configurable gains
 * and output limits. Useful for motor speed control, position tracking,
 * and maintaining stable heading.
 */
class PID
{
private:
    float kp;          ///< Proportional gain
    float ki;          ///< Integral gain
    float kd;          ///< Derivative gain
    float setpoint;    ///< Target value
    float integral;    ///< Accumulated integral term
    float prevError;   ///< Previous error for derivative calculation
    float outputMin;   ///< Minimum output limit
    float outputMax;   ///< Maximum output limit
    float integralMin; ///< Minimum integral limit (anti-windup)
    float integralMax; ///< Maximum integral limit (anti-windup)

public:
    /**
     * @brief Construct a new PID controller
     * @param Kp Proportional gain
     * @param Ki Integral gain
     * @param Kd Derivative gain
     * @param output_min Minimum output value (default: -100)
     * @param output_max Maximum output value (default: 100)
     */
    PID(float Kp, float Ki, float Kd, float output_min = -100.0f, float output_max = 100.0f);

    /**
     * @brief Set the target setpoint
     * @param target Desired target value
     */
    void setSetpoint(float target);

    /**
     * @brief Get current setpoint
     * @return float Current target value
     */
    float getSetpoint() const;

    /**
     * @brief Compute PID output based on current value
     * @param current Current measured value
     * @param dt Time step since last update (seconds)
     * @return float Control output value
     *
     * Calculates PID output using:
     * output = Kp*error + Ki*integral + Kd*derivative
     */
    float compute(float current, float dt);

    /**
     * @brief Reset integral and previous error
     *
     * Call this when starting control or changing setpoint
     * to prevent integral windup.
     */
    void reset();

    /**
     * @brief Set PID gains
     * @param Kp Proportional gain
     * @param Ki Integral gain
     * @param Kd Derivative gain
     */
    void setGains(float Kp, float Ki, float Kd);

    /**
     * @brief Set output limits
     * @param min Minimum output value
     * @param max Maximum output value
     */
    void setOutputLimits(float min, float max);

    /**
     * @brief Set integral limits (anti-windup)
     * @param min Minimum integral value
     * @param max Maximum integral value
     */
    void setIntegralLimits(float min, float max);

    /**
     * @brief Get current integral term
     * @return float Accumulated integral value
     */
    float getIntegral() const;
};

#endif // PID_H
