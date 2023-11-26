#ifndef MOVEMENT_TIMER_H
#define MOVEMENT_TIMER_H

#include "timer1.h"
#include "led.h"

class EventTimer
{
public:
    static void initialiseEventTimer();
    static uint16_t getNavigationCounter();
    static void resetNavigationCounter();
    static void setToggling(bool newToggling);
    static void update();

private:
    EventTimer();
    static void toggleLed();

private:
    static Timer1 _timer;
    static Led _led;
    static uint8_t _ledCounter;
    static uint16_t _navCounter;
    static bool _isToggling;
    static bool _isLedToggled;

    static EventTimer _eventTimer;
    static bool _isInitialised;
};

#endif