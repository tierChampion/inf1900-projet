#include <debug.h>
#include <comm.h>


Comm comm();

void display(uint8_t x) {
    char str[80];
    sprintf(str, "Debug message: %u\n", x);
    Comm::transmitData((uint8_t*)str, strlen(str));
}


void display(uint16_t x) {
    char str[80];
    sprintf(str, "Debug message: %u\n", x);
    Comm::transmitData((uint8_t*)str, strlen(str));
}

void display(float x) {
    char str[80];
    sprintf(str, "Debug message: %f\n", x);
    Comm::transmitData((uint8_t*)str, strlen(str));    
}

void display(char* x) {
    char str[80];
    sprintf(str, "Debug message: %s\n", x);
    Comm::transmitData((uint8_t*)str, strlen(str)); 
}