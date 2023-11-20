#include "master_navigation.h"

int main()
{

    // LineSensor sen;

    // while (true)
    // {
    //     sen.updateDetection();
    //     PRINT(sen.needRightAdjustment() ? "YES" : "NO");
    // }
    MasterNavigation nav = MasterNavigation();

    MovementCode moves[2];
    moves[0] = MovementCode::FORWARD;
    moves[1] = MovementCode::FORWARD;

    nav.executeMovementCodes(moves, 2);
}
