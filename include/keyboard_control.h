#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H

/**
 * @file keyboard_control.h
 * @brief Keyboard control interface for PiCar-X
 *
 * Provides ncurses-based keyboard control with visual feedback.
 */

#include "picarx.h"

/**
 * @brief Run keyboard control loop for PiCar-X
 * @param picar Pointer to initialized PiCarX object
 *
 * Starts an interactive ncurses interface for controlling the robot
 * with keyboard input. Displays controls and current status on screen.
 *
 * Controls:
 * - W/UP: Forward
 * - S/DOWN: Backward
 * - A/LEFT: Turn Left
 * - D/RIGHT: Turn Right
 * - Q: Rotate Left
 * - E: Rotate Right
 * - SPACE: Stop
 * - +/-: Adjust Speed
 * - X: Exit
 */
void runKeyboardControl(PiCarX *picar);

#endif // KEYBOARD_CONTROL_H
