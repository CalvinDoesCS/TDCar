#include "pid.h"
#include <algorithm>

PID::PID(float Kp, float Ki, float Kd, float output_min, float output_max)
    : kp(Kp),
      ki(Ki),
      kd(Kd),
      setpoint(0.0f),
      integral(0.0f),
      prevError(0.0f),
      outputMin(output_min),
      outputMax(output_max),
      integralMin(output_min),
      integralMax(output_max)
{
}

void PID::setSetpoint(float target)
{
    setpoint = target;
}

float PID::getSetpoint() const
{
    return setpoint;
}

float PID::compute(float current, float dt)
{
    // Calculate error
    float error = setpoint - current;

    // Proportional term
    float p_term = kp * error;

    // Integral term with anti-windup
    integral += error * dt;
    integral = std::max(integralMin, std::min(integralMax, integral));
    float i_term = ki * integral;

    // Derivative term
    float derivative = (dt > 0.0f) ? (error - prevError) / dt : 0.0f;
    float d_term = kd * derivative;

    // Calculate total output
    float output = p_term + i_term + d_term;

    // Clamp output to limits
    output = std::max(outputMin, std::min(outputMax, output));

    // Store error for next iteration
    prevError = error;

    return output;
}

void PID::reset()
{
    integral = 0.0f;
    prevError = 0.0f;
}

void PID::setGains(float Kp, float Ki, float Kd)
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}

void PID::setOutputLimits(float min, float max)
{
    outputMin = min;
    outputMax = max;
}

void PID::setIntegralLimits(float min, float max)
{
    integralMin = min;
    integralMax = max;
}

float PID::getIntegral() const
{
    return integral;
}
