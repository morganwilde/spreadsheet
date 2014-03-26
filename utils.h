#include <math.h>
#include <unistd.h>
#include <termios.h> // Unix API for terminal I/O
#include <sys/ioctl.h>
#include <limits.h>

// Utility functions

unsigned int absu(int value);
unsigned int maxu(unsigned int a, unsigned int b);
int digitCount(unsigned int value);
char getChar();

// Environment functions

typedef struct Env {
    int     w,  // Will hold the width of the terminal window and the
            h;  // Height
} Env;

Env     envGet();
