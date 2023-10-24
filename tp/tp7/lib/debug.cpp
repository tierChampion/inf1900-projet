#include <debug.h>
#include <comm.h>

// Overload fuction for various types (uint8_t, uint16_t, float, char)

void display(std::string x) {
    Comm comm = Comm();
    char str[80];
    sprintf(str, "Debug message: ", x);
    comm.transmitData((uint8_t*)str, strlen(str));
}