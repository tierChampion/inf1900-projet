#include <interpreter.h>

Interpreter::Interpreter() : _navigation(Navigation()),
                             _piezo(Piezo()), _led(Led(Port::A, PA0, PA1)), _loopManager(LoopManager()),
                             _eeprom(Memoire24CXXX()), _executeEnable(false)
{
}

void Interpreter::startInterpreting(uint8_t adress)
{
    uint8_t command[2];
    _eeprom.lecture(adress, (uint8_t *)command, 2);

    uint8_t scriptLenght = command[0];
    for (; scriptLenght > 0; scriptLenght--)
    {
        _eeprom.lecture(adress, (uint8_t *)command, 2);
        adress = executeCommand(adress, command);
    }
}

uint8_t Interpreter::executeCommand(uint8_t adress, uint8_t *command)
{
    _instruction = command[0];
    _operand = command[1];

    switch (_instruction)
    {
    case 0x01:
        _executeEnable = true;
        break;
    case 0xFF:
        _executeEnable = false;
        break;
    default:
        break;
    }
    if (_executeEnable)
        switch (_instruction)
        {
        case 0x01: // commancer
            break;
        case 0x02: // attendre
            for (int i = 0; i < _operand; i++)
            {
                _delay_ms(25);
            }
            break;
        case 0x44: // allumer del
            switch (_operand)
            {
            case 0x01:
                _led.setColor(LedColor::GREEN);
                break;
            case 0x02:
                _led.setColor(LedColor::RED);
                break;
            default:
                break;
            }
            break;
        case 0x45: // eteindre del
            _led.setColor(LedColor::OFF);
            break;
        case 0x48:
            // _piezo.play(operand); /// A CHANGER!!!
            break;
        case 0x09:
            _piezo.stop();
            break;
        case 0x60:
            _navigation.stop();
            break;
        case 0x61:
            _navigation.stop();
            break;
        case 0x62: // avant
            _navigation.moveStraight(Orientation::FORWARD, (_operand / 255) * 100);
            break;
        case 0x63: // arriere
            _navigation.moveStraight(Orientation::BACKWARD, (_operand / 255) * 100);
            break;
        case 0x64:                                                             // right
            _navigation.syncSpeedTurn(Side::RIGHT, Orientation::FORWARD, 100); // A CHANGER!!!
            _delay_ms(1000);
            _navigation.moveStraight(Orientation::FORWARD);
            break;
        case 0x65:                                                            // left
            _navigation.syncSpeedTurn(Side::LEFT, Orientation::FORWARD, 100); // A CHANGER!!!!
            _delay_ms(1000);
            _navigation.moveStraight(Orientation::FORWARD);
            break;
        case 0xC0:
            _loopManager.startLoop(adress, _operand);
            break;
        case 0xC1:
            return _loopManager.stopLoop(adress);
            break;
        case 0xFF:
            _executeEnable = false;
            break;
        default:
            break;
        }
    return adress + 2;
}
