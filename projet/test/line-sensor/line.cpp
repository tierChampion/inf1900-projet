#include "pins.h"
#include <led.h>
#include <navigation.h>
#include <avr/interrupt.h>
#include "line_sensor.h"
#include <button.h>

int main()
{
    Navigation n = Navigation();
    LineSensor lineSensor = LineSensor();
    Led led = Led(Port::A, PA6, PA7);
    bool adjustRight = false;

    n.moveStraight(Orientation::FORWARD, 80);
    _delay_ms(100);

    n.moveStraight(Orientation::FORWARD, 80);

    bool running = true;
    while (running)
    {
        lineSensor.updateDetection();
        switch (lineSensor.readSensorArray())
        {
        case 0b00010:
        case 0b00110:
        case 0b00011:
             n.adjustWheel(Side::LEFT, 10);
             break;
        case 0b01100:
        case 0b11000:
        case 0b01000:
            n.adjustWheel(Side::RIGHT, 30);
            break;
        case 0b00100:
            n.moveStraight(Orientation::FORWARD);
            break;
        case 0b00000:
                 n.stop();
                 running = false;
             break;
        default:
            break;
        }

        if (lineSensor.getStructure() != LineStructure::FORWARD) {
            PRINT(static_cast<uint8_t>(lineSensor.getStructure()));
            n.realForward();
            _delay_ms(1000);
            n.stop();
            running = false;
        }
    }
}
