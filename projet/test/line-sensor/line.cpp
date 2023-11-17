#include "pins.h"
#include <led.h>
#include <navigation.h>
#include <avr/interrupt.h>
#include "line_sensor.h"
#include <button.h>

/*
enum class SysState
{
    INIT,
    DETECTING,
    PATHFINDING,
    MOVING,
    TURNING
};
Button selection = Button(GeneralInterruptType::INT_0, false);
Button validation = Button(GeneralInterruptType::INT_1, false);
LineSensor sensor = LineSensor();
*/
Led led(Port::A, PA6, PA7);
Navigation navigation = Navigation();
ISR(PCINT0_vect)
{
    led.setColor(LedColor::RED);

    switch (sensor.readSensorArray())
    {
    case 0b00001:
    case 0b00011:
        navigation.adjustWheel(Side::LEFT);
        break;
    case 0b11000:
    case 0b10000:
        navigation.adjustWheel(Side::RIGHT);
    case 0b00100:
        navigation.moveStraight(Orientation::FORWARD, 45);
        led.setColor(LedColor::OFF);

    default:
        break;
    }
    PCIFR |= (1 << PCIF0)
}
ISR(INT0_vect)
{
}
ISR(INT1_vect)
{
}

void initInterrupt()
{
    selection.setSenseControl(SenseControl::FALLING_EDGE);
    validation.setSenseControl(SenseControl::FALLING_EDGE);
    selection.enable();
    validation.enable();
    cli();
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT2) | (1 << PCINT6) | (1 << PCINT4);
    sei();
}
void disableInterrupt()
{
    cli();
    PCICR &= ~(1 << PCIE0);
    sei();
}
void enableInterrupt()
{
    cli();
    PCICR |= (1 << PCIE0);
    sei();
}
int main()
{
    initInterrupt();
    while (true)
    {

        navigation.moveStraight(Orientation::FORWARD, 45);
        sensor.updateDetection();
    }
    /* ****  A WAY OF FINDING THE COIN  ****
    uint8_t DETECTOR = 0;
    uint8_t j = 0;
    bool breakLoop = false;

    for (j; j <= 6; i += 3)
    {
        navigation.forward();
        cli();
        sensor.updateDetection();
        _delay_ms(50);
        sensor.updateDetection();
        switch (sensor.getStructure())
        {
        case LineStruture::FORWARD:
            DETECTOR |= 0b111 << j;
            break;
        case LineStruture::NONE:
            DETECTOR |= 0b001 << j;
            breakLoop = true;
            break;
        case LineStruture::RIGHT:
            DETECTOR |= 0b010 << j;
            breakLoop = true;
            break;
        case LineStruture::RIGHT_FORWARD:
            DETECTOR |= 0b100 << j;
            break;
        case LineStruture::LEFT:
            DETECTOR |= 0b011 << j;
            breakLoop = true;
            break;
        case LineStruture::LEFT_FORWARD:
            DETECTOR |= 0b101 << j;
            break;
        default:
            break;
        }
        sei();
        if (breakLoop)
        {
            break;
        }
    }
    cli();
    navigation.pivot90(right);
    navigation.pivot90(right);
    sei();
    for (j; j >= 0; j -= 3)
    {
        navigation.backward();
    }
    cli();
    navigation.pivot90(right);
    navigation.pivot90(right);
    sei();
    switch (DETECTOR) // 3 derniers bits pour encodage de la 1er detection
    {                 // 3 bits du milieu pour la 2eme et les 2 premiers pour la 3eme
    case 0b10100111:
        node = 1; //(1,1)
        orientation = EAST;
        break;
    case 0b00000011:
        node = 1;
        orientation = SOUTH;
        break;
    case 0b00000010:
        node = 22; //(1,4)
        orientation = NORTH;
        break;
    case 0b00011111:
        node = 22;
        orientation = EAST;
        break;
    case 0b10100100:
        node = 28; //(4,7)
        orientation = WEST;
        break;
    case 0b11101111:
        node = 28;
        orientation = NORTH;
        break;
    case 0b10111100:
        node = 7;
        orientation = SOUTH;
        break;
    case 0b00001011:
        node = 7;
        orientation = WEST;
        break;
    default:
        break;
    }
*/
    /* *****PATH GIVEN IN ARRAY*****  *
        uint8_t indexNextNode;
        uint8_t indexNode;
        uint8_t i = 0;
        for (i; array.lenght() > i; i++)
        {
            indexNode = array[i];
            indexNextNode = array[i + 1];
            if (indexNodeNext == indexNode - 1)
            {
                switch (orientation)
                {
                case WEST:
                    navigation.forward();
                    break;
                case EAST:
                    cli();
                    navigation.pivot90(right);
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case SOUTH:
                    cli();
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case NORTH:
                    cli();
                    navigation.pivot90(left);
                    sei();
                    navigation.forward();
                    break;
                default:
                    break;
                }
                orientation=WEST;
            }
            else if (indexNextNode == indexNode + 1)
            {
                switch (orientation)
                {
                case EAST:
                    navigation.forward();
                    break;
                case WEST:
                    cli();
                    navigation.pivot90(right);
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case NORT:
                    cli();
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case SOUTH:
                    cli();
                    navigation.pivot90(left);
                    sei();
                    navigation.forward();
                    break;
                default:
                    break;
                }
                orientation=EAST;

            }
            else if (indexNextNode == indexNode - 7)
            {
                switch (orientation)
                {
                case NORTH:
                    navigation.forward();
                    break;
                case SOUTH:
                    cli();
                    navigation.pivot90(right);
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case WEST:
                    cli();
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case EAST:
                    cli();
                    navigation.pivot90(left);
                    sei();
                    navigation.forward();
                    break;
                default:
                    break;
                }
                orientation=NORTH;

            }
            else if (indexNextNode == indexNode + 7)
            {
                switch (orientation)
                {
                case SOUTH:
                    navigation.forward();
                    break;
                case NORTH:
                    cli();
                    navigation.pivot90(right);
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case EAST:
                    cli();
                    navigation.pivot90(right);
                    sei();
                    navigation.forward();
                    break;
                case WEST:
                    cli();
                    navigation.pivot90(left);
                    sei();
                    navigation.forward();
                    break;
                default:
                    break;
                }
                orientation=SOUTH;

            }
        }
    */
}
