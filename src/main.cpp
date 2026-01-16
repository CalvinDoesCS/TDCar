
#include "picarx.h"
#include "motor.h"
#include "servo.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <keyboard_control.h>
#include <ultrasonic.h>
#include <pid.h>
#include <chrono>
#include <stdlib.h>

int main()
{
    printf("Initializing PiCar-X...\n");

    // Initialize WiringPi (MUST be called before any GPIO operations)
    if (wiringPiSetupGpio() == -1)
    {
        printf("ERROR: Failed to initialize wiringPi!\n");
        return 1;
    }
    printf("WiringPi initialized.\n");

    // Initialize I2C for Robot HAT
    int fd = wiringPiI2CSetup(ROBOT_HAT_ADDR);
    if (fd < 0)
    {
        printf("ERROR: Failed to initialize I2C!\n");
        return 1;
    }

    printf("I2C initialized.\n");

    // Create PWM controllers
    // Motor LeftDrive (left): GPIO23 (dir), P13 (speed)
    // Motor RightDrive (right): GPIO24 (dir), P12 (speed)
    // Servo steering: P2 (PWM channel 2)
    RobotHAT_PWM motorDLeft_pwm(fd, 13);
    RobotHAT_PWM motorDRight_pwm(fd, 12);
    RobotHAT_PWM steering_pwm(fd, 2);

    // Create Motor objects
    Motor motorDLeft(23, &motorDLeft_pwm);
    Motor motorDRight(24, &motorDRight_pwm, true);

    // Create Servo object for steering
    Servo steeringServo(&steering_pwm);

    steeringServo.setAngleOffset(-2);

    steeringServo.center();
    // Ultrasonic sensor with longer timeout for more reliable readings
    Ultrasonic ultrasonicSensor(27, 22, 0.001f); // Trigger: D2 (GPIO27), Echo: D3 (GPIO22), timeout: 0.5s

    // Set a reasonable timeout (allows measuring up to ~85m, more than enough)
    ultrasonicSensor.setTimeout(0.3f);

    // Create PiCarX robot
    PiCarX picar(&motorDLeft, &motorDRight, &steeringServo);

    printf("PiCar-X initialized!\n\n");

    // Create PID controller for distance following
    // Tuned for accurate positioning at 30cm
    float Kp = 2.5f; // Proportional: stronger response to error
    float Ki = 0.01f; // Integral: 
    float Kd = 1.5f; // Derivative: dampens oscillations
    PID distancePID(Kp, Ki, Kd, -100.0f, 100.0f);
    distancePID.setSetpoint(30.0f);               // Target distance: 30 cm from obstacle
    distancePID.setIntegralLimits(-50.0f, 50.0f); // Allow more integral windup

    printf("Starting distance following with PID control...\n");
    printf("Target distance: 30 cm\n");
    printf("Press Ctrl+C to stop\n\n");

    auto lastTime = std::chrono::high_resolution_clock::now();

    // Moving average filter for ultrasonic readings
    const int filterSize = 5;
    float distanceBuffer[filterSize] = {0};
    int bufferIndex = 0;
    bool bufferFilled = false;
    while (true)
    {
        // Get current time for dt calculation
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Measure distance to obstacle (get average of 3 readings)
        float rawDistance = ultrasonicSensor.getAverageDistance(8);

        // Add to moving average buffer
        distanceBuffer[bufferIndex] = rawDistance;
        bufferIndex = (bufferIndex + 1) % filterSize;
        if (bufferIndex == 0)
            bufferFilled = true;

        // Calculate filtered distance (moving average)
        float distance = 0;
        int count = bufferFilled ? filterSize : bufferIndex;
        for (int i = 0; i < count; i++)
        {
            distance += distanceBuffer[i];
        }
        distance /= count;

        // Compute PID output based on distance error
        // PID computes: setpoint - current = 30 - distance
        // If distance > 30: error is negative -> speed negative -> move forward
        // If distance < 30: error is positive -> speed positive -> move backward
        float speed = -distancePID.compute(distance, dt); // Negate for correct direction

        float error = distancePID.getSetpoint() - distance;

        // Display status
        printf("Distance: %.2f cm | Error: %.2f | Speed: %.1f | I: %.2f\n",
               distance, error, speed, distancePID.getIntegral());

        if (abs(error) < 0.5)
        {
            picar.stop();
            continue;
        }
        // Reduced dead zone for better precision
        if (speed > 2.0f)
        {
            // Move forward (toward obstacle)
            int motorSpeed = (int)speed;
            if (motorSpeed < 20)
                motorSpeed = 20; // Minimum to overcome friction
            picar.forward(motorSpeed);
        }
        else if (speed < -2.0f)
        {
            // Move backward (away from obstacle)
            int motorSpeed = (int)(-speed);
            if (motorSpeed < 20)
                motorSpeed = 20; // Minimum to overcome friction
            picar.backward(motorSpeed);
        }
        else
        {
            // Within dead zone - stop
            picar.stop();
        }

        delay(25); // 50ms loop time (20Hz)
    }

    // Cleanup
    picar.stop();
    endwin(); // End ncurses mode

    printf("\nPiCar-X control ended. Goodbye!\n");

    return 0;
}