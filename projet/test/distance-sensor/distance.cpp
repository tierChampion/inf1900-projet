#include "distance_sensor.h"

int main() {

    DistanceSensor sensor = DistanceSensor();

    while(!sensor.isClose()) {
    }

    PRINT("CLOSE ENOUGH");
}
