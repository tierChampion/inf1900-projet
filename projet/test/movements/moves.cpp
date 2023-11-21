#include "master_navigation.h"
#include "led.h"
#include "pathfindingMode.h"
#include "event_timer.h"

int main()
{
    MasterNavigation nav = MasterNavigation();

// 195 => 25 ms
// 42 => center on intersection
// 60 ~ 90 degrees
// 90

    MovementCode moves[1];
    moves[0] = MovementCode::FORWARD;
    // moves[1] = MovementCode::LEFT_FORWARD;

    nav.executeMovementCodes(moves, 1);

    // PathfindingMode mode = PathfindingMode();

    // mode.run(1, 0);
}
