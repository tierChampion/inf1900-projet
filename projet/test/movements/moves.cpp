#include "master_navigation.h"
#include "led.h"
#include "pathfinding_mode.h"
#include "event_timer.h"

int main()
{
    MasterNavigation nav = MasterNavigation();
    nav.driveOneUnit();

}
