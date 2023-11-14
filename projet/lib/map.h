#ifndef MAP_H
#define MAP_H

#include "commons.h"

const uint8_t NODE_COUNT = 28;

struct MapElement {
    uint8_t _westNode;
    uint8_t _eastNode;
    uint8_t _northNode;
    uint8_t _southNode;
};

class Map {

public:
    Map();
    void placePillar(uint8_t position);
    void removePillar();

private:
    MapElement nodes[NODE_COUNT];
    uint8_t pillar;
};

#endif