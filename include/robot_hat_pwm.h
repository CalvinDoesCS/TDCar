#ifndef ROBOT_HAT_PWM_H
#define ROBOT_HAT_PWM_H

/**
 * @file robot_hat_pwm.h
 * @brief PWM controller for SunFounder Robot HAT
 * 
 * Provides I2C-based PWM control for the Robot HAT board.
 * Communicates with the onboard MCU to generate hardware PWM signals.
 */

/// Robot HAT I2C Address
#define ROBOT_HAT_ADDR 0x14

/// PWM Channel register base address
#define REG_CHN 0x20

/// Prescaler register base address
#define REG_PSC 0x40

/// Auto-reload (period) register base address
#define REG_ARR 0x44

/// Robot HAT MCU clock frequency in Hz
#define CLOCK_FREQ 72000000.0

/**
 * @class RobotHAT_PWM
 * @brief PWM controller class for Robot HAT
 * 
 * Controls PWM channels (0-19) via I2C communication with the Robot HAT MCU.
 * Each PWM channel can have its own duty cycle, but channels sharing the same
 * timer (groups of 4) must have the same frequency.
 */
class RobotHAT_PWM {
private:
    int fd;            ///< I2C file descriptor
    int channel;       ///< PWM channel number (0-19)
    int timer_index;   ///< Hardware timer index (0-6)
    int freq;          ///< PWM frequency in Hz
    int arr;           ///< Auto-reload register value (period)
    
    /**
     * @brief Write a 16-bit value to an I2C register
     * @param reg Register address to write to
     * @param value 16-bit value to write
     */
    void i2c_write(int reg, int value);

public:
    /**
     * @brief Construct a new RobotHAT_PWM object
     * @param fd_handle I2C file descriptor from wiringPiI2CSetup()
     * @param pwm_channel PWM channel number (0-19, or P0-P19)
     * 
     * Initializes the PWM channel and sets default frequency to 50Hz.
     */
    RobotHAT_PWM(int fd_handle, int pwm_channel);
    
    /**
     * @brief Set PWM frequency
     * @param frequency Desired frequency in Hz (1-65535)
     * 
     * Calculates and sets the prescaler and period values.
     * Note: All channels on the same timer will share this frequency.
     */
    void set_freq(int frequency);
    
    /**
     * @brief Set PWM duty cycle as a percentage
     * @param percent Duty cycle percentage (0.0 - 100.0)
     * 
     * Sets the pulse width as a percentage of the period.
     * - 0% = always LOW (off)
     * - 50% = half duty cycle
     * - 100% = always HIGH (full power)
     */
    void pulse_width_percent(float percent);
};

#endif // ROBOT_HAT_PWM_H
