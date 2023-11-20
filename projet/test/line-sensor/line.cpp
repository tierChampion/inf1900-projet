#include "navigation.h"
#include <avr/interrupt.h>
#include "line_sensor.h"

const uint8_t LEFT_ADJUST = 10; // pour le robot 16, a 150 avec +5 a gauche
const uint8_t RIGHT_ADJUST = 30; // pour le robot 16, a 150 avec +5 a gauche
const uint8_t CENTERING_WAIT = 1000; // pour le robot 16 a 150 avec +5 a gauche

int main()
{
    Navigation n = Navigation();
    LineSensor lineSensor = LineSensor();

    n.jumpStart();

    n.moveStraight(Orientation::FORWARD);

    bool running = true;
    while (running)
    {
        lineSensor.updateDetection();

        if (lineSensor.getStructure() == LineStructure::FORWARD)
        {
            if (lineSensor.needLeftAdjustment())
            {
                n.adjustWheel(Side::LEFT, LEFT_ADJUST);
            }
            else if (lineSensor.needRightAdjustment())
            {
                n.adjustWheel(Side::RIGHT, RIGHT_ADJUST);
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
            _delay_ms(CENTERING_WAIT); // to verify and change if needed!
            n.stop();
            running = false;
        }
    }
}
