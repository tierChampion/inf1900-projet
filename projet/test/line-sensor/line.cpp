#include "navigation.h"
#include <avr/interrupt.h>
#include "line_sensor.h"

int main()
{
    Navigation n = Navigation();
    LineSensor lineSensor = LineSensor();

    n.moveStraight(Orientation::FORWARD, 100);
    _delay_ms(100);

    n.moveStraight(Orientation::FORWARD);

    bool running = true;
    while (running)
    {
        lineSensor.updateDetection();

        if (lineSensor.getStructure() == LineStructure::FORWARD)
        {
            if (lineSensor.needLeftAdjustment())
            {
                n.adjustWheel(Side::LEFT, 10);
            }
            else if (lineSensor.needRightAdjustment())
            {
                n.adjustWheel(Side::RIGHT, 30);
            }
            else
            {
                n.moveStraight(Orientation::FORWARD);
            }
        }
        else if (lineSensor.detectsIntersection())
        {
            PRINT(static_cast<uint8_t>(lineSensor.getStructure()));
            n.realForward();
            _delay_ms(1000); // to verify and change if needed!
            n.stop();
            running = false;
        }
    }
}
