#include <interpreter.h>

Interpreter::Interpreter() : _navigation(new Navigation()),
                             _piezo(new Piezo()), _led(new Led(Port::A, PA0, PA1)), _loopManager(new LoopManager()),
                             _eeprom(new Memoire24CXXX()), _executeEnable(false)
{
}

Interpreter::~Interpreter()
{
    delete _navigation;
    delete _piezo;
    delete _led;
    delete _loopManager;
    delete _eeprom;
}

void Interpreter::startInterpreting(uint8_t adress)
{
    uint8_t command[2];
    while (true)
    {
        _eeprom->lecture(adress, (uint8_t *)command, 2);
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
                _led->setColor(LedColor::GREEN);
                break;
            case 0x02:
                _led->setColor(LedColor::RED);
                break;
            default:
                break;
            }
            break;
        case 0x45: // eteindre del
            _led->setColor(LedColor::OFF);
            break;
        case 0x48:
            _piezo->play(Note::CSHARP, Octave::LOW); /// A CHANGER!!!
            break;
        case 0x09:
            _piezo->stop();
            break;
        case 0x60:
            _navigation->stop();
            break;
        case 0x61:
            _navigation->stop();
            break;
        case 0x62: // avant
            _navigation->moveStraight(Orientation::FORWARD, (_operand / 255) * 100);
            break;
        case 0x63: // arriere
            _navigation->moveStraight(Orientation::BACKWARD, (_operand / 255) * 100);
            break;
        case 0x64:                                                              // right
            _navigation->syncSpeedTurn(Side::RIGHT, Orientation::FORWARD, 100); // A CHANGER!!!
            _delay_ms(1000);
            _navigation->moveStraight(Orientation::FORWARD);
            break;
        case 0x65:                                                             // left
            _navigation->syncSpeedTurn(Side::LEFT, Orientation::FORWARD, 100); // A CHANGER!!!!
            _delay_ms(1000);
            _navigation->moveStraight(Orientation::FORWARD);
            break;
        case 0xC0:
            _loopManager->startLoop(adress, _operand);
            break;
        case 0xC1:
            return _loopManager->stopLoop(adress);
            break;
        case 0xFF:
            _executeEnable = false;
            break;
        default:
            break;
        }
    return adress + 2;
}
