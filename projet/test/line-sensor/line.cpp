#include "line_sensor.h"

int main() {

    LineSensor sensor = LineSensor();

    while(true) {
        sensor.updateDetection();
        PRINT(structureToString(sensor.getStructure()));
        _delay_ms(1000);
    }
}
