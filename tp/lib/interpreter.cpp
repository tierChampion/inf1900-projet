#include <interpreter.h>

Interpreter::Interpreter() : _navigation(Navigation()),
                             _piezo(Piezo()), _led(Led(Port::A, PA0, PA1)), _loopManager(LoopManager()),
                             _eeprom(Memoire24CXXX()), _executeEnable(false)
{
}

void Interpreter::interpreteByteCode(uint16_t adress)
{

    // TODO cette partie, loading doit etre different
    uint8_t command[2];
    _eeprom.lecture(adress, (uint8_t *)_commands, MAX_LOOP_INSTRUCTIONS);
    _executeEnable = true;
    uint8_t i = (uint8_t)0;
    while (_executeEnable && i < MAX_LOOP_INSTRUCTIONS)
    {
        command[0] = _commands[i];
        command[1] = _commands[i + 1];
        if (static_cast<Instruction>(command[0]) == Instruction::DBC)
        {
            i = interpreteLoop(i);
            continue;
        }
        else
        {
            i = executeCommand(i, command);
        }
    }
}
uint8_t Interpreter::interpreteLoop(uint8_t index)
{
    uint8_t command[2];
    do
    {
        if (index >= MAX_LOOP_INSTRUCTIONS)
            break;

        command[0] = _commands[index];
        command[1] = _commands[index + 1];
        index = executeCommand(index, command);

    } while (_loopManager.loopStarted() && !_loopManager.loopEnded());
    return index;
}

uint8_t Interpreter::executeCommand(uint8_t index, uint8_t *command)
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
            _loopManager.startLoop(index, _operand);
            break;

        case Instruction::FBC:
            return _loopManager.stopLoop(index);
            break;

        default:
            break;
        }
    }
    return index + 2;
}
