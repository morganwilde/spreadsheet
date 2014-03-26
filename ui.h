#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

char   *borderHorizontal();
char   *borderVertical(char *message);
void    printDisplay(char *messages[], int count);
