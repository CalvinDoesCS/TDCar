#ifndef ULTRASONIC_H
#define ULTRASONIC_H

/**
 * @file ultrasonic.h
 * @brief Ultrasonic distance sensor (HC-SR04) control class
 *
 * Provides distance measurement using trigger and echo pins.
 * Measures distance by timing ultrasonic pulse reflections.
 */

/**
 * @class Ultrasonic
 * @brief Ultrasonic distance sensor controller
 *
 * Controls an HC-SR04 or similar ultrasonic sensor to measure
 * distances. Uses GPIO pins for trigger and echo signals.
 */
class Ultrasonic
{
private:
    int trigPin;   ///< GPIO pin for trigger signal
    int echoPin;   ///< GPIO pin for echo signal
    float timeout; ///< Maximum wait time for echo (microseconds)

    /**
     * @brief Wait for echo pin to reach specified state
     * @param state Desired pin state (HIGH or LOW)
     * @param timeout_us Timeout in microseconds
     * @return true if state reached, false if timeout
     */
    bool waitForEcho(int state, long timeout_us);

public:
    /**
     * @brief Construct a new Ultrasonic sensor object
     * @param trig_pin GPIO pin number for trigger
     * @param echo_pin GPIO pin number for echo
     * @param max_timeout Maximum measurement timeout in seconds (default: 0.1)
     *
     * Initializes GPIO pins and sets up the sensor for distance measurement.
     */
    Ultrasonic(int trig_pin, int echo_pin, float max_timeout = 0.1f);

    /**
     * @brief Measure distance to nearest object
     * @return float Distance in centimeters, or -1 if measurement failed
     *
     * Sends ultrasonic pulse and measures echo return time.
     * Distance = (time * speed_of_sound) / 2
     * Speed of sound = 343 m/s at 20°C
     */
    float getDistance();

    /**
     * @brief Measure distance in inches
     * @return float Distance in inches, or -1 if measurement failed
     */
    float getDistanceInches();

    /**
     * @brief Set maximum timeout for measurement
     * @param timeout_sec Timeout in seconds
     *
     * Limits how long to wait for echo response.
     * Longer timeout allows measuring greater distances.
     */
    void setTimeout(float timeout_sec);

    /**
     * @brief Get multiple readings and return average
     * @param samples Number of samples to average (default: 5)
     * @return float Average distance in centimeters
     *
     * Takes multiple measurements and returns the average,
     * filtering out failed readings (-1 values).
     */
    float getAverageDistance(int samples = 5);
};

#endif // ULTRASONIC_H
