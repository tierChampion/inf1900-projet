#ifndef COMM_H
#define COMM_H

#include <avr/io.h>

class Comm {
    public:
        Comm();

        void transmissionUART(uint8_t donnee);
};

#endif