#include "navigation.h"

Navigation::Navigation(WritePin dirLeftPin, WritePin dirRightPin)
{
    _timerPWM = Timer0();
    _timerPWM.setCounterValue(0);
    _timerPWM.setPrescalar(TimerPrescalar::NO_PRESCALAR);
    _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::CLEAR);
    _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::CLEAR);
    _timerPWM.setWaveMode(TimerWaveMode::PWM_PHASE_CORRECT);
    _leftWheel = Wheel(dirLeftPin, Side::LEFT, &_timerPWM);
    _rightWheel = Wheel(dirRightPin, Side::RIGHT, &_timerPWM);
    _timerPWM.start();
}

void Navigation::turn(Direction direction, float speed)
{
    switch (direction)
    {
    case Direction::LEFT:
        _leftWheel.setSpeed(Direction::FORWARD, 0);
        _rightWheel.setSpeed(Direction::FORWARD, speed);
        break;
    case Direction::RIGHT:
        _leftWheel.setSpeed(Direction::FORWARD, speed);
        _rightWheel.setSpeed(Direction::FORWARD, 0);
        break;
    default:
        break;
    }
}

void Navigation::controledTurn(Side turn, Orientation orientation, float speed, uint8_t turnStrength)
{
    // Set roue princiaple
    if (turn == Side::LEFT) {
       if (orientation == Orientation::FORWARD) {
            _rightWheel.setSpeed(Direction::FORWARD, speed);
       }
       else {
            _rightWheel.setSpeed(Direction::BACKWARD, speed);
       } 
    }
    else {
       if (orientation == Orientation::FORWARD) {
            _leftWheel.setSpeed(Direction::FORWARD, speed);
       }
       else {
            _leftWheel.setSpeed(Direction::BACKWARD, speed);
       }
    }

    // Set seconde roue
    float otherSpeed = speed * ((float)turnStrength / 255.f);

    if (turn == Side::LEFT) {
       if (orientation == Orientation::FORWARD) {
            _leftWheel.setSpeed(Direction::FORWARD, otherSpeed);
       }
       else {
            _leftWheel.setSpeed(Direction::BACKWARD, otherSpeed);
       } 
    }
    else {
       if (orientation == Orientation::FORWARD) {
            _rightWheel.setSpeed(Direction::FORWARD, otherSpeed);
       }
       else {
            _rightWheel.setSpeed(Direction::BACKWARD, otherSpeed);
       }
    }
}

void Navigation::move(Direction direction, float speed)
{
    //_timerPWM.setCompareValue(TimerCompare::A, 128);
    //_timerPWM.setCompareValue(TimerCompare::B, 128);
    _leftWheel.setSpeed(direction, speed);
    _rightWheel.setSpeed(direction, speed);
}

void Navigation::stop()
{
    move(Direction::FORWARD, 0);
}
