#include <interpreter.h>

Interpreter::Interpreter() : _navigation(Navigation()),
                             _piezo(Piezo()), _led(Led(Port::A, PA0, PA1)), _loopManager(LoopManager()),
                             _eeprom(Memoire24CXXX()), _executeEnable(false)
{
}

void Interpreter::startInterpreting(uint8_t address)
{

    // TODO cette partie, loading doit etre different
    uint8_t command[2];
    _eeprom.lecture(address, (uint8_t *)command, 2);

    uint8_t scriptLenght = command[0];
    for (; scriptLenght > 0; scriptLenght--)
    {
        _eeprom.lecture(address, (uint8_t *)command, 2);
        address = executeCommand(address, command);
    }
}

uint8_t Interpreter::executeCommand(uint8_t address, uint8_t *command)
{
    _instruction = static_cast<Instruction>(command[0]);
    _operand = command[1];

    switch (_instruction)
    {
    case Instruction::DBT:
        _executeEnable = true;
        break;
    case Instruction::FIN:
        _executeEnable = false;
        break;
    default:
        break;
    }

    if (_executeEnable)
    {
        switch (_instruction)
        {
        case Instruction::ATT: // attendre
            configurableDelayMS(_operand);
            break;

        case Instruction::DAL: // allumer del
            if (_operand == 1)
                _led.setColor(LedColor::GREEN);
            else if (_operand == 2)
                _led.setColor(LedColor::RED);
            break;

        case Instruction::DET: // eteindre del
            _led.setColor(LedColor::OFF);
            break;

        case Instruction::SGO:
            _piezo.play(_operand); /// A CHANGER!!!
            break;

        case Instruction::SAR:
            _piezo.stop();
            break;

        case Instruction::MAR:
            _navigation.stop();
            break;
        // TODO check les mouvements et la navigation
        case Instruction::MAV: // avant
            _navigation.moveStraight(Orientation::FORWARD, (_operand / 255) * 100);
            break;

        case Instruction::MRE: // arriere
            _navigation.moveStraight(Orientation::BACKWARD, (_operand / 255) * 100);
            break;
        // TODO tourner sur soit meme
        case Instruction::TRD:                                                 // right
            _navigation.syncSpeedTurn(Side::RIGHT, Orientation::FORWARD, 100); // A CHANGER!!!
            _delay_ms(1000);
            _navigation.moveStraight(Orientation::FORWARD);
            break;

        case Instruction::TRG:                                                // left
            _navigation.syncSpeedTurn(Side::LEFT, Orientation::FORWARD, 100); // A CHANGER!!!!
            _delay_ms(1000);
            _navigation.moveStraight(Orientation::FORWARD);
            break;

        case Instruction::DBC:
            _loopManager.startLoop(address, _operand);
            break;

        case Instruction::FBC:
            return _loopManager.stopLoop(address);
            break;

        default:
            break;
        }
    }
    return address + 2;
}
