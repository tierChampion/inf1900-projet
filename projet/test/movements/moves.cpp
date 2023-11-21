#include "master_navigation.h"
#include "led.h"
#include "pathfinding_mode.h"
#include "event_timer.h"

int main()
{
    //MasterNavigation nav = MasterNavigation();

    PRINT("PATH MODE SIZE IN BYTES:");
    PRINT(static_cast<uint16_t>(sizeof(PathfindingMode)));

    PRINT("MAP SIZE IN BYTES:");
    PRINT(static_cast<uint16_t>(sizeof(Map)));
}
