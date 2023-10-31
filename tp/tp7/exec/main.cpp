#include "navigation.h"
#include "timer1.h"
#include "pins.h"

int main()
{
    Navigation nav = Navigation();

    nav.controlledTurn(Side::RIGHT, Orientation::FORWARD, 100, 255);

    while(true) {}

    return 0;
}