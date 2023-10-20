#include <avr/io.h>

class Comm {
    public:
        Comm();

        void transmissionUART(uint8_t donnee);
};