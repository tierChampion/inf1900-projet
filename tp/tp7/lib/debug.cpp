#include <debug.h>
#include <comm.h>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>


Comm comm;

const uint16_t MAX_BUFFER_SIZE = 7;

void display(uint8_t x)
{
    char str[80];
    sprintf(str, "Debug message (uint8_t): %u\n", x);
    Comm::transmitData((uint8_t *)str, strlen(str));
}


void display(uint16_t x)
{
    char str[80];
    sprintf(str, "Debug message (uint16_t): %u\n", x);
    Comm::transmitData((uint8_t *)str, strlen(str));
}


void display(float x)
{
    char buf[MAX_BUFFER_SIZE];
    dtostre(x, buf, 2, 'e');
    char str[80];
    sprintf(str, "Debug message (float): %s\n", buf);
    Comm::transmitData((uint8_t *)str, strlen(str));
}

void display(char *x)
{
    char str[80];
    sprintf(str, "Debug message (char): %s\n", x);
    Comm::transmitData((uint8_t *)str, strlen(str));
}

