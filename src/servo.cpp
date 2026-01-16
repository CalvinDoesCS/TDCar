#include "servo.h"
#include <algorithm>

/**
 * @brief Construct a new Servo object
 */
Servo::Servo(RobotHAT_PWM *pwm_controller, int min_angle, int max_angle)
    : pwm(pwm_controller),
      currentAngle(0),
      minAngle(min_angle),
      maxAngle(max_angle),
      minPulseWidth(0.5),
      maxPulseWidth(2.5),
      angleOffset(0)
{

    // Set PWM frequency to 50Hz (standard for servos)
    pwm->set_freq(50);

    // Initialize to center position
    center();
}

/**
 * @brief Convert angle to PWM percentage
 */
float Servo::angleToPWM(int angle)
{
    // Clamp angle to valid range
    angle = std::max(minAngle, std::min(maxAngle, angle));

    // Map angle to pulse width (0.5ms to 2.5ms for standard servo)
    // For 50Hz PWM, period is 20ms
    // 0.5ms = 2.5% duty cycle, 2.5ms = 12.5% duty cycle

    // Map angle range to pulse width range
    float angleRange = maxAngle - minAngle;
    float pulseRange = maxPulseWidth - minPulseWidth;
    float pulseWidth = minPulseWidth + ((angle - minAngle) / angleRange) * pulseRange;

    // Convert pulse width (ms) to percentage of 20ms period
    float percentage = (pulseWidth / 20.0) * 100.0;

    return percentage;
}

/**
 * @brief Set servo angle
 */
void Servo::setAngle(int angle)
{
    // Store requested angle
    currentAngle = angle;

    // Apply offset and clamp to valid range
    int adjustedAngle = angle + angleOffset;
    adjustedAngle = std::max(minAngle, std::min(maxAngle, adjustedAngle));

    // Convert angle to PWM and apply
    float pwmPercent = angleToPWM(adjustedAngle);
    pwm->pulse_width_percent(pwmPercent);
}

/**
 * @brief Get current servo angle
 */
int Servo::getAngle()
{
    return currentAngle;
}

/**
 * @brief Set servo to center position
 */
void Servo::center()
{
    setAngle(0);
}

/**
 * @brief Set custom pulse width range
 */
void Servo::setPulseWidthRange(float min_pulse, float max_pulse)
{
    minPulseWidth = min_pulse;
    maxPulseWidth = max_pulse;

    // Reapply current angle with new pulse width settings
    setAngle(currentAngle);
}

/**
 * @brief Set angle offset for calibration
 */
void Servo::setAngleOffset(int offset)
{
    angleOffset = offset;

    // Reapply current angle with new offset
    setAngle(currentAngle);
}
