#include "keyboard_control.h"
#include <ncurses.h>
#include <unistd.h>
#include <sys/time.h>

void runKeyboardControl(PiCarX *picar)
{
    // Initialize ncurses
    initscr();             // Start ncurses mode
    cbreak();              // Disable line buffering
    noecho();              // Don't echo keypresses
    keypad(stdscr, TRUE);  // Enable arrow keys
    nodelay(stdscr, TRUE); // Non-blocking input
    curs_set(0);           // Hide cursor

    // Display instructions
    mvprintw(0, 0, "=== PiCar-X Keyboard Control ===");
    mvprintw(2, 0, "Hold keys to control");
    mvprintw(4, 0, "W/UP    - Forward");
    mvprintw(5, 0, "S/DOWN  - Backward");
    mvprintw(6, 0, "A/LEFT  - Steer Left");
    mvprintw(7, 0, "D/RIGHT - Steer Right");
    mvprintw(8, 0, "Q       - Rotate Left");
    mvprintw(9, 0, "E       - Rotate Right");
    mvprintw(10, 0, "C       - Center Steering");
    mvprintw(11, 0, "SPACE   - Stop");
    mvprintw(12, 0, "X       - Exit");
    mvprintw(13, 0, "+/-     - Adjust Speed");
    mvprintw(15, 0, "Speed: 85%% | Steering: 0 | Status: STOPPED");
    refresh();

    int ch;
    int speed = 85;
    int steeringAngle = 0;
    int currentAction = -1;

    struct timeval lastKeyTime;
    gettimeofday(&lastKeyTime, NULL);

    bool running = true;

    while (running)
    {
        ch = getch();

        // Get current time
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);

        long timeSinceLastKey = (currentTime.tv_sec - lastKeyTime.tv_sec) * 1000 +
                                (currentTime.tv_usec - lastKeyTime.tv_usec) / 1000;

        // If no key pressed for 100ms, stop the car
        if (ch == -1 && timeSinceLastKey > 100 && currentAction != -1)
        {
            picar->stop();
            mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: STOPPED          ", speed, steeringAngle);
            currentAction = -1;
        }

        // Process key press
        if (ch != -1)
        {
            gettimeofday(&lastKeyTime, NULL);

            switch (ch)
            {
            case 'w':
            case 'W':
            case KEY_UP:
                if (currentAction != 'w')
                {
                    picar->setSteeringAngle(steeringAngle);
                    picar->forward(speed);
                    mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: FORWARD          ", speed, steeringAngle);
                    currentAction = 'w';
                }
                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                if (currentAction != 's')
                {
                    picar->setSteeringAngle(steeringAngle);
                    picar->backward(speed);
                    mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: BACKWARD         ", speed, steeringAngle);
                    currentAction = 's';
                }
                break;

            case 'a':
            case 'A':
            case KEY_LEFT:
                steeringAngle = (steeringAngle > -45) ? steeringAngle - 5 : -45;
                picar->setSteeringAngle(steeringAngle);
                mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: STEER LEFT       ", speed, steeringAngle);
                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                steeringAngle = (steeringAngle < 45) ? steeringAngle + 5 : 45;
                picar->setSteeringAngle(steeringAngle);
                mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: STEER RIGHT      ", speed, steeringAngle);
                break;

            case 'q':
            case 'Q':
                if (currentAction != 'q')
                {
                    picar->setSteeringAngle(steeringAngle);
                    picar->rotateLeft(speed);
                    mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: ROTATE LEFT      ", speed, steeringAngle);
                    currentAction = 'q';
                }
                break;

            case 'e':
            case 'E':
                if (currentAction != 'e')
                {
                    picar->setSteeringAngle(steeringAngle);
                    picar->rotateRight(speed);
                    mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: ROTATE RIGHT     ", speed, steeringAngle);
                    currentAction = 'e';
                }
                break;

            case 'c':
            case 'C':
                steeringAngle = 0;
                picar->centerSteering();
                mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: CENTER STEERING   ", speed, steeringAngle);
                break;

            case ' ':
                picar->stop();
                mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: STOPPED           ", speed, steeringAngle);
                currentAction = -1;
                break;

            case '+':
            case '=':
                speed = (speed < 100) ? speed + 10 : 100;
                mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: SPEED UP          ", speed, steeringAngle);
                break;

            case '-':
            case '_':
                speed = (speed > 10) ? speed - 10 : 10;
                mvprintw(15, 0, "Speed: %d%% | Steering: %d | Status: SPEED DOWN        ", speed, steeringAngle);
                break;

            case 'x':
            case 'X':
                running = false;
                break;
            }
        }

        refresh();
        usleep(10000); // 10ms delay
    }

    // Cleanup
    picar->stop();
    endwin(); // End ncurses mode
}
