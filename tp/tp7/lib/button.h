#include <avr/io.h>
typedef volatile uint8_t *Register

enum Edges
{
    FALLING_EDGE,
    RISING_EDGE,
    ANY_EDGE
};

enum Interruption
{
    INT0,
    INT1,
    INT2
};

struct Pin
{
    Pin(Register mode_, Register port_, Register pin_, uint8_t position);
    Register mode;
    Register port;
    Register pin;
    uint8_t position;
};

class Button
{
public:
Button(Pin pin);
 
bool isButtonPressed();
void setOnInterrupt();
void setOffInterrupt();

private:
struct Pin pin_;
};
