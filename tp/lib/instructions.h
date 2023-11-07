#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "commons.h"

const uint8_t INSTRUCTION_SIZE = 2;

enum class Instruction : uint8_t {
    DBT = 0x01,
    ATT = 0x02,
    DAL = 0x44,
    DET = 0x45,
    SGO = 0x48,
    SAR = 0x09,
    MAR = 0x61,
    MAV = 0x62,
    MRE = 0x63,
    TRD = 0x64,
    TRG = 0x65,
    DBC = 0xC0,
    FBC = 0xC1,
    FIN = 0xFF
};

#endif