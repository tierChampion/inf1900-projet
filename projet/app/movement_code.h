#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include "commons.h"

enum class MovementCode
{
    NOTHING,
    FORWARD, // forward to the next intersection
    FORWARD_1, // forward one unit on the map
    LEFT,
    RIGHT,
    UTURN,
};

#endif
