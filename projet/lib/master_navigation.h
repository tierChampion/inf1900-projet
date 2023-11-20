#ifndef MASTER_NAVIGATION_H
#define MASTER_NAVIGATION_H

#include "navigation.h"
#include "line_sensor.h"

class MasterNavigation
{
public:
    MasterNavigation();

private:
    Navigation _navigation;
    LineSensor _lineSensor;
};

#endif