#ifndef UTILS_H
#define UTILS_H
#include "utils.h"
#endif

unsigned int absu(int value) {
    return (value < 0) ? -((unsigned int)value) : (unsigned int)value;
}

unsigned int maxu(unsigned int a, unsigned int b) {
    return (a > b) ? a : b;
}

int digitCount(unsigned int value) {
    return (int)(log10(value)) + 1;
}

char getChar() {
    // Disables the COOKED mode of the terminal
    char            key = 0;        // Stores keystrokes in ASCII
    struct termios  old = {0};      // Current terminal settings

    if (tcgetattr(0, &old) < 0)     // Read current attributes
        perror("tcsetattr()");

    // Update them

    // Local modes
    old.c_lflag &= ~ICANON;         // Enable canonical mode
    old.c_lflag &= ~ECHO;           // Echo input characters
    // Special characters
    old.c_cc[VMIN] = 1;             // Minimum number of characters for read
    old.c_cc[VTIME] = 0;            // Timeout for read

    // Store changes

    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");

    // Record a single key stroke
    if (read(0, &key, 1) < 0)
        perror("read()");

    // Revert back

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    
    return key;
}

// Interpret CML arguments
int interpretArguments(int argc, char **argv, int *limit) {
    // Argument interpreter
    // Error codes
    // 0 - OK
    // 1 - No argument detected
    // 2 - Non-number first argument/overflow negative
    // 3 - Negative number/overflow positive

    // Check if there are any arguments
    if (argc >= 2) {
        // This program accepts only the first parameter as a limit
        int number = atoi(argv[1]);
        // Check if this was actually a number
        if (number < 10 && number >= 0) {
            // Since this should be a one digit number
            // it should have values between 0-9.
            // Their ASCII representations range from 48-57
            if ((argv[1][0] >= 48) && (argv[1][0] <= 57)) {
                *limit = number;
                return 0;
            } else {
                // Not a number
                return 2;
            }
        } else if (number >= 10 && number <= INT_MAX) {
            // Multi-digit number - OK
            *limit = number;
            return 0;
        } else if (number < 0) {
            // Number is negative
            return 3;
        } else {
            // Non a number
            return 2;
        }
    }
    
    // No arguments
    return 1;
}

// Reads terminal window information
Env envGet() {
    // Get window meta data
    struct winsize terminal;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
    // Store it
    Env env;
    env.w = terminal.ws_col;
    env.h = terminal.ws_row;

    return env;
}
