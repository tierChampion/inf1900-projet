#include "navigation.h"

const uint8_t DEFAULT_SPEED = 110;      // 150 pour le robot 16. baisser a 80?
const uint8_t DEFAULT_TURN_SPEED = 100; // pour le robot 16
const uint8_t LEFT_REAL_ADJUST = 10;    // pour le robot 16
const uint8_t DEFAULT_ADJUST = 5;       // pour le robot 16
const uint16_t TURN_DELAY = 650;
// constants to start the robot if it doesnt move
const uint8_t JUMP_START_SPEED = 150;
const uint8_t JUMP_START_DELAY = 25;

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
    _speed = DEFAULT_SPEED;
    _leftWheel.setSpeed(orientation, _speed + LEFT_REAL_ADJUST);
    _rightWheel.setSpeed(orientation, _speed);
}

void Navigation::realForward()
{
    _speed = DEFAULT_SPEED;
    _leftWheel.setSpeed(Orientation::FORWARD, (_speed + 30));
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
        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED);
        _leftWheel.setSpeed(Orientation::BACKWARD, DEFAULT_SPEED);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED);
        _rightWheel.setSpeed(Orientation::BACKWARD, DEFAULT_SPEED);
    }

    _delay_ms(TURN_DELAY);
    stop();
}

void Navigation::pivot(Side turn)
{
    if (turn == Side::LEFT)
    {
        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_TURN_SPEED);
        _leftWheel.setSpeed(Orientation::BACKWARD, DEFAULT_TURN_SPEED + LEFT_REAL_ADJUST);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, DEFAULT_TURN_SPEED + LEFT_REAL_ADJUST);
        _rightWheel.setSpeed(Orientation::BACKWARD, DEFAULT_TURN_SPEED);
    }
}

void Navigation::adjustWheel(Side turn, uint8_t intensity)
{
    if (turn == Side::LEFT)
    {
        _leftWheel.setSpeed(Orientation::FORWARD, (DEFAULT_SPEED + LEFT_REAL_ADJUST) - (intensity + DEFAULT_ADJUST));

        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, (DEFAULT_SPEED + LEFT_REAL_ADJUST));

        _rightWheel.setSpeed(Orientation::FORWARD, DEFAULT_SPEED - (intensity + DEFAULT_ADJUST));
    }
}

void Navigation::jumpStart()
{
    moveStraight(Orientation::FORWARD, JUMP_START_SPEED);
    _delay_ms(JUMP_START_DELAY);
}

void Navigation::turnJumpStart(Side turn)
{
    if (turn == Side::LEFT)
    {
        _rightWheel.setSpeed(Orientation::FORWARD, JUMP_START_SPEED);
        _leftWheel.setSpeed(Orientation::BACKWARD, JUMP_START_SPEED + LEFT_REAL_ADJUST);
    }
    else
    {
        _leftWheel.setSpeed(Orientation::FORWARD, JUMP_START_SPEED + LEFT_REAL_ADJUST);
        _rightWheel.setSpeed(Orientation::BACKWARD, JUMP_START_SPEED);
    }
    _delay_ms(JUMP_START_DELAY);
}