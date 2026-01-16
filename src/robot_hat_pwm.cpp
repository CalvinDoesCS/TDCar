#include "robot_hat_pwm.h"
#include <wiringPiI2C.h>
#include <unistd.h>
#include <stdio.h>
#include <cmath>

void RobotHAT_PWM::i2c_write(int reg, int value) {
    int value_h = (value >> 8) & 0xFF;
    int value_l = value & 0xFF;
    
    // Write 3 bytes: register, value_high, value_low
    unsigned char buf[3] = {(unsigned char)reg, (unsigned char)value_h, (unsigned char)value_l};
    write(fd, buf, 3);
}

RobotHAT_PWM::RobotHAT_PWM(int fd_handle, int pwm_channel) {
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

void RobotHAT_PWM::set_freq(int frequency) {
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

void RobotHAT_PWM::pulse_width_percent(float percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    
    int pulse_width = (int)((percent / 100.0) * arr);
    
    // Write to channel register
    i2c_write(REG_CHN + channel, pulse_width);
}
