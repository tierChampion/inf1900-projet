#include <interpreter.h>

const uint16_t MAXIMUM_READ = 127;

Interpreter::Interpreter() : _navigation(Navigation()),
                             _piezo(Piezo()), _led(Led(Port::A, PA0, PA1)), _loopManager(LoopManager()),
                             _eeprom(Memoire24CXXX()), _executeEnable(true)
{
}

void Interpreter::interpretBytecode()
{
    uint8_t command[INSTRUCTION_SIZE];
    uint16_t address = 0;

    loadBytecode();

    while (_executeEnable && address <= Comm::MAX_RECEIVE_SIZE)
    {
        command[0] = _commands[address];
        command[1] = _commands[address + 1];
        address = executeCommand(address, command);
    }
}

void Interpreter::loadBytecode() 
{
    for (uint8_t i = 0; i < Comm::MAX_RECEIVE_SIZE / MAXIMUM_READ; i++)
    {
        uint16_t address = i * (MAXIMUM_READ + 1);
        _eeprom.lecture(address, &_commands[address], MAXIMUM_READ);
    }
}

uint8_t Interpreter::executeCommand(uint8_t index, uint8_t *command)
{
    _instruction = static_cast<Instruction>(command[0]);
    _operand = command[1];

    switch (_instruction)
    {
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
            case Instruction::ATT:
                configurableDelayMS(_operand);
                break;

            case Instruction::DAL: 
                if (_operand == 1)
                    _led.setColor(LedColor::GREEN);
                else if (_operand == 2)
                    _led.setColor(LedColor::RED);
                break;

            case Instruction::DET: 
                _led.setColor(LedColor::OFF);
                break;

            case Instruction::SGO:
                _piezo.play(_operand); 
                break;

            case Instruction::SAR:
                _piezo.stop();
                break;

            case Instruction::MAR:
                _navigation.stop();
                break;

            case Instruction::MAV:
                _navigation.moveStraight(Orientation::FORWARD, toPercentage(_operand));
                break;

            case Instruction::MRE:
                _navigation.moveStraight(Orientation::BACKWARD, toPercentage(_operand));
                break;

            case Instruction::TRD:
                _navigation.pivot90(Side::RIGHT);
                break;

            case Instruction::TRG:
                _navigation.pivot90(Side::LEFT);
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

    return index + INSTRUCTION_SIZE;
}
