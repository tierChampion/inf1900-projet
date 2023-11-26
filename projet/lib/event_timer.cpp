#include "event_timer.h"

Timer1 EventTimer::_timer = Timer1();
Led EventTimer::_led = Led(Port::A, PA6, PA7);
uint8_t EventTimer::_ledCounter = 0;
uint16_t EventTimer::_navCounter = 0;
bool EventTimer::_isToggling = false;
bool EventTimer::_isLedToggled = true;

bool EventTimer::_isInitialised = false;

ISR(TIMER1_COMPA_vect)
{
    EventTimer::update();
}

EventTimer::EventTimer()
{
    _timer.setWaveMode(TimerWaveMode::CTC);
    _timer.setCounterValue(0);
    _timer.setCompareValue(TimerCompare::A, 195);
    _timer.setCompareMode(TimerCompare::A, TimerCompareMode::DISCONNECTED);
    _timer.setPrescalar(TimerPrescalar::THOUSAND_TWENTY_FOUR);
    _timer.setInterrupt(TimerInterrupt::COMPARE_A);

    _timer.start();
    _isInitialised = true;
}

void EventTimer::initialiseEventTimer()
{
    _eventTimer = EventTimer();
}

void EventTimer::update()
{
    EventTimer::_ledCounter++;
    EventTimer::_navCounter++;

    if (EventTimer::_isToggling)
        toggleLed();
}

void EventTimer::toggleLed()
{
    if (_ledCounter % 5 == 0)
    {
        if (!EventTimer::_isLedToggled)
        {
            _led.setColor(LedColor::GREEN);
        }
        else
        {
            _led.setColor(LedColor::OFF);
        }

        EventTimer::_isLedToggled = !EventTimer::_isLedToggled;
        _ledCounter = 0;
    }
}

void EventTimer::setToggling(bool newToggling)
{
    EventTimer::_isToggling = newToggling;

    if (!EventTimer::_isToggling)
    {
        _led.setColor(LedColor::OFF);
    }
}

uint16_t EventTimer::getNavigationCounter()
{
    return EventTimer::_navCounter;
}

void EventTimer::resetNavigationCounter()
{
    if (!EventTimer::_isInitialised)
        initialiseEventTimer();

    EventTimer::_navCounter = 0;
}
