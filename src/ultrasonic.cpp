#include "ultrasonic.h"
#include <wiringPi.h>
#include <chrono>

Ultrasonic::Ultrasonic(int trig_pin, int echo_pin, float max_timeout)
    : trigPin(trig_pin),
      echoPin(echo_pin),
      timeout(max_timeout * 1000000.0f) // Convert to microseconds
{
    // Setup pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Initialize trigger pin to LOW
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
}

bool Ultrasonic::waitForEcho(int state, long timeout_us)
{
    auto start = std::chrono::high_resolution_clock::now();

    while (digitalRead(echoPin) != state)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();

        if (elapsed > timeout_us)
        {
            return false; // Timeout
        }
    }

    return true;
}

float Ultrasonic::getDistance()
{
    // Ensure trigger is LOW
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Send 10us pulse to trigger
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Wait for echo to go HIGH (start of pulse)
    if (!waitForEcho(HIGH, timeout))
    {
        return -1.0f; // Timeout - no echo received
    }

    // Record start time
    auto pulseStart = std::chrono::high_resolution_clock::now();

    // Wait for echo to go LOW (end of pulse)
    if (!waitForEcho(LOW, timeout))
    {
        return -1.0f; // Timeout - echo stuck HIGH
    }

    // Record end time
    auto pulseEnd = std::chrono::high_resolution_clock::now();

    // Calculate pulse duration in microseconds
    auto pulseDuration = std::chrono::duration_cast<std::chrono::microseconds>(pulseEnd - pulseStart).count();

    // Calculate distance in centimeters
    // Speed of sound = 343 m/s = 0.0343 cm/us
    // Distance = (time * speed) / 2 (divide by 2 for round trip)
    // Distance = (pulseDuration * 0.0343) / 2
    // Distance = pulseDuration * 0.01715
    float distance = pulseDuration * 0.01715f;

    return distance;
}

float Ultrasonic::getDistanceInches()
{
    float cm = getDistance();
    if (cm < 0)
        return -1.0f;

    // Convert cm to inches (1 inch = 2.54 cm)
    return cm / 2.54f;
}

void Ultrasonic::setTimeout(float timeout_sec)
{
    timeout = timeout_sec * 1000000.0f; // Convert to microseconds
}

float Ultrasonic::getAverageDistance(int samples)
{
    float total = 0.0f;
    int validSamples = 0;

    for (int i = 0; i < samples; i++)
    {
        float distance = getDistance();

        if (distance >= 0)
        {
            total += distance;
            validSamples++;
        }

        // Small delay between samples
        delayMicroseconds(10000); // 10ms
    }

    if (validSamples == 0)
        return -1.0f; // All measurements failed

    return total / validSamples;
}
