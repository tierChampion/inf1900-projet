#include "master_navigation.h"
#include "led.h"
#include "pathfindingMode.h"
#include "corners_detector.h"
volatile uint16_t gCounter = 0;
volatile uint16_t gNavCounter = 0;
volatile uint8_t toggle = 0;

Led led;
CornersDetector detector;
ISR(TIMER1_COMPA_vect)
{
    detector.intRoutineTimer1();
    gCounter++;
    gNavCounter++;

    if (gCounter % 5 == 0)
    {
        if (toggle == 0)
        {
            led.setColor(LedColor::RED);
            toggle = 1;
        }
        else
        {
            led.setColor(LedColor::OFF);
            toggle = 0;
        }
        gCounter = 0;
    }
}

int main()
{ ////
    MasterNavigation nav = MasterNavigation();

    Timer1 eventTimer = Timer1();

    led = Led(Port::A, PA0, PA1);
    led.setColor(LedColor::OFF);

    eventTimer.setWaveMode(TimerWaveMode::CTC);
    eventTimer.setCounterValue(0);
    eventTimer.setCompareValue(TimerCompare::A, 195);
    eventTimer.setCompareMode(TimerCompare::A, TimerCompareMode::DISCONNECTED);
    eventTimer.setPrescalar(TimerPrescalar::THOUSAND_TWENTY_FOUR);
    eventTimer.setInterrupt(TimerInterrupt::COMPARE_A);

    nav.turn(Side::LEFT);
    LineSensor lineSensor;
    eventTimer.start();
    // bool running = true;
    detector.detectCorner(nav, lineSensor);
    // 195 => 25 ms
    // 42 => center on intersection
    // 60 ~ 90 degrees
    // 90
    /*
        while (running)
        {
            if (gNavCounter >= 90)
            {
                nav.stop();
                running = false;
                gNavCounter = 0;
            }
        }
    */
    // MovementCode moves[3];
    // moves[0] = MovementCode::FORWARD;
    // moves[1] = MovementCode::LEFT_FORWARD;

    // nav.executeMovementCodes(moves, 2);

    // PathfindingMode mode = PathfindingMode();

    // mode.run(1, 0);
}
