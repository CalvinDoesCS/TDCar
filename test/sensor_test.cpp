#include <wiringPi.h>
#include <iostream>
#include <chrono>

int main() {
    wiringPiSetupGpio(); // Use BCM numbering

    int trigPin = 27; // D2 → Trigger
    int echoPin = 22; // D3 → Echo

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(trigPin, LOW); // Make sure trigger is low
    delay(500);                 

    std::cout << "Starting ultrasonic sensor reading..." << std::endl;

    while (true) {
        // 1️⃣ Send 10µs pulse to trigger
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // 2️⃣ Wait for echo to go HIGH and measure duration
        auto startTime = std::chrono::high_resolution_clock::now();
        while (digitalRead(echoPin) == LOW); // Wait for HIGH
        auto pulseStart = std::chrono::high_resolution_clock::now();

        while (digitalRead(echoPin) == HIGH); // Wait for LOW
        auto pulseEnd = std::chrono::high_resolution_clock::now();

        // 3️⃣ Calculate pulse duration in microseconds
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(pulseEnd - pulseStart).count();

        // 4️⃣ Convert to distance in cm (speed of sound / 2)
        double distance = duration / 58.0;

        std::cout << "Distance: " << distance << " cm" << std::endl;

        delay(500); // Wait 0.5s before next reading
    }

    return 0;
}
