#include <avr/io.h>

typedef volatile uint8_t *Register;

class Led {
public:
    Led(Register port, Register mode, uint8_t pinGreen, uint8_t pinRed);

    void setOff();
    void setGreen();
    void setRed();

private:
    volatile uint8_t *_port;
    volatile uint8_t *_mode;
    uint8_t _pinGreen;
    uint8_t _pinRed;
};
