#include "navigation.h"

const uint8_t DEFAULT_SPEED_PERCENTAGE = 80;
const uint16_t TURN_DELAY = 650;

Navigation::Navigation()
{
    _timerPWM = Timer0();
    _timerPWM.setCounterValue(0);
    _timerPWM.setPrescalar(TimerPrescalar::NO_PRESCALAR);
    _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::CLEAR);
    _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::CLEAR);
    _timerPWM.setWaveMode(TimerWaveMode::PWM_PHASE_CORRECT);

    _leftEnablePin = WritePin(Port::B, PB3);
    _rightEnablePin = WritePin(Port::B, PB4);
    WritePin pinDirL = WritePin(Port::B, PB5);
    WritePin pinDirR = WritePin(Port::B, PB2);

    _leftWheel = Wheel(pinDirL, Side::LEFT, &_timerPWM);
    _rightWheel = Wheel(pinDirR, Side::RIGHT, &_timerPWM);
    _timerPWM.start();
    _speed = 0;
}

Navigation::~Navigation()
{
    _timerPWM.stop();
    _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::DISCONNECTED);
    _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::DISCONNECTED);
}

void Navigation::moveStraight(Orientation orientation, uint8_t speed)
{
    _speed = speed;
    _leftWheel.setSpeed(orientation, speed);
    _rightWheel.setSpeed(orientation, speed);
}

void Navigation::moveStraight(Orientation orientation)
{
    _speed = DEFAULT_SPEED_PERCENTAGE;
    _leftWheel.setSpeed(orientation, _speed);
    _rightWheel.setSpeed(orientation, _speed);
}

void Navigation::realForward()
{
    _speed = DEFAULT_SPEED_PERCENTAGE;
    _leftWheel.setSpeed(Orientation::FORWARD, _speed + 5);
    _rightWheel.setSpeed(Orientation::FORWARD, _speed);
}

void Navigation::stop()
{
    moveStraight(Orientation::FORWARD, 0);
}

void Navigation::pivot90(Side turn)
{

    if (turn == Side::LEFT)
    {
        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED_PERCENTAGE);
        _leftWheel.setSpeed(Orientation::BACKWARD, DEFAULT_SPEED_PERCENTAGE);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED_PERCENTAGE);
        _rightWheel.setSpeed(Orientation::BACKWARD, DEFAULT_SPEED_PERCENTAGE);
    }

    _delay_ms(TURN_DELAY);
    stop();
}

void Navigation::adjustWheel(Side turn, uint8_t intensity)
{
    if (turn == Side::LEFT)
    {
        _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED_PERCENTAGE - intensity);

        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED_PERCENTAGE);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED_PERCENTAGE + 5);

        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED_PERCENTAGE - 5 - intensity);
    }
}
