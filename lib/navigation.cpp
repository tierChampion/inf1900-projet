#include "navigation.h"

const uint8_t DEFAULT_SPEED = 110;     // 110 pour le robot 16.
const uint8_t DEFAULT_TURN_SPEED = 90; // 90 pour le robot 16.
const uint8_t LEFT_REAL_ADJUST = 0;    // 0 pour le robot 16
const uint8_t RIGHT_REAL_ADJUST = 5;   // 5 pour le robot 16
const uint8_t DEFAULT_ADJUST = 5;      // pour le robot 16

// constants to start the robot if it doesnt move
const uint8_t JUMP_START_DELAY = 50;
const uint8_t JUMP_START_TURN_DELAY = 100;

Navigation::Navigation() : _timerPWM(Timer0()),
                           _leftEnablePin(WritePin(Port::B, PB3)),
                           _rightEnablePin(WritePin(Port::B, PB4))
{
    _timerPWM.setCounterValue(0);
    _timerPWM.setPrescalar(TimerPrescalar::NO_PRESCALAR);
    _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::CLEAR);
    _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::CLEAR);
    _timerPWM.setWaveMode(TimerWaveMode::PWM_PHASE_CORRECT);

    WritePin pinDirL = WritePin(Port::B, PB5);
    WritePin pinDirR = WritePin(Port::B, PB6);
    _leftWheel = Wheel(pinDirL, Side::LEFT, &_timerPWM);
    _rightWheel = Wheel(pinDirR, Side::RIGHT, &_timerPWM);

    _timerPWM.start();
}

Navigation::~Navigation()
{
    _timerPWM.stop();
    _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::DISCONNECTED);
    _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::DISCONNECTED);
}

void Navigation::moveStraight(Orientation orientation, uint8_t speed)
{
    _leftWheel.setSpeed(orientation, speed);
    _rightWheel.setSpeed(orientation, speed);
}

void Navigation::moveStraight(Orientation orientation)
{
    _leftWheel.setSpeed(orientation, DEFAULT_SPEED + LEFT_REAL_ADJUST);
    _rightWheel.setSpeed(orientation, DEFAULT_SPEED + RIGHT_REAL_ADJUST);
}

void Navigation::realForward()
{
    _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED + LEFT_REAL_ADJUST); // to test (was 30)
    _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED + RIGHT_REAL_ADJUST);
}

void Navigation::stop()
{
    moveStraight(Orientation::FORWARD, 0);
}

void Navigation::pivot(Side turn)
{
    if (turn == Side::LEFT)
    {
        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_TURN_SPEED + RIGHT_REAL_ADJUST);
        _leftWheel.setSpeed(Orientation::BACKWARD, DEFAULT_TURN_SPEED + LEFT_REAL_ADJUST);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_TURN_SPEED + LEFT_REAL_ADJUST);
        _rightWheel.setSpeed(Orientation::BACKWARD, DEFAULT_TURN_SPEED + RIGHT_REAL_ADJUST);
    }
}

void Navigation::adjustWheel(Side turn, uint8_t intensity)
{
    if (turn == Side::LEFT)
    {
        _leftWheel.setSpeed(Orientation::FORWARD, (DEFAULT_SPEED + LEFT_REAL_ADJUST) - (intensity + DEFAULT_ADJUST));

        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED + RIGHT_REAL_ADJUST);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, (DEFAULT_SPEED + LEFT_REAL_ADJUST));

        _rightWheel.setSpeed(Orientation::FORWARD, (DEFAULT_SPEED + RIGHT_REAL_ADJUST) - (intensity + DEFAULT_ADJUST));
    }
}

void Navigation::jumpStart()
{
    moveStraight(Orientation::FORWARD, MAXIMUM_8BIT - LEFT_REAL_ADJUST - RIGHT_REAL_ADJUST);
    _delay_ms(JUMP_START_DELAY);
}

void Navigation::turnJumpStart(Side turn)
{
    if (turn == Side::LEFT)
    {
        _rightWheel.setSpeed(Orientation::FORWARD, MAXIMUM_8BIT - LEFT_REAL_ADJUST);
        _leftWheel.setSpeed(Orientation::BACKWARD, MAXIMUM_8BIT - RIGHT_REAL_ADJUST);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, MAXIMUM_8BIT - RIGHT_REAL_ADJUST);
        _rightWheel.setSpeed(Orientation::BACKWARD, MAXIMUM_8BIT - LEFT_REAL_ADJUST);
    }
    _delay_ms(JUMP_START_TURN_DELAY);
}