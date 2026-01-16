#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <stdio.h>
#include <cmath>

// Robot HAT I2C Address
#define ROBOT_HAT_ADDR 0x14

// PWM Registers (from robot_hat library)
#define REG_CHN 0x20   // Channel register
#define REG_PSC 0x40   // Prescaler register  
#define REG_ARR 0x44   // Period register
#define CLOCK_FREQ 72000000.0

class RobotHAT_PWM {
    private:
        int fd;
        int channel;
        int timer_index;
        int freq;
        int arr;  // Period value
        
        void i2c_write(int reg, int value) {
            int value_h = (value >> 8) & 0xFF;
            int value_l = value & 0xFF;
            
            // Write 3 bytes: register, value_high, value_low
            unsigned char buf[3] = {(unsigned char)reg, (unsigned char)value_h, (unsigned char)value_l};
            write(fd, buf, 3);
        }
    
    public:
        RobotHAT_PWM(int fd_handle, int pwm_channel) {
            fd = fd_handle;
            channel = pwm_channel;
            freq = 50;
            
            // Calculate timer index
            if (channel < 16) {
                timer_index = channel / 4;
            } else if (channel == 16 || channel == 17) {
                timer_index = 4;
            } else if (channel == 18) {
                timer_index = 5;
            } else {
                timer_index = 6;
            }
            
            set_freq(50);
        }
    
    void set_freq(int frequency) {
        freq = frequency;
        
        // Calculate prescaler and period (simplified version)
        int st = (int)sqrt(CLOCK_FREQ / freq);
        if (st <= 0) st = 1;
        
        int psc = st;
        arr = (int)(CLOCK_FREQ / freq / psc);
        
        // Write prescaler and period registers
        i2c_write(REG_PSC + timer_index, psc);
        i2c_write(REG_ARR + timer_index, arr);
        
        printf("PWM Channel %d: freq=%d, psc=%d, arr=%d\n", channel, freq, psc, arr);
    }
    
    void pulse_width_percent(float percent) {
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;
        
        int pulse_width = (int)((percent / 100.0) * arr);
        
        // Write to channel register
        i2c_write(REG_CHN + channel, pulse_width);
    }
};

void testMotor(const char* motorName, int dirPin, RobotHAT_PWM* pwm) {
    printf("\n========================================\n");
    printf("Testing %s\n", motorName);
    printf("========================================\n");
    
    // Setup direction pin
    pinMode(dirPin, OUTPUT);
    
    // Forward at 75% speed
    printf("Forward at 75%% speed...\n");
    digitalWrite(dirPin, LOW);  // Direction
    pwm->pulse_width_percent(75);
    sleep(3);
    
    // Stop
    printf("Stopping...\n");
    pwm->pulse_width_percent(0);
    sleep(1);
    
    // Backward at 75% speed
    printf("Backward at 75%% speed...\n");
    digitalWrite(dirPin, HIGH);  // Opposite direction
    pwm->pulse_width_percent(75);
    sleep(3);
    
    // Stop
    printf("Stopping...\n");
    pwm->pulse_width_percent(0);
    sleep(1);
    
    printf("%s test complete.\n", motorName);
}

int main() {
    printf("PiCar-X Motor Test (Robot HAT Protocol)\n");
    printf("========================================\n\n");
    
    // Initialize WiringPi for GPIO
    if (wiringPiSetupGpio() == -1) {
        printf("ERROR: Failed to initialize wiringPi!\n");
        return 1;
    }
    
    // Initialize I2C
    int fd = wiringPiI2CSetup(ROBOT_HAT_ADDR);
    if (fd < 0) {
        printf("ERROR: Could not initialize I2C!\n");
        return 1;
    }
    printf("I2C initialized at 0x%02X\n\n", ROBOT_HAT_ADDR);
    
    // Create PWM objects for motors
    // Motor 1 (left): D4=GPIO23 (dir), P13=PWM_channel_13 (speed)
    // Motor 2 (right): D5=GPIO24 (dir), P12=PWM_channel_12 (speed)
    RobotHAT_PWM motor1_pwm(fd, 13);  // P13 = channel 13
    RobotHAT_PWM motor2_pwm(fd, 12);  // P12 = channel 12
    
    char choice;
    printf("\nWhich motor to test?\n");
    printf("1 - Motor 1 (Left - GPIO23/P13)\n");
    printf("2 - Motor 2 (Right - GPIO24/P12)\n");
    printf("3 - Both motors\n");
    printf("Enter choice (1/2/3): ");
    scanf(" %c", &choice);
    
    if (choice == '1' || choice == '3') {
        testMotor("MOTOR 1 (LEFT)", 23, &motor1_pwm);
    }
    
    if (choice == '2' || choice == '3') {
        testMotor("MOTOR 2 (RIGHT)", 24, &motor2_pwm);
    }
    
    printf("\n========================================\n");
    printf("Test complete!\n");
    printf("========================================\n");
    
    return 0;
}
