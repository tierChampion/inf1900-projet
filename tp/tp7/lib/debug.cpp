#include <debug.h>
#include <comm.h>

void display(std::string x) {
    Comm comm = Comm();
    char str[80];
    sprintf(str, "Debug message: ", x);
    comm.transmitData((uint8_t*)str, strlen(str));
}