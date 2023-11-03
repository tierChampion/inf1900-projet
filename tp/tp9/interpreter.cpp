#include <avr/io.h>
#include <util/delay.h>
#include <navigation.h>
#include <led.h>
Navigation navigation{};
Led led = Led(Port::A, PA0, PA1);
bool start = false;
void execute(uint16_t command)
{
    uint8_t instruction = (command >> 8);
    uint8_t operand = command;

    switch (instruction)
    {
    case 0x01:
        start = true;
        break;
    case 0xFF:
        start = false;
        break;
    default:
        break;
    }
    if (start)
        switch (instruction)
        {
        case 0x01: // commancer
            break;
        case 0x02: // attendre
            for (int i = 0; i < operand; i++)
            {
                _delay_ms(25);
            }
            break;
        case 0x44: // allumer del
            if (operand == 1)
                break;
        case 0x45:
            /* code */
            break;
        case 0x48:
            /* code */
            break;
        case 0x09:
            /* code */
            break;
        case 0x60:
            /* code */
            break;
        case 0x61:
            /* code */
            break;
        case 0x62:
            /* code */
            break;
        case 0x63:
            /* code */
            break;
        case 0x64:
            /* code */
            break;
        case 0x65:
            /* code */
            break;
        case 0xC0:
            /* code */
            break;
        case 0xC1:
            /* code */
            break;
        case 0xFF:
            /* code */
            break;
        default:
            break;
        }
}
int main()
{
}