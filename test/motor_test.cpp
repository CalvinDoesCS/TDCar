#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

int main() {
    wiringPiSetupGpio(); // Use BCM numbering

    int motorDir = 24; // MOTOR 1 DIR
    int motorPwm = 18; // Example PWM pin (change if needed)

    // Setup pins
    pinMode(motorDir, OUTPUT);
    softPwmCreate(motorPwm, 0, 100); // PWM 0-100%

    // Forward
    digitalWrite(motorDir, HIGH);
    softPwmWrite(motorPwm, 50); // 50% speed
    sleep(2);

    // Stop
    softPwmWrite(motorPwm, 0);

    return 0;
}
