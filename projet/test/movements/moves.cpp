#include "master_navigation.h"
#include "pathfindingMode.h"

int main()
{
    // MasterNavigation nav = MasterNavigation();

    // MovementCode moves[3];
    // moves[0] = MovementCode::FORWARD;
    // moves[1] = MovementCode::RIGHT_FORWARD;

    // nav.executeMovementCodes(moves, 2);

    PathfindingMode mode = PathfindingMode();

    mode.run(1, 0);
}
